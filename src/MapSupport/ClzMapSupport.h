#ifndef CLZMAPSUPPORT_H
#define CLZMAPSUPPORT_H

#include <QObject>
#include <QJsonObject>

// 地图支持,用于地图业务实现,后续会剥离出去
namespace clz {
class GeoMap;
class ClzMapSupport : public QObject
{
    Q_OBJECT
public:
    ClzMapSupport(QObject* parent = 0);

    void map_init(const QString& process_name, GeoMap* map);

    void clear_layer(const QString& layer_name);

private slots:
    void slot_map_support_mark_insert(const QJsonObject& jo);
    void slot_map_support_mark_update(const QJsonObject& jo);

private:
    QString m_process_name;
    GeoMap* m_map;
};

}

#endif // CLZMAPSUPPORT_H
