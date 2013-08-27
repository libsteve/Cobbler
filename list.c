#include "list.h"

void node_destroy(node *);

define_primative_class_begin(node, NodePrimative, PrimativeClass)
	primative_class_using_destroy(node_destroy) 
define_primative_class_end(node)

void node_destroy(node *n) {
	disown(n->value);
	n->next->prev = n->prev;
	n->prev->next = n->next;
	PrimativeClass.destroy((primative *)n);
}

node *node_initialize(node *n, node *prev, primative* value, node *next) {
	if (n) {
		n->prev = prev;
		n->value = own(value);
		n->next = next;
		prev->next = n;
		next->prev = n;
	}
	return n;
}

primative *node_value(node *n) {
	if (n) {
		return n->value;
	}
	return NULL;
}

list *list_initialize(list *, va_list *args);
void list_destroy(list *);

define_primative_class_begin(list, ListPr, NodePrimative)
	primative_class_using_initialize(list_initialize)
	primative_class_using_destroy(list_destroy) 
define_primative_class_end(list)

list *list_initialize(list *l, va_list *args) {
	node_initialize((node *)l, NULL, (primative *)&PrimativeNull, NULL);
	return l;
}

void list_destroy(list *l) {
	node *it = ((node *)l)->next;
	while (it != (node *)l) {
		node *next = it->next;
		disown(it);
		it = next;
	}
	NodePrimative.destroy((primative *)l);
}

primative *list_head(list *l) {
	primative *p = ((node *)l)->next->value;
	return autodisown(own(p));
}

primative *list_tail(list *l) {
	primative *p = ((node *)l)->prev->value;
	return autodisown(own(p));
}

list *list_push(list *l, primative *p) {
	node *n = make(NodePrimative);
	n = node_initialize(n, ((node *)l)->prev, p, (node *)l);
	if (n) l->length += 1;
	return l;
}

list *list_rpush(list *l, primative *p) {
	node *n = make(NodePrimative);
	n = node_initialize(n, (node *)l, p, ((node *)l)->next);
	if (n) l->length += 1;
	return l;
}

primative *list_peek(list *l) {
	if (l->length > 0) {
		primative *p = ((node *)l)->prev->value;
		return autodisown(own(p));
	}
	return NULL;
}

primative *list_rpeek(list *l) {
	if (l->length > 0) {
		primative *p = ((node *)l)->next->value;
		return autodisown(own(p));
	}
	return NULL;
}

list *list_pop(list *l, primative *output p) {
	primative *value = NULL;
	if (l->length > 0) {
		node *prev = ((node *)l)->prev;
		value = own(prev->value);
		((node *)l)->prev = prev->prev;
		prev->prev->next = (node *)l;
		l->length -= 1;
		disown(prev);
	}
	autodisown(value);
	if (p != NULL) {
		*p = value;
	}
	return l;
}

list *list_rpop(list *l, primative *output p) {
	primative *value = NULL;
	if (l->length > 0) {
		node *next = ((node *)l)->next;
		value = own(next->value);
		((node *)l)->next = next->next;
		next->next->prev = (node *)l;
		l->length -= 1;
		disown(next);
	}
	autodisown(value);
	if (p != NULL) {
		*p = value;
	}
	return l;
}