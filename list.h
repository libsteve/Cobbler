#ifndef LIST_H
#define LIST_H

#include "primitive.h"

//////
// node

primitive_declare(node);

extern void method(node, destroy);

primitive_define(node, primitive, {
        struct node *next;
        struct node *prev;
        struct primitive *value;
    },
    virtual(node, destroy));

extern node *method(node, initialize, node *prev, primitive *value, node *next);

//////
// list

primitive_declare(list);

extern list   *method(list, create);
extern void    method(list, destroy);

primitive_define(list, node, {
        size_t length;
    },
    virtual(list, create),
    virtual(list, destroy));

extern primitive *method(list, head);
extern primitive *method(list, tail);

extern list *method(list,  push, primitive *p);
extern list *method(list, rpush, primitive *p);

extern primitive *method(list,  peek);
extern primitive *method(list, rpeek);

extern list *method(list,  pop, primitive *output p);
extern list *method(list, rpop, primitive *output p);

#define VALUE(it) (((node *)it)->value)
#define LIST_FOREACH(it, l) for(node *it = ((node *)l)->next; ((node *)l) != it; it = it->next)

#endif
