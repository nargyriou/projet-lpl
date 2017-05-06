DEBUG=yes
CC = gcc

ifeq ($(DEBUG),yes)
	CFLAGS = -Wall -Werror -Wextra -std=c11 -g
else
	CFLAGS = -Wall -Werror -Wextra -std=c11
endif

LDFLAGS = -lm -lcurses
EXEC = main
INCLUDES = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)

all: $(EXEC)

main: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c -Iinclude -o $@ $< 
obj .:
	@mkdir -p obj

.PHONY: clean
clean :
	rm obj/* $(EXEC)

.PHONY: tests
tests:
	tests/runtests.sh $(TESTS)