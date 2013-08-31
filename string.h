#ifndef string_H
#define string_H

#include "primitive.h"

define_primitive_begin(string, primitive)
	char *string;
	size_t length;
define_primitive_end(string)

char string_charAt(string *str, unsigned int index);

bool string_isEqual(string *str, string *other);

#define PSTRING(__string) autodisown(make(string, __string))

#endif