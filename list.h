#ifndef LIST_H
#define LIST_H

#include "primitive.h"

define_primitive_begin(node, primitive)
	struct node *next;
	struct node *prev;
	struct primitive *value;
define_primitive_end(node)

extern node *node_initialize(node *, node *prev, primitive *value, node *next);

define_primitive_begin(list, node)
	size_t length;
define_primitive_end(list)

extern primitive *list_head(list *l);
extern primitive *list_tail(list *l);

extern list *list_push (list *l, primitive *p);
extern list *list_rpush(list *l, primitive *p);

extern primitive *list_peek (list *l);
extern primitive *list_rpeek(list *l);

extern list *list_pop (list *l, primitive *output p);
extern list *list_rpop(list *l, primitive *output p);

#define VALUE(it) (((node *)it)->value)
#define LIST_FOREACH(it, l) for(node *it = ((node *)l)->next; ((node *)l)->value != &primitiveNull; it = it->next)

#endif