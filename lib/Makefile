#	$Id: Makefile.in,v 1.2 2001/08/10 15:36:57 sandro Exp $

srcdir = .


CC = cc
CFLAGS = -g -I.. -ansi -funroll-loops -finline-functions -O3 

LIBS = 
RANLIB = ranlib

LIB = libsisp.a
OBJS = sisp$U.o xmalloc$U.o xstrdup$U.o xrealloc$U.o xcalloc$U.o

.c.o:
	$(CC) -c -I. -I$(srcdir) $(CFLAGS) $<

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

$(OBJS): sisp.h
