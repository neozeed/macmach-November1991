/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	quit.c,v $
 * Revision 2.2  91/04/11  11:09:15  mrt
 * 	Copied from libcs
 * 
 * Revision 1.3  90/12/11  17:58:02  mja
 * 	Add copyright/disclaimer for distribution.
 * 
 * Revision 1.2  88/12/13  13:52:41  gm0w
 * 	Rewritten to use varargs.
 * 	[88/12/13            gm0w]
 */
/*
 *  quit  --  print message and exit
 *
 *  Usage:  quit (status,format [,arg]...);
 *	int status;
 *	(... format and arg[s] make up a printf-arglist)
 *
 *  Quit is a way to easily print an arbitrary message and exit.
 *  It is most useful for error exits from a program:
 *	if (open (...) < 0) then quit (1,"Can't open...",file);
 *
 */

#include <stdio.h>
#include <varargs.h>

quit (status, fmt, va_alist)
int status;
char *fmt;
va_dcl
{
	va_list args;

	fflush(stdout);
	va_start(args);
	(void) vfprintf(stderr, fmt, args);
	va_end(args);
	exit(status);
}
