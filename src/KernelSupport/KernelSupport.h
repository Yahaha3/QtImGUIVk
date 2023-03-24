#ifndef KERNELSUPPORT_H
#define KERNELSUPPORT_H

#include <QObject>
#include <QJsonValue>
namespace eqnx_dh {
class InstanceContainer;
}

namespace clz {
class ClzLayout;
}

class KernelInstanceData;
class InstanceData;
class KernelSupport : public QObject
{
    Q_OBJECT
public:
    explicit KernelSupport(QObject *parent = nullptr);
    void init();

    void ui_init(clz::ClzLayout* cpsgr_layout,
                 clz::ClzLayout* card_layout,
                 clz::ClzLayout* status_layout);
    void update_cpsgr_layout();
    void update_card_layout();
    void update_status_layout();

    KernelInstanceData* instance_data();

signals:
//    void sig_kernel_inited();
    void sig_kernel_state_data_update(const QString& uri, QJsonValue value);

private:
    eqnx_dh::InstanceContainer& sk_instance();
    void build_data_center_connection(InstanceData& instance);

private slots:
    void slot_kernel_heart_beat(const QJsonObject& jo);
    void slot_kernel_workbench_info_recv(const QString& jo);
    void slot_kernel_state_info_recv(const QString& jo);
    void slot_kernel_mode_changed(const QJsonObject& jo);
    void slot_kernel_instance_list(const QJsonObject& jo);

    void slot_cpsgrs_info_recv(const QString& idsn);
    void slot_lscards_info_recv(const QJsonArray& lscards, const QString& idsn);

    void slot_image_btn_clicked(const QString& id);

private:
    std::shared_ptr<KernelInstanceData> m_instance_data;

    clz::ClzLayout* m_cpsgr_layout;
    clz::ClzLayout* m_card_layout;
    clz::ClzLayout* m_status_layout;
};

#endif // KERNELSUPPORT_H
