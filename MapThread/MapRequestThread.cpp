#include "MapRequestThread.h"
#include "GeoFunction/LayerBackend/GeoBackendOSM.h"
#include "Map/GeoMap.h"
#include "Map/TileManager.h"
#include <QCoreApplication>
#include <QFile>

//#ifdef WIN32
//#include <Windows.h>
//#include <MMSystem.h>
////#pragma comment(lib, "winmm.lib")
//#else
//#include <sys/time.h>
//#endif


//void sleep(unsigned long dwMilliseconds)
//{
//#ifdef WIN32
//    timeBeginPeriod(1);
//    ::Sleep(dwMilliseconds);
//    timeEndPeriod(1);
//#else
//    std::this_thread::sleep_for(std::chrono::milliseconds(dwMilliseconds));
//#endif
//}

//#define USE_NETWORKREQUEST 1

clz::MapRequestThread::MapRequestThread(GeoMap* map, QObject *parent) : QObject(parent)
{
    m_map.reset(map);

#ifdef USE_NETWORKREQUEST
    m_net_manager = std::make_shared<QNetworkAccessManager>();
    connect(m_net_manager.get(), &QNetworkAccessManager::finished, this, &MapRequestThread::slot_network_request_reply);
#else
    auto path = QCoreApplication::applicationDirPath() + "/image0.png";
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        m_test_data = file.readAll();
    }
//    start(1);
#endif
}

void clz::MapRequestThread::start(int num)
{
    for(int thrd = 1; thrd < num + 1; ++thrd){
        m_works.emplace_back([this]{
            while (true) {
//              if(m_stop) return;
                TilePos one;
                std::unique_lock<std::mutex> lock(m_queue_mutex);
                if(m_queue.empty()) continue;
                one = std::move(m_queue.front());
                m_queue.pop();
                if(m_map){
                    m_map->get_tile_manager()->set_tile_origin_data(one, m_test_data);
                }
                m_working++;
//                bool success = false;
//                sleep(1);
            }
        });
    }
}

void clz::MapRequestThread::stop()
{

}

void clz::MapRequestThread::add_request(clz::TilePos tilepos)
{

#ifdef USE_NETWORKREQUEST
    QUrl url(m_map->geo_backend()->build(tilepos));
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Windows; U; MSIE "
                         "6.0; Windows NT 5.1; SV1; .NET "
                         "CLR 2.0.50727)");
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    // 此处耗时明显
    QNetworkReply* reply = m_net_manager->get(request);
    reply->setProperty("TILE_POS", QVariant::fromValue(tilepos));
#else
//        std::unique_lock<std::mutex> lock(m_queue_mutex);
//        m_queue.emplace(tilepos);
    if(m_map){
        m_map->get_tile_manager()->set_tile_origin_data(tilepos, m_test_data);
    }
#endif
}

QByteArray clz::MapRequestThread::get_test_data() const
{
    return m_test_data;
}

void clz::MapRequestThread::slot_network_request_reply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }
    const auto info = reply->readAll();
    const auto tilePos = reply->property("TILE_POS").value<TilePos>();
    if(m_map){
        m_map->get_tile_manager()->set_tile_origin_data(tilePos, info);
    }
    reply->abort();
    reply->close();
    reply->deleteLater();
}
