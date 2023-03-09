#ifndef COMMONSUPPORT_H
#define COMMONSUPPORT_H
#include "GeoFunction/GeoMapGlobal.h"
#include "aos_kernel_geometry.h"
#include "aos_kernel_missions_global.h"
#include <armadillo>

namespace clz {

class CommonSupport
{
public:
    CommonSupport();

    static aos::PointPack geopos_2_pointpack(const clz::GeoPos& point);
    static clz::GeoPos pointpack_2_geopos(const aos::PointPack& ppk);
    static QVector<clz::GeoPos> seqpack_to_geo_points(const aos::SeqPack& sp);
    static QList<QVector<clz::GeoPos>> map_queue_to_list_geo_points(const QQueue<arma::vec3>& main_traj,
                                                                    const QMap<QPair<QString, int>, QQueue<arma::vec3>> map_queue);
    static aos::PhaseNaturalType get_geojson_type(const QJsonObject& jo);
    static bool check_geojson_type(const QJsonObject& jo, aos::PhaseNaturalType type);

};

}

#endif // COMMONSUPPORT_H
