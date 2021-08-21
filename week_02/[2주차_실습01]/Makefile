CC = gcc
CFLAGS = -W -Wall
TARGET = animal_exe
OBJECTS = dog.o blackcow.o turtle.o main.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean :
	rm *.o animal_exe
