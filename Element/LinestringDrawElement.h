#ifndef LINESTRINGDRAWELEMENT_H
#define LINESTRINGDRAWELEMENT_H

#include "GraphicsDrawElement.h"

namespace clz {

class LinestringDrawElement : public GraphicsDrawElement
{
public:
    LinestringDrawElement(const QVector<clz::GeoPos>& vertices, QColor color = Qt::black);

    void element_projection() override;
    QRectF proj_shape() const override;
    void paint() override;
private:
};

}


#endif // LINESTRINGDRAWELEMENT_H
