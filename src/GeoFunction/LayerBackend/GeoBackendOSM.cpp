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

int GeoBackendOSM::min_level() const
{
    return 0;
}

int GeoBackendOSM::max_level() const
{
    return 19;
}

QString GeoBackendOSM::label()
{
    return " OpenStreetMap Contributors";
}
