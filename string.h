#ifndef string_H
#define string_H

#include "primatives.h"

typedef struct string {
	inherit_primative(struct primative);
	char *string;
	size_t length;
} string;

extern primative_class StringPrimative;

char string_charAt(string *str, unsigned int index);

#define PSTRING(__string) autodisown(make(StringPrimative, __string))

#endif