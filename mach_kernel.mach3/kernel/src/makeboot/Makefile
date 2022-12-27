#
# Mach Operating System
# Copyright (c) 1990 Carnegie-Mellon University
# All rights reserved.  The CMU software License Agreement specifies
# the terms and conditions for use and redistribution.
#
#
# HISTORY
# $Log:	Makefile,v $
# Revision 2.2  91/05/08  13:09:21  dbg
# 	Created.
# 	[90/10/01            dbg]
# 
# Revision 2.1.1.1  91/02/26  11:16:58  dbg
# 	Created.
# 	[90/10/01            dbg]
# 

all:		makeboot.x

install:	makeboot.install

makeboot.install: $(DDIR)/makeboot

$(DDIR)/makeboot: makeboot.x
	cp -p makeboot.x $(DDIR)/makeboot
	strip $(DDIR)/makeboot
	chmod 755 $(DDIR)/makeboot

OBJS =	build_boot.o

# Base of source tree.  Look here for include files.
#IDIR = ../..

PMAXCFLAGS = -Dvoid=int
SUNCFLAGS = -Dvoid=int
#CFLAGS= $(CDEBUGFLAGS) $($(MACHINE)CFLAGS) -I$(IDIR)
CFLAGS= $(CDEBUGFLAGS) $($(MACHINE)CFLAGS)

makeboot.x: $(OBJS)
	$(CC) -o makeboot.x $(CFLAGS) $(OBJS) -lmach

.c.o:
	$(CC) -c -MD $(CFLAGS) $*.c
	md -u Makedep -d $*.d

OBJBASE = $(MAKECONF:h)/../../../obj/@sys/${BCSBBASE?$(BCSBBASE:t):latest}/kernel
BINBASE = $(MAKECONF:h)/../../../release/@sys/${BCSBBASE?$(BCSBBASE:t):latest}/bin

release:
	-if [ -f $(BINBASE)/makeboot ]; then true; else ln -s $(OBJBASE)/bin/makeboot $(BINBASE); fi

-include Makedep
