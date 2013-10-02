#ifndef VALUE_H
#define VALUE_H

#include "primitive.h"
#include "string.h"

primitive_declare(value);

void value_destroy(value *v);
value *value_copy(value *v);

primitive_define(value, primitive, {
		void *bytes;
		size_t size;
		string *type;
	}, 
	using_destroy(value_destroy),
	using_copy(value_copy));

value *value_initialize(value *v, void *bytes, size_t size);
value *value_initializeWithType(value *v, void *bytes, size_t size, string *type);
void *value_value(value *v, size_t output size);
string *value_type(value *v);

#endif