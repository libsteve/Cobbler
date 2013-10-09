#include "Cobbler/list.h"

// primitive methods
list *
method(list, create)
{
    return SuperCreate(this, NULL, this);
}

void 
method(list, destroy)
{
    node *it = ((node *)this)->next;
    while (it != (node *)this) {
        node *next = it->next;
        disown(it);
        it = next;
    }
    SuperDestroy();
}

// iterator interface override methods
iterator *
method(list, iter_get)
{
    if (PrimitiveCast(node, this)->next != this) {
        return PrimitiveCast(node, this)->next;
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
    primitive *p = ((node *)this)->next->value;
    return autodisown(own(p));
}

primitive *
method(list, tail)
{
    primitive *p = ((node *)this)->prev->value;
    return autodisown(own(p));
}

list *
method(list, push, primitive *p)
{
    node *n = create(node);
    n = static_call(node, initialize, n, ((node *)this)->prev, p, (node *)this);
    if (n) this->length += 1;
    return this;
}

list *
method(list, rpush, primitive *p)
{
    node *n = create(node);
    n = static_call(node, initialize, n, (node *)this, p, ((node *)this)->next);
    if (n) this->length += 1;
    return this;
}

primitive *
method(list, peek)
{
    if (this->length > 0) {
        primitive *p = ((node *)this)->prev->value;
        return autodisown(own(p));
    }
    return NULL;
}

primitive *
method(list, rpeek)
{
    if (this->length > 0) {
        primitive *p = ((node *)this)->next->value;
        return autodisown(own(p));
    }
    return NULL;
}

list *
method(list, pop, primitive *output p)
{
    primitive *value = NULL;
    if (this->length > 0) {
        node *prev = ((node *)this)->prev;
        value = own(prev->value);
        ((node *)this)->prev = prev->prev;
        prev->prev->next = (node *)this;
        this->length -= 1;
        disown(prev);
    }
    autodisown(value);
    if (p != NULL) {
        *p = value;
    }
    return this;
}

list *
method(list, rpop, primitive *output p)
{
    primitive *value = NULL;
    if (this->length > 0) {
        node *next = ((node *)this)->next;
        value = own(next->value);
        ((node *)this)->next = next->next;
        next->next->prev = (node *)this;
        this->length -= 1;
        disown(next);
    }
    autodisown(value);
    if (p != NULL) {
        *p = value;
    }
    return this;
}
