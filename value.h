#ifndef VALUE_H
#define VALUE_H

#include "primatives.h"
#include "string.h"

define_primative_begin(value, primative)
	void *bytes;
	size_t size;
	string *type;
define_primative_end(value)

value *value_initialize(value *v, void *bytes, size_t size);
value *value_initializeWithType(value *v, void *bytes, size_t size, string *type);
void *value_value(value *v, size_t output size);
string *value_type(value *v);

#endif