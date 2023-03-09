#ifndef CFLXSUPPORT_H
#define CFLXSUPPORT_H

#include "GeoFunction/GeoMapGlobal.h"

namespace aos {
class Actor;
class MissionPointEx;
}
namespace clz {
class GeoMap;
class CflxSupport
{
public:
    CflxSupport();

    void map_init(GeoMap* map);

    ClzResult init_upload_mission_phases(aos::Actor* actor);
    ClzResult init_download_mission_phases(aos::Actor* actor);

private:
    /**
      * @b 从actor获取生成missionpoint需要的数据
      * @p points: 图形描述点
      * @p mark_point: 图形控制点
      * @p indices: 全局索引
      * @p vertices_icon: 控制点可用图标
      * @p mpxs: 数据对象
    */
    void generate_mission_point_before(QList<QVector<clz::GeoPos>>& list_points
                                      , QVector<clz::GeoPos>& mark_points
                                      , QVector<int>& indices
                                      , QList<QList<QString>>& vertices_icon
                                      , QList<aos::MissionPointEx*> mpxs);

    QVector<clz::GeoPos> get_estimated_route_with_mission_points(QList<aos::MissionPointEx*> mpxs);
    QList<QVector<clz::GeoPos>> get_estimated_routes_with_mission_points(QList<aos::MissionPointEx*> mpxs);
private:
    GeoMap* m_map;
};

}

#endif // CFLXSUPPORT_H
