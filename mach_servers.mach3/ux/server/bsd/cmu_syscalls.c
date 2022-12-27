/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	cmu_syscalls.c,v $
 * Revision 2.5  90/08/07  22:41:28  rpd
 * 	One more twist for mips arguments since execve aligns
 * 	them on a 16-byte boundary.  Bug report from gm@osf.
 * 	[90/08/03            af]
 * 	Corrected conditionals and include file references.
 * 	[88/08/23  00:54:03  mwyoung]
 * 
 * 	Correct use of vm_map_copy in table(TBL_ARGUMENTS, ...) to
 * 	account for USRSTACK not being page-aligned.
 * 	[88/08/16  23:33:17  mwyoung]
 * 
 * Revision 2.4  90/06/02  15:21:08  rpd
 * 	More updates for new IPC.
 * 	[90/05/03            rpd]
 * 	Added TBL_BOOTTIME, TBL_MACHFACTOR.
 * 	Fixed TBL_LOADAVG to use host_info.
 * 	[90/04/27            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:30:10  rpd]
 * 
 * Revision 2.3  90/03/14  21:25:18  rwd
 * 	Change table to get avenrun from host_load call.
 * 	[90/01/28            rwd]
 * 
 * Revision 2.2  89/11/29  15:27:11  af
 * 	out-of-kernel version...
 * 	[89/01/02            dbg]
 * 
 * Revision 2.1  89/08/04  14:05:23  rwd
 * Created.
 * 
 * Revision 2.7  88/12/19  02:30:59  mwyoung
 * 	Remove lint.
 * 	[88/12/08            mwyoung]
 * 
 * Revision 2.6  88/10/18  03:13:42  mwyoung
 * 	Be alert for kmem_alloc_wait returning zero, indicating that the
 * 	request can never succeed.  [The case in which this regularly
 * 	applies is the TBL_ARGUMENTS call, where the user may request
 * 	an absurd amount of data.]
 * 	[88/09/26            mwyoung]
 * 
 * Revision 2.5  88/08/29  23:58:34  mwyoung
 * 	Corrected conditionals and include file references.
 * 	
 * 	Corrected include file references.
 * 	
 * 	Correct use of vm_map_copy in table(TBL_ARGUMENTS, ...) to
 * 	account for USRSTACK not being page-aligned.
 * 
 * Revision 2.4.1.1.1.1  88/08/23  00:46:01  mwyoung
 * Corrected include file references.
 * 
 * Revision 2.4  88/07/15  15:16:23  mja
 * Fix tl_avenrun reference for new union type.
 * 
 * 15-Mar-88  David Golub (dbg) at Carnegie-Mellon University
 *	MACH:  added TBL_PROCINFO to avoid having to read proc table
 *	from /dev/kmem.  Fixed table call to not return EINVAL if at
 *	least one element has been read.
 *
 *  3-Mar-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Removed more lint.
 *
 * 19-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Collapse MACH conditionals.
 *
 *  3-Oct-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	De-linted.
 *
 * 29-Sep-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Replaced uses of sizeof(sigcode) with SIGCODE_SIZE.
 *
 * 18-Sep-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	De-linted.
 *
 * 17-Aug-87  David Golub (dbg) at Carnegie-Mellon University
 *	Use kernel_pageable_map instead of user_pt_map in table() to
 *	avoid deadlocks.
 *
 * 10-Jul-87  David Golub (dbg) at Carnegie-Mellon University
 *	Use reference count on task (MACH) or map (not TT) to keep a
 *	task's map from vanishing during the TBL_UAREA and TBL_ARGUMENTS
 *	calls.
 *
 * 30-May-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Split out ttyloc and fsparam portions of table call to simplify
 *	it (the vax compiler really has problem groking this file,
 *	hopefully this will put an end to the problem).  While I was at
 *	it, I reformatted the whole file.
 *
 * 26-May-87  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	MACH: Fixed the TBL_UAREA call to allocate a struct user from
 *	kernel data space rather than on the kernel stack.  Also, adjust
 *	the pointers in the TBL_ARGUMENTS call down over the sigcode (which
 *	now resides above the user stack) to keep ps and friends happy.
 *	I may have even got it right this time.
 *
 *  3-May-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Deleted some dead code.
 *
 * 15-Apr-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Add TBL_LOADAVG to retrieve load average vector and scale
 *	factor.
 *	[ V5.1(F8) ]
 *
 * 17-Mar-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Restored xutimes() call that we mistakenly thought could go
 *	away after the 4.2 directory conversion.  It is needed in
 *	general by the restore program to do a better job of recovering
 *	the original file system state from a dump.
 *	[ V5.1(F7) ]
 *
 *  8-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Use new fake_u call for MACH.
 *
 *  2-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Removed dead SPIN_LOCK code, simplified table() a bit to prevent
 *	compiler schain botch.
 *
 *  1-Mar-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Updated for latest u-area hacks.
 *
 * 27-Jan-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	Implemented rpause() system call;  revised table() call: to
 *	permit set as well as get operation and slightly optimized
 *	vm_map_remove() exit path, to support new TBL_AID, TBL_MODES
 *	(to eventually replace [gs]etaid() and [gs]etmodes()) and
 *	TBL_MAXUPRC options, and to fix minor bug which neglected to
 *	check element length in single element only tables; simplified
 *	setmodes() call (in anticipation of its eventual removal).
 *	[ V5.1(F1) ]
 *
 *  5-Jan-87  David Golub (dbg) at Carnegie-Mellon University
 *	Fixed the hack in TBL_UAREA to copy wired-down U areas, now that
 *	the vm_map code is fixed (if !SPIN_LOCKS).  De-linted.
 *
 *  7-Nov-86  David Golub (dbg) at Carnegie-Mellon University
 *	Added TBL_ARGUMENTS, also for MACH only.
 *	Fixed TBL_UAREA call to give back the U-area itself, not the
 *	block of pages with the U-area and kernel-stack somewhere in it.
 *	This hides the difference between the romp (which puts its U-area
 *	above the kernel stack) and the other machines (which put it
 *	below the kernel stack).
 *	This gets lots easier when MACH is in use.
 *
 * 16-Aug-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Fixed test condition in the "copy u area" loop to
 *	use byte units (i.e. removed "atop" usage).
 *
 *  7-Aug-86  David Golub (dbg) at Carnegie-Mellon University
 *	Merge with official 4.3 release.
 *
 * 29-Jul-86  David Golub (dbg) at Carnegie-Mellon University
 *	vm_deallocate doesn't work on kernel maps... use vm_map_remove
 *	instead.  XXX
 *
 * 28-Jul-86  David Golub (dbg) at Carnegie-Mellon University
 *	Added TBL_UAREA to table() call to get U area for process.  For
 *	now, it only works under the MACH virtual memory.  May be
 *	slightly flaky if thread is unwired while looking at U area.
 *
 * 25-Jul-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added TBL_INCLUDE_VERSION and TBL_FSPARAM.
 *
 * 14-Jul-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	CS_AID: conditionalized setaid() and getaid().  Added include of
 *	cs_aid.h
 *
 * 25-Apr-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_GENERIC:  Added PRIVMODES variable which contains the
 *	privileged UMODE_* modes which only the super-user may set
 *	and otherwise may not be cleared.
 *
 * 23-Mar-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Removed includes of ../vax/mtpr.h and ../vax/reg.h.
 *
 * 03-Mar-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	Change privilege restrictions in setmodes() to prevent some
 *	modes from ever being disabled once set (UMODE_NEWDIR in
 *	particular).
 *
 * 21-Feb-86  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added temporary xutimes() call to allow the super-user to
 *	also set the inode change time.
 *	[V1(1)]
 *
 * 30-Aug-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added U_TTYD table.
 *	[V1(1)]
 *
 * 30-Mar-83  Mike Accetta (mja) at Carnegie-Mellon University
 *	Added table() system call.  We only need this to retrieve the
 *	terminal location information at the moment but the interface
 *	has been designed with a general purpose mechanism in mind.
 *	It may still need some work (V3.06h).
 *
 * 22-Jun-82  Mike Accetta (mja) at Carnegie-Mellon University
 *	Created (V3.05a).
 *
 **********************************************************************
 */
