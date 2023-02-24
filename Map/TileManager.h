#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <QMap>
#include <QVector>
#include <iostream>
#include <GeoFunction/GeoMapGlobal.h>

namespace clz {

class GeoMap;
class TileManager
{
public:
    TileManager(GeoMap* map);

    // 将请求到的数据更新到tiles中
    void set_tile_origin_data(const TilePos& pos, QByteArray data);
    // 返回一个vulkan纹理
    ImTextureID get_tile_texture_id(const TilePos& pos, bool& ok);

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

private:

    // 全部瓦片, 考虑释放问题
    QMap<int, QMap<TilePos, std::shared_ptr<Tile>>> m_tiles;

    GeoMap* m_map;
};

}

#endif // TILEMANAGER_H
