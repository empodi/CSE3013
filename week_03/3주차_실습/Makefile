CC = g++
CXXFLAGS = -W -Wall
TARGET = main_exe
OBJECTS = main.o array.o rangearray.o

$(TARGET) : $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm *.o main_exe

