/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	setroot.c,v $
 * Revision 2.2  91/09/12  16:43:42  bohman
 * 	Created.
 * 	[91/09/11  15:02:41  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/setroot.c
 */

char	*root_name = "hdXa";

get_root_device()
{
    extern	bootdev;

    if (root_name[2] == 'X')
	root_name[2] = '0' + ((bootdev >> 5) & 0x7);
}
