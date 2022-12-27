/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	machine_routines.h,v $
 * Revision 2.2  91/09/12  16:41:40  bohman
 * 	Created.
 * 	[91/09/11  14:52:23  bohman]
 * 
 */

/*
 * Check for machine dependent
 * kernel IPC interfaces here.
 */
#define MACHINE_SERVER(in, out)	\
    (macserver_server((in), (out)))
