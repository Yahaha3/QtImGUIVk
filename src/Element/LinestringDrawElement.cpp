#include "LinestringDrawElement.h"
#include <QPolygonF>
#include "Map/GeoMap.h"

clz::LinestringDrawElement::LinestringDrawElement(const QVector<clz::GeoPos> &vertices, QColor color)
{
    set_vertices(vertices);
    set_back_color(color);
    set_selectable(false);
    m_proj_vertices.resize(vertices.size());
    m_plot_vertices = new ImPlotPointArray(vertices.size());
    element_projection();
}

void clz::LinestringDrawElement::element_projection()
{
    auto map = get_map();
    if(!map) return;
    auto projection = map->get_map_projection();
    if(!projection) return;
    auto vertices = get_vertices();
    for(int i = 0, si = vertices.size(); i < si; ++i){
        auto pv = projection->geoToProj(vertices[i]);
        m_proj_vertices[i] = pv;
        auto pp = map->geopos2implot(vertices[i]);
        m_plot_vertices->set(pp, i);
    }
}

QRectF clz::LinestringDrawElement::proj_shape() const
{
    QPolygonF polygon(m_proj_vertices);
    return polygon.boundingRect();
}

void clz::LinestringDrawElement::paint()
{
    ImPlot::SetNextLineStyle(get_back_color());
    ImPlot::PlotLine(clz::empty.toStdString().data(),
                     m_plot_vertices->x_array(),
                     m_plot_vertices->y_array(),
                     m_plot_vertices->size());
}
