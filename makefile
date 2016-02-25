CC=gcc
CFLAGS=-Wall -g -std=c99 -c -lm
LDFLAGS=-std=c99
EXEC=hex
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

src/%.o: src/%.c
	$(CC) -o $@ $< $(CFLAGS)
	
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	
clean:
	rm -rf *.o $(EXEC)