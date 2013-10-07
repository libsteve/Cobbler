#include "string.h"

#include <string.h>

string *
method(string, create, const char *c_string) {
    this->length = strlen(c_string);
    this->str = strdup(c_string);
    return this;
}

void 
method(string, destroy) {
    free(this->str);
    SuperDestroy(primitive, this);
}

string *
method(string, copy) {
    return create(string, this->str);
}

char
method(string, charAt, unsigned int index) {
    if (this->length == 0) {
        return '\0';
    }
    if (index >= this->length) {
        index = this->length - 1;
    }
    return this->str[index];
}

bool
method(string, isEqual, string *other) {
    if (strcmp(this->str, other->str) == 0)
        return true;
    return false;
}

const char *
method(string, c_string) {
    return this->str;
}
