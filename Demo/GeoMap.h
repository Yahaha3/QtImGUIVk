#ifndef GEOMAP_H
#define GEOMAP_H

#include <QObject>
#include <QVector>
#include "implot.h"
//#include "imgui.h"
#include <mutex>
#include <QMap>
#include "MapThread/MapRequestThread.h"

namespace clz {

#define PI 3.14159265359
#define TILE_SIZE    256
#define MAX_ZOOM      19

class GeoMap
{
public:
    GeoMap();

    const QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>>& get_region(ImPlotRect view, ImVec2 pixels);

    void set_tile_origin_data(const TilePos& pos, QByteArray data);
private:
    bool append_region(int z, double min_x, double min_y, double size_x, double size_y);

    std::shared_ptr<Tile> request_tile(TilePos pos);
    void download_tile(TilePos pos);
    std::shared_ptr<Tile> get_tile(TilePos pos);
    std::shared_ptr<Tile> load_tile(TilePos pos);

private:
    QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>> m_region;
    QMap<clz::TilePos, std::shared_ptr<clz::Tile>> m_tiles;

    std::mutex m_tile_mutex;

    std::shared_ptr<MapRequestThread> m_request_thread;
};

}

#endif // GEOMAP_H
