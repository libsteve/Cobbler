Cobbler: A C Object System
=======

Cobbler is an attempt to add objects to the C language in order to better understand the implementations of different kinds of object systems.

A primitive static object system is being developed using just C Macros and functions.

A dynamic object system library will also be created using a C framework.

## Goals

### A Static Primitive Object System

- To create a simple static object system with simple data inheritance using pure C.
- To better understand the usage of C Macros.
- To better understand implementations of static object systems (such as that of C++).
- To add a type of inherited struct that implements reference counting.

### A Dynamic Object System

- To create a simple dynamic object system framework with data and method inheritance using pure C.
- To better understand dynamic object system implementations that take advantage of message passing.
- To better understand meta-programming in dynamic object systems.

### Desired Features

- To implement ordered multiple inheritance in a dynamic object system.
- To add state machine behavior to a dynamic object system (such as that of the Plaid language).

## Progress

The primitive static object system is seeing progress. It has not yet seen any testing, but seems to compile fine... so far.
Implemented primitive objects include a linked-list primitive, string primitive, and a value/number primitive.
Primitive objects follow the reference counting memory management model, with a posix-compliant thread-safe autorelease implementation. (These are known as own, disown, and autodisown)

## Primitive Objects Usage

To define a new primitive object, use the `primitive_define()` macro function.
Pass in the name of the primitive, the super primitive, a structure to represent the data, and any virtual methods used. Virtual methods are added using the `using_virtual()` macro function after the structure is defined.

	primitive_define(my_primitive, primitive, {
			int a_number;
			char a_char;
		},
		virtual(my_primitive, create),
		virtual(my_primitive, destroy));

You can forward-declare the primitive using the `primitive_declare()` macro.
This can be useful when using the `method()` macro for declaring virtual methods for the primitive before you define the primitive itself.

	primitive_declare(my_primitive)

	my_primitive *method(my_primitive, create, int a_number, char a_char);
	void          method(my_primitive, destroy);
	int           method(my_primitive, getNumber);

	primitive_define(my_primitive, primitive, {
			int a_number;
			char a_char;
		},
		virtual(my_primitive, create),
		virtual(my_primitive, destroy),
		virtual(my_primitive, getNumber));

The `method()` macro can be used for defining non-virtual methods too.

	int method(my_primitive, non_virtual_method);

Virtual methods are called using the `virtual_call()` macro function, whereas non-virtual methods can either be called directly or through the `static_call()` macro function. 

The `virtual_call()` macro must be supplied with the return type, followed by the virtual method name, the primitive instance, and then finally any function arguments for the method.

The `static_call()` macro must be supplied with the primitive name, followed by the method name, the primitive instance, and then finally any function arguments for the method.

	my_primitive *p = create(my_primitive, 10, 'a');

	int result = virtual_call(int, getNumber, p);
	int other  = static_call(my_primitive, non_virtual_method, p);

