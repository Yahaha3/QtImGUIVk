#include "CommonSupport.h"
#include "aos_kernel_gis_global.h"
#include <QQueue>

clz::CommonSupport::CommonSupport()
{

}

aos::PointPack clz::CommonSupport::geopos_2_pointpack(const clz::GeoPos &point)
{
    aos::PointPack ppk;
    ppk.append(point.longitude());
    ppk.append(point.latitude());
    if(!point.is_2d()){
        ppk.append(point.altitude());
    }
    return ppk;
}

clz::GeoPos clz::CommonSupport::pointpack_2_geopos(const aos::PointPack &ppk)
{
    clz::GeoPos point;
    if(ppk.size() >= 2){
        point.setLon(ppk[0]);
        point.setLat(ppk[1]);
    }
    if(ppk.size() >= 3){
        point.setAlt(ppk[2]);
    }
    return point;
}

QVector<clz::GeoPos> clz::CommonSupport::seqpack_to_geo_points(const aos::SeqPack &sp)
{
    QVector<clz::GeoPos> points;
    for(auto point: sp){
        if(point.size() >= 2){
            points.append(clz::GeoPos{point[1], point[0]});
        }
    }
    return points;
}

QList<QVector<clz::GeoPos> > clz::CommonSupport::map_queue_to_list_geo_points(const QQueue<arma::vec3> &main_traj, const QMap<QPair<QString, int>, QQueue<arma::vec3> > map_queue)
{
    QList<QVector<clz::GeoPos>> list_points;
    QVector<clz::GeoPos> points;

    auto process_mq = [&points, &list_points](const QQueue<arma::vec3>& mq){
        points.clear();
        for(auto p: mq){
            points.append(clz::GeoPos{p[1], p[0]});
        }
        list_points.append(points);
    };

    process_mq(main_traj);
    for(auto key: map_queue.keys()){
        auto mq = map_queue[key];
        process_mq(mq);
    }
    return list_points;
}

aos::PhaseNaturalType clz::CommonSupport::get_geojson_type(const QJsonObject &jo)
{
    auto result = aos::PhaseNaturalType::PhaseNaturalPoint;
    if(!jo.contains(aos::geojson_features)) return result;
    auto features = jo[aos::geojson_features].toArray();
    if(features.size() < 1) return result;
    auto feature = features.first().toObject();
    auto geometry = feature[aos::geojson_geometry].toObject();
    auto type = geometry[aos::geojson_type].toString();
    return GET_STRUCT_INSTANCE(aos::phase_naturalname_to_type)[type];
}

bool clz::CommonSupport::check_geojson_type(const QJsonObject &jo, aos::PhaseNaturalType type)
{
    auto geojson_type = get_geojson_type(jo);
    return geojson_type == type;
}
