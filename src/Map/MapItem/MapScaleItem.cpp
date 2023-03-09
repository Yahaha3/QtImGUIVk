#include "MapScaleItem.h"
#include "Map/Projection/Projection.h"
#include <QPointF>
#include <qmath.h>
#include "GeoFunction/GeoFunctionUtility.h"
// 在270px -- 120px之间显示地图比例尺
clz::MapScaleItem::MapScaleItem()
{
    m_scale_pt = std::make_shared<ImPlotPointArray>(4);
}

void clz::MapScaleItem::paint()
{
    calculate_item();
    ImPlot::SetNextLineStyle(get_back_color());
    ImPlot::PlotLine("##ScaleItem", m_scale_pt->x_array(), m_scale_pt->y_array(), m_scale_pt->size());

    ImPlot::GetPlotDrawList()->AddText({(float)m_label_pos.x, (float)m_label_pos.y},
                                       IM_COL32_BLACK,
                                       get_length_meters().toLocal8Bit());
}

// 目前只考虑水平方向
void clz::MapScaleItem::calculate_item()
{
    // 计算缩放比例以及缩放值
    auto limits = ImPlot::GetPlotLimits();
    auto size = ImPlot::GetPlotSize();
    auto per_with = limits.X.Size() / 60;
    auto per_height = limits.Y.Size() / 60;

    ImPlotPoint view_pt1(limits.X.Max - per_with, limits.Y.Max - per_height);
    auto view_px1 = GeoFunctionUtility::implot2px(view_pt1);
    auto view_px2 = ImPlotPoint(view_px1.x - m_default_length_px, view_px1.y);
    ImPlotPoint view_pt2 = GeoFunctionUtility::px2implot(view_px2);

    auto geo_pt1 = get_map()->implot2geopos(view_pt1);
    auto geo_pt2 = get_map()->implot2geopos(view_pt2);
    int new_length_px = m_default_length_px;
    int new_length_meters = static_cast<int>(get_map()->get_map_projection()->geodesicMeters(geo_pt1, geo_pt2));
    const double meters_log = qMax(1.0, log10(new_length_meters));
    const int meters10 = static_cast<int>(pow(10, floor(meters_log)));
    const double correction = static_cast<double>(meters10) / new_length_meters;
    new_length_meters = meters10;
    new_length_px = ceil(correction * new_length_px);
    if(new_length_px < m_min_length_px){
        const double factor = pow(2, ceil(log(m_min_length_px / new_length_px) * M_LOG2E));
        new_length_meters *= factor;
        new_length_px *= factor;
    }

    auto ratio = new_length_px / size.x;
    auto implot_with = limits.X.Size() * ratio;

    auto x_min = limits.X.Max - per_with - implot_with;
    auto x_max = limits.X.Max - per_with;
    auto y_min = limits.Y.Max - per_height * 2;
    auto y_max = limits.Y.Max - per_height;

    auto x_array = m_scale_pt->x_array();
    auto y_array = m_scale_pt->y_array();
    x_array[0] = x_min;
    x_array[1] = x_min;
    x_array[2] = x_max;
    x_array[3] = x_max;

    y_array[0] = y_min;
    y_array[1] = y_max;
    y_array[2] = y_max;
    y_array[3] = y_min;

    // 计算缩放值的位置
    ImPlotPoint label_of_plot_pos(x_min + per_with, y_min - per_height / 3);

    m_label_pos = GeoFunctionUtility::implot2px(label_of_plot_pos);
    m_length_meters = new_length_meters;
}

QString clz::MapScaleItem::get_length_meters()
{
    if(m_length_meters > 1000){
        return QString("%1 km").arg(QString::number(static_cast<int>(m_length_meters) / 1000));
    } else {
        return QString("%1 m").arg(QString::number(static_cast<int>(m_length_meters)));
    }
}
