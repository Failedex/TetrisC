
IDIR=/usr/local/include
LDIR=/usr/local/lib
CC=gcc
OBJ= main.o physics.o randomizer.o srs.o control.o textdisplay.o

CFLAGS=-L$(LDIR) -I$(IDIR) -lraylib -lm

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: main
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

clean: $(OBJ)
	rm -f $^
