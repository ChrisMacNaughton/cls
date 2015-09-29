CC     = gcc
CFLAGS = -g
RM = rm -f

default: all

all: cls

cls: cls.c
	$(CC) $(CFLAGS) -o cls cls.c

clean veryclean:
	$(RM) cls

run: all
	./cls