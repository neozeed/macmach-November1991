# Makefile for mach_init, Mach 2.5 version.

PROG= mach_init

DESTDIR=

SERVERS= ../../usr/include/servers.mach2

MANEXT=8

BINDIR=${DESTDIR}/etc

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS= -o bin -g bin -m 544

CFLAGS= -O -MD

SRCS=	main.c service.c

CGENS=	serviceServer.c

OBJS=	main.o service.o serviceServer.o

LIBS=	-lmach

all: ${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

serviceServer.c: ${SERVERS}/service.defs
	mig $? -header service.h -server $@ -user /dev/null

install:
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man:
	@echo "*** need man pages for mach_init"
#	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG} ${CGENS}

.depend: ${SRCS} ${CGENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend
