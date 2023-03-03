#ifndef CLZMAPSUPPORT_H
#define CLZMAPSUPPORT_H

#include <QObject>
#include <QJsonObject>
namespace clz {
class GeoMap;
class ClzMapSupport : public QObject
{
    Q_OBJECT
public:
    ClzMapSupport(QObject* parent = 0);

    void map_init(const QString& process_name, GeoMap* map);

private slots:
    void slot_map_support_mark_insert(const QJsonObject& jo);
    void slot_map_support_mark_update(const QJsonObject& jo);

private:
    QString m_process_name;
    GeoMap* m_map;
};

}

#endif // CLZMAPSUPPORT_H
