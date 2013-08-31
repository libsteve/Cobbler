#include "number.h"

define_primative_class_begin(number, value)
define_primative_class_end(number)

number *number_initWithInteger(number *num, int64_t integer) {
	return (number *)value_initializeWithType((value *)num, &integer, sizeof(integer), PSTRING("int"));
}

number *number_initWithUnsignedInteger(number *num, uint64_t integer) {
	return (number *)value_initializeWithType((value *)num, &integer, sizeof(integer), PSTRING("uint"));
}

number *number_initWithFloating(number *num, double floating) {
	return (number *)value_initializeWithType((value *)num, &floating, sizeof(floating), PSTRING("float"));
}

number *number_initWithCharacter(number *num, char character) {
	return (number *)value_initializeWithType((value *)num, &character, sizeof(character), PSTRING("char"));
}

number *number_initWithBoolean(number *num, bool boolean) {
	return (number *)value_initializeWithType((value *)num, &boolean, sizeof(boolean), PSTRING("bool"));
}

#define value_getValue(type, val) (*((type *)((value *)val)->bytes))

int64_t number_integer(number *num) {
	if (string_isEqual(value_type((value *)num), PSTRING("int")))
		return value_getValue(int64_t, num);
	return 0;
}

uint64_t number_unsignedInteger(number *num) {
	if (string_isEqual(value_type((value *)num), PSTRING("uint")))
		return value_getValue(uint64_t, num);
	return 0;
}

double number_floating(number *num) {
	if (string_isEqual(value_type((value *)num), PSTRING("float")))
		return value_getValue(double, num);
	return 0.0;
}

char number_character(number *num) {
	if (string_isEqual(value_type((value *)num), PSTRING("char")))
		return value_getValue(char, num);
	return '\0';
}

bool number_boolean(number *num) {
	if (string_isEqual(value_type((value *)num), PSTRING("bool")))
		return value_getValue(bool, num);
	return false;
}