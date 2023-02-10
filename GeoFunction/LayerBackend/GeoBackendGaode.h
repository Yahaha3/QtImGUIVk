#ifndef GEOBACKENDGAODE_H
#define GEOBACKENDGAODE_H

#include "GeoBackend.h"

class GeoBackendGaode: public GeoBackend
{
public:
    GeoBackendGaode();
    void init() override;
    QString build(clz::TilePos pos) override;
    int min_level() const override;
    int max_level() const override;
    QString label() override;
};

#endif // GEOBACKENDGAODE_H
