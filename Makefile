CC = gcc
CFLAGS = -Wall -g

all: main.o central.o
	$(CC) $(CFLAGS) -o all main.o central.o

main.o: main.c central.h
	$(CC) $(CFLAGS) -c main.c

central.o: central.c central.h
	$(CC) $(CFLAGS) -c central.c

clean:
	rm -f *.o *.exe *~ all
