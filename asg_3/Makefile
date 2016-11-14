CC=gcc
CFLAGS=-std=gnu89 -Wall -lpthread
PROG=smallsh
CODE=$(PROG).c
OBJ=$(PROG).o

all:$(PROG)

$(PROG):
	$(CC) -o $(PROG) $(CODE) $(CFLAGS)
clean:
	rm  $(PROG)
