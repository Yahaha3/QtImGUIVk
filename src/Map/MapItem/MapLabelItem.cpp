#include "MapLabelItem.h"

clz::MapLabelItem::MapLabelItem()
{

}

void clz::MapLabelItem::paint()
{
    auto size = ImPlot::GetPlotSize();
    auto geo_pos = get_map()->implot2geopos(ImPlot::GetPlotMousePos());
    QString llabel = QString(" %1, %2 ")
            .arg(QString::number(geo_pos.longitude(), 'f', 7))
            .arg(QString::number(geo_pos.latitude(), 'f', 7));
    ImPlot::GetPlotDrawList()->AddText({size.x / 2 - llabel.size() * 2, 50}, IM_COL32_BLACK, llabel.toLocal8Bit());
}
