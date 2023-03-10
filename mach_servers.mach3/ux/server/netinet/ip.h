/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 ************************************************************************
 * HISTORY
 * $Log:	ip.h,v $
 * Revision 2.2  89/11/29  15:29:09  af
 * 	Byte-order.
 * 	[89/11/16  17:11:45  af]
 * 
 * Revision 2.1  89/08/04  14:22:48  rwd
 * Created.
 * 
 * 24-Jul-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Changed conditional on CS_BUGFIX to CMU to avoid unnecessary
 *	kernel features dependencies.
 *
 * 16-May-87  Jay Kistler (jjk) at Carnegie-Mellon University
 *	Merged in code for IP multicast.
 *
 *  4-Mar-87   Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	Changed conditional on HC to CS_BUGFIX and included appropriate 
 *	file.
 *
 * 24-Jan-87  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	HC: Added field name for union at the end of ip_timestamp to
 *	make HiC happy.
 *
 * 21-Oct-86  David Golub (dbg) at Carnegie-Mellon University
 *	Use different versions of time-stamp option definition
 *	for big-ender and little-enter machines.
 *
 * 21-Oct-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Added versions of ip_hl and ip_v for the 68000.
 *
 *  7-Oct-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Added ns32000 versions of ip_hl and ip_v.
 *
 * 17-Feb-86  Bill Bolosky (bolosky) at Carnegie-Mellon University
 *	romp: Added RT versions of ip_hl and ip_v.
 *
 ************************************************************************
 */

#ifdef	KERNEL
#include "multicast_agent.h"
#else	KERNEL
#include <sys/features.h>
#endif	KERNEL

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)ip.h	7.1 (Berkeley) 6/5/86
 */

/*
 * Definitions for internet protocol version 4.
 * Per RFC 791, September 1981.
 */
#define	IPVERSION	4

/*
 * Structure of an internet header, naked of options.
 *
 * We declare ip_len and ip_off to be short, rather than u_short
 * pragmatically since otherwise unsigned comparisons can result
 * against negative integers quite easily, and fail in subtle ways.
 */
struct ip {
#if	BYTE_MSF
	u_char	ip_v:4,			/* version */
		ip_hl:4;		/* header length */
#else	BYTE_MSF
	u_char	ip_hl:4,		/* header length */
		ip_v:4;			/* version */
#endif	BYTE_MSF
	u_char	ip_tos;			/* type of service */
	short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	short	ip_off;			/* fragment offset field */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
};

/*
 * Definitions for options.
 */
#define	IPOPT_COPIED(o)		((o)&0x80)
#define	IPOPT_CLASS(o)		((o)&0x60)
#define	IPOPT_NUMBER(o)		((o)&0x1f)

#define	IPOPT_CONTROL		0x00
#define	IPOPT_RESERVED1		0x20
#define	IPOPT_DEBMEAS		0x40
#define	IPOPT_RESERVED2		0x60

#define	IPOPT_EOL		0		/* end of option list */
#define	IPOPT_NOP		1		/* no operation */

#define	IPOPT_RR		7		/* record packet route */
#define	IPOPT_TS		68		/* timestamp */
#define	IPOPT_SECURITY		130		/* provide s,c,h,tcc */
#define	IPOPT_LSRR		131		/* loose source route */
#define	IPOPT_SATID		136		/* satnet id */
#define	IPOPT_SSRR		137		/* strict source route */
#if	MULTICAST_AGENT
#define IPOPT_MULTICAST		159		/* multicast routing (UNOFFICIAL) */
#define IPOPT_MULTICAST_OPTLEN	8		/* length of multicast option */
#endif	MULTICAST_AGENT

/*
 * Offsets to fields in options other than EOL and NOP.
 */
#define	IPOPT_OPTVAL		0		/* option ID */
#define	IPOPT_OLEN		1		/* option length */
#define IPOPT_OFFSET		2		/* offset within option */
#define	IPOPT_MINOFF		4		/* min value of above */
#if	MULTICAST_AGENT
#define IPOPT_MULTICASTOFF	4		/* where groupaddr begins */
#endif	MULTICAST_AGENT

/*
 * Time stamp option structure.
 */
struct	ip_timestamp {
	u_char	ipt_code;		/* IPOPT_TS */
	u_char	ipt_len;		/* size of structure (variable) */
	u_char	ipt_ptr;		/* index of current entry */
#if	BYTE_MSF
	u_char	ipt_oflw:4,		/* overflow counter */
		ipt_flg:4;		/* flags, see below */
#else	BYTE_MSF
	u_char	ipt_flg:4,		/* flags, see below */
		ipt_oflw:4;		/* overflow counter */
#endif	BTYE_MSF
	union {
		n_long	ipt_time[1];
		struct	ipt_ta {
			struct in_addr ipt_addr;
			n_long ipt_time;
		} ipt_ta[1];
	}
#if	CMU
	 ipt_union;
#endif	CMU
};

/* flag bits for ipt_flg */
#define	IPOPT_TS_TSONLY		0		/* timestamps only */
#define	IPOPT_TS_TSANDADDR	1		/* timestamps and addresses */
#define	IPOPT_TS_PRESPEC	2		/* specified modules only */

/* bits for security (not byte swapped) */
#define	IPOPT_SECUR_UNCLASS	0x0000
#define	IPOPT_SECUR_CONFID	0xf135
#define	IPOPT_SECUR_EFTO	0x789a
#define	IPOPT_SECUR_MMMM	0xbc4d
#define	IPOPT_SECUR_RESTR	0xaf13
#define	IPOPT_SECUR_SECRET	0xd788
#define	IPOPT_SECUR_TOPSECRET	0x6bc5

/*
 * Internet implementation parameters.
 */
#define	MAXTTL		255		/* maximum time to live (seconds) */
#define	IPFRAGTTL	15		/* time to live for frag chains */
#define	IPTTLDEC	1		/* subtracted when forwarding */

#define	IP_MSS		576		/* default maximum segment size */
