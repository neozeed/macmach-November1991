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
 * $Log:	mac2_init.c,v $
 * Revision 2.2  91/09/12  16:41:09  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  14:46:40  bohman]
 * 
 * Revision 2.2  90/09/04  17:22:12  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/mac2_init.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>
#include <mach/vm_param.h>

vm_offset_t	avail_start, avail_end;
vm_offset_t	virtual_avail, virtual_end;

int	cold;

mac2_init()
{
    page_size = MAC2_PGBYTES;
    vm_set_page_size();

    /*
     * Setup the interrupt vector
     * table.
     */
    ivect_tbl = (struct ivect *)avail_start;
    avail_start += MAC2_PGBYTES;

    ivect_init();
    set_vector_base(ivect_tbl);

    /*
     * Setup the kernel pmap and
     * switch to the kernel translation
     * tree.  After this returns, we can
     * no longer do MacOS calls.
     */
    pmap_bootstrap();

    /*
     * Initialize the interface to the
     * macintosh operating system code.
     * After this returns, we can again
     * do MacOS calls.
     */
    os_interface_init();

    /*
     * Find the console screen and
     * initialize it enough so that
     * kernel printfs work.
     */
    cons_printf_init();
}
