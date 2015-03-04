LDFLAGS=-lwiringPi

all: test

test: anslut.o test.c

clean:
	rm anslut.o test
