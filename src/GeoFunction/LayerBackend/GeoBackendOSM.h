#ifndef GEOBACKENDOSM_H
#define GEOBACKENDOSM_H

#include "GeoBackend.h"

class GeoBackendOSM : public GeoBackend
{
public:
    GeoBackendOSM();
    void init() override;
    QString build(clz::TilePos pos) override;
    int min_level() const override;
    int max_level() const override;
    QString label() override;
};

#endif // GEOBACKENDOSM_H
