#ifndef GEOMAP_H
#define GEOMAP_H

#include <QObject>
#include <QVector>
//#include "common/Image.h"
#include "implot.h"
#include "imgui.h"

#define PI 3.14159265359
#define TILE_SIZE    256
#define MAX_ZOOM      19


namespace clz {


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

struct TilePos {
    int x, y, z;
};

enum TileState : int {
    Unavailable = 0, // tile not available
    Loaded,          // tile has been loaded into  memory
    Downloading,     // tile is downloading from server
    OnDisk           // tile is saved to disk, but not loaded into memory
};

struct Tile {
    Tile() : state(TileState::Unavailable) {  }
//    ImageVk image;
    TileState state;
};

class GeoMap
{
public:
    GeoMap();

    const QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>>& get_region(ImPlotRect view, ImVec2 pixels);

private:
    QVector<QPair<clz::TilePos, std::shared_ptr<clz::Tile>>> m_region;

};

}

#endif // GEOMAP_H
