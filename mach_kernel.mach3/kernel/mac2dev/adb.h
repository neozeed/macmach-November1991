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
 * $Log:	adb.h,v $
 * Revision 2.2  91/09/12  16:45:48  bohman
 * 	Created.
 * 	[91/09/11  15:24:40  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/adb.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2DEV_ADB_H_
#define _MAC2DEV_ADB_H_

#define ADB_INFO	0

typedef struct {
    unsigned char	addr;	/* original address */
    unsigned char	type;
} adb_info_t;

#define ADB_INFO_COUNT	(sizeof (adb_info_t) / sizeof (int))

#define ADB_FLUSH	1

typedef union {
    struct {
	unsigned char	cmd;
	unsigned char	len;
    } cmd;
    struct {
	unsigned char	addr:4,
			 cmd:4;
#define ADB_reset	0
#define ADB_flush	1
    } gen;
    struct {
	unsigned char	addr:4,
			 cmd:2,
#define ADB_listen	2
#define ADB_talk	3
			 reg:2;
    } reg;
} adb_cmd_t;

#define ADB_ADDR_KEYBOARD	2
#define ADB_ADDR_MOUSE		3

#endif	_MAC2DEV_ADB_H_
