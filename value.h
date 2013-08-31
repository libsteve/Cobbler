#ifndef VALUE_H
#define VALUE_H

#include "primitive.h"
#include "string.h"

define_primitive_begin(value, primitive)
	void *bytes;
	size_t size;
	string *type;
define_primitive_end(value)

value *value_initialize(value *v, void *bytes, size_t size);
value *value_initializeWithType(value *v, void *bytes, size_t size, string *type);
void *value_value(value *v, size_t output size);
string *value_type(value *v);

#endif