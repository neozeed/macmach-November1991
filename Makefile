# Makefile for /usr/src.

# Do "make MACH=mach2" to build a Mach 2.5 system.
# Do "make MACH=mach3" to build a Mach 3.0 system.
MACH=	mach3
#MACH=	mach2

# The /usr/tmp partition should have at least 250MB free.

# A system build consists of three steps.  First, a temporary
# compile environment is created with PATH, CPATH and LPATH set
# to the local environment.  Next, the final compile environment
# is built using the temporary compile environment.  The final
# environment replaces the temporary compile environment.  The
# third step is building and installing the entire system using
# the final compile environment.

# Use "make debug" to enter a shell inside the object tree.
# Use "make config", "make clean" and "make unconfig" inside the source tree.

# This is the Mach 3.0 version string.
version.mach3=${target_machine} version 01; bsd4.3-tahoe; Mach 3.0

# This is the Mach 2.5 version string.
version.mach2=${target_machine} version 01; bsd4.3-tahoe; Mach 2.5

# This is the destination tree where things are installed.
DESTDIR= /usr/tmp/${target_machine}.${MACH}

# The source tree is normally accessed read-only.
# Do "make config" to make it possible to actually compile within the
# source tree.  Use "make unconfig" to undo "make config".
SRCDIR=	/usr/src

# This is the object tree where things are normally created.
OBJDIR=	/usr/tmp/${target_machine}.obj.${MACH}

# This is a temporary tree for the initial and final compile environment.
TMP=/usr/tmp/${target_machine}.tmp.${MACH}
MKINC=${TMP}/usr/include
MKLIB=${TMP}/usr/lib

# Define this to include motif.
MOTIF=motif

# These are all of the things that make up a system.
# note: usr/include sets protection modes for all of usr/include
# note: usr/man creates whatis database from all of usr/man
THINGS=	lib usr/lib X11 ${MOTIF} bin usr/bin usr/ucb usr/old etc \
	usr/include mach_kernel mach_servers vmunix usr/man

# These are standard system directories in ${DESTDIR} and ${TMP}.
MKDIR=	/tmp /lib /bin /etc /etc/newconfig /dev /usr \
	/usr/etc /usr/tmp \
	/usr/include /usr/include/X11 \
	/usr/bin /usr/bin/X11 \
	/usr/lib /usr/lib/X11 /usr/lib/learn /usr/lib/tmac \
	/usr/games /usr/games/lib /usr/games/hide \
	/usr/man /usr/man/man1 /usr/man/man2 /usr/man/man3 /usr/man/man4 \
	/usr/man/man5 /usr/man/man6 /usr/man/man7 /usr/man/man8 \
	/usr/man/man.old /usr/man/manx /usr/man/info /usr/man/doc \
	/usr/man/doc/X11 /usr/ucb /usr/spool /usr/adm /usr/old /usr/users

# These are things to configure as mach2 or mach3.
CONFIG= . lib/libc/${target_machine}/csu etc/etc.${target_machine} bin/adb \
	lib usr/lib bin usr/bin usr/ucb usr/old etc usr/include

# These are the things that make up the temporary compile environment.
CPATH=	${TMP}/lib/gcc-include:${TMP}/usr/include:
LPATH=	${TMP}/lib:${TMP}/usr/lib
PATH=	${TMP}/etc:${TMP}/bin:${TMP}/usr/bin:${TMP}/usr/ucb:${TMP}/usr/old
BOOT=	usr/bin/mig usr/include lib/gcc lib/libc usr/lib/libdbm \
	usr/lib/libln usr/lib/libm usr/lib/libtermcap etc/chown \
	bin/ar bin/as bin/awk bin/cat bin/chmod bin/cmp bin/cp \
	bin/csh bin/date bin/echo bin/ed bin/expr bin/false bin/hostname \
	bin/ld bin/ln bin/ls bin/machine bin/make bin/md bin/mkdir \
	bin/mv bin/nm bin/pwd bin/rm bin/sed bin/sh bin/size bin/strip \
	bin/tar bin/test bin/true usr/bin/basename usr/bin/bison \
	usr/bin/find usr/bin/fixit usr/bin/join usr/bin/lex usr/bin/lorder \
	usr/bin/mkdep usr/bin/ranlib usr/bin/sort usr/bin/touch \
	usr/bin/tsort usr/bin/uucp usr/bin/wh usr/bin/xinstall usr/bin/yacc \
	usr/ucb/ex usr/ucb/grep usr/ucb/what usr/ucb/xstr

# The initial compile environment is built from the local environment.
CPATH1=/lib/gcc-include:/usr/include
LPATH1=/lib:/usr/lib
PATH1=/etc:/bin:/usr/bin:/usr/ucb:/usr/old

