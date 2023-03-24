#include "KernelSupport.h"
#include "socket_function_include.h"
#include "InstanceListItem.h"
#include "KernelInstanceData.h"
#include "DataCenter/Function/CpsgrsDataCenter.h"
#include "DataCenter/Function/LsCardsDataCenter.h"
#include "ClzWidgets/ClzImageButton.h"
#include "ClzWidgets/ClzCardWidget.h"
#include "ClzWidgets/ClzLayout.h"
#include "aos_kernel_variable_global.h"

KernelSupport::KernelSupport(QObject *parent) : QObject(parent)
{
    m_instance_data = std::make_shared<KernelInstanceData>();
    sk_instance().check_environmental();
}

void KernelSupport::init()
{
    auto instance = &sk_instance();
//    instance->check_environmental();
    {
        connect(instance, &eqnx_dh::InstanceContainer::sig_control_info,
                this, &KernelSupport::slot_kernel_workbench_info_recv);
        connect(instance, &eqnx_dh::InstanceContainer::sig_state_info,
                this, &KernelSupport::slot_kernel_state_info_recv);
        connect(instance, &eqnx_dh::InstanceContainer::sig_kernel_heart_beat,
                this, &KernelSupport::slot_kernel_heart_beat);
        connect(instance, &eqnx_dh::InstanceContainer::sig_kernel_mode_changed_with_jo,
                this, &KernelSupport::slot_kernel_mode_changed);
        connect(instance, &eqnx_dh::InstanceContainer::sig_instance_list,
                this, &KernelSupport::slot_kernel_instance_list);
    }
}

eqnx_dh::InstanceContainer &KernelSupport::sk_instance()
{
    return *eqnx_dh::InstanceContainer::handle();
}

void KernelSupport::build_data_center_connection(InstanceData &instance)
{
    connect(dynamic_cast<eqnx_dh::CpsgrsDataCenter*>(instance.cpsgrs_function),
            &eqnx_dh::CpsgrsDataCenter::sig_cpsgrs_info_recv,
            this, &KernelSupport::slot_cpsgrs_info_recv);
    connect(dynamic_cast<eqnx_dh::LsCardsDataCenter*>(instance.lscards_function),
            &eqnx_dh::LsCardsDataCenter::sig_lscards_info_recv,
            this, &KernelSupport::slot_lscards_info_recv);
}

void KernelSupport::ui_init(clz::ClzLayout *cpsgr_layout, clz::ClzLayout *card_layout, clz::ClzLayout *status_layout)
{
    m_cpsgr_layout = cpsgr_layout;
    m_card_layout = card_layout;
    m_status_layout = status_layout;
}

void KernelSupport::update_cpsgr_layout()
{
    if(!m_cpsgr_layout) return;
    bool ok = false;
    auto instance = m_instance_data->current_instance_data(ok);
    if(!ok) return;
    auto cpsgrs = dynamic_cast<eqnx_dh::CpsgrsDataCenter*>(instance.cpsgrs_function);
    for(auto list: cpsgrs->cpsgrs_order){
        for(auto sig: list){
            auto cpsgr_ = cpsgrs->cpsgrs.value(sig);
            auto cpsgr = cpsgr_[eqnx_dh::cpsgr].toObject();
            auto vdp = cpsgr[eqnx_dh::vdp].toObject();
            auto icon = vdp[eqnx_dh::cfg_icon].toString();
#ifdef LANGUAGE_EN
            auto name = vdp[eqnx_dh::cfg_name].toString();
#else
#endif
            if(!cpsgr[eqnx_dh::by_button].toBool(false)){
                // 可能需要填充右键菜单
            } else if(!m_cpsgr_layout->contain_widget(sig)){
                // 生成一个按钮并填充进入layout中
                auto ipath = clz::GeoFunctionUtility::cpsgr_icon_path(icon);
                auto btn = new clz::ClzImageButton(ipath, name); // 有一些参数还需要放进来,保证后续可以构建指令发送
                btn->set_object_id(sig); // 谨慎使用
                m_cpsgr_layout->add_widget(btn);
                connect(btn, &clz::ClzImageButton::sig_image_btn_clicked, this, &KernelSupport::slot_image_btn_clicked);
            }

        }
    }
}

void KernelSupport::update_card_layout()
{
    if(!m_card_layout) return;
    bool ok = false;
    auto instance = m_instance_data->current_instance_data(ok);
    if(!ok) return;
    for(auto card: instance.cards){
        auto items = card[eqnx_dh::items].toArray();
        auto title = card[eqnx_dh::title].toString();
        auto csig = card[eqnx_dh::sig].toString();
        for(auto item_: items){
            auto item = item_.toObject();
            auto def = item[eqnx_dh::def].toObject();
            auto access = static_cast<aos::VariableAccessComboOptions>(def[eqnx_dh::access].toInt());
//            if(access == aos::VariableAccessComboNone) continue;
            // 目前只考虑两级菜单
            QStringList contexts, uris;
            QJsonArray options;
            if(access == aos::VariableAccessComboLabel){
                auto sitems = item[eqnx_dh::items].toArray();
                for(auto sitem_: sitems){
                    auto sitem = sitem_.toObject();
//                    auto sig = sitem[eqnx_dh::sig].toString();
                    auto sdef = sitem[eqnx_dh::def].toObject();
                    auto saccess = static_cast<aos::VariableAccessComboOptions>(sdef[eqnx_dh::access].toInt());
                    if(saccess == aos::VariableAccessComboNone) continue;
#ifdef LANGUAGE_EN
                    auto context = sdef[eqnx_dh::name].toString();
#else
#endif
                    auto option = sdef[eqnx_dh::options].toObject();
                    auto uri = sdef[eqnx_dh::uri].toString();
                    contexts.append(context);
                    uris.append(uri);
                    options.append(option);
                }
                if(!m_card_layout->contain_widget(csig)){
                    auto card = new clz::ClzCardWidget();
                    card->set_object_id(csig);
                    card->set_widget_name(title);
                    card->init_card(contexts, uris, options);
                    connect(this, &KernelSupport::sig_kernel_state_data_update, card, &clz::ClzCardWidget::slot_card_data_update);
                    m_card_layout->add_widget(card);
                }
            }
        }
    }
}

void KernelSupport::update_status_layout()
{
    if(!m_status_layout) return;
    bool ok = false;
    auto instance = m_instance_data->current_instance_data(ok);
    if(!ok) return;
}

KernelInstanceData *KernelSupport::instance_data()
{
    return m_instance_data.get();
}

void KernelSupport::slot_kernel_heart_beat(const QJsonObject &jo)
{
    auto idsns = sk_instance().ks_container()->idsn_list();
    bool ok = false;
    for(auto idsn: idsns){
        InstanceData& instance = m_instance_data->instance_data(idsn, ok);
        if(sk_instance().ks_container()->kernel_sock_dto_or_null(idsn)
                && ok && !instance.init_request){
            instance.cpsgrs_function->send(idsn);
            instance.lscards_function->send(idsn);
            instance.init_request = true;
            std::cout << "request cpsgrs and lscards" << std::endl;
        }
    }
}

void KernelSupport::slot_kernel_workbench_info_recv(const QString &info)
{

}

void KernelSupport::slot_kernel_state_info_recv(const QString &info)
{
    auto jo = aos::AosKernelCommon::string_to_json_object(info);
    auto type = static_cast<aos::StatePlayerType>(jo[aos::PYLD_TYPENAME].toInt());
    if(type == aos::StatePlayerKernelState){
        auto pack_alias = jo[eqnx_dh::pack_alias].toString();
        auto states = jo[eqnx_dh::states].toObject();
        for(auto state: states.keys()){
            auto match = QString("%1/%2").arg(pack_alias).arg(state);
            auto value = states[state];
            emit sig_kernel_state_data_update(match, value);
        }
    }
}

void KernelSupport::slot_kernel_mode_changed(const QJsonObject &info)
{
    auto idsn = info[eqnx_dh::idsn].toString();
    bool ok = false;
    auto instance = m_instance_data->instance_data(idsn, ok);
    if(!ok) return;
    m_cpsgr_layout->delete_elements();
    instance.cpsgrs_function->send(idsn);
}
#include "DatabaseCflx/db_cflx_include.h"
#include "Entity/EntityPool.h"
#include "Entity.h"
void KernelSupport::slot_kernel_instance_list(const QJsonObject &jo)
{
    /*
      * 当前运行的实例,用于初始化左下角英雄列表,并从此处开始初始化右侧卡片
      * 并且需要请求一次当前可用的功能列表
    */
    auto instances = jo[aos::instree_instances].toArray();
    for(auto instance_: instances){
        aos::InstanceRootItem instance;
        instance.from_json(instance_.toObject());
        auto items = instance.items();
        for(auto it: items){
            auto idsn = it->idsn;
            if(m_instance_data->contains_instance(idsn)) continue;
            auto actor = ConfluxHelper::instance()->actor_or_null_with_idsn(idsn);
            if(!actor) continue;
            auto entity = aos::EntityPool::instance().get_entity_or_null(actor->entity_sig());
            InstanceData instance;
            instance.idsn = idsn;
            instance.entity_name = actor->display_name();
            instance.entity_image = entity->parameters_.image_;
            m_instance_data->append_instance_data(idsn, instance);
            build_data_center_connection(instance);
            m_instance_data->m_current_use_instance = idsn;
        }
    }
}

void KernelSupport::slot_cpsgrs_info_recv(const QString &idsn)
{
    if(idsn == m_instance_data->m_current_use_instance){
        update_cpsgr_layout();
    }
}

void KernelSupport::slot_lscards_info_recv(const QJsonArray &lscards, const QString &idsn)
{
    bool ok = false;
    InstanceData& instance = m_instance_data->instance_data(idsn, ok);
    if(!ok) return;
    // 此时还需要读取原数据,校验当前数据是哪里的cards,先默认是右侧卡片吧
    for(auto lscard_: lscards){
        auto lscard = lscard_.toObject();
        auto items = lscard[eqnx_dh::items].toArray();
        if(items.isEmpty()) continue;
        auto sig = lscard[eqnx_dh::sig].toString();
        if(!instance.cards.contains(sig)){
            instance.cards.insert(sig, lscard);
//            auto title = lscard[eqnx_dh::title].toString();
//            std::cout << "insert function card: " << title.toStdString() << std::endl;
        }
    }
    if(idsn == m_instance_data->m_current_use_instance){
        update_card_layout();
    }
}
#include "CmdCpSgrsOutput.h"
#include "DataCenter/Function/NormalCpsgrDataCenter.h"
void KernelSupport::slot_image_btn_clicked(const QString &id)
{
    // 只有cpsgr按钮绑在这里了,其他按钮单独响应
    bool ok = false;
    auto instance = m_instance_data->current_instance_data(ok);
    if(!ok) return;
    auto cpsgrs = dynamic_cast<eqnx_dh::CpsgrsDataCenter*>(instance.cpsgrs_function);
    auto cpsgr = dynamic_cast<eqnx_dh::NormalCpsgrDataCenter*>(instance.cpsgr_function);
    if(cpsgrs && cpsgrs->cpsgrs.contains(id)){
//        aos::CpSgrCallRebuild::build_cpsgrvars();
        cpsgr->cpsgrsig = id;
        cpsgr->send(instance.idsn);
    }
}
