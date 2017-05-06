DEBUG=yes
CC = gcc

ifeq ($(DEBUG),yes)
	CFLAGS = -Wall -Werror -Wextra -std=c11 -g
else
	CFLAGS = -Wall -Werror -Wextra -std=c11
endif

LDFLAGS = -lm
EXEC = main
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

main: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< 
	
clean :
	rm *.o $(EXEC)
