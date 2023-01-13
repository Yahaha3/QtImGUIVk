#include "GeoMap.h"

using namespace clz;

GeoMap::GeoMap()
{

}

const QVector<QPair<TilePos, std::shared_ptr<Tile> > > &GeoMap::get_region(ImPlotRect view, ImVec2 pixels)
{
    double min_x = std::clamp(view.X.Min, 0.0, 1.0);
    double min_y = std::clamp(view.Y.Min, 0.0, 1.0);
    double size_x = std::clamp(view.X.Size(), 0.0, 1.0);
    double size_y = std::clamp(view.Y.Size(), 0.0, 1.0);

    double pix_occupied_x = (pixels.x / view.X.Size()) * size_x;
    double pix_occupied_y = (pixels.y / view.Y.Size()) * size_y;
    double units_per_tile_x = view.X.Size() * (TILE_SIZE / pix_occupied_x);
    double units_per_tile_y = view.Y.Size() * (TILE_SIZE / pix_occupied_y);

    int z    = 0;
    double r = 1.0 / pow(2,z);
    while (r > units_per_tile_x && r > units_per_tile_y && z < MAX_ZOOM)
        r = 1.0 / pow(2,++z);

    return m_region;
}