# The final compile environment is augmented by the system being built.
CPATH2=	${DESTDIR}/usr/include
LPATH2=	${DESTDIR}/lib:${DESTDIR}/usr/lib
PATH2=	${DESTDIR}/usr/bin:${DESTDIR}/usr/ucb:${DESTDIR}/usr/old

# The default operation is to make a compile environment and build a system.
all:	${MACH} ${TMP}/initial_stamp ${TMP}/final_stamp
	@echo "=== Building `cat ${OBJDIR}/VERSION`"
	@echo "=== `cat ${TMP}/initial_stamp`"
	@echo "=== `cat ${TMP}/final_stamp`"
	@-cd ${OBJDIR}; for i in ${THINGS}; do [ -d $$i ] && ( \
	  CPATH="${CPATH2}:${CPATH}"; \
	  LPATH="${LPATH2}:${LPATH}"; \
	  PATH="${PATH2}:${PATH}"; \
	  MKINC="${MKINC}"; MKLIB="${MKLIB}"; \
	  export CPATH LPATH PATH MKINC MKLIB; \
	  echo "=== Building $$i"; \
	  cd $$i; \
	    make -k; \
	    make -k DESTDIR=${DESTDIR} install.man; \
	    make -k DESTDIR=${DESTDIR} install; \
	); done
	@echo "=== Creating ${DESTDIR}/.fixit"; \
	  fixit -update ${DESTDIR} \
	    < ${OBJDIR}/etc/etc.${target_machine}/root.fixit \
	    >${DESTDIR}/.fixit
	@echo "=== ${MACH} build ${DESTDIR} complete."

# Run a shell in the object tree with the compile environment for debugging.
debug:	${MACH} ${TMP}/initial_stamp ${TMP}/final_stamp
	-@echo "Entering ${MACH} compile environment."; \
	CPATH="${CPATH2}:${CPATH}"; \
	LPATH="${LPATH2}:${LPATH}"; \
	PATH="${PATH2}:${PATH}"; \
	MKINC="${MKINC}"; MKLIB="${MKLIB}"; \
	DESTDIR="${DESTDIR}"; \
	OBJDIR="${OBJDIR}"; \
	export CPATH LPATH PATH MKINC MKLIB DESTDIR OBJDIR; \
	echo "DESTDIR=$$DESTDIR"; \
	echo "OBJDIR=$$OBJDIR"; \
	cd ${OBJDIR}; \
	sh
	@echo "Leaving ${MACH} compile environment."

# Catch make with MACH not defined.
nomach:	FRC
	@echo "MACH must be defined as mach2 or mach3."
	@false

# Set the Mach 2.5 version string in the object directory.
mach2:	${SRCDIR} ${OBJDIR} ${DESTDIR} ${TMP}
	@echo "\"${version.mach2}\"" >${OBJDIR}/VERSION

# Set the Mach 3.0 version string in the object directory.
mach3:	${SRCDIR} ${OBJDIR} ${DESTDIR} ${TMP}
	@echo "\"${version.mach3}\"" >${OBJDIR}/VERSION

# Set the Mach 2.5 version string in the source directory.
srcver.mach2: FRC
	@echo "\"${version.mach2}\"" >${SRCDIR}/VERSION

# Set the Mach 3.0 version string in the source directory.
srcver.mach3: FRC
	@echo "\"${version.mach3}\"" >${SRCDIR}/VERSION

# Build an initial compile environment in the temporary tree.
# Use the local environment and part of the initial as it is built.
${TMP}/initial_stamp:
	@echo "=== Creating initial environment from local environment"; \
	  CPATH="${CPATH}:${CPATH1}"; \
	  LPATH="${LPATH}:${LPATH1}"; \
	  PATH="${PATH}:${PATH1}"; \
	  export CPATH LPATH PATH; \
	  ( cd ${OBJDIR}/usr/include; make DESTDIR=${TMP} mach ); \
	  for i in ${BOOT}; do \
	    echo "=== Building initial $$i, `date`"; \
	    cd ${OBJDIR}/$$i; \
	    make clean; \
	    make DESTDIR=${TMP}; \
	    make DESTDIR=${TMP} install; \
	    make clean; \
	  done
	@echo "Initial environment created `date`" >${TMP}/initial_stamp

