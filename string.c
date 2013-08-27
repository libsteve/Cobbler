#include "string.h"

#include <string.h>

string *string_initialize(string *, va_list *args);
void string_destroy(string *);
string *string_copy(string *);

primative_class StringPrimative = {
	.size            = sizeof(struct string),
	.initialize      = (init_fn)&string_initialize,
	.destroy         = (destroy_fn)&string_destroy,
	.copy            = (copy_fn)*string_copy,
	.super_primative = &PrimativeClass
};

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