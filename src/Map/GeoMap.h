#ifndef GEOMAP_H
#define GEOMAP_H

#include <QObject>
#include "implot.h"
//#include "imgui.h"
#include <mutex>
#include "MapThread/MapRequestThread.h"
#include "TileManager.h"

class Projection;
namespace clz {

class MapItem;
class GraphicsElement;
class OperatorContainer;

#define TILE_SIZE    256

class GeoMap
{
public:
    GeoMap();

    // 计算可视范围
    const QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>>& get_region(ImPlotRect view, ImVec2 pixels);

    // 获取当前地图后置
    std::shared_ptr<GeoBackend> geo_backend() const;

    // 设置与返回当前地图使用的投影
    void set_map_projection(Projection* projection);
    std::shared_ptr<Projection> get_map_projection() const;

    // 瓦片请求
    std::shared_ptr<MapRequestThread> get_request_thread() const;
    std::shared_ptr<OperatorContainer> map_oc() const;
    // 当前缩放层级
    int current_zoom() const;

    // 通用功能
    clz::GeoPos implot2geopos(ImPlotPoint pt);
    ImPlotPoint geopos2implot(clz::GeoPos pt);
    // 测试
    void test();

    // 地图更新
    void map_timeout();

    //对象
    std::shared_ptr<TileManager> get_tile_manager() const;

    // 需要一定时间准备的对象初始化
    void sub_items_init();

private:
    // 根据可视
    bool append_region(int z, double min_x, double min_y, double size_x, double size_y);

    // 初始化用到的items
    void init_map_items();

    // 瓦片更新
    void tiles_update();
    // 组件更新
    void items_update();

private:
    // 当前可视区域瓦片集合
    QVector<QPair<TilePos, std::shared_ptr<Tile>>> m_region;
    // 瓦片管理
    std::shared_ptr<TileManager> m_tile_manager;
//    std::mutex m_tile_mutex;
    // 瓦片请求类
    std::shared_ptr<MapRequestThread> m_request_thread;

    // 地图后置
    std::shared_ptr<GeoBackend> m_backend;

    // 当前地图缩放层级
    int m_zoom;

    // 当前地图使用的投影
    std::shared_ptr<Projection> m_map_projection;

    // 地图用到的items
    QList<MapItem*> m_map_items;

    // 与地图绑定的根节点
    std::shared_ptr<GraphicsElement> m_root_item;

    // 地图呈现对象容器
    std::shared_ptr<OperatorContainer> m_op_container;
};

}

#endif // GEOMAP_H
