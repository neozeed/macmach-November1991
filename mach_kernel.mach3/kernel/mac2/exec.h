/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	exec.h,v $
 * Revision 2.2  91/09/12  16:40:27  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:43:04  bohman]
 * 
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)exec.h	7.1 (Berkeley) 6/4/86
 */

#ifndef	_MAC2_EXEC_H_
#define _MAC2_EXEC_H_

/*
 * Header prepended to each a.out file.
 */
struct exec {
	unsigned short	a_machtype,	/* machine type and magic number */
			a_magic;
	unsigned long	a_text;		/* size of text segment */
	unsigned long	a_data;		/* size of initialized data */
	unsigned long	a_bss;		/* size of uninitialized data */
	unsigned long	a_syms;		/* size of symbol table */
	unsigned long	a_entry;	/* entry point */
	unsigned long	a_trsize;	/* size of text relocation */
	unsigned long	a_drsize;	/* size of data relocation */
};

#define OMAGIC	0407		/* old impure format */
#define NMAGIC	0410		/* read-only text */
#define ZMAGIC	0413		/* demand load format */

#define M_68020		2	/* sun compatable: 68020/30 */

#endif	_MAC2_EXEC_H_
