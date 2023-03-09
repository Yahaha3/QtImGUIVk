#include "MapItem.h"

clz::MapItem::MapItem()
{
    m_map = nullptr;
}

clz::MapItem::~MapItem()
{

}

void clz::MapItem::set_map(clz::GeoMap *geomap)
{
    m_map.reset(geomap);
}

std::shared_ptr<clz::GeoMap> clz::MapItem::get_map() const
{
    return m_map;
}

void clz::MapItem::set_back_color(const QColor &color)
{
    m_back_color = color;
}

ImVec4 clz::MapItem::get_back_color()
{
    return ImVec4(m_back_color.red(), m_back_color.blue(), m_back_color.green(), m_back_color.alpha());
}
