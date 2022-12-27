/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
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
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	cthreads.h,v $
 * Revision 2.4  91/05/14  17:59:25  mrt
 * 	Correcting copyright
 * 
 * Revision 2.3  91/02/14  14:21:26  mrt
 * 	Added new Mach copyright
 * 	[91/02/13  12:39:26  mrt]
 * 
 * Revision 2.2  90/11/05  14:38:00  rpd
 * 	Created.
 * 	[90/11/01            rwd]
 * 
 */

#ifndef _MACHINE_CTHREADS_H_
#define _MACHINE_CTHREADS_H_

typedef int spin_lock_t;
#define SPIN_LOCK_INITIALIZER 0
#define spin_lock_init(s) *(s)=0
#define spin_lock_locked(s) (*(s) != 0)

#endif _MACHINE_CTHREADS_H_
