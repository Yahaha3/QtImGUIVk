
#include "GeoMap.h"
#include "GeoFunction/LayerBackend/GeoBackendGaode.h"
#include "Projection/ProjectionESG3857.h"
#include "GeoFunction/GeoFunctionUtility.h"
#include "map_include.h"
#include "Element/element_include.h"
#include "Operator/OperatorContainer.h"
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
    m_tile_manager = std::make_shared<TileManager>(this);
    m_op_container = std::make_shared<OperatorContainer>();

    set_map_projection(new ProjectionESG3857());
    init_map_items();

    m_root_item->set_element_id(clz::root_item);
    m_op_container->set_map(this);
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

std::shared_ptr<MapRequestThread> GeoMap::get_request_thread() const
{
    return m_request_thread;
}

std::shared_ptr<OperatorContainer> GeoMap::map_oc() const
{
    return m_op_container;
}

int GeoMap::current_zoom() const
{
    return m_zoom;
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
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include "ClzWidgets/ClzCardWidget.h"
void GeoMap::test()
{
    static clz::ClzCardWidget* card = nullptr;
    if(!card){
        card = new clz::ClzCardWidget();
        card->set_widget_name("test");
        QStringList contexts = {"Context1"};
        QStringList uris = {"Context/context"};
        QJsonArray options = {QJsonObject()};
        card->init_card(contexts, uris, options);

        card->move(QPoint(50,50));
        card->resize(QSize(300,300));
    }
    static int count = -1;
    count ++;
    card->update_card("Context/context", count);
    card->paint();
}

void GeoMap::map_timeout()
{
    tiles_update();
    items_update();
    map_oc()->operators_update();
}

std::shared_ptr<TileManager> GeoMap::get_tile_manager() const
{
    return m_tile_manager;
}

void GeoMap::sub_items_init()
{
    get_request_thread()->start();
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
            std::shared_ptr<Tile> tile = get_tile_manager()->request_tile(pos);
            use_tile.append(pos);

            ok = true;
            if(tile == nullptr || tile->state != TileState::Loaded){
                ok = false;
                TilePos parent_pos;
                tile = get_tile_manager()->get_parent_tile(pos, ok);
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
    get_tile_manager()->remove_out_of_view(use_tile, z);
    return covered;
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

void GeoMap::tiles_update()
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
            auto ID = get_tile_manager()->get_tile_texture_id(position, ok);
            if(ok){
                ImPlot::PlotImage("##Tiles", ID, bmin, bmax, {0,0}, {1,1}, col);
                if (debug)
                    ImPlot::PlotText(position.tozxy().toStdString().c_str(),(bmin.x+bmax.x)/2,(bmin.y+bmax.y)/2);
            }
        }
    }
}

void GeoMap::items_update()
{
    for(auto item: m_map_items){
        item->paint();
    }
}
