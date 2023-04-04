#ifndef MAPREQUESTTHREAD_H
#define MAPREQUESTTHREAD_H
#include <iostream>
#include <thread>
#include <QThread>
#include <queue>
#include <GeoFunction/GeoMapGlobal.h>
#include <Database/ClzDataBaseManager.h>
#include <QTimer>
#include <QSet>
#include <QMutex>

class GeoBackend;
namespace clz {
class GeoMap;

enum RequestDBResultEnum {
    RequestError,
    RawImageNull,
    TileTimeout,
    Offline,
};

struct RequestDBResult {
    QByteArray raw;
    QSet<clz::RequestDBResultEnum> result;
    clz::TilePos origin_tile;
    bool has_result(RequestDBResultEnum res) const{
        return result.contains(res);
    }
};

class MapRequestThread : public QThread
{
    Q_OBJECT
public:
    MapRequestThread(GeoMap* map, QObject *parent = nullptr);
    ~MapRequestThread() = default;

//    void start(int num);
    void run() override;
    void stop();
    void add_request(TilePos tilepos);
    QByteArray get_test_data() const;

signals:
    void sig_tile_info_response(const RequestDBResult& result);
private:
    void reload_request();
    void reload_result();

private slots:
    void slot_reqeuest_timeout();

private:
    QMutex m_request_mutex;
    QList<TilePos> m_queue;
    QList<TilePos> m_store_queue;
    QSet<QString> m_store_queue_keys;

    QList<RequestDBResult> m_store_result;
    QList<RequestDBResult> m_result;


    bool m_stop = false;
    bool m_reload = false;

    std::atomic<int> m_working;
    std::atomic<int> m_downloads;
    std::atomic<int> m_fails;

    std::shared_ptr<ClzDataBaseManager> m_cdbm;

    GeoMap* m_map;
    std::shared_ptr<QTimer> m_timer;
    QString m_brand;
    QString m_style;
    QString m_language;

    QByteArray m_test_data;
};

}

#endif // MAPREQUESTTHREAD_H
