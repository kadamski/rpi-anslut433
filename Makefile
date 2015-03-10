LDFLAGS=-lwiringPi

.PHONY: all web clean

all: test

test: anslut.o test.c

web: test
	cd web && while true; do ./test.py; sleep 5; done

clean:
	rm anslut.o test
