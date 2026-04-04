CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lgmp
SRC = src/main.c src/cli.c src/utils.c 
OBJ = $(SRC:.c=.o)
TARGET = piengine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) src/main.c -o pi $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

run:
	./piengine
