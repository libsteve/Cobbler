#ifndef string_H
#define string_H

#include "primitive.h"

primitive_declare(string);

string *method(string, initialize, char *);
void method(string, destroy);
string *method(string, copy);

primitive_define(string, primitive, {
		char *str;
		size_t length;
	},
	using_virtual(string, initialize),
	using_virtual(string, destroy),
	using_virtual(string, copy));

char string_charAt(string *str, unsigned int index);

bool string_isEqual(string *str, string *other);

#define PSTRING(__string) autodisown(make(string, __string))

#endif