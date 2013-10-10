#include "Cobbler/list.h"

// primitive methods
list *
method(list, create)
{
    return (list *)SuperCreate(this, NULL, this);
}

void 
method(list, destroy)
{
    foreach(it, this) {
        it = iter_delete(it);
    }
    SuperDestroy();
}

// iterator interface override methods
iterator *
method(list, iter_get)
{
    if (PrimitiveCast(node, this)->next != PrimitiveCast(node, this)) {
        return (iterator *)PrimitiveCast(node, this)->next;
    }
    return NULL;
}

iterator *
method(list, iter_rget)
{
    if (PrimitiveCast(node, this)->prev != PrimitiveCast(node, this)) {
        return (iterator *)PrimitiveCast(node, this)->prev;
    }
    return NULL;
}

bool
method(list, iter_isValid)
{
    return false;
}

// list methods
primitive *
method(list, head)
{
    iterator *it = iter_get(this);
    if (it)
        return iter_value(it);
    return NULL;
}

primitive *
method(list, tail)
{
    iterator *it = iter_rget(this);
    if (it)
        return iter_value(it);
    return NULL;
}

list *
method(list, push, primitive *p)
{
    node *n = create(node, PrimitiveCast(node, iter_rget(this)), p, PrimitiveCast(node, this));
    this->length += 1;
    return this;
}

list *
method(list, rpush, primitive *p)
{
    node *n = create(node, PrimitiveCast(node, this), p, PrimitiveCast(node, iter_get(this)));
    this->length += 1;
    return this;
}

primitive *
method(list, peek)
{
    iterator *it = iter_rget(this);
    if (it)
        return iter_value(it);
    return NULL;
}

primitive *
method(list, rpeek)
{
    iterator *it = iter_get(this);
    if (it)
        return iter_value(it);
    return NULL;
}

list *
method(list, pop, primitive *output p)
{
    iterator *it = iter_rget(this);
    if (it) {
        primitive *result = autodisown(own(iter_value(it)));
        iter_delete(it);
        if (p) 
            *p = result;
    }
    return this;
}

list *
method(list, rpop, primitive *output p)
{
    iterator *it = iter_rget(this);
    if (it) {
        primitive *result = autodisown(own(iter_value(it)));
        iter_delete(it);
        if (p) 
            *p = result;
    }
    return this;
}
