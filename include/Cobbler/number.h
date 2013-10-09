#ifndef NUMBER_H
#define NUMBER_H

#include "primitive.h"
#include "value.h"

primitive_define(number, value, {});

number *number_initWithInteger        (number *num, int64_t  integer);
number *number_initWithUnsignedInteger(number *num, uint64_t integer);
number *number_initWithFloating       (number *num, double   floating);
number *number_initWithCharacter      (number *num, char     character);
number *number_initWithBoolean        (number *num, bool     boolean);

int64_t  number_integer        (number *num);
uint64_t number_unsignedInteger(number *num);
double   number_floating       (number *num);
char     number_character      (number *num);
bool     number_boolean        (number *num);

#endif
