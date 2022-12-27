/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	DeskBus.h,v $
 * Revision 2.2  91/09/12  16:49:51  bohman
 * 	Created.
 * 	[91/09/11  16:25:31  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/DeskBus.h
 */

/************************************************************

Created: Thursday, September 7, 1989 at 3:33 PM
	DeskBus.h
	C Interface to the Macintosh Libraries


	Copyright Apple Computer, Inc.	1987 -1989
	All rights reserved

************************************************************/


#ifndef __DESKBUS__
#define __DESKBUS__

typedef struct ADBOpBlock {
    Ptr 	dataBuffPtr;		/*address of data buffer*/
    ProcPtr	opServiceRtPtr;	 	/*service routine pointer*/
    Ptr 	opDataAreaPtr;		/*optional data area address*/
} ADBOpBlock, *ADBOpBPtr;

typedef struct ADBDataBlock {
    char	devType;			/*device type*/
    char	origADBAddr;		/*original ADB Address*/
    Ptr		dbServiceRtPtr;	 	/*service routine pointer*/
    Ptr		dbDataAreaAddr; 		/*data area address*/
} ADBDataBlock, *ADBDBlkPtr;

typedef struct ADBSetInfoBlock {
    ProcPtr	siServiceRtPtr;	 	/*service routine pointer*/
    Ptr		siDataAreaAddr; 		/*data area address*/
} ADBSetInfoBlock, *ADBSInfoPtr;

#endif
