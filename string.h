#ifndef string_H
#define string_H

#include "primatives.h"

declare_primative_begin(string, StringPrimative, primative)
	char *string;
	size_t length;
declare_primative_end(string)

char string_charAt(string *str, unsigned int index);

#define PSTRING(__string) autodisown(make(StringPrimative, __string))

#endif