#ifndef string_H
#define string_H

#include "primitive.h"

primitive_define(string, primitive, {
	char *string;
	size_t length;
});

char string_charAt(string *str, unsigned int index);

bool string_isEqual(string *str, string *other);

#define PSTRING(__string) autodisown(make(string, __string))

#endif