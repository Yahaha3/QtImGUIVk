#ifndef OPERATORMARK_H
#define OPERATORMARK_H

#include "OperatorBase.h"
#include "Element/ImageDrawElement.h"
// 可动目标,考虑插值行为

namespace clz {
class OperatorMark : public OperatorBase
{
public:
    OperatorMark(GeoMap* map, QString signature, QObject* parent = 0);

    ClzOpResult generate_operator(const clz::GeoPos &position, bool noted = true) override;

    ClzOpResult translate_to(clz::GeoPos position) override;
    void rotate(double angle) override;
    void refresh() override;

private:
    QString get_mark_icon_path();
private:
    // mark中的插值对象需要单独管理
    bool m_interpolation;
    const QSize m_mark_size = QSize(40, 40);
    double m_mark_azimuth;
};
}

#endif // OPERATORMARK_H