# Build a final compile environment in the temporary tree.
# Use the initial environment and part of the final as it is built.
${TMP}/final_stamp:
	@echo "=== Creating final environment from initial environment"; \
	  CPATH="${CPATH}"; LPATH="${LPATH}"; PATH="${PATH}"; \
	  export CPATH LPATH PATH; \
	  for i in ${BOOT}; do \
	    echo "=== Building final $$i, `date`"; \
	    cd ${OBJDIR}/$$i; \
	      make clean; \
	      make DESTDIR=${TMP}; \
	      make DESTDIR=${TMP} install; \
	      make clean; \
	  done
	@echo "=== Preparing X11 and MOTIF"; \
	  CPATH="${CPATH}"; LPATH="${LPATH}"; PATH="${PATH}"; \
	  export CPATH LPATH PATH; \
	  cd ${OBJDIR}/X11/config; make imake; \
	  cd ${OBJDIR}/X11; \
	    make Makefile; make Makefiles; make includes; make depend; \
	  [ -d ${OBJDIR}/motif ] && { \
	    cd ${OBJDIR}/motif/config; make imake; \
	    cd ${OBJDIR}/motif; \
	      make Makefile; make Makefiles; make includes; make depend; \
	  }
	@echo "Final environment created `date`" >${TMP}/final_stamp

# Create object directory of symbolic links into the source tree.
# Configure by creating symbolic links to *.mach2 or *.mach3.
${OBJDIR}:
	@echo "=== Creating object tree ${OBJDIR}"
	fixit -ln ${SRCDIR} ${OBJDIR} >/dev/null
	-@for i in ${CONFIG}; do ( \
	  cd ${OBJDIR}/$$i; \
	  for j in *.mach2; do \
	    [ "$$j" != "*.mach2" ] && rm -f `basename $$j .mach2`; \
	  done; \
	  for j in *.mach3; do \
	    [ "$$j" != "*.mach3" ] && rm -f `basename $$j .mach3`; \
	  done; \
	  for j in *.${MACH}; do \
	    [ "$$j" != "*.${MACH}" ] && { \
	      echo "=== Using ${MACH} version of $$i/`basename $$j .${MACH}`."; \
	      ln -s $$j `basename $$j .${MACH}`; \
	    }; \
	  done; \
	); done

# Create all standard directories in the destination tree.
${DESTDIR}:
	@echo "=== Creating destination tree ${DESTDIR}"
	-@[ -d ${DESTDIR} ] || mkdir ${DESTDIR}
	@chown root.bin ${DESTDIR}; chmod 755 ${DESTDIR}
	-@for i in ${MKDIR}; do \
	  [ -d ${DESTDIR}$$i ] || mkdir ${DESTDIR}$$i; \
	  chown root.bin ${DESTDIR}$$i; chmod 755 ${DESTDIR}$$i; \
	done
	@chmod 777 ${DESTDIR}/tmp ${DESTDIR}/usr/tmp
	@chown games.bin ${DESTDIR}/usr/games/lib ${DESTDIR}/usr/games/hide

# Create all standard directories in the temporary tree.
${TMP}:
	@echo "=== Creating temporary tree ${TMP}"
	-@[ -d ${TMP} ] || mkdir ${TMP}
	@chown root.bin ${TMP}; chmod 755 ${TMP}
	-@for i in ${MKDIR}; do \
	  [ -d ${TMP}$$i ] || mkdir ${TMP}$$i; \
	  chown root.bin ${TMP}$$i; chmod 755 ${TMP}$$i; \
	done
	@chmod 777 ${TMP}/tmp ${TMP}/usr/tmp
	@chown games.bin ${TMP}/usr/games/lib ${TMP}/usr/games/hide

# Unconfigure the source, remove symbolic links to *.mach2 and *.mach3.
# The source tree must be read/write to do this.
unconfig: ${SRCDIR} FRC
	@echo "=== Removing mach2 or mach3 configuration from ${SRCDIR}."
	@rm -f ${SRCDIR}/VERSION
	-@for i in ${CONFIG}; do ( \
	  cd ${SRCDIR}/$$i; \
	  for j in *.mach2; do \
	    [ "$$j" != "*.mach2" ] && rm -f `basename $$j .mach2`; \
	  done; \
	  for j in *.mach3; do \
	    [ "$$j" != "*.mach3" ] && rm -f `basename $$j .mach3`; \
	  done; \
	); done

# Configure the source tree, creating symbolic links to *.mach2 or *.mach3.
# The source tree must be read/write to do this.
config:	unconfig srcver.${MACH}
	@echo "=== Configuring ${SRCDIR} for ${MACH}."
	-@for i in ${CONFIG}; do ( \
	  cd ${SRCDIR}/$$i; \
	  for j in *.${MACH}; do \
	    [ "$$j" != "*.${MACH}" ] && { \
	      echo "=== Using ${MACH} version of $$i/`basename $$j .${MACH}`."; \
	      ln -s $$j `basename $$j .${MACH}`; \
	    }; \
	  done; \
	); done

# Clean the source tree.
# The source tree must be read/write to do this
clean:	${SRCDIR}
	@echo "=== Cleaning ${SRCDIR}."
	@-cd ${SRCDIR}; for i in ${THINGS}; do [ -d $$i ] && ( \
	  echo "=== Cleaning $$i, `date`"; \
	  cd $$i; make -k clean; \
	); done

FRC:
