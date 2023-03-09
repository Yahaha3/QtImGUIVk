#include "OperatorLineString.h"

clz::OperatorLineString::OperatorLineString(clz::GeoMap *map, QString signature, QObject *parent)
    : OperatorBase(map, signature, parent)
{
    set_layer(clz::layer_name_mission_phases);
}

clz::ClzResult clz::OperatorLineString::generate_operator(const QVector<clz::GeoPos> &vertices, bool noted)
{
    ClzResult result;
    auto projection = map()->get_map_projection();
    if(!projection){
        result = ClzMapError;
        return result;
    }
    update_vertices(vertices);
    auto body = new LinestringDrawElement(m_vertices);
    m_element_body = body->get_element_id();
    auto plist = clz::GeoFunctionUtility::geo_list_to_proj_list(vertices, projection.get());
    m_center = clz::GeoFunctionUtility::get_polygon_center(plist);
    result = ClzSuccess;
    return result;
}

void clz::OperatorLineString::update_vertices(QVector<clz::GeoPos> vertices, bool noted)
{
    m_vertices.clear();
    m_vertices << vertices;
    auto projection = map()->get_map_projection();
    if(!projection) return;
    auto plist = clz::GeoFunctionUtility::geo_list_to_proj_list(vertices, projection.get());
    m_center = clz::GeoFunctionUtility::get_polygon_center(plist);

    auto body = element_body();
    if(!body) return;
    body->set_vertices(vertices);

    auto center = element_center();
    if(!center) return;
    center->set_center(m_center);
}

void clz::OperatorLineString::refresh()
{
    if(element_body()){
        element_body()->paint();
    }

    if(element_center()){
        element_center()->paint();
    }
}

void clz::OperatorLineString::generate_linestring()
{

}

QString clz::OperatorLineString::get_mark_icon_path()
{
    return clz::empty;
}
