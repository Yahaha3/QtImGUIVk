#ifndef OPERATORCONTAINER_H
#define OPERATORCONTAINER_H

#include "Map/GeoMap.h"
#include "OperatorBase.h"
#include <QTimer>

namespace clz {

class OperatorContainer
{
public:
    OperatorContainer();
    void set_map(GeoMap* map);

    OperatorBase* get_operator_mark(const QString& signature);
    OperatorBase* get_operator_linestring(const QString& signature);

    ClzResult remove_operator(const QString& signature);
    void remove_operators(const QStringList& signatures);
    void remove_similar_operator(const QString& fragment);

    // 找到一个 "可用" 的对象
    OperatorBase* get_valid_operator(const QString& signature, ClzResult& result) const;
    // 找到一个对象
    OperatorBase* get_affirm_operator(const QString& signature, ClzResult& result) const;

    // 找打一组名字相似的对象
    QList<OperatorBase*> get_similar_operator(const QString& name_fragment);
    // 找到一组以此起头的对象
    QList<OperatorBase*> get_startwith_operator(const QString& name_start);

    void clear_operators();
    void clear_operators(const QString& layer_name);

    OperatorBase* get_current_operator();
    void set_current_operator(OperatorBase* op);
    bool has_operator(const QString& signature);

    // mark中的插值对象需要做额外处理
    void operators_update();


private:
    GeoMap* m_map;
    // 全部可操作对象
    QHash<QString, OperatorBase*> m_operators;

    // 当前操作对象
    OperatorBase* m_current_operator = nullptr;
};

}


#endif // OPERATORCONTAINER_H
