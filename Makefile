
IDIR=/usr/local/include
LDIR=/usr/local/lib
CC=gcc
# OBJ= somestuff.o other.o

CFLAGS=-L$(LDIR) -I$(IDIR) -lraylib -lm

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: main
main: main.o physics.o randomizer.o srs.o control.o
	$(CC) -o $@ $^ $(CFLAGS) 

clean: main main.o physics.o randomizer.o srs.o control.o
	rm -f $^
