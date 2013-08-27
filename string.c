#include "string.h"

#include <string.h>

string *string_initialize(string *, va_list *args);
void string_destroy(string *);
string *string_copy(string *);

define_primative_class_begin(string, StringPrimative, PrimativeClass)
	primative_class_using_initialize(string_initialize)
	primative_class_using_destroy(string_destroy)
	primative_class_using_copy(string_copy) 
define_primative_class_end(string)

string *string_initialize(string *str, va_list *args) {
	char *c_string = va_arg(*args, char *);
	str->length = strlen(c_string);
	str->string = strdup(c_string);
	return str;
}

void string_destroy(string *str) {
	free(str->string);
	PrimativeClass.destroy((primative *)str);
}

string *string_copy(string *str) {
	return (string *)make(StringPrimative, str->string);
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