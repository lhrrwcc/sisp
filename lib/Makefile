srcdir = .
CC = cc
CFLAGS = -ansi -funroll-loops -finline-functions -O3 

LIBS = 
RANLIB = ranlib

LIB = libsisp.a
OBJS = libsisp$U.o 

.c.o:
	$(CC) -c -I.. -I. -I$(srcdir) $(CFLAGS) $<

all: $(LIB)

$(LIB): $(OBJS)
	rm -f $(LIB)
	ar rc $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

.PHONY: install uninstall clean realclean distclean mostlyclean

install:

uninstall:

clean: 
	-rm -f $(LIB) *.o *.a *.*~

distclean: clean 
	-rm -f Makefile

realclean: distclean

mostlyclean: clean

$(OBJS): libsisp.h
