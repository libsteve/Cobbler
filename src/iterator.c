#include "Cobbler/primitive.h"
#include "Cobbler/iterator.h"

//////
// iterator interface functions

iterator *iter_get(void *it)
{
	return virtual_call(iterator *, iter_get, it);
}

iterator *iter_rget(void *it)
{
	return virtual_call(iterator *, iter_rget, it);
}

iterator *iter_next(void *it)
{
	return iter_get(virtual_call(iterator *, iter_next, it));
}

iterator *iter_prev(void *it)
{
	if (PrimitiveHasVirtualMethod(it, iter_prev))
		return iter_get(virtual_call(void *, iter_prev, it));
	return NULL;
}

primitive *iter_value(void *it)
{
	return virtual_call(primitive *, iter_value, it);
}

iterator *iter_delete(void *it)
{
	if (PrimitiveHasVirtualMethod(it, iter_delete))
		return virtual_call(iterator *, iter_delete, it);
	return NULL;
}

bool iter_hasNext(void *it)
{
	return virtual_call(bool, iter_hasNext, it);
}

bool iter_hasPrev(void *it)
{
	if (PrimitiveHasVirtualMethod(it, iter_hasPrev))
		return virtual_call(bool, iter_hasPrev, it);
	return NULL;
}

bool iter_isValid(void *it)
{
	return virtual_call(bool, iter_isValid, it);
}

//////
// node functions

// primitive methods
node *
method(node, create, node *prev, primitive *value, node *next)
{
	this->prev = prev;
	this->next = next;
	if (prev) prev->next = this;
	if (next) next->prev = this;
	this->value = own(value);
	return this;
}

void
method(node, destroy)
{
	if (this->value) {
		disown(this->value);
		this->value = NULL;
	}
}

// iterator methods
node *
method(node, iter_get)
{
	return this;
}

node 
*method(node, iter_rget)
{
	return static_call(node, iter_get, this);
}

node *
method(node, iter_next)
{
	if (static_call(node, iter_hasNext, this)) 
		return this->next;
	return NULL;
}

node *
method(node, iter_prev)
{
	if (static_call(node, iter_hasPrev, this)) 
		return this->prev;
	return NULL;
}

node *
method(node, iter_delete)
{
	if (static_call(node, iter_hasNext, this) && 
		static_call(node, iter_hasPrev, this)) {
		this->next->prev = this->prev;
		this->prev->next = this->next;
		node *prev = this->prev;
		disown(this);
		return prev;
	} else if (static_call(node, iter_hasNext, this)) {
		this->next->prev = NULL;
		node *next = this->next;
		disown(this);
		return next;
	} else if (static_call(node, iter_hasPrev, this)) {
		this->prev->next = NULL;
		node *prev = this->prev;
		disown(this);
		return prev;
	}
	disown(this);
	return NULL;
}

primitive *
method(node, iter_value)
{
	return static_call(node, value, this);
}

bool 
method(node, iter_hasNext)
{
	return this->next && iter_isValid(this->next);
}

bool 
method(node, iter_hasPrev)
{
	return this->prev && iter_isValid(this->prev);
}

bool
method(node, iter_isValid)
{
	return true;
}


// node methods
primitive *
method(node, value)
{
	return this->value;
}

void
method(node, setValue, primitive *value)
{
	if (this->value) {
		disown(this->value);
	}
	this->value = own(value);
}