#include <cmucs.h>
#include <mach.h>

#include <mach_no_kernel.h>
#if	CMUCS

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/buf.h>
#include <sys/inode.h>
#include <sys/file.h>
#include <sys/proc.h>
#include <sys/conf.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/mount.h>
#include <sys/fs.h>
#include <sys/kernel.h>
#include <sys/table.h>

#if	MACH
#if	MACH_NO_KERNEL
#include <uxkern/import_mach.h>
#else	MACH_NO_KERNEL
#include <vm/vm_user.h>
#include <vm/vm_map.h>
#include <vm/vm_kern.h>
#include <vm/vm_param.h>
#include <sys/task.h>
#endif	MACH_NO_KERNEL
#include <machine/vmparam.h>	/* only way to find the user stack (argblock) */
#endif	MACH

#include <sys/version.h>

/*
 *  Set account ID of process (deprecated).
 */

setaid()
{
	register struct a {
		int aid;
	} *uap;

	uap = (struct a *)u.u_ap;
	if (suser()) {
		u.u_aid = uap->aid;
	}
}

/*
 *  Get account ID of process (deprecated).
 */

getaid()
{
	u.u_r.r_val1 = u.u_aid;
}

/*
 *  Set process modes (deprecated).
 */

setmodes()
{
	register struct a {
		int modes;
	} *uap;

	uap = (struct a *)u.u_ap;
	u.u_modes = uap->modes;
}

