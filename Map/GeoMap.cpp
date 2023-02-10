
#include "common/Image.h"
#include "GeoMap.h"
#include "GeoFunction/LayerBackend/GeoBackendGaode.h"
#include "Projection/ProjectionESG3857.h"
#include "GeoFunction/GeoFunctionUtility.h"
#include "map_include.h"
#include "Element/element_include.h"
#include <QList>

using namespace clz;

class RootElement : public GraphicsElement
{
public:
    explicit RootElement(GeoMap* map)
    {
        m_map.reset(map);
    }
    virtual ~RootElement() = default;

    GeoMap * get_map() const override final
    {
        return m_map.get();
    }
private:
    std::shared_ptr<GeoMap> m_map = nullptr;
};

GeoMap::GeoMap()
{
    m_request_thread = std::make_shared<MapRequestThread>(this);
    m_backend = std::make_shared<GeoBackendGaode>();
    m_root_item = std::make_shared<RootElement>(this);
    set_map_projection(new ProjectionESG3857());
    init_map_items();

    m_root_item->set_item_id(clz::root_item);
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

    m_zoom    = 0;
    double r = 1.0 / pow(2,m_zoom);
    // 通过将当前感应尺和实际size对比获得实际缩放level
    while (r > units_per_tile_x && r > units_per_tile_y && m_zoom < geo_backend()->max_level())
        r = 1.0 / pow(2,++m_zoom);

    m_region.clear();
    if(!append_region(m_zoom, min_x, min_y, size_x, size_y) && m_zoom > 0){
//        append_region(--z, min_x, min_y, size_x, size_y);
    }
//    qDebug() << "tile size: " << m_tiles.size();
    return m_region;
}

void GeoMap::set_tile_origin_data(const TilePos &pos, QByteArray data)
{
    auto tile = get_tile(pos);
    if(tile->state == Unavailable && !tile->image){
        tile->image = std::make_shared<TileImage>();
        tile->image->set_origin_data(data.toStdString());
        tile->state = Ready;
    }
//    load_tile(pos);
}

ImTextureID GeoMap::get_tile_texture_id(const TilePos &pos, bool &ok)
{
    ok = false;
    auto tile = get_tile(pos);
    if(tile->image){
        auto ID = tile->image->get_image_texture(ok);
        if(ok){ return ID; }
    }
    return nullptr;
}

std::shared_ptr<GeoBackend> GeoMap::geo_backend() const
{
    return m_backend;
}

void GeoMap::set_map_projection(Projection *projection)
{
    m_map_projection.reset(projection);
}

std::shared_ptr<Projection> GeoMap::get_map_projection() const
{
    return m_map_projection;
}

GeoPos GeoMap::implot2geopos(ImPlotPoint pt)
{
    auto scene_pos = clz::GeoFunctionUtility::implot2scene(pt);
    auto proj_pos = get_map_projection()->sceneToMap(QPointF(scene_pos.x, scene_pos.y));
    auto geo_pos = get_map_projection()->projToGeo(proj_pos);
    return geo_pos;
}

ImPlotPoint GeoMap::geopos2implot(GeoPos pt)
{
    auto proj_pos = get_map_projection()->geoToProj(pt);
    auto scene_pos = get_map_projection()->mapToScene(proj_pos);
    auto plot_pos = clz::GeoFunctionUtility::scene2implot(ImPlotPoint(scene_pos.x(), scene_pos.y()));
    return plot_pos;
}

void GeoMap::test()
{
    static clz::LinestringDrawElement* line = nullptr;
    if(!line){
        QVector<clz::GeoPos> vertices;
        vertices.append(clz::GeoPos(30, 60));
        vertices.append(clz::GeoPos(30, 120));
        line = new clz::LinestringDrawElement(vertices);
    }
    line->paint();
}

void GeoMap::update_tiles()
{
    static bool debug = false;
    if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_A))){
        debug = !debug;
    }
    auto size = ImPlot::GetPlotSize();
    auto limits = ImPlot::GetPlotLimits();
    auto region = get_region(limits, size);
    for(auto& pair: region){
        auto position = pair.first;
        auto tile     = pair.second;
        auto [bmin, bmax] = position.bounds();
        if(tile){
            auto col = debug ? ((position.x % 2 == 0 && position.y % 2 != 0) ||
                                (position.x % 2 != 0 && position.y %2 == 0)) ?
                       ImVec4(1,0,1,1) : ImVec4(1,1,0,1) : ImVec4(1,1,1,1);
            bool ok = false;
            auto ID = get_tile_texture_id(position, ok);
            if(ok){
                ImPlot::PlotImage("##Tiles", ID, bmin, bmax, {0,0}, {1,1}, col);
                if (debug)
                    ImPlot::PlotText(position.tozxy().toStdString().c_str(),(bmin.x+bmax.x)/2,(bmin.y+bmax.y)/2);
            }
        }
    }

}

