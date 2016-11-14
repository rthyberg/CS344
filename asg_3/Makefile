CC=gcc
CFLAGS=-std=gnu89 -Wall -lpthread
PROG=thybergr.adventure
CODE=$(PROG).c
OBJ=$(PROG).o
DIR=thybergr.rooms.*

all:$(PROG)

$(PROG):
	$(CC) -o $(PROG) $(CODE) $(CFLAGS)
clean:
	rm  $(PROG)
	rm -r $(DIR)