/*
 *  Get process modes (deprecated).
 */

getmodes()
{
	u.u_r.r_val1 = (u_char)u.u_modes;
}

#if	CMUCS
/*
 *  rpause - resource pause control
 *
 *  errno  = error number of interest
 *  etype  = type of error (used to distinguish between different
 *	     cases of resource exhaustion which all map to the
 *	     same error number)
 *  action = {SAME, ENABLE, DISABLE}
 *
 *  Return: the error type(s) currently enabled for the specified number.
 *
 *  N.B.  This routine is overly general for the current implementation
 *  which supports only ENOSPC and fails to distinguish among the different
 *  error types.
 */

rpause()
{
	register struct a {
		int errno;	/* error number */
		int etype;	/* type of error */
		int action;	/* action to take */
	} *uap;
	int bits, oldbits;

	uap = (struct a *)u.u_ap;
	switch (uap->errno) {
	case ENOSPC:
		if (uap->etype == RPAUSE_ALL) {
			bits = URPS_NOSPC;
			break;
		}
		/* fall through */
	default:
		u.u_error = EINVAL;
		return;
	}

	oldbits = (u.u_rpause&bits);
	switch (uap->action) {
	case RPAUSE_ENABLE:
		u.u_rpause |= bits;
		break;
	case RPAUSE_DISABLE:
		u.u_rpause &= ~bits;
		break;
	case RPAUSE_SAME:
		break;
	default:
		u.u_error = EINVAL;
		return;
	}
	u.u_r.r_val1 = oldbits?RPAUSE_ALL:0;
}
#endif	CMUCS



/*
 *  xutimes() - extended utimes() call to update inode creation time
 *
 *  This call is identical to the normal utimes() call except that it expects
 *  three elements in the time value array rather than two.  If the caller is
 *  privileged, the third element is used to set the inode creation time of the
 *  corresponding inode.  If the caller is not privileged, the operation
 *  performs the same funtion as the standard utimes() call to update only the
 *  access and modification times.
 */

xutimes()
{
	register struct a {
		char	*fname;
		struct	timeval *tptr;
	} *uap = (struct a *)u.u_ap;
	register struct inode *ip;
	struct timeval tv[3];

	if ((ip = owner(uap->fname, FOLLOW)) == NULL)
		return;
	if (ip->i_fs->fs_ronly) {
		u.u_error = EROFS;
		iput(ip);
		return;
	}
	u.u_error = copyin((caddr_t)uap->tptr, (caddr_t)tv, sizeof (tv));
	if (u.u_error == 0) {
		if (u.u_uid == 0) {
			ip->i_flag &= ~ICHG;
			ip->i_ctime = tv[2].tv_sec;
			ip->i_flag |= IACC|IUPD|IMOD;
		}
		else
			ip->i_flag |= IACC|IUPD|ICHG;
		iupdat(ip, &tv[0], &tv[1], 0);
	}
	iput(ip);
}

