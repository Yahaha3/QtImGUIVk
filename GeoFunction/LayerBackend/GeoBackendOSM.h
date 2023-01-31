#ifndef GEOBACKENDOSM_H
#define GEOBACKENDOSM_H

#include "GeoBackend.h"

class GeoBackendOSM : public GeoBackend
{
public:
    GeoBackendOSM();
    void init() override;
    QString build(clz::TilePos pos) override;
};

#endif // GEOBACKENDOSM_H
