# Makefile for Mach 3.0 mach_servers.

DESTDIR=

STARTUP= ux

THINGS=	mach_init ux

all:	FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Building $$i."; \
	  cd $$i; make; \
	) done

install: FRC
	-[ -d ${DESTDIR}/mach_servers ] || mkdir ${DESTDIR}/mach_servers
	chown root.bin ${DESTDIR}/mach_servers
	chmod 555 ${DESTDIR}/mach_servers
	@for i in ${THINGS}; do ( \
	  echo "=== Installing $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install; \
	) done
	cd ${DESTDIR}/mach_servers; rm -f startup; ln ${STARTUP} startup

install.man: FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Installing man pages for $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install.man; \
	) done

clean:	FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Cleaning $$i."; \
	  cd $$i; make clean; \
	) done

FRC:
