CC = gcc
LINK = gcc
LIB = ar
RANLIB = ranlib
CFLAGS = -Wall -funsigned-char -o $@
CFLAGS += -g
LFLAGS = $(CFLAGS)

%.o:    %.c
	$(CC) -c $(CFLAGS) $<

all:	verhoeff

verhoeff:	verhoeff.o
	$(LINK) $(LFLAGS) -o $@ $<

