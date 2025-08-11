CC := cc
CFLAGS := -Wall -Wextra -std=c11 -O2 -g

TARGET := central
OBJS := main.o central.o

.PHONY: all obj run clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c central.h
	$(CC) $(CFLAGS) -c main.c -o $@

central.o: central.c central.h
	$(CC) $(CFLAGS) -c central.c -o $@

# compila apenas o módulo 2 (BST) sem linkar
obj: central.o

run: $(TARGET)
	./$(TARGET)

test: run

clean:
	rm -f $(TARGET) $(OBJS)


