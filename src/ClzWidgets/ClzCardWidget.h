#ifndef CLZCARDWIDGET_H
#define CLZCARDWIDGET_H

#include <QObject>
#include <QJsonValue>
#include "ClzWidgets/ClzWidget.h"

// card 为左右,左右的形式显示数据

namespace clz {

class ClzCardWidget: public ClzWidget
{
    Q_OBJECT
public:
    ClzCardWidget(ClzWidget* parent = 0);

    bool init_card(const QStringList& context,
                   const QStringList& uri,
                   const QStringList& formats,
                   const QList<QJsonArray>& options);
    bool update_card(const QString& uri, QJsonValue value);

    void init() override;
    void paint() override;


public slots:
    void slot_card_data_update(const QString& uri, QJsonValue value);

private:
    // 左侧目录
    QStringList m_context;
    // 路径列表
    QStringList m_uris;
    // 路径与值的映射
    QHash<QString, QString> m_value_map;
    // 目录与路径的映射
    QHash<QString, QString> m_context_uri_map;
    // 目录与格式的映射
    QHash<QString, QString> m_context_format_map;
    // 目录与选项的映射
    QHash<QString, QJsonArray> m_context_option_map;

    const QString m_colon = ":";
};
}

#endif // CLZCARDWIDGET_H
