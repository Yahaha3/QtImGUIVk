#include "ClzDataBaseManager.h"
#include <QUuid>
#include <iostream>
#include "clz_database_common.h"
#include <QVariant>
#include <QThread>

clz::ClzDataBaseManager::ClzDataBaseManager()
{
//    m_connection_id = QUuid::createUuid().toString();
    m_inited = false;
}

clz::ClzDataBaseManager::~ClzDataBaseManager()
{

}

void clz::ClzDataBaseManager::init()
{
    QSqlDatabase* db = get_database();

    // open database
    if(!db->open()){
        std::cout << "database open error!" << std::endl;
    }

    QSqlQuery query(*db);
    query.exec(clz::check_db_exist);
    if(!query.next()){
        // 当前表不存在
        query.exec(clz::create_map_cache_db);
    }
    db->close();
    m_inited = true;
}

void clz::ClzDataBaseManager::close()
{
    m_database->close();
}

bool clz::ClzDataBaseManager::map_tile(const QString &brand, const QString &style, int x, int y, int z, QByteArray &data, QString &update_time, const QString &language)
{
    if(!m_inited) return false;
    auto db = get_database();
    bool res = false;
    if(!db->isOpen()){
        res = db->open();
    }
//    if(!m_database || !m_database->isOpen()) return false;
    QString sql = GET_QSTRING_FROM_CONST_CHAR_PTR(clz::query_map_cache_db)
            .arg(brand).arg(style)
            .arg(QString::number(x))
            .arg(QString::number(y))
            .arg(QString::number(z));
//            .arg(language);
//    qDebug() << sql;
    QSqlQuery query(*db);
    res = query.exec(sql);
//    std::cout << "query one database info!" << std::endl;
    if(query.next()){
        data = query.value(0).toString().toUtf8();
        update_time = query.value(1).toString().toUtf8();
        return true;
    }
    db->close();
    return false;
}

bool clz::ClzDataBaseManager::cache_map_tile(const QString &brand, const QString &style, int x, int y, int z, const QString &data, const QString &update_time, const QString &language)
{
    if(!m_inited) return false;
    if(!m_database || !m_database->isOpen()) return false;
    QByteArray data_tmp;
    QString time_tmp;
    map_tile(brand, style, x, y, z, data_tmp, time_tmp, language);
    QString sql;
    if(time_tmp.isEmpty()){
        sql = QString(clz::insert_map_cache_db)
                .arg(brand).arg(style)
                .arg(QString::number(x))
                .arg(QString::number(y))
                .arg(QString::number(z))
                .arg(QString(data))
                .arg(update_time)
                .arg(language);
    } else {
        sql = QString(clz::update_map_cache_db)
                .arg(QString(data)).arg(update_time)
                .arg(brand).arg(style)
                .arg(QString::number(x))
                .arg(QString::number(y))
                .arg(QString::number(z))
                .arg(language);
    }
    QSqlQuery query(*m_database.get());
    return query.exec(sql);
}

QSqlDatabase *clz::ClzDataBaseManager::get_database()
{
    auto thread = QThread::currentThreadId();
    if(!m_thread_databases.contains(thread)){
        auto db = new QSqlDatabase();
        *db = QSqlDatabase::addDatabase("QSQLITE", m_connection_id);
        db->setDatabaseName("C:/Users/lan19/PhugiaOS/common/os-configpad-data.db3");
        db->setUserName("");
        db->setPassword("");
        m_thread_databases.insert(thread, db);
    }
    return m_thread_databases.value(thread);
}
