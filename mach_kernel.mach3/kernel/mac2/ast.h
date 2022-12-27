/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	ast.h,v $
 * Revision 2.2  91/09/12  16:38:39  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:21:19  bohman]
 * 
 * Revision 2.2  90/08/30  11:00:33  bohman
 * 	Created.
 * 	[90/08/29  10:57:27  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ast.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2_AST_H_
#define _MAC2_AST_H_

#include <kern/macro_help.h>

#define MACHINE_AST

#define	aston(n) \
MACRO_BEGIN						\
{							\
    register	s = spl7();				\
\
    current_thread_pcb()->pcb_ast |= TRACE_AST;		\
\
    splx(s);						\
}							\
MACRO_END

#define	astoff(n) \
MACRO_BEGIN						\
{							\
    register	s = spl7();				\
\
    current_thread_pcb()->pcb_ast &= ~TRACE_AST;	\
\
    splx(s);						\
}							\
MACRO_END

#endif _MAC2_AST_H_
