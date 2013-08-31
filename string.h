#ifndef string_H
#define string_H

#include "primatives.h"

define_primative_begin(string, primative)
	char *string;
	size_t length;
define_primative_end(string)

char string_charAt(string *str, unsigned int index);

bool string_isEqual(string *str, string *other);

#define PSTRING(__string) autodisown(make(string, __string))

#endif