#ifndef MAPREQUESTTHREAD_H
#define MAPREQUESTTHREAD_H
#include <iostream>
#include <thread>
#include <mutex>
#include <QObject>
#include <queue>
#include <GeoFunction/GeoMapGlobal.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class GeoBackend;
namespace clz {
class GeoMap;

class MapRequestThread : public QObject
{
    Q_OBJECT
public:
    MapRequestThread(GeoMap* map, QObject *parent = nullptr);
    ~MapRequestThread() = default;

    void start(int num);
    void stop();
    void add_request(TilePos tilepos);
    QByteArray get_test_data() const;

private slots:
    void slot_network_request_reply(QNetworkReply* reply);

private:
    std::vector<std::thread> m_works;
    std::mutex m_queue_mutex;
    std::queue<TilePos> m_queue;
    bool m_stop = false;

    std::atomic<int> m_working;
    std::atomic<int> m_downloads;
    std::atomic<int> m_fails;

    std::shared_ptr<QNetworkAccessManager> m_net_manager;

    std::shared_ptr<GeoMap> m_map;

    QByteArray m_test_data;
};

}

#endif // MAPREQUESTTHREAD_H
