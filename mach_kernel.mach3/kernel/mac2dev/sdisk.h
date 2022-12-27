/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	sdisk.h,v $
 * Revision 2.2  91/09/12  16:47:39  bohman
 * 	Created.
 * 	[91/09/11  16:02:51  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/sdisk.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#define SD_PART_INFO	0

typedef struct {
    unsigned		parts;		/* num of partitions from map */
    unsigned		length;		/* of partition in blocks */
    unsigned char	name[32];
    unsigned char	type[32];
} sd_part_info_t;

#define SD_PART_INFO_COUNT	(sizeof (sd_part_info_t) / sizeof (int))

#define SD_REC_SIZE		512
#define SD_REC_SIZE_LOG2	9
