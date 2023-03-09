#ifndef OPERATORMARK_H
#define OPERATORMARK_H

#include "OperatorBase.h"
#include "Element/ImageDrawElement.h"
#include "Element/LinestringDrawElement.h"
// 可动目标,考虑插值行为

namespace clz {
class OperatorMark : public OperatorBase
{
public:
    OperatorMark(GeoMap* map, QString signature, QObject* parent = 0);

    ClzResult generate_operator(const clz::GeoPos &position, bool noted = true) override;

    ClzResult translate_to(clz::GeoPos position) override;
    void rotate(double angle) override;
    void refresh() override;

    void enable_interpolation(bool interpolation);
    void enable_trajectory(bool trajectory);

private:
    void update_interpolation();
    qint64 current_timestamp();
    QString get_mark_icon_path();
    LinestringDrawElement* element_trajectory();

private:
    // mark中的插值对象需要单独管理
    const int m_delay = 500;
    bool m_interpolation = false;
    bool m_has_trajectory = false;
    QString m_trajectory = clz::empty;

    const QSize m_mark_size = QSize(40, 40);
    double m_mark_azimuth;

    QMap<qint64, QPair<GeoPos, double>> m_interpolation_infos;
};
}

#endif // OPERATORMARK_H
