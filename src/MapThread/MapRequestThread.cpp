#include "MapRequestThread.h"
#include "GeoFunction/LayerBackend/GeoBackendOSM.h"
#include "Map/GeoMap.h"
#include "Map/TileManager.h"
#include <QCoreApplication>
#include <QFile>
#include <QDateTime>

#define OVERTIME 7776000 * 3

clz::MapRequestThread::MapRequestThread(GeoMap* map, QObject *parent) : QThread(parent)
{
    m_map = map;
    m_cdbm = std::make_shared<clz::ClzDataBaseManager>();
    m_cdbm->init();

    m_timer = std::make_shared<QTimer>();
    connect(m_timer.get(), &QTimer::timeout, this, &clz::MapRequestThread::slot_reqeuest_timeout);
    m_timer->start(10);

    m_brand = "google";
    m_style = "mix";

    m_working = 0;
}

void clz::MapRequestThread::run()
{
    while (!m_stop) {
        if(m_reload) continue;
        TilePos one;
        QMutexLocker lock(&m_request_mutex);
        if(m_queue.empty()) continue;
        one = std::move(m_queue.front());
        m_queue.takeFirst();
        lock.unlock();
        {
//            std::cout << "query tile count in thread: " << m_working._My_val << std::endl;
            QString update_time;
            RequestDBResult result;
            result.origin_tile = one;
            bool ok = m_cdbm->map_tile(m_brand, m_style, one.x, one.y, one.z, result.raw, update_time, m_language);
            auto time = QDateTime::currentMSecsSinceEpoch();
            if(!ok) result.result.insert(RequestDBResultEnum::RequestError);
            if(result.raw.isEmpty()) result.result.insert(RequestDBResultEnum::RawImageNull);
            if(time - update_time.toDouble() > OVERTIME) result.result.insert(RequestDBResultEnum::TileTimeout);
            m_store_result.append(result);
//            std::cout << "still items in queue count: " << m_queue.count() << std::endl;
        }
        m_working++;
        msleep(1);
    }
}

void clz::MapRequestThread::stop()
{
    m_stop = true;
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
    if(!m_store_queue_keys.contains(tilepos.tozxy()))
    {
        m_store_queue_keys.insert(tilepos.tozxy());
//        std::cout << "add request: "  << tilepos.tozxy().toStdString() << std::endl;
        m_store_queue.append(tilepos);
    }
#endif
}

QByteArray clz::MapRequestThread::get_test_data() const
{
    return m_test_data;
}

void clz::MapRequestThread::reload_request()
{
    // 重新装填请求以及结果
    m_reload = true;
//    std::cout << "reload tiles"  << std::endl;
    {
        // 一种刷新方式吧,每次都只刷新当前装填的请求,若在当前装填间隔内没有请求到的则直接抛弃
        m_queue.clear();
        for(int si = m_store_queue.size(), i = si - 1; i > -1; i--){
            if(!m_queue.contains(m_store_queue[i])){
                m_queue.append(m_store_queue[i]);
            }
        }
        m_store_queue.clear();
        m_store_queue_keys.clear();
    }
    m_reload = false;
}

void clz::MapRequestThread::reload_result()
{
    m_reload = true;
    {
        for(int i = 0, si = m_store_result.size(); i < si; i++){
            m_result.append(m_store_result[i]);
        }
        m_store_result.clear();
    }
    m_reload = false;
}

void clz::MapRequestThread::slot_reqeuest_timeout()
{
    static int count = 0;
    if(m_reload) return;
    for(auto result: m_result){
        if(m_map){
            if(!result.has_result(RequestDBResultEnum::RawImageNull)){
                auto array = QByteArray::fromHex(result.raw);
                m_map->get_tile_manager()->set_tile_origin_data(result.origin_tile, array);
            }
            if(result.has_result(RequestDBResultEnum::TileTimeout) ||
               result.has_result(RequestDBResultEnum::RawImageNull)){
                // 超时需要请求网络数据
            }
        }
    }
    m_result.clear();

    if(count % 27 == 0){
        reload_result();
    }

    if(count % 270 == 0){
        reload_request();
        count = 0;
    }
    count++;
}
