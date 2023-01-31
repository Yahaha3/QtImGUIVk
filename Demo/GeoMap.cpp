
#include "common/Image.h"
#include "GeoMap.h"

using namespace clz;

int long2tilex(double lon, int z) {
    return (int)(floor((lon + 180.0) / 360.0 * (1 << z)));
}

int lat2tiley(double lat, int z) {
    double latrad = lat * PI/180.0;
    return (int)(floor((1.0 - asinh(tan(latrad)) / PI) / 2.0 * (1 << z)));
}

double tilex2long(int x, int z) {
    return x / (double)(1 << z) * 360.0 - 180;
}

double tiley2lat(int y, int z) {
    double n = PI - 2.0 * PI * y / (double)(1 << z);
    return 180.0 / PI * atan(0.5 * (exp(n) - exp(-n)));
}

GeoMap::GeoMap()
{
    m_request_thread = std::make_shared<MapRequestThread>(this);
//    m_request_thread->start(1);
}

const QVector<QPair<TilePos, std::shared_ptr<Tile> > > &GeoMap::get_region(ImPlotRect view, ImVec2 pixels)
{
    double min_x = std::clamp(view.X.Min, 0.0, 1.0);
    double min_y = std::clamp(view.Y.Min, 0.0, 1.0);
    double size_x = std::clamp(view.X.Size(), 0.0, 1.0);
    double size_y = std::clamp(view.Y.Size(), 0.0, 1.0);

    double pix_occupied_x = (pixels.x / view.X.Size()) * size_x;
    double pix_occupied_y = (pixels.y / view.Y.Size()) * size_y;
    // 计算瓦片在当前视口中实际占用的size,相当于规范化操作
    double units_per_tile_x = view.X.Size() * (TILE_SIZE / pix_occupied_x);
    double units_per_tile_y = view.Y.Size() * (TILE_SIZE / pix_occupied_y);

    int z    = 0;
    double r = 1.0 / pow(2,z);
    // 通过将当前感应尺和实际size对比获得实际缩放level
    while (r > units_per_tile_x && r > units_per_tile_y && z < MAX_ZOOM)
        r = 1.0 / pow(2,++z);

    m_region.clear();
    if(!append_region(z, min_x, min_y, size_x, size_y) && z > 0){
        append_region(--z, min_x, min_y, size_x, size_y);
    }
    return m_region;
}

void GeoMap::set_tile_origin_data(const TilePos &pos, QByteArray data)
{
    auto tile = get_tile(pos);
    tile->image = std::make_shared<TileImage>();
    tile->image->set_origin_data(data.toStdString());
}

bool GeoMap::append_region(int z, double min_x, double min_y, double size_x, double size_y)
{
    int k = pow(2, z);
    double r = 1.0 / k;
    int xa = min_x * k;
    int xb = xa + ceil(size_x / r) + 1;
    int ya = min_y * k;
    int yb = ya + ceil(size_y / r) + 1;
    xb = std::clamp(xb, 0, k);
    yb = std::clamp(yb, 0, k);
    bool covered = true;
    for(int x = xa; x < xb; x++){
        for(int y = ya; y < yb; y++){
            TilePos pos(x, y, z);
            std::shared_ptr<Tile> tile = request_tile(pos);
            m_region.push_back({pos, tile});
            if(tile == nullptr || tile->state != TileState::Loaded){
                covered = false;
            }
        }
    }
    return covered;
}

std::shared_ptr<Tile> GeoMap::request_tile(TilePos pos)
{
    std::lock_guard<std::mutex> lock(m_tile_mutex);
    if(m_tiles.contains(pos)){
        return get_tile(pos);
    } else {
        download_tile(pos);
    }
    return nullptr;

}

void GeoMap::download_tile(TilePos pos)
{
    m_request_thread->add_request(pos);
}

std::shared_ptr<Tile> GeoMap::get_tile(TilePos pos)
{
    if(!m_tiles.contains(pos)){
        m_tiles[pos] = std::make_shared<clz::Tile>();
    }
    if(m_tiles[pos]->state == Loaded){
        return m_tiles.value(pos);
    } else if(m_tiles[pos]->state == OnDisk){
        return load_tile(pos);
    }
    return m_tiles.value(pos);
}

std::shared_ptr<Tile> GeoMap::load_tile(TilePos pos)
{
    if(m_tiles[pos]->image->loadFromOrigin()){
        m_tiles[pos]->state = Loaded;
        return m_tiles[pos];
    }
    m_tiles.remove(pos);
    return nullptr;
}
