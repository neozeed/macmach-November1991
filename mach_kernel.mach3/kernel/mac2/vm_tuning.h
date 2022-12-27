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
 * $Log:	vm_tuning.h,v $
 * Revision 2.2  91/09/12  16:45:18  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  15:14:30  bohman]
 * 
 * Revision 2.2  90/08/30  11:03:47  bohman
 * 	Created.
 * 	[90/08/29  12:03:46  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/vm_tuning.h
 */

#ifndef _MAC2_VM_TUNING_H_
#define _MAC2_VM_TUNING_H_

#define VM_PAGE_INACTIVE_TARGET(free)	((free) / 3)

#endif	_MAC2_VM_TUNING_H_
