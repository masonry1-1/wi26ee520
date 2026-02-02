CC=gcc
CFLAGS=-Wall -Wextra -std=c11

all: hw2

hw2: main.o unit_tests.o solutions.o
	$(CC) $(CFLAGS) -o hw2 main.o unit_tests.o solutions.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

unit_tests.o: unit_tests.c solutions.h
	$(CC) $(CFLAGS) -c unit_tests.c

solutions.o: solutions.c solutions.h
	$(CC) $(CFLAGS) -c solutions.c

clean:
	rm -f *.o hw2
