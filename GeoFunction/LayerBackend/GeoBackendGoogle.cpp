#include "GeoBackendGoogle.h"

GeoBackendGoogle::GeoBackendGoogle()
{
    init();
}

void GeoBackendGoogle::init()
{
    tile_server = "http://map.aossci.com:8081/base/map/google/${z}/${x}/${y}.png";
}

QString GeoBackendGoogle::build(clz::TilePos pos)
{
    auto url = tile_server;
    url.replace("${z}", QString::number(pos.z));
    url.replace("${x}", QString::number(pos.x));
    url.replace("${y}", QString::number(pos.y));
    return url;
}

int GeoBackendGoogle::min_level() const
{
    return 0;
}

int GeoBackendGoogle::max_level() const
{
    return 19;
}

QString GeoBackendGoogle::label()
{
    return "Copyrights @Google";
}
