#include "CherileeGeomerty.h"

clz::CherileeGeomerty::CherileeGeomerty()
{

}

std::vector<double> clz::CherileeGeomerty::WGS84toECEF(const std::vector<double> &lla)
{
    std::vector<double> result(3);

    const double longitude = lla[0];
    const double latitude = lla[1];
    const double altitude = lla[2];

    const double a = 6378137.0;
    const double b = 6356752.31424518;
    const double E = (a * a - b * b) / (a * a);
    const double COSLAT = cos(latitude * M_PI / 180.0);
    const double SINLAT = sin(latitude * M_PI / 180.0);
    const double COSLONG = cos(longitude * M_PI / 180.0);
    const double SINLONG = sin(longitude * M_PI / 180.0);
    const double N = a / (sqrt(1 - E * SINLAT * SINLAT));
    const double NH = N + altitude;

    result[0] = NH * COSLAT * COSLONG;
    result[1] = NH * COSLAT * SINLONG;
    result[2] = (b * b * N / (a * a) + altitude) * SINLAT;

    return result;
}

std::vector<double> clz::CherileeGeomerty::ECEFtoWGS84(const std::vector<double> &pos)
{
    std::vector<double> result(3);
    const double x = pos[0];
    const double y = pos[1];
    const double z = pos[2];
    const double a =  6378137.0;
    const double b =  6356752.31424518;
    const double c = sqrt(((a * a) - (b * b)) / (a * a));
    const double d = sqrt(((a * a) - (b * b)) / (b * b));
    const double p = sqrt((x * x) + (y * y));
    const double q = atan2((z * a), (p * b));
    const double Longitude = atan2(y, x);
    const double Latitude = atan2((z + (d * d) * b * pow(sin(q), 3)), (p - (c * c) * a * pow(cos(q), 3)));
    const double N = a / sqrt(1 - ((c * c) * pow(sin(Latitude), 2)));

    result[0] = Longitude * 180.0 / M_PI;
    result[1] = Latitude * 180.0 / M_PI;
    result[2] = (p / cos(Latitude)) - N;

    return result;
}

double clz::CherileeGeomerty::distance(double lon1, double lat1, double lon2, double lat2)
{
    const double R = 6378137.0; // metres
    const double latr1 = lat1 * M_PI/180.0; // lat, lon in radians
    const double latr2 = lat2 * M_PI/180;
    const double dlatr = (lat2-lat1) * M_PI/180;
    const double dlonr = (lon2-lon1) * M_PI/180;

    const double a = sin(dlatr/2) * sin(dlatr/2) + cos(latr1) * cos(latr2) * sin(dlonr/2) * sin(dlonr/2);
    const double c = 2 * atan2(sqrt(a), sqrt(1-a));

    const double d = R * c; // in metres
    return d;
}

double clz::CherileeGeomerty::heading(double lon1, double lat1, double lon2, double lat2)
{
    double lonr1 = lon1 * M_PI/180.0;
    double lonr2 = lon2 * M_PI/180.0;

    double latr1 = lat1 * M_PI/180.0;
    double latr2 = lat2 * M_PI/180.0;

    const double y = sin(lonr2-lonr1) *cos(latr2);
    const double x =cos(latr1)*sin(latr2) - sin(latr1)*cos(latr2)*cos(lonr2-lonr1);
    const double theta = atan2(y, x);
    const double heading = fmod(theta * 180.0/M_PI + 360.0, 360.0);
    return heading; // in degrees
}

double clz::CherileeGeomerty::pitch_noroll(double lon1, double lat1, double h1, double lon2, double lat2, double h2)
{
    double dh = h2 - h1;
    double ds = distance(lon1, lat1, lon2, lat2);
    return atan2(dh, ds);
}

double clz::CherileeGeomerty::angle_2_radain(double angle)
{
    return M_PI * angle / 180;
}

double clz::CherileeGeomerty::radain_2_angle(double radain)
{
    return 180 * radain / M_PI;
}
