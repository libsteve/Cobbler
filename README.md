Cobbler: A C Object System
=======

Cobbler is an attempt to add objects to the C language in order to better understand the implementations of different kinds of object systems.

A primitive static object system is being developed using just C Macros and functions.

A dynamic object system library will also be created using a C framework.

##Goals

###A Static Primitive Object System

- To create a simple static object system with simple data inheritance using pure C.
- To better understand the usage of C Macros.
- To better understand implementations of static object systems (such as that of C++).
- To add a type of inherited struct that implements reference counting.

###A Dynamic Object System

- To create a simple dynamic object system framework with data and method inheritance using pure C.
- To better understand dynamic object system implementations that take advantage of message passing.
- To better understand meta-programming in dynamic object systems.

###Desired Features

- To implement ordered multiple inheritance in a dynamic object system.
- To add state machine behavior to a dynamic object system (such as that of the Plaid language).

##Progress

The primitive static object system is seeing progress. It has not yet seen any testing, but seems to compile fine... so far.
Implemented primitive objects include a linked-list primitive, string primitive, and a value/number primitive.
Primitive objects follow the reference counting memory management model, with a posix-compliant thread-safe autorelease implementation. (These are known as own, disown, and autodisown)

##Things to come

- Tests for the primitive static object system.
- A dynamic object system framework, implemented using the primitive object system.
