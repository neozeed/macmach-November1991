# /usr/src/ucb/w/Makefile for MacMach

DESTDIR=

CFLAGS= -O

SRCS=	w.c

OBJS=	w.o

all:	w .depend

w:	${OBJS}
	$(CC) $(CFLAGS) -o $@ ${OBJS} -lmach

install: FRC
	install -c -s -o bin -g bin -m 755 w ${DESTDIR}/usr/ucb/w

install.man: FRC
	install -c -o bin -g bin -m 644 uptime.man ${DESTDIR}/usr/man/man1/uptime.1
	install -c -o bin -g bin -m 644 w.man ${DESTDIR}/usr/man/man1/w.1
	rm -f ${DESTDIR}/usr/ucb/uptime
	ln -s w ${DESTDIR}/usr/ucb/uptime

clean:	FRC
	rm -f .depend ${OBJS} core w

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:

