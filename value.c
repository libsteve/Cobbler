#include "value.h"

void
method(value, destroy) {
    if (this) {
        if (this->bytes)
            free(this->bytes);
        if (this->type)
            disown(this->type);
        SuperDestroy(this);
    }
}

value *
method(value, copy) {
    if (this) {
        value *new = create(value);
        new = value_initializeWithType(new, this->bytes, this->size, this->type);
        return new;
    } else return NULL;
}

value *
method(value, initialize, void *bytes, size_t size) {
    if (this && this->bytes == NULL) {
        this->bytes = calloc(size, sizeof(char));
        for (int i=0; i<size; i++) {
            ((char *)this->bytes)[i] = ((char *)bytes)[i];
        }
        this->size = size;
    }
    return this;
}

value *
method(value, initializeWithType, void *bytes, size_t size, string *type) {
    this = static_call(value, initialize, this, bytes, size);
    if (this && this->type == NULL) {
        this->type = copy(type);
    }
    return this;
}

void *
method(value, value, size_t output size) {
    size_t out_size = 0;
    char *out_value = NULL;
    if (this) {
        if (this->bytes) {
            out_value = this->bytes;
            out_size = this->size;
        }
    }
    if (size) {
        *size = out_size;
    }
    return out_value;
}

string *
method(value, type) {
    if (this) {
        return this->type;
    }
    return NULL;
}