/*
 *  table - get/set element(s) from system table
 *
 *  This call is intended as a general purpose mechanism for retrieving or
 *  updating individual or sequential elements of various system tables and
 *  data structures.
 *
 *  One potential future use might be to make most of the standard system
 *  tables available via this mechanism so as to permit non-privileged programs
 *  to access these common SYSTAT types of data.
 *
 *  Parameters:
 *
 *  id		= an identifer indicating the table in question
 *  index	= an index into this table specifying the starting
 *		  position at which to begin the data copy
 *  addr	= address in user space to receive/supply the data
 *  nel		= number of table elements to retrieve/update
 *  lel		= expected size of a single element of the table.  If this
 *		  is smaller than the actual size, extra data will be
 *		  truncated from the end.  If it is larger, holes will be
 *		  left between elements copied to/from the user address space.
 *
 *		  The intent of separately distinguishing these final two
 *		  arguments is to insulate user programs as much as possible
 *		  from the common change in the size of system data structures
 *		  when a new field is added.  This works so long as new fields
 *		  are added only to the end, none are removed, and all fields
 *		  remain a fixed size.
 *
 *  Returns:
 *
 *  val1	= number of elements retrieved/updated (this may be fewer than
 *		  requested if more elements are requested than exist in
 *		  the table from the specified index).
 *
 *  Note:
 *
 *  A call with lel == 0 and nel == MAXSHORT can be used to determine the
 *  length of a table (in elements) before actually requesting any of the
 *  data.
 */

#define	MAXLEL	(sizeof(long))	/* maximum element length (for set) */

