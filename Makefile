TARGET=ddo1
CC?=clang
#CFLAGS=-I./lib -Wall -Wextra -Wpedantic -lSDL2
CFLAGS=-I./lib -Wall -Wextra -lSDL2
CFILES=./src/*.c

default:
	$(CC) $(CFLAGS) -o $(TARGET) $(CFILES)
clean:
	rm -f $(TARGET)
debug:
	$(CC) $(CFLAGS) -o $(TARGET) -g $(CFILES)
run: default
	./$(TARGET)
all: default
