#include "OperatorMark.h"
#include "eqnx_common_global.h"
#include "Element/ElementContainer.h"

clz::OperatorMark::OperatorMark(clz::GeoMap *map, QString signature, QObject *parent)
    : OperatorBase(map, signature, parent)
{
    set_layer(clz::layer_name_mark);
}

clz::ClzResult clz::OperatorMark::generate_operator(const clz::GeoPos &position, bool noted)
{
    m_center = position;
    auto mark = new ImageDrawElement();
    m_element_body = mark->get_element_id();
    bool ok = false;
    auto data = clz::GeoFunctionUtility::get_image_bytearray(get_mark_icon_path(), ok);
    if(!ok) return ClzIconError;
    mark->load_image(data);
    mark->set_flag(IgnoreScale);
    mark->set_geometry(m_center, m_mark_size, QPoint(), 0);
    return ClzSuccess;
}

clz::ClzResult clz::OperatorMark::translate_to(clz::GeoPos position)
{
    auto body = dynamic_cast<ImageDrawElement*>(element_body());
    if(!body) return ClzError;
    if(!m_interpolation){
        m_center = position;
        body->set_geometry(position, m_mark_azimuth);
    } else {
        auto ct = QDateTime::currentMSecsSinceEpoch();
        if( m_interpolation_infos.isEmpty() || m_interpolation_infos.lastKey() != ct){
            m_interpolation_infos.insert(ct, qMakePair(position, m_mark_azimuth));
        }
        if(m_interpolation_infos.size() > 7){
            auto fk = m_interpolation_infos.firstKey();
            m_interpolation_infos.remove(fk);
        }
    }
    return ClzSuccess;
}

void clz::OperatorMark::rotate(double angle)
{
    auto body = dynamic_cast<ImageDrawElement*>(element_body());
    if(!body) return;
    if(!m_interpolation){
        m_mark_azimuth = angle;
        body->set_geometry(m_center, angle);
    } else {
        // 更新最近一个插值数据中的值
        auto key = m_interpolation_infos.lastKey();
        m_interpolation_infos[key].second = angle;
    }
}

void clz::OperatorMark::refresh()
{
    // 可能会涉及到位置校验
    if(element_body()){
        if(m_interpolation){
            update_interpolation();
        }
        element_body()->paint();
    }
}

void clz::OperatorMark::enable_interpolation(bool interpolation)
{
    m_interpolation = interpolation;
}

void clz::OperatorMark::enable_trajectory(bool trajectory)
{
    m_trajectory = trajectory;
}

void clz::OperatorMark::update_interpolation()
{
    auto projection = map()->get_map_projection();
    if(!projection) return;
    auto body = dynamic_cast<ImageDrawElement*>(element_body());
    if(!body) return;
    auto ct = current_timestamp();
    if(m_interpolation_infos.size() > 1){
        qint64 t0, t1;
        bool match = false;
        auto keys = m_interpolation_infos.keys();
        for(int i = 0, si = keys.size(); i < si; i++){
            if(keys[i] > ct && i > 0){
                t1 = keys[i];
                t0 = keys[i - 1];
                match = true;
                break;
            }
        }
        if(match){
            auto p0 = m_interpolation_infos[t0].first;
            auto p1 = m_interpolation_infos[t1].first;
            auto a0 = m_interpolation_infos[t0].second;
            auto a1 = m_interpolation_infos[t1].second;

            auto distance = projection->geodesicMeters(p0, p1);
            auto da = a1 - a0;
            if(abs(da - 360) < 1e3) {da = 0;}
            auto rate = (double)(ct - t0) / (double)(t1 - t0);
            if(rate <= 0) {rate = 0;}
//            std::cout << "rate: " << rate << std::endl;
            auto target_distance = distance * rate;
            m_mark_azimuth = da * rate + a0;
            m_center = clz::GeoFunctionUtility::get_target_lonlat_with_distance(p1, p0, target_distance);
            body->set_geometry(m_center, m_mark_azimuth);
        }
    }
}

qint64 clz::OperatorMark::current_timestamp()
{
    return QDateTime::currentMSecsSinceEpoch() - m_delay;
}

QString clz::OperatorMark::get_mark_icon_path()
{
    return icon_template();
//    auto path = clz::GeoFunctionUtility::kernel_apps_dir()
//            + eqnx::other::map_animation_path
//            + icon_template() + eqnx::other::png_suffix;
//    return path;
}

clz::LinestringDrawElement *clz::OperatorMark::element_trajectory()
{
    if(!m_has_trajectory) return nullptr;
    if(m_trajectory == clz::empty){
        QVector<clz::GeoPos> fp = {m_center};
        auto trajectory = new LinestringDrawElement(fp);
        m_trajectory = trajectory->get_element_id();
        return trajectory;
    }
    auto trajectory = clz::ElementContainer::instance().get_element(m_trajectory);
    return dynamic_cast<LinestringDrawElement*>(trajectory);
}
