Cobbler: A C Object System
=======

Cobbler is an attempt to add objects to the C language in order to better understand the implementations of dynamic object systems such as that of Smalltalk and Objective-C.

Class definitions, known as `primitives`, are created using C Macros to describe the properties and behavior of objects. Memory management of instances is done with reference counting, using `own` and `disown`, and are capable of being assigned to a thread-specific pool of objects to automatically release at the end of some scope with `autodisown`.

## Basic Usage

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

