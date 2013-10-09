#include "Cobbler/number.h"

number *number_initWithInteger(number *num, int64_t integer) {
    return (number *)static_call(value, initializeWithType, (value *)num, &integer, sizeof(integer), pstring("int"));
}

number *number_initWithUnsignedInteger(number *num, uint64_t integer) {
    return (number *)static_call(value, initializeWithType, (value *)num, &integer, sizeof(integer), pstring("uint"));
}

number *number_initWithFloating(number *num, double floating) {
    return (number *)static_call(value, initializeWithType, (value *)num, &floating, sizeof(floating), pstring("float"));
}

number *number_initWithCharacter(number *num, char character) {
    return (number *)static_call(value, initializeWithType, (value *)num, &character, sizeof(character), pstring("char"));
}

number *number_initWithBoolean(number *num, bool boolean) {
    return (number *)static_call(value, initializeWithType, (value *)num, &boolean, sizeof(boolean), pstring("bool"));
}

#define value_getValue(type, val) (*((type *)((value *)val)->bytes))

int64_t number_integer(number *num) {
    if (static_call(string, isEqual, static_call(value, type, (value *)num), pstring("int")))
        return value_getValue(int64_t, num);
    return 0;
}

uint64_t number_unsignedInteger(number *num) {
    if (static_call(string, isEqual, static_call(value, type, (value *)num), pstring("uint")))
        return value_getValue(uint64_t, num);
    return 0;
}

double number_floating(number *num) {
    if (static_call(string, isEqual, static_call(value, type, (value *)num), pstring("float")))
        return value_getValue(double, num);
    return 0.0;
}

char number_character(number *num) {
    if (static_call(string, isEqual, static_call(value, type, (value *)num), pstring("char")))
        return value_getValue(char, num);
    return '\0';
}

bool number_boolean(number *num) {
    if (static_call(string, isEqual, static_call(value, type, (value *)num), pstring("bool")))
        return value_getValue(bool, num);
    return false;
}
