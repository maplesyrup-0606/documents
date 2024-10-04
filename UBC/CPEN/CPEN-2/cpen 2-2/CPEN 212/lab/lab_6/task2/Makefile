CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
OBJ = cpen212vm.o main.o

all: cpen212vm

cpen212vm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

cpen212vm.o: cpen212vm.c cpen212vm.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c cpen212vm.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f cpen212vm $(OBJ)
