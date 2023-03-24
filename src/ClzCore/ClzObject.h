#ifndef CLZOBJECT_H
#define CLZOBJECT_H

#include <QObject>
#include "common/clz_geomap_global.h"
#include <QSet>
/*
 * clz-object是layout widget的基础
 * layout widget 由此派生
*/
namespace clz {

class ClzObject : public QObject
{
    Q_OBJECT
public:
    explicit ClzObject(ClzObject *parent = nullptr);
    ~ClzObject();

    void set_parent(ClzObject* obj);
    ClzObject* parent() const;

    void add_child(ClzObject* obj);
    void remove_child(ClzObject* obj);
    bool has_child(ClzObject* obj);
    bool has_child(const QString& id);
    void delete_elements();
    int count_childrens() const;
    QStringList childrens() const;

    void set_object_id(QString id = clz::empty);
    QString get_object_id() const;

signals:

private:
    QString m_object_id;
    QString m_parent;
    QSet<QString> m_childrens;
};

}

#endif // CLZOBJECT_H
