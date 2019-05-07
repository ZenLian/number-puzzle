CC=g++
CFLAGS=-lncurses -std=c++11

all: puzzle

puzzle: puzzle.cc
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f puzzle
