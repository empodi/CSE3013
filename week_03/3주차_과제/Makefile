CC = g++
CXXFLAGS = -W -Wall
TARGET = main_exe
OBJECTS = String.o main.o

$(TARGET):$(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm *.o main_exe
