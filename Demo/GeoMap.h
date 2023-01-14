#ifndef GEOMAP_H
#define GEOMAP_H

#include <QObject>
#include <QVector>
#include "implot.h"
//#include "imgui.h"

class ImageVk;

namespace clz {

#define PI 3.14159265359
#define TILE_SIZE    256
#define MAX_ZOOM      19


struct TilePos {
    int x, y, z;
};

enum TileState : int {
    Unavailable = 0, // tile not available
    Loaded,          // tile has been loaded into  memory
    Downloading,     // tile is downloading from server
    OnDisk           // tile is saved to disk, but not loaded into memory
};

typedef ImageVk TileImage;
struct Tile {
    Tile() : state(TileState::Unavailable) {  }
    std::shared_ptr<TileImage> image;
    TileState state;
};

class GeoMap
{
public:
    GeoMap();

    const QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>>& get_region(ImPlotRect view, ImVec2 pixels);

private:
    bool append_region(int z, double min_x, double min_y, double size_x, double size_y);

private:
    QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>> m_region;

};

}

#endif // GEOMAP_H
