#include "TileManager.h"
#include "common/Image.h"
#include "GeoMap.h"
#include "MapThread/MapRequestThread.h"
#include "GeoFunction/LayerBackend/GeoBackend.h"

clz::TileManager::TileManager(clz::GeoMap *map): m_map(map)
{

}

void clz::TileManager::set_tile_origin_data(const clz::TilePos &pos, QByteArray data)
{
    auto tile = get_tile(pos);
    if(tile->state == Unavailable && !tile->image){
        tile->image = std::make_shared<TileImage>();
        tile->image->set_origin_data(data.toStdString());
        tile->state = Ready;
    }
}

ImTextureID clz::TileManager::get_tile_texture_id(const clz::TilePos &pos, bool &ok)
{
    ok = false;
    auto tile = get_tile(pos);
    if(tile->image){
        auto ID = tile->image->get_image_texture(ok);
        if(ok){ return ID; }
    }
    return nullptr;
}

std::shared_ptr<clz::Tile> clz::TileManager::request_tile(clz::TilePos pos)
{
    if(contains_tile(pos)){
        return get_tile(pos);
    } else {
        download_tile(pos);
    }
    return nullptr;
}

void clz::TileManager::download_tile(clz::TilePos pos)
{
    m_map->get_request_thread()->add_request(pos);
}

std::shared_ptr<clz::Tile> clz::TileManager::get_tile(clz::TilePos pos)
{
    if(!contains_tile(pos) || !m_tiles[pos.z][pos]){
        m_tiles[pos.z][pos] = std::make_shared<clz::Tile>();
    }
    if(m_tiles[pos.z][pos]->state == Loaded){
        return m_tiles[pos.z][pos];
    } else if(m_tiles[pos.z][pos]->state == Ready){
        return load_tile(pos);
    }
    return m_tiles[pos.z][pos];
}

std::shared_ptr<clz::Tile> clz::TileManager::load_tile(clz::TilePos pos)
{
    if(m_tiles[pos.z][pos]->image->loadFromOrigin()){
        m_tiles[pos.z][pos]->state = Loaded;
        // 在完成加载后需要清除上下关联的瓦片
        clear_relative_tile(pos);
        return m_tiles[pos.z][pos];
    }
    return nullptr;
}

std::shared_ptr<clz::Tile> clz::TileManager::get_parent_tile(clz::TilePos &pos, bool &ok)
{
    bool result = true;
    while (!ok && result) {
        pos = pos.parent(pos.z - 1, result);
        if(contains_tile(pos)){
            ok = true;
            break;
        }
    }
    return m_tiles[pos.z][pos];
}

bool clz::TileManager::contains_tile(clz::TilePos pos)
{
    bool level_contains, tile_contains;
    level_contains = m_tiles.contains(pos.z);
    if(level_contains){
        tile_contains = m_tiles[pos.z].keys().contains(pos);
    }
    return level_contains && tile_contains;
}

void clz::TileManager::clear_relative_tile(clz::TilePos pos)
{
    // 1, 移除被当前瓦片覆盖的所有瓦片,即当前瓦片之下的瓦片
    auto backend = m_map->geo_backend();

    if(!backend) return;
    {
        const int from = pos.z + 1;
        const int to = backend->max_level();
        for(int zz = from; zz <= to; ++zz){
            auto tiles = existing_tiles(zz);
            for(auto target: tiles){
                if(pos.contains(target)){
                    remove_tile(target);
                }
            }
        }
    }
    // 2, 若在当前瓦片之上,则要首先计算当前瓦片之上的瓦片是否已被全部覆盖,若是则可以移除
    {
        const int from = backend->min_level();
        const int to = pos.z - 1;
        for(int zz = from; zz <= to; ++zz){
            auto tiles = existing_tiles(zz);
            for(auto target: tiles){
                if(target.contains(pos))
                {
                    const int zoom_delta = m_map->current_zoom() - target.z + 1;
                    const int ncount = static_cast<int>(pow(2, zoom_delta));
                    int count = ncount;
                    auto current_tiles = existing_tiles(m_map->current_zoom());
                    for(auto tile: current_tiles){
                        if(target.contains(tile)) {count--;}
                        if(count == 0) break;
                    }
                    if(count == 0){remove_tile(target);}
                }
            }
        }
    }
}

QList<clz::TilePos> clz::TileManager::existing_tiles(int zoom) const
{
    if(!m_tiles.contains(zoom)) return QList<TilePos>();
    return m_tiles[zoom].keys();
}

void clz::TileManager::remove_out_of_view(QList<clz::TilePos> exist, int zoom)
{
    auto tiles = existing_tiles(zoom);
    for(auto tile: tiles){
        if(!exist.contains(tile)){
            remove_tile(tile);
        }
    }
}

void clz::TileManager::remove_tile(clz::TilePos pos)
{
    if(contains_tile(pos)){
        auto tile = m_tiles[pos.z].take(pos);
        tile.reset();
    }
}
