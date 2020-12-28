CC=g++
CFLAGS=-g -Wall

all: test 

test:
	$(CC) $(CFLAGS) -I./inc/ src/* main.cpp -o test -lX11

clean:
	rm test
