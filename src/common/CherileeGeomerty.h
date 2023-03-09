#ifndef CHERILEEGEOMERTY_H
#define CHERILEEGEOMERTY_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

#define M_PI 3.141592653589793238

namespace clz {
class CherileeGeomerty
{
public:
    CherileeGeomerty();

    static std::vector<double> WGS84toECEF(const std::vector<double>& lla);
    static std::vector<double> ECEFtoWGS84(const std::vector<double>& pos);

    static double distance(double lon1, double lat1,
                           double lon2, double lat2);

    static double heading(double lon1, double lat1,
                          double lon2, double lat2);

    static double pitch_noroll(double lon1, double lat1, double h1,
                               double lon2, double lat2, double h2);

    static double angle_2_radain(double radain);
    static double radain_2_angle(double angle);
};
}

#endif // CHERILEEGEOMERTY_H
