flags=-O2 -Wall -std=c2x
# This ldflag only works if you have birchutils.h installed in the system.
# Find more: https://youtu.be/0OcmaLP-tT8?si=4yxgNJQLJqfLBtYd
ldflags=-lbu

.PHONY: all clean

all: clean ritzvm

ritzvm: ritzvm.o
	cc $(flags) $^ -o $@ $(ldflags)

ritzvm.o: ritzvm.c ritzvm.h
	cc $(flags) -c $<

clean:
	rm -f *.o ritzvm
