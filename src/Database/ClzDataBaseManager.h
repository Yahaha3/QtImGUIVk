#ifndef CLZDATABASEMANAGER_H
#define CLZDATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QMap>

#include <QObject>
namespace clz {
class ClzDataBaseManager
{
public:
    ClzDataBaseManager();
    ~ClzDataBaseManager();

    void init();
    void close();

    bool map_tile(const QString& brand, const QString& style, int x, int y, int z,
                  QByteArray& data, QString& update_time, const QString& language);
    bool cache_map_tile(const QString &brand, const QString &style, int x, int y, int z,
                        const QString &data, const QString &update_time, const QString &language);

    QSqlDatabase* get_database();
private:
    std::shared_ptr<QSqlDatabase> m_database = nullptr;;
    QString m_connection_id;
    bool m_inited = false;

    QMap<Qt::HANDLE, QSqlDatabase*> m_thread_databases;
};
}

#endif // CLZDATABASEMANAGER_H
