	CC = gcc
CFLAGS = -O2
LIBS = -lgmp

all:
	$(CC) $(CFLAGS) src/main.c -o pi $(LIBS)

run:
	./pi
