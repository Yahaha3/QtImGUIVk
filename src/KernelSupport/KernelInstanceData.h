#ifndef KERNELINSTANCEDATA_H
#define KERNELINSTANCEDATA_H

#include <QObject>
#include <QMap>
#include <QHash>
#include "DataCenter/DataCenterBase.h"
// 界面所需的内核相关数据
struct InstanceData {
    // 内核唯一标识
    QString idsn;
    // 实体名称
    QString entity_name;
    // 用於展示的實體圖片
    QString entity_image;
    // 右側卡片
    QHash<QString, QJsonObject> cards;
    // 頂部狀態欄
    QHash<QString, QJsonObject> status;
    // 其他數據，如視頻推流等

    // 是否完成初始化请求
    bool init_request = false;

    // 内核函數調用
    // 1,当前可用模式列表,仅当前
    eqnx_dh::DataCenterBase* cpsgrs_function = nullptr;
    // 2,当前应当显示的卡片,可能返回多个位置应当显示的卡片
    eqnx_dh::DataCenterBase* lscards_function = nullptr;
    // 3,较为通用的cpsgr模式请求,无返回值需求
    eqnx_dh::DataCenterBase* cpsgr_function = nullptr;

    QString current_mode() const;
};


class KernelInstanceData
{
public:
    KernelInstanceData();

    bool contains_instance(const QString& idsn);
    InstanceData& current_instance_data(bool& ok);
    InstanceData& instance_data(const QString& idsn, bool& ok);
    void append_instance_data(const QString& idsn, InstanceData& instance);
    void remove_instance_data(const QString& idsn);
    void clear_instance_data();

private:
    friend class KernelSupport;
    QMap<QString, InstanceData> m_instances;
    InstanceData m_null_instance;
    QString m_current_use_instance;
};

#endif // KERNELINSTANCEDATA_H
