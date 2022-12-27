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
 * $Log:	if_en.h,v $
 * Revision 2.2  91/09/12  16:49:29  bohman
 * 	Created.
 * 	[91/09/11  16:18:08  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2if/if_en.h
 */

typedef unsigned char	ether_address_t[6];

typedef struct {
    ether_address_t	dest;
    ether_address_t	src;
    unsigned short	type;
} ether_header_t;

#define EN_ENBL_PROTO		1

typedef unsigned	en_enbl_proto_t;

#define EN_ADD_MULTI		2
#define EN_DEL_MULTI		3

typedef unsigned	en_address_status_t[2];

#define EN_ADDRESS_STATUS_COUNT	2

