#include "GeoBackendOSM.h"

GeoBackendOSM::GeoBackendOSM()
{
    init();
}

void GeoBackendOSM::init()
{
    tile_server = "http://a.tile.openstreetmap.org/${z}/${x}/${y}.png";
}

QString GeoBackendOSM::build(clz::TilePos pos)
{
    auto url = tile_server;
    url.replace("${z}", QString::number(pos.z));
    url.replace("${x}", QString::number(pos.x));
    url.replace("${y}", QString::number(pos.y));
    return url;
}
