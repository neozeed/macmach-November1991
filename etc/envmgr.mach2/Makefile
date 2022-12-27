# Makefile for envmgr, Mach 2.5 version.

PROG=	envmgr

SERVERS= ../../usr/include/servers.mach2

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD -I${SERVERS}

IFLAGS=	-o root -g bin -m 544

SRCS=	envmgrmain.c envprocs.c

CGENS=	env_mgrServer.c

OBJS=	envmgrmain.o envprocs.o env_mgrServer.o

LIBS=	-lmach

all:	${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

env_mgrServer.c: ${SERVERS}/env_mgr.defs
	mig -server $@ -header /dev/null -user /dev/null $?

install:
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man:
	@echo "*** need man pages for envmgr"
#	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG} ${CGENS}

.depend: ${SRCS} ${CGENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend
