#include "list.h"

void node_destroy(node *);

primitive_class_define(node, primitive, 
	using_destroy(node_destroy));

void node_destroy(node *n) {
	disown(n->value);
	n->next->prev = n->prev;
	n->prev->next = n->next;
	SuperDestroy(n);
}

node *node_initialize(node *n, node *prev, primitive* value, node *next) {
	if (n) {
		n->prev = prev;
		n->value = own(value);
		n->next = next;
		prev->next = n;
		next->prev = n;
	}
	return n;
}

primitive *node_value(node *n) {
	if (n) {
		return n->value;
	}
	return NULL;
}

list *list_initialize(list *);
void list_destroy(list *);

primitive_class_define(list, node,
	using_initialize(list_initialize),
	using_destroy(list_destroy));

// define_primitive_begin(list_null, primitive)
// define_primitive_end(list_null)
// define_primitive_class_begin(list_null, primitive)
// 	, .ownership_count = 9999
// define_primitive_class_end(list_null)

list *list_initialize(list *l) {
	node_initialize((node *)l, NULL, NULL, NULL);
	return l;
}

void list_destroy(list *l) {
	node *it = ((node *)l)->next;
	while (it != (node *)l) {
		node *next = it->next;
		disown(it);
		it = next;
	}
	SuperDestroy(l);
}

primitive *list_head(list *l) {
	primitive *p = ((node *)l)->next->value;
	return autodisown(own(p));
}

primitive *list_tail(list *l) {
	primitive *p = ((node *)l)->prev->value;
	return autodisown(own(p));
}

list *list_push(list *l, primitive *p) {
	node *n = make(node);
	n = node_initialize(n, ((node *)l)->prev, p, (node *)l);
	if (n) l->length += 1;
	return l;
}

list *list_rpush(list *l, primitive *p) {
	node *n = make(node);
	n = node_initialize(n, (node *)l, p, ((node *)l)->next);
	if (n) l->length += 1;
	return l;
}

primitive *list_peek(list *l) {
	if (l->length > 0) {
		primitive *p = ((node *)l)->prev->value;
		return autodisown(own(p));
	}
	return NULL;
}

primitive *list_rpeek(list *l) {
	if (l->length > 0) {
		primitive *p = ((node *)l)->next->value;
		return autodisown(own(p));
	}
	return NULL;
}

list *list_pop(list *l, primitive *output p) {
	primitive *value = NULL;
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

list *list_rpop(list *l, primitive *output p) {
	primitive *value = NULL;
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