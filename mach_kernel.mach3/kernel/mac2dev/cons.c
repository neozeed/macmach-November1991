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
 * $Log:	cons.c,v $
 * Revision 2.2  91/09/12  16:45:59  bohman
 * 	Created Mach 3.0 version from 2.5 version.
 * 	[91/09/11  15:25:22  bohman]
 * 
 * Revision 2.2  90/08/30  11:04:04  bohman
 * 	Created.
 * 	[90/08/29  12:40:32  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/cons.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <device/conf.h>
#include <device/tty.h>
#include <device/io_req.h>
#include <device/buf.h>		/* for struct uio (!) */

#include <sys/types.h>

#include <mac2/act.h>
#include <mac2/clock.h>

#include <mac2os/Types.h>
#include <mac2os/Video.h>

#include <mac2dev/font.h>
#include <mac2dev/video.h>
#include <mac2dev/adb.h>

/*
 * MAC2 console driver;
 * this should be flushed as
 * soon as possible.
 */

struct tty  cons;

#define	cons_vp  &video[0]

struct cons_data {
    vm_offset_t		cons_addr;	/* address of screen bitmap */
    long	 	cons_mem_x;	/* screen x width in pixels */
    long 		cons_mem_y;	/* screen y height in pixels */
    long 		cons_scr_x;	/* memory x width in pixels */
    long 		cons_scr_y;	/* memory y height in pixels */
    int			cons_xpos;	/* cursor x position */
    int			cons_ypos;	/* cursor y position */
    struct font		cons_font;	/* font structure */
    struct act		*cons_act;
    int			cons_flags;
#define	CONS_SCREEN_EXISTS	0x00000001
} cons_data;

struct {
    unsigned long	transData;
    unsigned long	state;
    unsigned short	modflags;
#define KEY_MOD_CTRL	0x1000
#define KEY_MOD_OPT	0x0800
#define KEY_MOD_CAPS	0x0400
#define KEY_MOD_SHFT	0x0200
#define KEY_MOD_CMD	0x0100
    struct act		*key_act;
    unsigned char	repeat_code;
} key_data;

#define KEY_CLOCK_LIST	0

int cnstart(), cninput(), cnrepeat(), cnoutput(), ttrstrt();

extern unsigned char	font_524[], font_521[];

io_return_t
cnopen(dev, flag, ior)
dev_t		dev;
int		flag;
io_req_t	ior;
{
    register struct tty		*tp;
    register io_return_t	result;

    tp = &cons;
    tp->t_addr = 0;
    tp->t_oproc = cnstart;
    tp->t_stop = nulldev;
    if ((tp->t_state&TS_ISOPEN) == 0) {
	if (Xadb_query(ADB_ADDR_KEYBOARD, -1, 0) != ADB_ADDR_KEYBOARD)
	    return (D_NO_SUCH_DEVICE);
	ttychars(tp);
	tp->t_flags = EVENP|ODDP|ECHO|XTABS|CRMOD;
	tp->t_ispeed = tp->t_ospeed = B9600;
	tp->t_state = TS_CARR_ON;
    }

    /*
     * allocate console output
     * activity if none exists.
     */
    if (cons_data.cons_act == 0) {
	cons_data.cons_act = (struct act *)makesoftact(cnoutput, SR_IPL0);
	if (cons_data.cons_act == 0)
	    panic("cnopen: allocate output act");
    }

    /*
     * allocate key repeat
     * activity if none exists.
     */
    if (key_data.key_act == 0) {
	key_data.key_act = (struct act *)makeact(cnrepeat, SR_IPL1, 1);
	if (key_data.key_act == 0)
	    panic("cnopen: allocate repeat act");
	addact(KEY_CLOCK_LIST, key_data.key_act, &actclock);
    }

    if ((tp->t_state&TS_ISOPEN) == 0) {
	if (!Xadb_open(ADB_ADDR_KEYBOARD, cninput))
	    return (D_IO_ERROR);
    }

    result =  char_open(dev, tp, flag, ior);
    if (result != D_SUCCESS) {
	if ((tp->t_state&TS_ISOPEN) == 0)
	    Xadb_close(ADB_ADDR_KEYBOARD);

	return (result);
    }

    tp->t_state |= TS_ISOPEN;

    return (D_SUCCESS);
}

void
cnclose(dev)
dev_t	dev;
{
    Xadb_close(ADB_ADDR_KEYBOARD);

    cancelact(key_data.key_act);
    key_data.repeat_code = 0;
    key_data.modflags = 0;
    key_data.state = 0;

    ttyclose(&cons);
}

io_return_t
cnread(dev, uio)
dev_t		dev;
struct uio	*uio;
{
    return ((*linesw[cons.t_line].l_read)(&cons, uio));
}

io_return_t
cnwrite(dev, uio)
    dev_t dev;
    struct uio *uio;
{
    return (char_write(&cons, uio));
}

boolean_t
cnportdeath(dev, port)
dev_t		dev;
mach_port_t	port;
{
    return (tty_portdeath(&cons, port));
}

io_return_t
cngetstat(dev, flavor, data, count)
dev_t		dev;
int		flavor;
int *		data;		/* pointer to OUT array */
unsigned int	*count;		/* out */
{
    return (tty_get_status(&cons, flavor, data, count));
}

io_return_t
cnsetstat(dev, flavor, data, count)
dev_t		dev;
int		flavor;
int *		data;
unsigned int	count;
{
    return (tty_set_status(&cons, flavor, data, count));
}

cninput(cmd, data)
adb_cmd_t	cmd;
unsigned char	data[];
{

    if ((cons.t_state&TS_ISOPEN)
	&& cmd.reg.cmd == ADB_talk
	&& cmd.reg.reg == 0
	&& cmd.cmd.len == 2) {
	cnkey(data[0]); cnkey(data[1]);
    }
}

cnkey(key_code)
register unsigned char		key_code;
{
    register unsigned long	key_char;

    switch (key_code) {
      case 0x36:
	key_data.modflags |= KEY_MOD_CTRL;
	break;

      case 0xb6:
	key_data.modflags &= ~KEY_MOD_CTRL;
	break;

      case 0x3a:
	key_data.modflags |= KEY_MOD_OPT;
	break;

      case 0xba:
	key_data.modflags &= ~KEY_MOD_OPT;
	break;

      case 0x39:
	key_data.modflags |= KEY_MOD_CAPS;
	break;

      case 0xb9:
	key_data.modflags &= ~KEY_MOD_CAPS;
	break;

      case 0x38:
	key_data.modflags |= KEY_MOD_SHFT;
	break;

      case 0xb8:
	key_data.modflags &= ~KEY_MOD_SHFT;
	break;

      case 0x37:
	key_data.modflags |= KEY_MOD_CMD;
	break;

      case 0xb7:
	key_data.modflags &= ~KEY_MOD_CMD;
	break;

      case 0xff:
      case 0x7f:
	break;

      default:
	if (key_code & 0x80) {
	    if (key_code == key_data.repeat_code)
		key_data.repeat_code = 0;
	    break;
	}

	key_char = KeyTrans(key_data.transData,
			    (key_data.modflags | key_code),
			    &key_data.state);

	cancelact(key_data.key_act);
	key_data.repeat_code = (0x80 | key_code);
	runact(KEY_CLOCK_LIST, key_data.key_act, key_char, hz/2);

	(*linesw[cons.t_line].l_rint)(key_char, &cons);
	break;
    }
}

cnrepeat(key_char, list, act)
unsigned char	key_char;
int		list;
struct act	*act;
{

    if (key_data.repeat_code == 0)
	return;

    (*linesw[cons.t_line].l_rint)(key_char, &cons);

    runact(list, act, key_char, hz/20);
}

cnstart(tp)
register struct tty *tp;
{
    if (tp->t_state & (TS_TIMEOUT|TS_BUSY|TS_TTSTOP))
	return;

    if (tp->t_outq.c_cc <= TTLOWAT(tp))
	tt_write_wakeup(tp);

    if (tp->t_outq.c_cc == 0)
	return;

    tp->t_state |= TS_BUSY;
    softact(cons_data.cons_act, tp);
}

/*
 * Called by console output activity
 * at IPL0 to do actual output.
 */
cnoutput(tp)
register struct tty *tp;
{
    register c, s;

    for (;;) {
	s = spl7();
	if ((tp->t_state&TS_TTSTOP) || (tp->t_outq.c_cc == 0))
	    break;
	c = getc(&tp->t_outq);
	if (c <= 0177 || tp->t_flags & LITOUT)
	    mac2_putc(c);
	else {
	    timeout(ttrstrt, (caddr_t)tp, (c&0177));
	    tp->t_state |= TS_TIMEOUT;
	    break;
	}
	splx(s);
    }

    tp->t_state &= ~TS_BUSY;
    if (tp->t_outq.c_cc <= TTLOWAT(tp))
	tt_write_wakeup(tp);

    splx(s);
}

/*
 * ARGHH !!!
 */
cngetc()
{
    panic("cngetc");
}

/*
 * Used by kernel printf() to
 * output characters.
 */
cnputc(c)
register c;
{
    register s = spl7();

    if (c == '\n')
	mac2_putc('\r');

    mac2_putc(c);
    splx(s);
}

/*
 * mac2_putc(c)
 *
 * displays the character on the screen.
 */
mac2_putc(c)
register c;
{
    register struct cons_data *cd = &cons_data;
    register struct font *fp = &cd->cons_font;
    extern panicstr;

    if ((cd->cons_flags&CONS_SCREEN_EXISTS) == 0)
	return;

    font_invert(fp, cd->cons_xpos, cd->cons_ypos);
    c &= 0177;
    switch (c) {
	case '\b':
	    if (cd->cons_xpos)
		cd->cons_xpos--;
	    font_char(fp, cd->cons_xpos, cd->cons_ypos, ' ');	/* XXX */
	    break;

	case '\r':
	    cd->cons_xpos = 0;
	    break;

	case '\n':
	    cd->cons_ypos++;
	    if (cd->cons_ypos > (fp->font_maxy-1)) {
		cd->cons_ypos = fp->font_maxy-1;
		font_scrollup(fp, 0, fp->font_maxy-1, 1);
	    }
	    break;

	/* provide some control with a simple termcap entry like
		ma|MacMach console:\
			:am:do=^J:le=^H:bs:co#89:li#28:\
			:nd=^L:up=^K:cl=^Z:sr=^R:dl=^X:
		for the 480*640 pixel screens
		little Mac's (like SE30) have co#71:li#19
		and the large 2 page screen is co#159:li#52
	*/
	case 013:	/* up=^K */
	    cd->cons_ypos--;
	    if (cd->cons_ypos < 0)
		cd->cons_ypos = 0;
	    break;

	case 014:	/* nd=^L */
	    cd->cons_xpos++;
	    if (cd->cons_xpos >= fp->font_maxx)
		cd->cons_xpos--;
	    break;

	case 032:	/* cl=^Z */
	    video_setup(cons_vp, FALSE);	/* XXX */
	    cons_clear();
	    cd->cons_xpos = 0;
	    cd->cons_ypos = 0;
	    font_invert(&cd->cons_font, cd->cons_xpos, cd->cons_ypos);
	    break;

	case 022:	/* sr=^R */
	    cd->cons_ypos--;
	    if (cd->cons_ypos < 0) {
		cd->cons_ypos = 0;
		font_scrolldown(fp, 0, fp->font_maxy-1, 1);
	    }
	    break;

	case 030:	/* dl=^X */
	    font_scrollup(fp, cd->cons_ypos, fp->font_maxy-1, 1);
	    break;

	default:
	    if (font_char(fp, cd->cons_xpos, cd->cons_ypos, c)) {
		cd->cons_xpos++;
		if (cd->cons_xpos >= fp->font_maxx) {
		    cd->cons_xpos = 0;
		    cd->cons_ypos++;
		    if (cd->cons_ypos > (fp->font_maxy-1)) {
			cd->cons_ypos = fp->font_maxy-1;
			font_scrollup(fp, 0, fp->font_maxy-1, 1);
		    }
		}
	    }
	    break;
    }
    font_invert(fp, cd->cons_xpos, cd->cons_ypos);
}

/*
 * cons_printf_init()
 *
 * Initialize console screen device so
 * that kernel printfs work.
 */
cons_printf_init()
{
    VPBlock v;

    if (video_cons_find(&v) == TRUE)
	cons_init(&v);

    /*
     * Load key translation resource.
     */
    if (key_data.transData == 0) {
	Handle	h = (Handle)GetResource(*(unsigned long *)"KCHR", 0);

	if (h) {
	    DetachResource(h);
	    HLock(h);
	    key_data.transData = (unsigned)*h;
	}
    }
}

cons_init(v)
VPBlock	*v;
{
    register struct cons_data *cd = &cons_data;
    register struct video *vp = cons_vp;

    cd->cons_flags |= CONS_SCREEN_EXISTS;
    cd->cons_mem_x = 8*v->vpRowBytes;
    cd->cons_mem_y = v->vpBounds.bottom - v->vpBounds.top;
    cd->cons_scr_x = v->vpBounds.right - v->vpBounds.left;
    cd->cons_scr_y = v->vpBounds.bottom - v->vpBounds.top;
    cd->cons_addr = vp->video_devbase + v->vpBaseOffset;
    cons_clear();
    cons_font_set(LEFT_SHAVE, RIGHT_SHAVE, TOP_SHAVE, BOTTOM_SHAVE);
}


/*
 * Reset the console screen
 * after it has been used for
 * something else.  Called from
 * video_close();
 */
cons_reset()
{
    register struct cons_data *cd = &cons_data;
    register s;

    if (cd->cons_flags&CONS_SCREEN_EXISTS) {
	s = spl7();
	cons_clear();
	font_invert(&cd->cons_font, cd->cons_xpos, cd->cons_ypos);
	splx(s);
    }
}

/*
 * Clear the console screen
 */
cons_clear()
{
    register struct cons_data *cd = &cons_data;
    register int i, lines, width, inc;
    register char *cp;
    
    cp = (char *)cd->cons_addr;
    lines = cd->cons_scr_y - 1;
    width = (cd->cons_scr_x+(8*sizeof(long))-1)/(8*sizeof(long)) - 1;
    inc = (cd->cons_mem_x/(8*sizeof(long)) - width - 1)*sizeof(long);
    do {
	i = width;
	do {
	    *(long *)cp = 0;
	    cp += sizeof(long);
	} while(i--);
	cp += inc;
    } while (lines--);
}

cons_force()
{
}

/*
 * Pick an appropriate font for
 * the console, and initialize
 * data structures.
 */
cons_font_set(left, right, top, bottom)
unsigned left, right, top, bottom;
{
    register struct fontrec *f;
    register struct cons_data *cd = &cons_data;
    register struct font *fp = &cd->cons_font;

    f = (struct fontrec *)font_524; /* Monaco 12 (slightly modified) */
    if ((cd->cons_scr_x - left - right) / f->fRectMax < 80)
	f = (struct fontrec *)font_521; /* Monaco 9 (slightly modified) */

    fp->font_pnt = f;
    fp->font_screen = cd;
    fp->font_inverse = 1;
    fp->font_height = f->chHeight;
    fp->font_theight = f->chHeight+f->leading;
    fp->font_width = f->fRectMax;
    fp->font_leading = f->leading;
    fp->font_rowwords = ((unsigned int)f->rowWords)<<4;
    fp->font_bitimage = (unsigned char *)&f->bitImage[0];
    fp->font_loctable = &f->bitImage[f->rowWords*fp->font_height];
    fp->font_owtable = &f->bitImage[f->rowWords*fp->font_height+
				    f->lastChar + 2 - f->firstChar];
    fp->font_linewidth = (fp->font_height+fp->font_leading) * cd->cons_mem_x;
    fp->font_maxx = (cd->cons_scr_x - left - right) / fp->font_width;
    fp->font_maxy = (cd->cons_scr_y / (fp->font_height+fp->font_leading)) - 
	top - bottom;
    fp->font_offset = (top * fp->font_linewidth) +
	(cd->cons_scr_x - (fp->font_width * fp->font_maxx))/2;
}
