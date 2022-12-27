# Makefile for netmsgserver lib, Mach 2.5 version.

CFLAGS=	-O -MD

SRCS=	vmtplib.c forward.S invoke.S recvreq.S getreply.S probeentity.S \
	sendreply.S

OBJS=	vmtplib.o forward.o invoke.o recvreq.o getreply.o probeentity.o \
	sendreply.o

all:	libvmtp.a .depend

libvmtp.a: ${OBJS}
	@ar cu libvmtp.a ${OBJS}
	ranlib libvmtp.a

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} libvmtp.a

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
