#ifndef CLZWIDGET_H
#define CLZWIDGET_H

#include <QObject>
#include "ClzCore/ClzObject.h"
#include <QPoint>
#include <QSize>

namespace clz {
class ClzWidget : public ClzObject
{
    Q_OBJECT
public:
    ClzWidget(ClzWidget* parent = 0);

    QPoint pos();
    QSize size();
    QString title();

    void set_widget_title(const QString& title);
    void move(QPoint position);
    void resize(QSize size);

    virtual void init() = 0;
    virtual void paint() = 0;

protected:
    QPoint m_position;
    QSize m_size;
    QString m_title;

};
}

#endif // CLZWIDGET_H
