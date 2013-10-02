#include "string.h"

#include <string.h>

string *string_initialize(string *str, char *c_string) {
	str->length = strlen(c_string);
	str->string = strdup(c_string);
	return str;
}

void string_destroy(string *str) {
	free(str->string);
	SuperDestroy(str);
}

string *string_copy(string *str) {
	return make(string, str->string);
}

char string_charAt(string *str, unsigned int index) {
	if (str->length == 0) {
		return '\0';
	}
	if (index >= str->length) {
		index = str->length - 1;
	}
	return str->string[index];
}

bool string_isEqual(string *str, string *other) {
	if (strcmp(str->string, other->string) == 0)
		return true;
	return false;
}