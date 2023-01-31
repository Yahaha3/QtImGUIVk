#include "MapRequestThread.h"
#include "GeoFunction/LayerBackend/GeoBackendOSM.h"
#include "Demo/GeoMap.h"

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

#define USE_NETWORKREQUEST 1

clz::MapRequestThread::MapRequestThread(GeoMap* map, QObject *parent) : QObject(parent)
{
    m_backend = std::make_shared<GeoBackendOSM>();
    m_map.reset(map);
#ifdef USE_NETWORKREQUEST
    m_net_manager = std::make_shared<QNetworkAccessManager>();
    connect(m_net_manager.get(), &QNetworkAccessManager::finished, this, &MapRequestThread::slot_network_request_reply);
#else
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
//    std::unique_lock<std::mutex> lock(m_queue_mutex);
//    m_queue.emplace(tilepos);

#ifdef USE_NETWORKREQUEST
    QUrl url(m_backend->build(tilepos));
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Windows; U; MSIE "
                         "6.0; Windows NT 5.1; SV1; .NET "
                         "CLR 2.0.50727)");
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    QNetworkReply* reply = m_net_manager->get(request);
    reply->setProperty("TILE_POS", QVariant::fromValue(tilepos));
#else
#endif
}

void clz::MapRequestThread::slot_network_request_reply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }
    const auto info = reply->readAll();
    const auto tilePos = reply->property("TILE_POS").value<TilePos>();
    if(m_map){
        m_map->set_tile_origin_data(tilePos, info);
    }
}
