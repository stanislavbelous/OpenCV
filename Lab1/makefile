CC=g++
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`
INCLUDE = -I "=opencv"
SOURCE = main.cpp
FLAGS = -o
FILE = "main"
all:
	$(CC) $(CFLAGS) $(INCLUDE) $(SOURCE) $(LDFLAGS) $(FLAGS) $(FILE)

clean:
	rm $(FILE)


