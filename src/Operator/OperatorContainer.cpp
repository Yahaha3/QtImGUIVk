#include "OperatorContainer.h"
#include "OperatorMark.h"
#include "OperatorLineString.h"

clz::OperatorContainer::OperatorContainer()
{

}

void clz::OperatorContainer::set_map(clz::GeoMap *map)
{
    m_map = map;
}

clz::OperatorBase *clz::OperatorContainer::get_operator_mark(const QString &signature)
{
    auto op = new OperatorMark(m_map, signature);
    m_operators.insert(signature, op);
    return op;
}

clz::OperatorBase *clz::OperatorContainer::get_operator_linestring(const QString &signature)
{
    auto op = new OperatorLineString(m_map, signature);
    m_operators.insert(signature, op);
    return op;
}

clz::ClzResult clz::OperatorContainer::remove_operator(const QString &signature)
{
    if(m_operators.contains(signature)){
        m_operators.value(signature)->destroy_operator();
        m_operators.value(signature)->deleteLater();
        m_operators[signature] = nullptr;
        m_operators.remove(signature);
        return ClzSuccess;
    }
    return ClzDontContains;
}

void clz::OperatorContainer::remove_operators(const QStringList &signatures)
{
    for(auto signature: signatures){
        remove_operator(signature);
    }
}

void clz::OperatorContainer::remove_similar_operator(const QString &fragment)
{
    auto list = m_operators.keys();
    for(auto key: list){
        if(key.contains(fragment)){
            remove_operator(key);
        }
    }
}

clz::OperatorBase *clz::OperatorContainer::get_valid_operator(const QString &signature, clz::ClzResult &result) const
{
    result = ClzError;
    if(m_operators.contains(signature)){
        auto op = m_operators.value(signature);
        result = ClzSuccess;
        if(!op->is_valid()){
            result = ClzNotVaild;
            return nullptr;
        }
        return op;
    }
    return nullptr;
}

clz::OperatorBase *clz::OperatorContainer::get_affirm_operator(const QString &signature, clz::ClzResult &result) const
{
    result = ClzError;
    if(m_operators.contains(signature)){
        auto op = m_operators.value(signature);
        result = ClzSuccess;
        return op;
    }
    return nullptr;
}

QList<clz::OperatorBase *> clz::OperatorContainer::get_similar_operator(const QString &name_fragment)
{
    QList<OperatorBase*> result;
    auto list = m_operators.keys();
    for(auto key: list){
        if(key.contains(name_fragment)){
            result.append(m_operators.value(key));
        }
    }
    return result;
}

QList<clz::OperatorBase *> clz::OperatorContainer::get_startwith_operator(const QString &name_start)
{
    QList<OperatorBase*> result;
    auto list = m_operators.keys();
    for(auto key: list){
        if(key.startsWith(name_start)){
            result.append(m_operators.value(key));
        }
    }
    return result;
}

void clz::OperatorContainer::clear_operators()
{
    remove_operators(m_operators.keys());
}

void clz::OperatorContainer::clear_operators(const QString &layer_name)
{
    QStringList remove_list;
    for(auto op: m_operators){
        if(op->layer()->get_element_id() == layer_name){
            remove_list.append(op->signature());
        }
    }
    remove_operators(remove_list);
}

clz::OperatorBase *clz::OperatorContainer::get_current_operator()
{
    return m_current_operator;
}

void clz::OperatorContainer::set_current_operator(clz::OperatorBase *op)
{
    m_current_operator = op;
}

void clz::OperatorContainer::operators_update()
{
    for(auto op: m_operators){
        op->refresh();
    }
}
