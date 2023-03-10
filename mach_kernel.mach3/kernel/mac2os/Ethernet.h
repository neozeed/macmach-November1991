/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	Ethernet.h,v $
 * Revision 2.2  91/09/12  16:50:21  bohman
 * 	Created.
 * 	[91/09/11  16:28:05  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2os/Ethernet.h
 */

#ifndef _Ethernet_
#define _Ethernet_

/*
	cs codes
*/

#define ESetGeneral		253
#define EGetInfo		252
#define ERdCancel		251
#define ERead			250
#define EWrite			249
#define EDetachPH		248
#define EAttachPH		247
#define EAddMulti		246
#define EDelMulti		245


/*
	packet header info
*/

#define EDestAddr	0
#define ESrcAddr	6
#define EType		12
#define EHdrSize	14
#define EMinDataSz	46
#define EMaxDataSz	1500
#define EMaxPacketSz (EHdrSize + EMaxDataSz)
#define ELimitedSz	768
#define EAddrSz		6
#define MAddrSz		6


/*
	errors
*/

#ifndef inProgress
#define inProgress	1
#endif
#define eLenErr		-92
#define eMultiErr	-91


/*
	parameter structures
*/

typedef struct wdsEntry {
    unsigned short	length;
    Ptr			ptr;
} wdsEntry, *wdsPtr;

typedef struct {
    short		xxx;
    wdsPtr		EWdsPointer;
} EWritePB;

typedef struct EProtoPB {
    short		EProtType;
    ProcPtr		EHandler;
} EProtoPB;

typedef struct EReadPB {
    short		EProtType;
    Ptr			EBuffPtr;
    short		EBuffSize;
    short		EDataSize;
} EReadPB;

typedef struct {
    short		xxx;
    Ptr		EKillQEl;
} ERdCancelPB;

typedef struct EInfoPB {
    short		xxx;
    Ptr			EBuffPtr;
    short		EBuffSize;
} EInfoPB;

typedef struct {
    ether_address_t	EMultiAddr;
} EMultiPB;

typedef struct Eiopb {
    Ptr		qLink;
    short		qType;
    unsigned short	ioTrap;
#define	Immed	0x0200
#define Async	0x0400
    Ptr			ioCmdAddr;
    ProcPtr		ioCompletion;
    OSErr		ioResult;
    StringPtr		ioNamePtr;
    short		ioVRefNum;
    short		ioRefNum;
    short		csCode;
    union {
	EWritePB	write;
	EProtoPB	proto;
	EReadPB		read;
	ERdCancelPB	rdcancel;
	EInfoPB		info;
	EMultiPB	multi;
    } p;
} Eiopb;

#endif
