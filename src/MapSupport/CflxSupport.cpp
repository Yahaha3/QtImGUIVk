#include "CflxSupport.h"
#include "DatabaseCflx/db_cflx_include.h"
#include "PathEstimator/PathEstimatorBase.h"
#include "MissionPointExDef.h"
#include "CommonSupport.h"
#include "eqnx_common_global.h"
#include "Operator/OperatorContainer.h"

clz::CflxSupport::CflxSupport()
{

}

void clz::CflxSupport::map_init(clz::GeoMap *map)
{
    m_map = map;
}

clz::ClzResult clz::CflxSupport::init_upload_mission_phases(aos::Actor* actor)
{
    ClzResult result = ClzNullptr;
    if(!actor){
        return result;
    }

    return result;
}

clz::ClzResult clz::CflxSupport::init_download_mission_phases(aos::Actor* actor)
{
    ClzResult result = ClzNullptr;
    if(!actor){
        return result;
    }
    auto asig = actor->actor_sig();
    auto actor_course = actor->current_actor_course();
    auto mpxs = actor_course->mpxs_upload();
    QList<QVector<clz::GeoPos>> list_points;
    QVector<clz::GeoPos> mark_points;
    QVector<int> indices;
    QList<QList<QString>> vertices_icon;
    generate_mission_point_before(list_points, mark_points, indices, vertices_icon, mpxs);
    if(list_points.size() > 0){
        auto name = QString("%1_%2_%3").arg(asig).arg(actor_course->actcrssig()).arg(eqnx::qgv::qgv_download);
        auto linestring = m_map->map_oc()->get_operator_linestring(name);
        linestring->generate_operator(list_points.first());
        result = ClzSuccess;
    } else {
        result = ClzEmptyArray;
    }
    return result;
}

void clz::CflxSupport::generate_mission_point_before(QList<QVector<clz::GeoPos> > &list_points, QVector<clz::GeoPos> &mark_points, QVector<int> &indices, QList<QList<QString> > &vertices_icon, QList<aos::MissionPointEx *> mpxs)
{
    QList<QString> features;
    QList<QString> icons;
    list_points = get_estimated_routes_with_mission_points(mpxs);
    for(auto mpx: mpxs)
    {
        if(mpx->is_virtual()) continue;
        auto lat = mpx->latitude();
        auto lon = mpx->longitude();
        auto height = mpx->height();
        if(lat == 0 || lon == 0) continue;
        clz::GeoPos position(lat, lon, height);
        bool ok;
        auto radius = mpx->radius(&ok);
        if(ok){
            position.setRadius(abs(radius));
        }

        auto feature = mpx->field_io_feature();
        features.append(GET_STRUCT_INSTANCE(aos::ppnt_io_field_features_name)[feature]);
        auto icon_alias = mpx->mpxdef()->vardef_pack()->config_icon();
        icons.append(icon_alias);
        // 增加一个type列表，用于显示同一点不同情况下的图标等状态
        mark_points.append(position);
        indices.append(mpx->global_index());
    }
    vertices_icon.append(features);
    vertices_icon.append(icons);
}

QVector<clz::GeoPos> clz::CflxSupport::get_estimated_route_with_mission_points(QList<aos::MissionPointEx *> mpxs)
{
    QMap<QString, aos::MissionPointJumpCounter> jump_counters;
    aos::SeqPack seqpack_trajectory;
    aos::PathEstimatorBase::estimate_trajectory(mpxs,
                                                jump_counters,
                                                seqpack_trajectory);
    return clz::CommonSupport::seqpack_to_geo_points(seqpack_trajectory);
}

QList<QVector<clz::GeoPos> > clz::CflxSupport::get_estimated_routes_with_mission_points(QList<aos::MissionPointEx *> mpxs)
{
    QQueue<arma::vec3> main_traj;
    QMap<QPair<QString, int>, QQueue<arma::vec3>> jump_trajs;
    aos::PathEstimatorBase::estimate_trajectories(mpxs, main_traj, jump_trajs);
    return clz::CommonSupport::map_queue_to_list_geo_points(main_traj, jump_trajs);
}
