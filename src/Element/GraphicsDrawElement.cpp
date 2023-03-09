#include "GraphicsDrawElement.h"
#include "Map/GeoMap.h"

clz::GraphicsDrawElement::GraphicsDrawElement()
{

}

void clz::GraphicsDrawElement::set_flags(QSet<clz::ItemFlag> flags)
{
    if(m_flags != flags){
        m_flags = flags;
    }
}

void clz::GraphicsDrawElement::set_flag(clz::ItemFlag flag, bool enable)
{
    auto new_flags = get_flags();
    if(enable){
        new_flags |= flag;
    } else {
        new_flags -= flag;
    }
    set_flags(new_flags);
}

QSet<clz::ItemFlag> clz::GraphicsDrawElement::get_flags() const
{
    return m_flags;
}

bool clz::GraphicsDrawElement::is_flag(clz::ItemFlag flag) const
{
    auto flags = get_flags();
    for(auto sflag: flags){
        if(sflag == flag) return true;
    }
    return false;
}

void clz::GraphicsDrawElement::refresh()
{
    element_projection();
}

void clz::GraphicsDrawElement::repaint()
{
    paint();
}

clz::GeoPos clz::GraphicsDrawElement::center()
{
    if(m_center.zero() && get_map()){
        auto projection = get_map()->get_map_projection();
        if(!projection) return m_center;
        auto plist = clz::GeoFunctionUtility::geo_list_to_proj_list(m_vertices, projection.get());
        m_center = projection->projToGeo(clz::GeoFunctionUtility::get_polyline_center(plist));
    }
    return m_center;
}

void clz::GraphicsDrawElement::set_center(const clz::GeoPos &position)
{
    if(m_center == position) return;
    m_center = position;
}

void clz::GraphicsDrawElement::set_vertices(const QVector<clz::GeoPos> &vertices)
{
    m_vertices.clear();
    m_vertices << vertices;
}

QVector<clz::GeoPos> clz::GraphicsDrawElement::get_vertices() const
{
    return m_vertices;
}

void clz::GraphicsDrawElement::set_back_color(const QColor &color)
{
    m_back_color = color;
}

void clz::GraphicsDrawElement::set_front_color(const QColor &color)
{
    m_front_color = color;
}

ImVec4 clz::GraphicsDrawElement::get_back_color() const
{
    return ImVec4(m_back_color.red(), m_back_color.blue(), m_back_color.green(), m_back_color.alpha());
}

ImVec4 clz::GraphicsDrawElement::get_front_color() const
{
    return ImVec4(m_front_color.red(), m_front_color.blue(), m_front_color.green(), m_front_color.alpha());;
}
