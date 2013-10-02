#include "value.h"

void value_destroy(value *v) {
	if (v) {
		if (v->bytes)
			free(v->bytes);
		if (v->type)
			disown(v->type);
		SuperDestroy(v);
	}
}

value *value_copy(value *v) {
	if (v) {
		value *new = make(value);
		new = value_initializeWithType(new, v->bytes, v->size, v->type);
		return new;
	} else return NULL;
}

primitive_class_define(value, primitive, 
	using_destroy(value_destroy),
	using_copy(value_copy));

value *value_initialize(value *v, void *bytes, size_t size) {
	if (v && v->bytes == NULL) {
		v->bytes = calloc(size, sizeof(char));
		for (int i=0; i<size; i++) {
			((char *)v->bytes)[i] = ((char *)bytes)[i];
		}
		v->size = size;
	}
	return v;
}

value *value_initializeWithType(value *v, void *bytes, size_t size, string *type) {
	v = value_initialize(v, bytes, size);
	if (v && v->type == NULL) {
		v->type = copy(type);
	}
	return v;
}

void *value_value(value *v, size_t output size) {
	size_t out_size = 0;
	char *out_value = NULL;
	if (v) {
		if (v->bytes) {
			out_value = v->bytes;
			out_size = v->size;
		}
	}
	if (size) {
		*size = out_size;
	}
	return out_value;
}

string *value_type(value *v) {
	if (v) {
		return v->type;
	}
	return NULL;
}