#ifndef VALUE_H
#define VALUE_H

#include "primitive.h"
#include "string.h"

primitive_declare(value);

void method(value, destroy);
value *method(value, copy);

primitive_define(value, primitive, {
		void *bytes;
		size_t size;
		string *type;
	}, 
	using_virtual(value, destroy),
	using_virtual(value, copy));

value *value_initialize(value *v, void *bytes, size_t size);
value *value_initializeWithType(value *v, void *bytes, size_t size, string *type);
void *value_value(value *v, size_t output size);
string *value_type(value *v);

#endif