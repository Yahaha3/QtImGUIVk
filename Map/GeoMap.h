#ifndef GEOMAP_H
#define GEOMAP_H

#include <QObject>
#include <QVector>
#include "implot.h"
//#include "imgui.h"
#include <mutex>
#include <QMap>
#include "MapThread/MapRequestThread.h"

class Projection;
namespace clz {

class MapItem;
class GraphicsElement;

#define TILE_SIZE    256

class GeoMap
{
public:
    GeoMap();

    // 计算可视范围
    const QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>>& get_region(ImPlotRect view, ImVec2 pixels);

    // 将请求到的数据更新到tiles中
    void set_tile_origin_data(const TilePos& pos, QByteArray data);

    // 返回一个vulkan纹理
    ImTextureID get_tile_texture_id(const TilePos& pos, bool& ok);

    // 获取当前地图后置
    std::shared_ptr<GeoBackend> geo_backend() const;

    // 设置与返回当前地图使用的投影
    void set_map_projection(Projection* projection);
    std::shared_ptr<Projection> get_map_projection() const;

    clz::GeoPos implot2geopos(ImPlotPoint pt);
    ImPlotPoint geopos2implot(clz::GeoPos pt);
    void test();

    // 地图更新
    void update_tiles();
    void update_items();

private:
    // 根据可视
    bool append_region(int z, double min_x, double min_y, double size_x, double size_y);

    // 请求一张瓦片,如果在内存中则直接返回
    std::shared_ptr<Tile> request_tile(TilePos pos);
    // 从网络或数据库请求瓦片
    void download_tile(TilePos pos);
    // 获取一张瓦片,如果没有则需要创建
    std::shared_ptr<Tile> get_tile(TilePos pos);
    // 加载瓦片,将图片数据加载至vulkan对象中
    std::shared_ptr<Tile> load_tile(TilePos pos);
    // 找到当前瓦片的一个可用父瓦片
    std::shared_ptr<Tile> get_parent_tile(TilePos& pos, bool& ok);

    // 是否含有这张瓦片
    bool contains_tile(TilePos pos);

    // 完成加载后需要清除上下关联的瓦片
    void clear_relative_tile(TilePos pos);

    // 当前地图层级已缓存的全部瓦片
    QList<TilePos> existing_tiles(int zoom) const;

    // 移除当前层级当前视口以外的瓦片
    void remove_out_of_view(QList<TilePos> exist, int zoom);

    // 移除一张瓦片
    void remove_tile(TilePos pos);

    // 初始化用到的items
    void init_map_items();

private:
    // 当前可视区域瓦片集合
    QVector<QPair<TilePos, std::shared_ptr<Tile>>> m_region;
    // 全部瓦片, 考虑释放问题
    QMap<int, QMap<TilePos, std::shared_ptr<Tile>>> m_tiles;

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
};

}

#endif // GEOMAP_H
