srcdir = .


DESTDIR =
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
mandir = ${prefix}/man/man1

INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL = /usr/bin/install -c

CC = cc
CFLAGS = -Wall -I$(srcdir)/.. -I$(srcdir)/
LDFLAGS = -lm
LIBS =   

LEX = @LEX@
YACC = @YACC@

PROG = sisp
OBJS = main.o lexer.o parser.o funcs.o object.o misc.o eval.o
.c.o:
	$(CC) -c -I$(srcdir) $(CFLAGS) $<

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJS) $(LIBS)

.PHONY: install uninstall clean realclean distclean mostlyclean

install: $(PROG)
	$(srcdir)/../mkinsdir.sh $(DESTDIR)$(bindir) $(DESTDIR)$(mandir)
	$(INSTALL_PROGRAM) $(PROG) $(DESTDIR)$(bindir)
	$(INSTALL_DATA) $(srcdir)/$(PROG).1 $(DESTDIR)$(mandir)/$(PROG).1

uninstall:
	rm -f $(DESTDIR)$(bindir)/$(PROG)
	rm -f $(DESTDIR)$(mandir)/$(PROG).1

clean: 
	-rm -f $(PROG) *.o *.*~

distclean: clean 
	-rm -f Makefile

realclean: distclean

mostlyclean: clean

$(OBJS): sisp.h extern.h  
