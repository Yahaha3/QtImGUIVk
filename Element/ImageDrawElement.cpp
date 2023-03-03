#include "ImageDrawElement.h"
#include "Map/map_include.h"

clz::ImageDrawElement::ImageDrawElement()
{
}

void clz::ImageDrawElement::element_projection()
{
    calculate_geometry();
}

QRectF clz::ImageDrawElement::proj_shape() const
{
    return get_map()->get_map_projection()->geoToProj(m_geo_rect);
}

void clz::ImageDrawElement::paint()
{
    if(!get_map()) return;

    auto ID = get_image_texture_id();
    if(!ID) return;
    // 1,直接给定矩形位置绘制
    // 2,给定指定位置和方向,长宽等,自行计算矩形并旋转后得到对应位置
    calculate_geometry();
    ImPlot::PlotImageQuad(clz::empty.toStdString().data(), ID, mbmin, mbmax, m_image_angle);
}

void clz::ImageDrawElement::load_image(const QByteArray &raw)
{
    m_image.set_origin_data(raw.toStdString());
    m_image.loadFromOrigin();
}

void clz::ImageDrawElement::update_image(uchar *raw, int width, int height)
{
    m_image.updateOrigin(raw, width, height);
}

void clz::ImageDrawElement::set_geometry(const clz::GeoRect &rect)
{
    m_geometry_type = GeometryType::ByRect;
    if(m_geo_rect == rect) return;
    m_geo_rect = rect;
//    calculate_geometry();
}

void clz::ImageDrawElement::set_geometry(const clz::GeoPos &position, const QSize &size, const QPoint anchor, const float &angle)
{
    m_geometry_type = GeometryType::ByPos;
    if(center() == position && m_image_size == size && abs(m_image_angle - angle) < 1e-3) return;
    m_image_size = size;
    m_image_anchor = anchor;
    set_geometry(position, angle);
}

void clz::ImageDrawElement::set_geometry(const clz::GeoPos &position, const float &angle)
{
    m_image_angle = angle;
    set_center(position);
//    calculate_geometry();
}

ImTextureID clz::ImageDrawElement::get_image_texture_id()
{
    bool ok = false;
    auto ID = m_image.get_image_texture(ok);
    if(!ok) return nullptr;
    return ID;
}

int clz::ImageDrawElement::width() const
{
    return m_image.Width;
}

int clz::ImageDrawElement::height() const
{
    return m_image.Height;
}

void clz::ImageDrawElement::calculate_geometry()
{
    if(m_geometry_type == GeometryType::None ||
                        get_map() == nullptr ||
       get_map()->get_map_projection() == nullptr){
        return;
    }

    if(m_geometry_type == GeometryType::ByRect){
        set_center(m_geo_rect.center());
        mbmin = get_map()->geopos2implot(m_geo_rect.topLeft());
        mbmax = get_map()->geopos2implot(m_geo_rect.bottomRight());
    } else if(m_geometry_type == GeometryType::ByPos){
        if(m_image_size.isEmpty()) return;
        const QPointF anchor = (m_image_anchor.isNull()) ?
                    QPointF(m_image_size.width() / 2, m_image_size.height() / 2) :
                    m_image_anchor;
        if(is_flag(IgnoreScale)){
            auto impt = get_map()->geopos2implot(center());
            auto pxpt = GeoFunctionUtility::implot2px(impt);
            auto pxrect = QRectF(QPointF(pxpt.x - anchor.x(), pxpt.y - anchor.y()), m_image_size);
            mbmin = GeoFunctionUtility::px2implot(ImPlotPoint(pxrect.topLeft().x(), pxrect.topLeft().y()));
            mbmax = GeoFunctionUtility::px2implot(ImPlotPoint(pxrect.bottomRight().x(), pxrect.bottomRight().y()));
        } else{
            const QPointF base_pos = get_map()->get_map_projection()->geoToProj(center());
            auto proj_rect = QRectF(base_pos - anchor, m_image_size);
            m_geo_rect = get_map()->get_map_projection()->projToGeo(proj_rect);
            mbmin = get_map()->geopos2implot(m_geo_rect.topLeft());
            mbmax = get_map()->geopos2implot(m_geo_rect.bottomRight());
        }
    }
}
