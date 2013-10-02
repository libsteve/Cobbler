#ifndef string_H
#define string_H

#include "primitive.h"

primitive_declare(string);

string *string_initialize(string *, char *);
void string_destroy(string *);
string *string_copy(string *);

primitive_define(string, primitive, {
		char *string;
		size_t length;
	},
	using_initialize(string_initialize),
	using_destroy(string_destroy),
	using_copy(string_copy));

char string_charAt(string *str, unsigned int index);

bool string_isEqual(string *str, string *other);

#define PSTRING(__string) autodisown(make(string, __string))

#endif