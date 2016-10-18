CC=gcc
CFLAGS= -Wall -lpthread
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
