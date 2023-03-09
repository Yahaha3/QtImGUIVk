#ifndef OPERATORLINESTRING_H
#define OPERATORLINESTRING_H

#include "OperatorBase.h"
#include "Element/LinestringDrawElement.h"

namespace clz {
class OperatorLineString : public OperatorBase
{
public:
    OperatorLineString(GeoMap* map, QString signature, QObject* parent = 0);

    ClzResult generate_operator(const QVector<clz::GeoPos> &vertices, bool noted = true) override;
    void update_vertices(QVector<clz::GeoPos>, bool noted = true) override;

    void refresh() override;

protected:
    virtual void generate_linestring();

private:
    QString get_mark_icon_path();
};
}

#endif // OPERATORLINESTRING_H
