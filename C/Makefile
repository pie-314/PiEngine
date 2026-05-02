CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lgmp -lmpfr

TARGET = piengine
SRC = \
	  src/main.c \
	  src/chudnovsky.c \
	  src/cli.c


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) src/*.o
