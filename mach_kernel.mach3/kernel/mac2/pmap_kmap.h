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
 * $Log:	pmap_kmap.h,v $
 * Revision 2.2  91/09/12  16:43:03  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:59:14  bohman]
 * 
 * Revision 2.2  90/08/30  11:02:44  bohman
 * 	Created.
 * 	[90/08/29  11:43:50  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pmap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2_PMAP_KMAP_H_
#define _MAC2_PMAP_KMAP_H_

/*
 * Describe special mappings
 * that must be entered in the
 * kernel pmap when mach is booted
 * on a 24 bit system.
 */
#define PMAP_ROM_COMPAT_ADDR	((vm_offset_t)0x00800000)
#define PMAP_ROM_COMPAT_SIZE	((vm_size_t)(1*1024*1024))

#define PMAP_IO_COMPAT_ADDR	((vm_offset_t)0x00f00000)
#define PMAP_IO_COMPAT_SIZE	((vm_size_t)(1*1024*1024))

#define PMAP_SLOT_COMPAT_ADDR(slot) \
    				((vm_offset_t)((slot) << 20))
#define PMAP_SLOT_COMPAT_SIZE	((vm_size_t)(1*1024*1024))

/*
 * Describe special mappings
 * that must be entered as
 * virt == phys in the kernel
 * pmap at startup.
 */
#define	PMAP_ROM_ADDR		((vm_offset_t)0x40000000)
#define PMAP_ROM_SIZE		((vm_size_t)(256*1024*1024))
#define PMAP_ROM_PROT		(VM_PROT_READ|VM_PROT_EXECUTE)

#define PMAP_IO_ADDR		((vm_offset_t)0x50000000)
#define PMAP_IO_SIZE		((vm_size_t)(256*1024*1024))
#define PMAP_IO_PROT		(VM_PROT_WRITE|VM_PROT_READ)

#define PMAP_SLOTS_ADDR		((vm_offset_t)0xf0000000)
#define PMAP_SLOTS_SIZE		((vm_size_t)(256*1024*1024))
#define PMAP_SLOTS_PROT		(VM_PROT_WRITE|VM_PROT_READ)

#define PMAP_SLOT_ADDR(slot)	((vm_offset_t)0xf0000000 | ((slot) << 24))
#define PMAP_SLOT_SIZE		((vm_size_t)(16*1024*1024))
#define PMAP_SLOT_PROT		(VM_PROT_WRITE|VM_PROT_READ)

#endif	_MAC2_PMAP_KMAP_H_
