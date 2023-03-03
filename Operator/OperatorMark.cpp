#include "OperatorMark.h"
#include "eqnx_common_global.h"

clz::OperatorMark::OperatorMark(clz::GeoMap *map, QString signature, QObject *parent)
    : OperatorBase(map, signature, parent)
{

}

clz::ClzOpResult clz::OperatorMark::generate_operator(const clz::GeoPos &position, bool noted)
{
    m_center = position;
    auto mark = new ImageDrawElement();
    m_element_body = mark->get_element_id();
    bool ok = false;
    auto data = clz::GeoFunctionUtility::get_image_bytearray(get_mark_icon_path(), ok);
    if(!ok) return IconError;
    mark->load_image(data);
    mark->set_flag(IgnoreScale);
    mark->set_geometry(m_center, m_mark_size, QPoint(), 0);
    return Success;
}

clz::ClzOpResult clz::OperatorMark::translate_to(clz::GeoPos position)
{
    auto body = dynamic_cast<ImageDrawElement*>(element_body());
    if(!body) return Error;
    m_center = position;
    body->set_geometry(position, m_mark_azimuth);
    return Success;
}

void clz::OperatorMark::rotate(double angle)
{
    auto body = dynamic_cast<ImageDrawElement*>(element_body());
    if(!body) return;
    m_mark_azimuth = angle;
    body->set_geometry(m_center, angle);
}

void clz::OperatorMark::refresh()
{
    // 可能会涉及到位置校验
    if(element_body()){
        element_body()->paint();
    }
}

QString clz::OperatorMark::get_mark_icon_path()
{
    auto path = clz::GeoFunctionUtility::kernel_apps_dir()
            + eqnx::other::map_animation_path
            + icon_template() + eqnx::other::png_suffix;
    return path;
}
