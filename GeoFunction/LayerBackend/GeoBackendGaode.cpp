#include "GeoBackendGaode.h"

GeoBackendGaode::GeoBackendGaode()
{
    init();
}

void GeoBackendGaode::init()
{
    tile_server = "http://webst04.is.autonavi.com/appmaptile?style=7&x=${x}&y=${y}&z=${z}";
}

QString GeoBackendGaode::build(clz::TilePos pos)
{
    auto url = tile_server;
    url.replace("${z}", QString::number(pos.z));
    url.replace("${x}", QString::number(pos.x));
    url.replace("${y}", QString::number(pos.y));
    return url;
}

int GeoBackendGaode::min_level() const
{
    return 0;
}

int GeoBackendGaode::max_level() const
{
    return 16;
}

QString GeoBackendGaode::label()
{
    return "Copyrights Gaode";
}