void GeoMap::update_items()
{
    for(auto item: m_map_items){
        item->paint();
    }
}

bool GeoMap::append_region(int z, double min_x, double min_y, double size_x, double size_y)
{
    int k = pow(2, z);
    double r = 1.0 / k;
    int xa = min_x * k;
    // 计算当前层级总共有多少瓦片
    int xb = xa + ceil(size_x / r) + 1;
    int ya = min_y * k;
    int yb = ya + ceil(size_y / r) + 1;
    xb = std::clamp(xb, 0, k);
    yb = std::clamp(yb, 0, k);
    bool covered = true, ok = false;
    QList<TilePos> tmp_list, use_tile;
    for(int x = xa; x < xb; x++){
        for(int y = ya; y < yb; y++){
            TilePos pos(x, y, z);
            std::shared_ptr<Tile> tile = request_tile(pos);
            use_tile.append(pos);

            ok = true;
            if(tile == nullptr || tile->state != TileState::Loaded){
                ok = false;
                TilePos parent_pos;
                tile = get_parent_tile(pos, ok);
                covered = false;
                if(ok){
                    use_tile.append(pos);
                }
            }

            if(ok && !tmp_list.contains(pos)){
                tmp_list.append(pos);
            } else {ok = false;}
            if(ok){m_region.push_back({pos, tile});}
        }
    }
    remove_out_of_view(use_tile, z);
    return covered;
}

std::shared_ptr<Tile> GeoMap::request_tile(TilePos pos)
{
//    std::lock_guard<std::mutex> lock(m_tile_mutex);
    if(contains_tile(pos)){
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

std::shared_ptr<Tile> GeoMap::load_tile(TilePos pos)
{
    if(m_tiles[pos.z][pos]->image->loadFromOrigin()){
        m_tiles[pos.z][pos]->state = Loaded;
        // 在完成加载后需要清除上下关联的瓦片
        clear_relative_tile(pos);
        return m_tiles[pos.z][pos];
    }
    return nullptr;
}

std::shared_ptr<Tile> GeoMap::get_parent_tile(TilePos &pos, bool &ok)
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

bool GeoMap::contains_tile(TilePos pos)
{
    bool level_contains, tile_contains;
    level_contains = m_tiles.contains(pos.z);
    if(level_contains){
        tile_contains = m_tiles[pos.z].keys().contains(pos);
    }
    return level_contains && tile_contains;
}

void GeoMap::clear_relative_tile(TilePos pos)
{
    // 1, 移除被当前瓦片覆盖的所有瓦片,即当前瓦片之下的瓦片
    {
        const int from = pos.z + 1;
        const int to = m_backend->max_level();
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
        const int from = m_backend->min_level();
        const int to = pos.z - 1;
        for(int zz = from; zz <= to; ++zz){
            auto tiles = existing_tiles(zz);
            for(auto target: tiles){
                if(target.contains(pos))
                {
                    const int zoom_delta = m_zoom - target.z + 1;
                    const int ncount = static_cast<int>(pow(2, zoom_delta));
                    int count = ncount;
                    auto current_tiles = existing_tiles(m_zoom);
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

QList<TilePos> GeoMap::existing_tiles(int zoom) const
{
    if(!m_tiles.contains(zoom)) return QList<TilePos>();
    return m_tiles[zoom].keys();
}

void GeoMap::remove_out_of_view(QList<TilePos> exist, int zoom)
{
    auto tiles = existing_tiles(zoom);
    for(auto tile: tiles){
        if(!exist.contains(tile)){
            remove_tile(tile);
        }
    }
}

void GeoMap::remove_tile(TilePos pos)
{
    if(contains_tile(pos)){
        auto tile = m_tiles[pos.z].take(pos);
        tile.reset();
    }
}

void GeoMap::init_map_items()
{
    auto label = new MapLabelItem();
    auto scale = new MapScaleItem();
    auto copyright = new MapCopyrightItem();
    label->set_map(this);
    scale->set_map(this);
    copyright->set_map(this);

    m_map_items.append(label);
    m_map_items.append(scale);
    m_map_items.append(copyright);
}
