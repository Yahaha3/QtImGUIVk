#include "ClzMapSupport.h"
#include "Operator/OperatorContainer.h"
#include "socket_function_include.h"
#include "Map/GeoMap.h"
#include "Operator/OperatorContainer.h"
#include "Operator/OperatorMark.h"

clz::ClzMapSupport::ClzMapSupport(QObject *parent)
{

}

void clz::ClzMapSupport::map_init(const QString &process_name, GeoMap* map)
{
    m_process_name = process_name;

    auto inst = eqnx_dh::InstanceContainer::handle();
    inst->set_process_name(process_name);
    {
        connect(inst, &eqnx_dh::InstanceContainer::sig_load_msg_arrived,
                this, &clz::ClzMapSupport::slot_map_support_mark_insert);
        connect(inst, &eqnx_dh::InstanceContainer::sig_drive_msg_arrived,
                this, &clz::ClzMapSupport::slot_map_support_mark_update);
    }

    m_map = map;
}

void clz::ClzMapSupport::slot_map_support_mark_insert(const QJsonObject &jo)
{
    auto idsn = jo[eqnx::eqco::eqco_name].toString();
    auto type = jo[eqnx::eqco::eqco_type].toString();
    auto position = jo[eqnx::eqco::eqco_start_pos].toArray();
    auto moc = m_map->map_oc();
    if(!moc) return;
    auto mark = moc->get_operator_mark(idsn);
    mark->set_icon_template(type);
    mark->generate_operator(GeoPos(position.at(1).toDouble(), position.at(0).toDouble()), false);
}

void clz::ClzMapSupport::slot_map_support_mark_update(const QJsonObject &jo)
{
    auto lat  = jo[eqnx::eqco::eqco_lat].toDouble();
    auto lon  = jo[eqnx::eqco::eqco_lon].toDouble();
    auto alt  = jo[eqnx::eqco::eqco_alt].toDouble();
    auto yaw  = jo[eqnx::eqco::eqco_yaw].toDouble();
    auto idsn = jo[eqnx::eqco::eqco_name].toString();
    auto moc = m_map->map_oc();
    if(!moc) return;
    ClzOpResult result;
    auto mark = moc->get_affirm_operator(idsn, result);
    if(result == Success){
        mark->translate_to(GeoPos(lat, lon));
        mark->rotate(yaw);
    }
}
