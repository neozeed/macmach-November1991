# Makefile for newsys.

PROG= newsys

CONFIG=	afs.andrew afs.macmach ftp.macmach disk.macmach macmach.proto

DESTDIR=

MANEXT=8

BINDIR=	${DESTDIR}/etc

CONFIGDIR= ${DESTDIR}/etc/newconfig

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

all:	${PROG}.sh

install:
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 ${CONFIG} ${CONFIGDIR}

install.man:
	@echo "*** need man page for newsys"
#	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
