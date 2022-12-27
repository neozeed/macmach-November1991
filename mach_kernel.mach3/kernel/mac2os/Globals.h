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
 * $Log:	Globals.h,v $
 * Revision 2.2  91/09/12  16:50:39  bohman
 * 	Created.
 * 	[91/09/11  16:29:59  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Globals.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Apple macOS Global variables
 * used by macmach
 */
#ifndef	_MAC2OS_GLOBALS_H_
#define	_MAC2OS_GLOBALS_H_

#define	G_MemTop	0x108	/* RAM highest addr+1 */
#define G_BufPtr	0x10c
#define G_Time		0x20c
#define G_DefVCBPtr	0x352
#define G_BootDrive	0x210
#define G_MemErr	0x220
#define G_ApplZone	0x2aa
#define G_HWCfgFlags	0xb22
#define G_hwCbFPU	12
#define G_hwCbAUX	9
#define G_MMU32bit	0xcb2
#define G_MMUInfoPtr	0xcb8
#define G_JSwapMMU	0xdbc
#define G_BootGlobPtr	0xddc

#ifndef ASSEMBLER
#define	VAR(name, type)	*((unsigned type *)G_##name)

#define MemTop		VAR(MemTop, long)
#define BufPtr		VAR(BufPtr, long)
#define ApplZone	VAR(ApplZone, long)
#define BootGlobPtr	VAR(BootGlobPtr, long)
#define MMUInfoPtr	VAR(MMUInfoPtr, long)
#define BootDrive	VAR(BootDrive, short)
#define DefVCBPtr	VAR(DefVCBPtr, long)
#endif

#endif	_MAC2OS_GLOBALS_H_
