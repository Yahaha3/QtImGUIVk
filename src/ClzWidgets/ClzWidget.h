#ifndef CLZWIDGET_H
#define CLZWIDGET_H

#include <QObject>
#include "ClzCore/ClzObject.h"
#include <QPoint>
#include <QSize>
#include "imgui.h"

namespace clz {
class ClzWidget : public ClzObject
{
    Q_OBJECT
public:
    ClzWidget(ClzWidget* parent = 0);

    ImVec2 impos();
    QPoint pos();

    QSize size();
    ImVec2 imsize();

    QString name();
    bool fold();

    void set_widget_name(const QString& name);
    void move(QPoint position);
    void resize(QSize size);

    virtual void init() = 0;
    virtual void paint() = 0;

protected:
    QPoint m_position;
    QSize m_size;
    QString m_name;
    bool m_widget_fold;
    ImU32 m_window_flags;
};
}

#endif // CLZWIDGET_H