table()
{
	register struct a {
		int id;
		int index;
		caddr_t addr;
		int nel;	/* >0 ==> get, <0 ==> set */
		u_int lel;
	} *uap = (struct a *)u.u_ap;
	caddr_t data;
	unsigned size;
	int error = 0;
	int set;
#if	MACH
	vm_offset_t	arg_addr;
	vm_size_t	arg_size;
	int		*ip;
	int		*lastp;
	struct proc	*p;
	vm_offset_t	copy_start, copy_end;
	vm_size_t	copy_size;
	vm_offset_t	dealloc_start;	/* area to remove from kernel map */
	vm_size_t	dealloc_end;
#endif	MACH

	/*
	 *  Verify that any set request is appropriate.
	 */
	set = 0;
	if (uap->nel < 0) {
		switch (uap->id) {
		case TBL_AID:
		case TBL_MAXUPRC:
			if (! suser())
				return;
			/* fall through */
		case TBL_MODES:
		        break;
		default:
			u.u_error = EINVAL;
			return;
		}
		set++;
		uap->nel = -(uap->nel);
	}

	u.u_r.r_val1 = 0;

	/*
	 *  Verify common case of only current process index.
	 */
	switch (uap->id) {
	case TBL_AID:
	case TBL_MODES:
	case TBL_U_TTYD:
	case TBL_MAXUPRC:
		if ((uap->index != u.u_procp->p_pid && uap->index != 0) ||
				(uap->nel != 1))
			goto bad;
		break;
	}
	/*
	 *  Main loop for each element.
	 */

	while (uap->nel > 0) {
		extern mach_port_t host_port;
		host_load_info_data_t load_info;
		unsigned int count;
		struct tbl_fsparam tf;
		dev_t nottyd;
		int iv;
		struct tbl_loadavg tl;
		struct tbl_machfactor tmf;
		struct tbl_procinfo tp;
#if	MACH_NO_KERNEL
		struct ttyloc tloc;
#endif	MACH_NO_KERNEL
#if	MACH
		dealloc_start = (vm_offset_t) 0;
#endif	MACH
		switch (uap->id) {
#if	CMUCS
		case TBL_TTYLOC:
#if	MACH_NO_KERNEL
			if (major(uap->index) >= nchrdev)
			    goto bad;
			if ((*cdevsw[major(uap->index)].d_ioctl)
				     (uap->index,
				      TIOCGLOC,
				      (caddr_t)&tloc,
				      0) != 0)
			    goto bad;
			data = (caddr_t)&tloc;
			size = sizeof(struct ttyloc);
			break;
#else	MACH_NO_KERNEL
			if (table_ttyloc(uap->index, &data, &size))
				break;
			goto bad;
#endif	MACH_NO_KERNEL
#endif	CMUCS
		case TBL_U_TTYD:
			if (u.u_ttyp)
				data = (caddr_t)&u.u_ttyd;
			else {
				nottyd = -1;
				data = (caddr_t)&nottyd;
			}
			size = sizeof (u.u_ttyd);
			break;
		case TBL_LOADAVG:
			if (uap->index != 0 || uap->nel != 1)
				goto bad;

			count = HOST_LOAD_INFO_COUNT;
			(void) host_info(host_port, HOST_LOAD_INFO,
					 &load_info, &count);

			bcopy((char *) load_info.avenrun,
			      (char *) tl.tl_avenrun.l,
			      sizeof tl.tl_avenrun.l);
			tl.tl_lscale = LOAD_SCALE;

			data = (caddr_t)&tl;
			size = sizeof (tl);
			break;
		case TBL_MACHFACTOR:
			if (uap->index != 0 || uap->nel != 1)
				goto bad;

			count = HOST_LOAD_INFO_COUNT;
			(void) host_info(host_port, HOST_LOAD_INFO,
					 &load_info, &count);

			bcopy((char *) load_info.mach_factor,
			      (char *) tmf.tmf_machfactor,
			      sizeof tmf.tmf_machfactor);
			tmf.tmf_lscale = LOAD_SCALE;

			data = (caddr_t)&tmf;
			size = sizeof (tmf);
			break;
		case TBL_BOOTTIME:
			if (uap->index != 0 || uap->nel != 1)
				goto bad;
			data = (caddr_t)&boottime;
			size = sizeof boottime;
			break;
		case TBL_INCLUDE_VERSION:
			if (uap->index != 0 || uap->nel != 1)
				goto bad;
			iv = INCLUDE_VERSION;
			data = (caddr_t)&iv;
			size = sizeof(iv);
			break;
		/* deprecated */
		case TBL_FSPARAM: {
			if (table_fsparam(uap->index, &tf)) {
				data = (caddr_t)&tf;
				size = sizeof(tf);
				break;
			}
			goto bad;
		}
		case TBL_MAXUPRC:
			data = (caddr_t)&u.u_maxuprc;
		        size = sizeof(u.u_maxuprc);
			break;
		case TBL_AID:
			data = (caddr_t)&u.u_aid;
			size = sizeof(u.u_aid);
			break;
		case TBL_MODES:
			data = (caddr_t)&u.u_modes;
			size = sizeof(u.u_modes);
			break;
#if	MACH
		case TBL_UAREA:
		   {
			struct user 	*fake;
#if	MACH_NO_KERNEL
#else	MACH_NO_KERNEL
			task_t		task;
#endif	MACH_NO_KERNEL

			/*
			 *	Lookup process by pid
			 */
			p = pfind(uap->index);
			if (p == (struct proc *)0) {
				/*
				 *	Proc 0 not in the hash table
				 */
				if (uap->index == 0)
					p = &proc[0];
				else {
					/*
					 *	No such process
					 */
					u.u_error = ESRCH;
					return;
				}
			}

#if	MACH_NO_KERNEL
			(void) vm_allocate(mach_task_self(),
					   &dealloc_start,
					   sizeof(struct user),
					   TRUE);
			fake = (struct user *)dealloc_start;

			fake_u(fake, p, p->p_thread);
#else	MACH_NO_KERNEL
			/*
			 *	Before we can block (any VM code), make
			 *	another reference to the task to keep it
			 *	alive.
			 */

			task = p->task;
			task_reference(task);

			fake = (struct user *)
				kmem_alloc_wait(kernel_pageable_map,
					round_page(sizeof(struct user)));

			if (fake == (struct user *) 0) {
				task_deallocate(task);
				u.u_error = ENOMEM;
				return;
			}


			task_lock(task);
			fake_u(fake, (thread_t) task->thread_list.next);
			task_unlock(task);
			task_deallocate(task);
#endif	MACH_NO_KERNEL

			data = (caddr_t) fake;
			size = (vm_size_t) sizeof(struct user);
			dealloc_start = (vm_offset_t) fake;
			dealloc_end = dealloc_start +
				round_page(sizeof(struct user));
			break;
		   }

		case TBL_ARGUMENTS:
			/*
			 *	Returns the top N bytes of the user stack, with
			 *	everything below the first argument character
			 *	zeroed for security reasons.
			 *	Odd data structure is for compatibility.
			 */
			/*
			 *	Lookup process by pid
			 */
			p = pfind(uap->index);
			if (p == (struct proc *)0) {
			/*
			 *	Proc 0 isn't in the hash table
			 */
				if (uap->index == 0)
					p = &proc[0];
				else {
					/*
					 *	No such process
					 */
					u.u_error = ESRCH;
					return;
				}
			 }
#if	MACH_NO_KERNEL
#else	MACH_NO_KERNEL
			/*
			 *	Get map for process
			 */
			proc_map = p->task->map;
#endif	MACH_NO_KERNEL

			/*
			 *	Copy the top N bytes of the stack.
			 *	On all machines we have so far, the stack grows
			 *	downwards.
			 *
			 *	If the user expects no more than N bytes of
			 *	argument list, use that as a guess for the
			 *	size.
			 */
			if ((arg_size = uap->lel) == 0) {
				error = EINVAL;
				goto bad;
			}

			arg_addr = trunc_page((vm_offset_t)USRSTACK - arg_size);
			copy_size = round_page(USRSTACK) - arg_addr;

#if	MACH_NO_KERNEL
			if (vm_read(p->p_task,
				    arg_addr,
				    copy_size,
				    &copy_start,
				    &copy_size) != KERN_SUCCESS)
				goto bad;

			copy_end = copy_start + copy_size;
#else	MACH_NO_KERNEL
			/*
			 *	Before we can block (any VM code), make another
			 *	reference to the map to keep it alive.
			 */
			vm_map_reference(proc_map);

			copy_start = kmem_alloc_wait(kernel_pageable_map,
								copy_size);

			if (copy_start == 0) {
				vm_map_deallocate(proc_map);
				u.u_error = ENOMEM;
				return;
			}
			copy_end = copy_start + copy_size;

			if (vm_map_copy(kernel_pageable_map, proc_map, copy_start,
			    copy_size, arg_addr,
			    FALSE, FALSE) != KERN_SUCCESS) {
				kmem_free_wakeup(kernel_pageable_map, copy_start,
					round_page(arg_size));
				vm_map_deallocate(proc_map);
				goto bad;
			}

			/*
			 *	Now that we've done the copy, we can release
			 *	the process' map.
			 */
			vm_map_deallocate(proc_map);
#endif	MACH_NO_KERNEL

			/*
			 *	Find top of stack in the copy, and
			 *	where to stop traversing (note roundoff
			 *	to integer boundary).
			 */

			ip = (int *) (copy_end -
				(round_page(USRSTACK) - USRSTACK));
			lastp = ip - (arg_size / sizeof(int));

			/*
			 *	Regardless whether we get all of the
			 *	bottom integer, we return the number of
			 *	bytes requested.
			 */

			data = ((caddr_t)ip) - arg_size;
			size = arg_size;

			/*
			 *	Now look down the stack for the bottom of the
			 *	argument list.  Since this call is otherwise
			 *	unprotected, we can't let the nosy user see
			 *	anything else on the stack.
			 *
			 *	The arguments are pushed on the stack by
			 *	execve() as:
			 *
			 *		.long	0
			 *		arg 0	(null-terminated)
			 *		arg 1
			 *		...
			 *		arg N
			 *		.long	0
			 *
			 */

#ifdef	mips
			/*
			 *	On Mips, there is a hole at the
			 *	top of the stack (see execve())
			 */
			ip -= (32 / sizeof(int));
			/*
			 *	..and that's not all.  The arguments, for
			 *	whatever long-sighted reason that might be,
			 *	start on a 16-byte aligned address.  So we
			 *	might have to go back as far as 3 more empty
			 *	words longer than usual.  Sigh.
			 */
			{
				int fudge = 0;
				if (ip[-2] == 0) fudge++;
				if (ip[-3] == 0) fudge++;
				if (ip[-4] == 0) fudge++;
				ip -= fudge;
			}
#endif	mips
			ip -= 2; /*skip trailing 0 word and assume at least one
				  argument.  The last word of argN may be just
				  the trailing 0, in which case we'd stop
				  there */
			while (*--ip)
				if (ip == lastp)
					break;			
			bzero(data, (unsigned) ((int)ip - (int)data));

			dealloc_start = copy_start;
			dealloc_end = copy_end;
			break;

		case TBL_PROCINFO:
		    {
			register struct utask	*utaskp;

			/*
			 *	Index is entry number in proc table.
			 */
			if (uap->index >= nproc || uap->index < 0)
			    goto bad;

			p = &proc[uap->index];
			if (p->p_stat == 0) {
			    bzero((caddr_t)&tp, sizeof(tp));
			    tp.pi_status = PI_EMPTY;
			}
			else {
			    tp.pi_uid	= p->p_uid;
			    tp.pi_pid	= p->p_pid;
			    tp.pi_ppid	= p->p_ppid;
			    tp.pi_pgrp	= p->p_pgrp;
			    tp.pi_flag	= p->p_flag;

#if	MACH_NO_KERNEL
			    if (p->p_task == MACH_PORT_NULL)
#else	MACH_NO_KERNEL
			    if (p->task == MACH_TASK_NULL)
#endif	MACH_NO_KERNEL
			    {
				tp.pi_status = PI_ZOMBIE;
			    }
			    else {
#if	MACH_NO_KERNEL
				utaskp = &p->p_utask;
#else	MACH_NO_KERNEL
				utaskp = p->task->u_address;
#endif	MACH_NO_KERNEL
				if (p->p_ttyp == 0)
				    tp.pi_ttyd = -1;
				else
				    tp.pi_ttyd = p->p_ttyd;
				bcopy(utaskp->uu_comm, tp.pi_comm,
				      MAXCOMLEN);
				tp.pi_comm[MAXCOMLEN] = '\0';

				if (p->p_flag & SWEXIT)
				    tp.pi_status = PI_EXITING;
				else
				    tp.pi_status = PI_ACTIVE;
			    }
			}

			data = (caddr_t)&tp;
			size = sizeof(tp);
			break;
		    }

#endif	MACH
		default:
		bad:
			/*
			 *	Return error only if all indices
			 *	are invalid.
			 */
			if (u.u_r.r_val1 == 0)
				u.u_error = EINVAL;
			return;
		}
		/*
		 * This code should be generalized if/when other tables
		 * are added to handle single element copies where the
		 * actual and expected sizes differ or the table entries
		 * are not contiguous in kernel memory (as with TTYLOC)
		 * and also efficiently copy multiple element
		 * tables when contiguous and the sizes match.
		 */
		size = MIN(size, uap->lel);
		if (size) {
			if (set) {
				char buff[MAXLEL];

			        error = copyin(uap->addr, buff, size);
				if (error == 0)
					bcopy(buff, data, size);
			}
			else {
				error = copyout(data, uap->addr, size);
			}
		}
#if	MACH
		if (dealloc_start != (vm_offset_t) 0) {
#if	MACH_NO_KERNEL
			(void) vm_deallocate(mach_task_self(),
					     dealloc_start,
					     dealloc_end - dealloc_start);
#else	MACH_NO_KERNEL
			kmem_free_wakeup(kernel_pageable_map, dealloc_start,
				dealloc_end - dealloc_start);
#endif	MACH_NO_KERNEL
		}
#endif	MACH
		if (error) {
			u.u_error = error;
			return;
		}
		uap->addr += uap->lel;
		uap->nel -= 1;
		uap->index += 1;
		u.u_r.r_val1 += 1;
	}
}

