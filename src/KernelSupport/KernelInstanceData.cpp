#include "KernelInstanceData.h"
#include "DataCenter/Function/CpsgrsDataCenter.h"
#include "DataCenter/Function/NormalCpsgrDataCenter.h"
#include "DataCenter/Function/LsCardsDataCenter.h"

KernelInstanceData::KernelInstanceData()
{

}

bool KernelInstanceData::contains_instance(const QString &idsn)
{
    return m_instances.contains(idsn);
}

InstanceData& KernelInstanceData::current_instance_data(bool& ok)
{
    return instance_data(m_current_use_instance, ok);
}

InstanceData &KernelInstanceData::instance_data(const QString &idsn, bool& ok)
{
    ok = false;
    if(!contains_instance(idsn)){
        return m_null_instance;
    }
    ok = true;
    return m_instances[idsn];
}

void KernelInstanceData::append_instance_data(const QString &idsn, InstanceData& instance)
{
    if(contains_instance(idsn)) return;
    if(!instance.cpsgr_function){
        instance.cpsgr_function = new eqnx_dh::NormalCpsgrDataCenter();
    }
    if(!instance.cpsgrs_function){
        instance.cpsgrs_function = new eqnx_dh::CpsgrsDataCenter();
    }
    if(!instance.lscards_function){
        instance.lscards_function = new eqnx_dh::LsCardsDataCenter();
    }
    m_instances.insert(idsn, instance);
}

void KernelInstanceData::remove_instance_data(const QString &idsn)
{
    if(contains_instance(idsn)){
        m_instances.remove(idsn);
    }
}

void KernelInstanceData::clear_instance_data()
{
    m_instances.clear();
}

QString InstanceData::current_mode() const
{
    auto cpsgrs = dynamic_cast<eqnx_dh::CpsgrsDataCenter*>(cpsgrs_function);
    if(cpsgrs){
#ifdef LANGUAGE_EN
        return cpsgrs->kmode[eqnx_dh::name].toString();
#else
        return cpsgrs->kmode[eqnx_dh::name_i18n].toString();
#endif
    }
    return QString();
}
