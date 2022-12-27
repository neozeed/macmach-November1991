/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	Disks.h,v $
 * Revision 2.2  91/09/12  16:49:59  bohman
 * 	Created.
 * 	[91/09/11  16:26:26  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Disks.h
 */

/************************************************************

Created: Thursday, September 7, 1989 at 3:51 PM
	Disks.h
	C Interface to the Macintosh Libraries


	Copyright Apple Computer, Inc.	 1985-1989
	All rights reserved

************************************************************/


#ifndef __DISKS__
#define __DISKS__

struct DrvSts {
	short track;
	char writeProt;
	char diskInPlace;
	char installed;
	char sides;
	struct QElem *qLink;
	short qType;
	short dQDrive;
	short dQRefNum;
	short dQFSID;
	char twoSideFmt;
	char needsFlush;
	short diskErrs;
};

typedef struct DrvSts DrvSts;
struct DrvSts2 {
	short track;
	char writeProt;
	char diskInPlace;
	char installed;
	char sides;
	struct QElem *qLink;
	short qType;
	short dQDrive;
	short dQRefNum;
	short dQFSID;
	short driveSize;
	short driveS1;
	short driveType;
	short driveManf;
	short driveChar;
	char driveMisc;
};

typedef struct DrvSts2 DrvSts2;

#endif
