/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	Retrace.h,v $
 * Revision 2.2  91/09/12  16:51:00  bohman
 * 	Created.
 * 	[91/09/11  16:31:35  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Retrace.h
 */

#ifndef _RETRACE_
#define _RETRACE_

typedef struct {
    Ptr			qLink;
    unsigned short	qType;
    void		(*vblAddr)();
    unsigned short	vblCount;
    unsigned short	vblPhase;
} VBLTask, *VBLTaskPtr;

#endif
