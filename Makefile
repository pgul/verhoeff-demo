CC = gcc
LINK = gcc
LIB = ar
RANLIB = ranlib
CFLAGS = -Wall -funsigned-char -o $@
CFLAGS += -g
CFLAGS += -DWITH_PTHREAD_MUTEX=1
#CFLAGS += -DDEBUG=1
LFLAGS = $(CFLAGS)

%.o:    %.c
	$(CC) -c $(CFLAGS) $<

all:	verhoeff

verhoeff:	verhoeff.o
	$(LINK) $(LFLAGS) -o $@ $<

