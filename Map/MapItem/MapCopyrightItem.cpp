#include "MapCopyrightItem.h"
#include "Map/GeoMap.h"
#include "GeoFunction/LayerBackend/GeoBackend.h"

clz::MapCopyrightItem::MapCopyrightItem()
{
    set_back_color(Qt::black);
}

void clz::MapCopyrightItem::paint()
{
    auto map = get_map();
    if(!map) return;

    auto pos  = ImPlot::GetPlotPos();
    auto size = ImPlot::GetPlotSize();
    const char* label = "Copyrights@ Gaode";
    auto bcolor = get_back_color();
    auto ucolor = IM_COL32(bcolor.x, bcolor.y, bcolor.z, bcolor.w);

    auto label_size = ImGui::CalcTextSize(label);
    auto label_off  = ImPlot::GetStyle().MousePosPadding;
    ImPlot::GetPlotDrawList()->AddText({pos.x + label_off.x, pos.y+size.y-label_size.y-label_off.y},
                                       ucolor,label);
}