#if	CMUCS
#if	MACH_NO_KERNEL
#else	MACH_NO_KERNEL
boolean_t table_ttyloc(index, data, size)
	int		index;
	caddr_t 	*data;
	unsigned	*size;
{
	int majdev, mindev;
	extern int ntty;
	extern int *nttysw[];

	majdev = major(index);
	mindev = minor(index);
	if (majdev >= ntty || mindev >= *(nttysw[majdev]))
		return(FALSE);
	*data = (caddr_t)&((&(cdevsw[majdev].d_ttys)[mindev])->t_ttyloc);
	*size = sizeof (struct ttyloc);
	return(TRUE);
}
#endif	MACH_NO_KERNEL
#endif	CMUCS

boolean_t table_fsparam(index, tf)
	int				index;
	register struct tbl_fsparam	*tf;
{
	register struct mount	*mp;
	register struct fs	*fs;

	for (mp = &mount[0]; mp < &mount[NMOUNT]; mp++)
		if (mp->m_bufp != 0 && mp->m_dev == index)
			continue;
	if (mp == &mount[NMOUNT])
		return(FALSE);

	fs = mp->m_bufp->b_un.b_fs;
	tf->tf_used = freefrags(fs);
	tf->tf_size = fs->fs_dsize;
	tf->tf_iused = freeinodes(fs);
	tf->tf_isize = fs->fs_ncg*fs->fs_ipg;
	return(TRUE);
}
#endif	CMUCS
