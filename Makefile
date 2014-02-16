CC = clang

###
# Cobbler source file rules

SRC = $(wildcard src/*.c)
OBJ = $(addprefix obj/, $(notdir $(SRC:.c=.o)))

###
# Cobbler demo file rules

examples 		= $(wildcard examples/*c)
examples_OBJ 	= $(addprefix obj/, $(notdir $(SRC:.c=.o)))
examples_X		= $(examples:.c=)

###
# Compiler Flags

CFLAGS = -I ./include -std=gnu11 -Wall -Werror
LFLAGS = 

LIBS = -lpthread

###
# Compile the Cobbler libraries

DYNAMIC	= Cobbler.so
STATIC	= Cobbler.a

all: $(DYNAMIC) $(STATIC)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

$(STATIC): $(OBJ)
	$(AR) -rcs $@ $(OBJ)

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj:
	mkdir -p obj

###
# Compile the Cobbler examples

examples: $(examples_X)

examples/%: examples/%.c $(STATIC) | obj
	$(CC) $< $(STATIC) $(CFLAGS) $(LIBS) -o $@

###
# Clean

clean:
	rm -Rf $(OBJ) $(examples_OBJ) $(STATIC) $(DYNAMIC)

realclean: clean
	rm -Rf ./obj $(examples_X)
