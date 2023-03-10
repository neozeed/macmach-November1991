/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie Mellon University
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
 * $Log:	ipc_kserver.c,v $
 * Revision 2.5  91/08/28  11:16:03  jsb
 * 	As a hack to avoid printfs from i860ipsc/spl.c,
 * 	defined sploff/splon as splsched/splx.
 * 	[91/08/27  21:59:34  jsb]
 * 
 * 	Renamed clport things to norma things.
 * 	[91/08/15  09:11:36  jsb]
 * 
 * Revision 2.4  91/08/03  18:19:22  jsb
 * 	Replaced spldcm/splx with sploff/splon.
 * 	[91/07/28  20:52:22  jsb]
 * 
 * 	Removed obsolete includes and vm and kmsg munging operations.
 * 	[91/07/17  14:14:11  jsb]
 * 
 * 	Moved MACH_MSGH_BITS_COMPLEX_{PORTS,DATA} to mach/message.h.
 * 	[91/07/04  13:12:09  jsb]
 * 
 * 	Use vm_map_copy_t page_lists instead of old style page_lists.
 * 	[91/07/04  10:20:35  jsb]
 * 
 * Revision 2.3  91/07/01  08:25:30  jsb
 * 	Changes for new vm_map_copy_t definition.
 * 	[91/06/29  16:38:27  jsb]
 * 
 * Revision 2.2  91/06/17  15:47:44  jsb
 * 	Moved here from ipc/ipc_clkobject.c.
 * 	[91/06/17  11:05:35  jsb]
 * 
 * Revision 2.2  91/06/06  17:05:23  jsb
 * 	First checkin.
 * 	[91/05/24  13:10:00  jsb]
 * 
 */
/*
 *	File:	norma/ipc_kserver.c
 *	Author:	Joseph S. Barrera III
 *	Date:	1991
 *
 *	Functions to support ipc between nodes in a single Mach cluster.
 */

#include <vm/vm_kern.h>
#include <vm/vm_page.h>
#include <mach/vm_param.h>
#include <mach/port.h>
#include <mach/message.h>
#include <kern/assert.h>
#include <kern/host.h>
#include <kern/sched_prim.h>
#include <kern/ipc_sched.h>
#include <kern/ipc_kobject.h>
#include <kern/zalloc.h>
#include <ipc/ipc_mqueue.h>
#include <ipc/ipc_thread.h>
#include <ipc/ipc_kmsg.h>
#include <ipc/ipc_port.h>
#include <ipc/ipc_pset.h>
#include <ipc/ipc_space.h>
#include <ipc/ipc_marequest.h>

#define	NUM_KSERVER_THREADS	4

#if	i860
#define	sploff	splsched
#define	splon	splx
#endif

/*
 * Problems:
 *	Kserver_awake should be used but isn't.
 *	We used to replenish here; should we still?
 *	Kernel_kmsg list is ugly; we should use a queue.
 */

ipc_kmsg_t kernel_kmsg = (ipc_kmsg_t) 0;
boolean_t kserver_awake = FALSE;
int kserver_awaken = 0;

/*
 * Service request, perhaps blocking; send reply, if any.
 */
norma_kserver_deliver(kmsg)
	ipc_kmsg_t kmsg;
{
	ipc_port_t port;

	kmsg = ipc_kobject_server(kmsg);
	if (kmsg != IKM_NULL) {
		ipc_kmsg_copyin_from_kernel(kmsg);
#if 1
		port = (ipc_port_t) kmsg->ikm_header.msgh_remote_port;
		if (port->ip_receiver == ipc_space_remote) {
			(void) norma_ipc_send(kmsg);
		} else {
			ipc_mqueue_send_always(kmsg);
		}
#else
		ipc_mqueue_send_always(kmsg);
#endif
	}
}

void
kserver_continue()
{
	int s;
	ipc_kmsg_t kmsg;

	for (;;) {
		kserver_awaken++;

		s = sploff();
		while (kernel_kmsg) {
			kmsg = kernel_kmsg;
			kernel_kmsg = kmsg->ikm_next;
			splon(s);
			norma_kserver_deliver(kmsg);
			s = sploff();
		}

		kserver_awake = FALSE;
		assert_wait((int) &kserver_awake, FALSE);
		(void) splon(s);
		thread_block(kserver_continue);
	}
}

void
kserver_thread()
{
	int s;
	
	thread_set_own_priority(0);	/* high priority */

	s = sploff();
	kserver_awake = FALSE;
	assert_wait((int) &kserver_awake, FALSE);
	(void) splon(s);

	thread_block(kserver_continue);
	kserver_continue();
	/*NOTREACHED*/
}

norma_ipc_kobject_send(kmsg)
	ipc_kmsg_t kmsg;
{
	if (kernel_kmsg) {
		ipc_kmsg_t km;
		for (km = kernel_kmsg; km->ikm_next; ) {
			km = km->ikm_next;
		}
		km->ikm_next = kmsg;
	} else {
		kernel_kmsg = kmsg;
	}
	kmsg->ikm_next = 0;
#if 0
	if (! kserver_awake) {
		thread_wakeup_one((int) &kserver_awake);
	}
#else
	thread_wakeup_one((int) &kserver_awake);
#endif
	return KERN_SUCCESS;
}

norma_kserver_startup()
{
	int i;

	for (i = 0; i < NUM_KSERVER_THREADS; i++) {
		(void) kernel_thread(kernel_task, kserver_thread);
	}
}
