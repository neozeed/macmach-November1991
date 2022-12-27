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
 * $Log:	conf.c,v $
 * Revision 2.2  91/09/12  16:39:29  bohman
 * 	Created.
 * 	[91/09/11  14:26:30  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/conf.c
 */

#include <device/conf.h>

extern int	block_io_mmap();

extern int	sd_open(), sd_close(), sd_read(), sd_write();
extern int	sd_getstat();
#define sd_name		"sd"

extern int	adb_open(), adb_close(), adb_read();
extern int	adb_getstat(), adb_setstat();
#define adb_name	"adb"

extern int	cnopen(), cnclose(), cnread(), cnwrite();
extern int	cngetstat(), cnsetstat(), cnportdeath();
#define cnname		"cons"

extern int	timeopen(), timeclose(), timeread(), timemmap();
#define timename	"time"

extern int	mhdopen(), mhdread(), mhdwrite();
#define mhdname		"hd"

extern int	enopen(), enoutput();
extern int	ensetinput(), engetstat(), ensetstat();
#define enname		"en"

extern int	video_open(), video_close(), video_mmap();
extern int	video_getstat(), video_setstat();
#define video_name	"video"

extern int	sony_open(), sony_close(), sony_rw();
extern int	sony_getstat(), sony_setstat();
#define sony_name	"sony"

extern int	kernmmap(), kerngetstat(), kernsetstat();
#define kernname	"kern"

/*
 * List of devices - console must be at slot 0
 */
struct dev_ops	dev_name_list[] =
{
    /*
        name,		open,		close,		read,
        write,		getstat,	setstat,	mmap,
        async_in,	reset,		port_death,	subdev,
	dev_info,
    */
    {
	cnname,		cnopen,		cnclose,	cnread,
	cnwrite,	cngetstat,	cnsetstat,	0,
	nodev,		nulldev,	cnportdeath,	0,
	nodev,
    },
    {
	adb_name,	adb_open,	adb_close,	adb_read,
	nodev,		adb_getstat,	adb_setstat,	0,
	nodev,		nulldev,	nulldev,	1,
	nodev,
    },
    {
	sd_name,	sd_open,	sd_close,	sd_read,
 	sd_write,	sd_getstat,	nulldev,	block_io_mmap,
	nodev,		nulldev,	nulldev,	0x20000000 /* XXX */,
	nodev,
    },
    {
	timename,	timeopen,	timeclose,	timeread,
	nulldev,	nulldev,	nulldev,	timemmap,
	nodev,		nulldev,	nulldev,	0,
	nodev,
    },
    {
	mhdname,	mhdopen,	nulldev,	mhdread,
	mhdwrite,	nulldev,	nulldev,	block_io_mmap,
	nodev,		nulldev,	nulldev,	32,
	nodev,
    },
    {
	enname,		enopen,		nulldev,	nodev,
	enoutput,	engetstat,	ensetstat,	0,
	ensetinput,	nulldev,	nulldev,	0,
	nodev,
    },
    {
	video_name,	video_open,	video_close,	nodev,
	nodev,		video_getstat,	video_setstat,	video_mmap,
	nodev,		nulldev,	nulldev,	1,
	nodev,
    },
    {
	sony_name,	sony_open,	sony_close,	sony_rw,
	sony_rw,	sony_getstat,	sony_setstat,	block_io_mmap,
	nodev,		nulldev,	nulldev,	1,
	nodev,
    },
    {
	kernname,	nulldev,	nulldev,	nodev,
	nodev,		kerngetstat,	kernsetstat,	kernmmap,
	nodev,		nulldev,	nulldev,	0,
	nodev,
    },
};

int	dev_name_count =
    sizeof (dev_name_list) / sizeof (dev_name_list[0]);

/*
 * Indirect list.
 */
struct dev_indirect dev_indirect_list[] = {
    /* console */
    {
	"console",	&dev_name_list[0],		0,
    },
};

int	dev_indirect_count =
    sizeof (dev_indirect_list) / sizeof (dev_indirect_list[0]);
