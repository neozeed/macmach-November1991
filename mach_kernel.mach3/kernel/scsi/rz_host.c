/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	rz_host.c,v $
 * Revision 2.2  91/08/24  12:28:02  af
 * 	Created.
 * 	[91/08/02  03:47:02  af]
 * 
 */
/*
 *	File: rz_host.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	7/91
 *
 *	Top layer of the SCSI driver: interface with the MI.
 *	This file contains operations specific to CPU-like devices.
 *
 * We handle here the case of other hosts that are capable of
 * sophisticated host-to-host communication protocols, we make
 * them look like... you'll see.
 *
 * There are two sides of the coin here: when we take the initiative
 * and when the other host does it.  Code for handling both cases is
 * provided in this one file.
 */

#include <mach/std_types.h>
#include <machine/machparam.h>		/* spl definitions */
#include <scsi/compat_30.h>

#include <scsi/scsi.h>
#include <scsi/scsi_defs.h>
#include <scsi/rz.h>

/* Since we have invented a new "device" this cannot go into the
   the 'official' scsi_devsw table.  Too bad. */

extern char	*schost_name();
extern scsi_ret_t
		schost_open(), schost_close();
extern int	schost_strategy(), schost_start();

scsi_devsw_t	scsi_host = {
	schost_name, 0, schost_open, schost_close, schost_strategy,
	schost_start, 0, 0
};

char *schost_name(internal)
	boolean_t	internal;
{
	return internal ? "sh" : "host";
}

scsi_ret_t
schost_open(sc, tgt)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
{
	return SCSI_RET_SUCCESS;	/* XXX if this is it, drop it */
}

scsi_ret_t
schost_close(sc, tgt)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
{
	return SCSI_RET_SUCCESS;	/* XXX if this is it, drop it */
}

schost_strategy(ior)
	register io_req_t	ior;
{
	return D_DEVICE_DOWN;
}


schost_start( sc, tgt, done)
	scsi_softc_t	*sc;
	target_info_t	*tgt;
	boolean_t	done;
{
	io_req_t		head, ior = tgt->ior;
	scsi_ret_t		ret;

}

