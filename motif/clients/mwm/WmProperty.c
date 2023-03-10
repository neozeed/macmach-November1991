#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmProperty.c	3.11 91/01/10";
#endif /* lint */
#endif /* REV_INFO */
 /*****************************************************************************
 ******************************************************************************
 *
 * (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
 * (c) Copyright 1987, 1988, 1989, 1990 HEWLETT-PACKARD COMPANY
 * ALL RIGHTS RESERVED
 *
 * 	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
 * AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
 * WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
 * ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
 * AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
 * SOFTWARE IS HEREBY TRANSFERRED.
 *
 * 	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
 * NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
 * FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS
 *
 * 	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
 * ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
 * SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Notice:  Notwithstanding any other lease or license that may pertain to,
 * or accompany the delivery of, this computer software, the rights of the
 * Government regarding its use, reproduction and disclosure are as set
 * forth in Section 52.227-19 of the FARS Computer Software-Restricted
 * Rights clause.
 *
 * (c) Copyright 1989,1990, 1991 Open Software Foundation, Inc.  Unpublished - all
 * rights reserved under the Copyright laws of the United States.
 *
 * RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
 * Government is subject to the restrictions as set forth in subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
 * at DFARS 52.227-7013.
 *
 * Open Software Foundation, Inc.
 * 11 Cambridge Center
 * Cambridge, MA   02142
 * (617)621-8700
 *
 * RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
 * "restricted rights."  Use, duplication or disclosure is subject to the
 * restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
 * "Commercial Computer Software- Restricted Rights (April 1985)."  Open
 * Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
 * the contract contains the Clause at 18-52.227-74 "Rights in Data General"
 * then the "Alternate III" clause applies.
 *
 * (c) Copyright 1989,1990, 1991 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED
 *
 *
 * Open Software Foundation is a trademark of The Open Software Foundation, Inc.
 * OSF is a trademark of Open Software Foundation, Inc.
 * OSF/Motif is a trademark of Open Software Foundation, Inc.
 * Motif is a trademark of Open Software Foundation, Inc.
 * DEC is a registered trademark of Digital Equipment Corporation
 * DIGITAL is a registered trademark of Digital Equipment Corporation
 * X Window System is a trademark of the Massachusetts Institute of Technology
 *
 ******************************************************************************
 *****************************************************************************/




/*
 * Included Files:
 */

#include "WmGlobal.h"
#include "WmICCC.h"
#include <stdio.h>

/*
 * include extern functions
 */

#include "WmColormap.h"
#include "WmError.h"
#include "WmResParse.h"



/*
 * Function Declarations:
 */

#ifdef _NO_PROTO
Colormap	FindColormap ();
PropMwmHints *	GetMwmHints();
PropMwmInfo *	GetMwmInfo ();
MenuItem *	GetMwmMenuItems ();
SizeHints *	GetNormalHints ();
PropWMState *	GetWMState ();
void		ProcessMwmMessages ();
void		ProcessWmColormapWindows ();
void		ProcessWmProtocols ();
void		SetMwmInfo ();
void		SetWMState ();
#else /* _NO_PROTO */
void ProcessWmProtocols (ClientData *pCD);
void ProcessMwmMessages (ClientData *pCD);
void SetMwmInfo (Window propWindow, long flags, Window wmWindow);
void SetWMState (Window window, int state, Window icon);
PropMwmInfo *GetMwmInfo (Window rootWindowOfScreen);
void ProcessWmColormapWindows (ClientData *pCD);
Colormap FindColormap (ClientData *pCD, Window window);

#endif /* _NO_PROTO */


/*
 * Global Variables:
 */

static SizeHints sizeHints;



/*************************************<->*************************************
 *
 *  SizeHints *
 *  GetNormalHints (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function replaces the XGetNormalHints Xlib function.  This function
 *  gets the information in the WM_NORMAL_HINTS property on the client window.
 *  The property encoding can be any of the supported versions (R2, R3+).
 *
 *
 *  Inputs:
 *  ------
 *  pCD = (client)
 *
 * 
 *  Outputs:
 *  -------
 *  Return = A pointer to a filled out SizeHints structure is returned.
 *           Default values are set if the WM_NORMAL_HINTS property could
 *           not be retrieved.
 *
 *************************************<->***********************************/

SizeHints *GetNormalHints (pCD)
    ClientData *pCD;

{
    PropSizeHints *property = NULL;
    Atom actualType;
    int actualFormat;
    unsigned long leftover;
    unsigned long nitems;


    /*
     * Retrieve the property data.
     *
     *     ICCC_R2 version:  nitems = PROP_SIZE_HINTS_ELEMENTS - 3
     *     ICCC_CURRENT version: nitems = PROP_SIZE_HINTS_ELEMENTS
     */

    if ((Success != XGetWindowProperty (DISPLAY, pCD->client,
			XA_WM_NORMAL_HINTS, 0L, (long)PROP_SIZE_HINTS_ELEMENTS,
			False, XA_WM_SIZE_HINTS, &actualType, &actualFormat,
			&nitems, &leftover, (unsigned char **)&property)) ||
	 (actualType != XA_WM_SIZE_HINTS) ||
	 (nitems < (PROP_SIZE_HINTS_ELEMENTS - 3)) ||
	 (actualFormat != 32))
    {
	/*
	 * Indicate no property values were retrieved:
	 */

	sizeHints.icccVersion = ICCC_UNKNOWN;
	sizeHints.flags = 0;
    }
    else
    {
	/*
	 * Parse the hint values out of the property data:
	 */

	sizeHints.flags = property->flags;
	sizeHints.x = property->x;
	sizeHints.y = property->y;
	sizeHints.width = property->width;
	sizeHints.height = property->height;
	sizeHints.min_width = property->minWidth;
	sizeHints.min_height = property->minHeight;
	sizeHints.max_width = property->maxWidth;
	sizeHints.max_height = property->maxHeight;
	sizeHints.width_inc = property->widthInc;
	sizeHints.height_inc = property->heightInc;
	sizeHints.min_aspect.x = property->minAspectX;
	sizeHints.min_aspect.y = property->minAspectY;
	sizeHints.max_aspect.x = property->maxAspectX;
	sizeHints.max_aspect.y = property->maxAspectY;


	if (nitems == (PROP_SIZE_HINTS_ELEMENTS - 3))
	{
	    /*
	     *  This is ICCC_R2.
	     */

	    sizeHints.icccVersion = ICCC_R2;
	}
	else
	{
	    /*
	     *  This is ICCC_CURRENT.
	     */

	    sizeHints.icccVersion = ICCC_CURRENT;
	    sizeHints.base_width = property->baseWidth;
	    sizeHints.base_height = property->baseHeight;
	    sizeHints.win_gravity = property->winGravity;
	}
    }


    /*
     * Free the property data buffer:
     */

    if (property)
    {
	XFree ((char *)property);
    }


    /*
     * Return the hints values:
     */

    return (&sizeHints);


} /* END OF FUNCTION GetNormalHints */



/*************************************<->*************************************
 *
 *  ProcessWmProtocols (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function reads and processes the WM_PROTOCOLS property that is
 *  associated with a client window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = (clientProtocols, clientProtocolCount, protocolFlags)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ProcessWmProtocols (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void ProcessWmProtocols (ClientData *pCD)
#endif /* _NO_PROTO */
{
    int rValue;
    Atom *property = NULL;
    Atom actualType;
    int actualFormat;
    unsigned long leftover;
    unsigned long nitems;
    int i;


    if (pCD->clientProtocols)
    {
	free (pCD->clientProtocols);
	pCD->clientProtocols = NULL;
    }
    pCD->clientProtocolCount = 0;
    pCD->protocolFlags = 0;


    /*
     * Read the WM_PROTOCOLS property.
     */

    rValue = XGetWindowProperty (DISPLAY, pCD->client, wmGD.xa_WM_PROTOCOLS, 0L,
		 (long)MAX_CLIENT_PROTOCOL_COUNT, False, AnyPropertyType,
		 &actualType, &actualFormat, &nitems, &leftover,
		 (unsigned char **)&property);


    if ((rValue != Success) || (actualType == None) || (actualFormat != 32))
    {
	/*
	 * WM_PROTOCOLS does not exist or it is an invalid type.
	 */

	pCD->clientProtocols = NULL;
    }
    else
    {
        if (!(pCD->clientProtocols = (Atom *)malloc (nitems * sizeof (Atom))))
        {
	    /* unable to allocate space */
	    Warning ("Insufficient memory for window management data");
        }
	else
	{
	    /*
	     * Save the protocols in the client data and look for predefined
	     * protocols.
	     */

    	    pCD->clientProtocolCount = nitems;

    	    for (i = 0; i < nitems; i++)
    	    {
		pCD->clientProtocols[i] = property[i];
		if (property[i] == wmGD.xa_WM_SAVE_YOURSELF)
		{
		    pCD->protocolFlags |= PROTOCOL_WM_SAVE_YOURSELF;
		}
		else if (property[i] == wmGD.xa_WM_TAKE_FOCUS)
		{
		    pCD->protocolFlags |= PROTOCOL_WM_TAKE_FOCUS;
		}
		else if (property[i] == wmGD.xa_WM_DELETE_WINDOW)
		{
		    pCD->protocolFlags |= PROTOCOL_WM_DELETE_WINDOW;
		}
		else if (property[i] == wmGD.xa_MWM_MESSAGES)
		{
		    pCD->protocolFlags |= PROTOCOL_MWM_MESSAGES;
		}
    	    }
	}
    }


    if (property)
    {
	XFree ((char *)property);
    }


} /* END OF FUNCTION ProcessWmProtocols */



/*************************************<->*************************************
 *
 *  ProcessMwmMessages (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function reads and processes the _MWM_MESSAGES property that is
 *  associated with a client window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = (mwmMessagesCount, mwmMessages)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ProcessMwmMessages (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void ProcessMwmMessages (ClientData *pCD)
#endif /* _NO_PROTO */
{
    int rValue;
    long *property = NULL;
    Atom actualType;
    int actualFormat;
    unsigned long leftover;
    unsigned long nitems;
    int i;


    if (pCD->mwmMessages)
    {
	free (pCD->mwmMessages);
	pCD->mwmMessages = NULL;
    }
    pCD->mwmMessagesCount = 0;


    /*
     * Read the _MWM_MESSAGES property.
     */

    rValue = XGetWindowProperty (DISPLAY, pCD->client, wmGD.xa_MWM_MESSAGES, 0L,
		 (long)MAX_MWM_MESSAGES_COUNT, False, AnyPropertyType,
		 &actualType, &actualFormat, &nitems, &leftover,
		 (unsigned char **)&property);


    if ((rValue != Success) || (actualType == None) || (actualFormat != 32))
    {
	/*
	 * _MWM_MESSAGES does not exist or it is an invalid type.
	 */

	pCD->mwmMessages = NULL;
    }
    else
    {
        if (!(pCD->mwmMessages = (long *)malloc (nitems * sizeof (long))))
        {
	    /* unable to allocate space */
	    Warning ("Insufficient memory for window management data");
        }
	else
	{
	    /*
	     * Save the protocols in the client data and look for predefined
	     * protocols.
	     */

    	    pCD->mwmMessagesCount = nitems;

    	    for (i = 0; i < nitems; i++)
    	    {
		if ((pCD->mwmMessages[i] = property[i]) == wmGD.xa_MWM_OFFSET)
		{
		    pCD->protocolFlags |= PROTOCOL_MWM_OFFSET;
		}
    	    }
	}
    }


    if (property)
    {
	XFree ((char *)property);
    }


} /* END OF FUNCTION ProcessMwmMessages */



/*************************************<->*************************************
 *
 *  SetMwmInfo (propWindow, flags, wmWindow)
 *
 *
 *  Description:
 *  -----------
 *  This function sets up the _MOTIF_WM_INFO property on the specified (usually
 *  the root) window.
 *
 *
 *  Inputs:
 *  ------
 *  propWindow = window on which the _MOTIF_WM_INFO property is to be set
 *
 *  flags = motifWmInfo.flags value
 *
 *  wmWindow = motifWmInfo.wmWindow value
 *
 * 
 *  Outputs:
 *  -------
 *  _MWM_INFO = this property is set on the specified window
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetMwmInfo (propWindow, flags, wmWindow)
    Window propWindow;
    long flags;
    Window wmWindow;

#else /* _NO_PROTO */
void SetMwmInfo (Window propWindow, long flags, Window wmWindow)
#endif /* _NO_PROTO */
{
    PropMwmInfo property;


    property.flags = flags;
    property.wmWindow = wmWindow;

    XChangeProperty (DISPLAY, propWindow, wmGD.xa_MWM_INFO, wmGD.xa_MWM_INFO,
	32, PropModeReplace, (unsigned char *)&property,
	PROP_MWM_INFO_ELEMENTS);

} /* END OF FUNCTION SetMwmInfo */



/*************************************<->*************************************
 *
 *  GetWMState (window)
 *
 *
 *  Description:
 *  -----------
 *  This function gets the WM_STATE property on a client top-level
 *  window.
 *
 *
 *  Inputs:
 *  ------
 *  window = client window from which the WM_STATE property is to be retrieved
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN = a pointer to the WM_STATE property value (NULL if not defined)
 *
 *
 *  Comments:
 *  --------
 *  This function will eventually be superceded when WM_STATE support is
 *  added to the official X release.
 * 
 *************************************<->***********************************/

PropWMState *GetWMState (window)
    Window window;

{
    int ret_val;
    PropWMState *property = NULL;
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;


    ret_val = XGetWindowProperty (DISPLAY, window, wmGD.xa_WM_STATE, 
		  0L, PROP_WM_STATE_ELEMENTS,
		  False, wmGD.xa_WM_STATE, 
		  &actual_type, &actual_format, 
		  &nitems, &leftover, (unsigned char **)&property);

    if (!((ret_val == Success) && (actual_type == wmGD.xa_WM_STATE) &&
         (nitems == PROP_WM_STATE_ELEMENTS)))
    {
        /*
         * The property could not be retrieved or is not correctly set up.
         */

        if (property)
        {
	    XFree ((char *)property);
	    property = NULL;
        }
    }

    return (property);

} /* END OF FUNCTION GetWMState */



/*************************************<->*************************************
 *
 *  SetWMState (window, state, icon)
 *
 *
 *  Description:
 *  -----------
 *  This function sets up the WM_STATE property on a client top-level
 *  window.
 *
 *
 *  Inputs:
 *  ------
 *  window = client window on which the WM_STATE property is to be set
 *
 *  state = state of the client application
 *
 *  icon = window manager's icon window
 *
 * 
 *  Outputs:
 *  -------
 *  WM_STATE = this property is set on the client window
 *
 *
 *  Comments:
 *  --------
 *  This function will eventually be superceded when WM_STATE support is
 *  added to the official X release.
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void SetWMState (window, state, icon)
    Window window;
    int state;
    Window icon;

#else /* _NO_PROTO */
void SetWMState (Window window, int state, Window icon)
#endif /* _NO_PROTO */
{
    PropWMState property;


    property.state = state;
    property.icon = icon;

    XChangeProperty (DISPLAY, window, wmGD.xa_WM_STATE, wmGD.xa_WM_STATE, 32,
	PropModeReplace, (unsigned char *)&property, PROP_WM_STATE_ELEMENTS);

} /* END OF FUNCTION SetWMState */



/*************************************<->*************************************
 *
 *  PropMwmHints *
 *  GetMwmHints (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function reads any _MWM_HINTS property that is associated with a 
 *  client window.
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 * 
 *  Outputs:
 *  -------
 *  RETURN = ptr to mwm hints property, or NULL ptr if failure
 *
 *************************************<->***********************************/

PropMwmHints *GetMwmHints (pCD)

    ClientData *pCD;

{
    int ret_val;
    PropMwmHints *property = NULL;
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;


    ret_val = XGetWindowProperty (DISPLAY, pCD->client, wmGD.xa_MWM_HINTS, 
		  0L, PROP_MWM_HINTS_ELEMENTS,
		  False, wmGD.xa_MWM_HINTS, 
		  &actual_type, &actual_format, 
		  &nitems, &leftover, (unsigned char **)&property);

    if ((ret_val == Success) && (actual_type == wmGD.xa_MWM_HINTS) &&
        (nitems == PROP_MWM_HINTS_ELEMENTS)) 
    {
	return (property);			/* indicate success */
    }


    /*
     * The property could not be retrieved or is not correctly set up.
     */

    if (property)
    {
	XFree ((char *)property);
    }

    return (NULL);			/* indicate failure */


} /* END OF FUNCTION GetMwmHints */



/*************************************<->*************************************
 *
 *  PropMwmInfo *
 *  GetMwmInfo (rootWindowOfScreen)
 *
 *
 *  Description:
 *  -----------
 *  This function reads the _MOTIF_WM_INFO property from the root window if
 *  it is setup.
 * 
 *  Inputs:
 *  ------
 *  pSD = pointer to screen data 
 *
 *  Outputs:
 *  -------
 *  RETURN = ptr to motif wm info property, or NULL ptr if no property
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
PropMwmInfo *GetMwmInfo (rootWindowOfScreen)
    Window rootWindowOfScreen;
#else /* _NO_PROTO */
PropMwmInfo *GetMwmInfo (Window rootWindowOfScreen)
#endif /* _NO_PROTO */
{
    int ret_val;
    PropMwmInfo *property = NULL;
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;


    ret_val = XGetWindowProperty (DISPLAY, rootWindowOfScreen,
                                     wmGD.xa_MWM_INFO,
                                     0L, PROP_MWM_INFO_ELEMENTS,
                                     False, wmGD.xa_MWM_INFO,
                                     &actual_type, &actual_format,
                                     &nitems, &leftover,
                                     (unsigned char **)&property);

    if ((ret_val == Success) && (actual_type == wmGD.xa_MWM_INFO) &&
        (nitems == PROP_MWM_INFO_ELEMENTS)) 
    {
	return (property);			/* indicate success */
    }


    /*
     * The property could not be retrieved or is not correctly set up.
     */

    if (property)
    {
	XFree ((char *)property);
    }

    return (NULL);			/* indicate failure */


} /* END OF FUNCTION GetMwmInfo */



/*************************************<->*************************************
 *
 *  ProcessWmColormapWindows (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function retrieves and processes the WM_COLORMAP_WINDOWS client
 *  window property.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 * 
 *  Outputs:
 *  -------
 *  pCD = (cmapWindows, clientCmapList, clientCmapCount, clientCmapIndex)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void ProcessWmColormapWindows (pCD)
    ClientData *pCD;

#else /* _NO_PROTO */
void ProcessWmColormapWindows (ClientData *pCD)
#endif /* _NO_PROTO */
{
    int rValue;
    Window *property = NULL;
    Atom actualType;
    int actualFormat;
    unsigned long leftover;
    unsigned long nitems;
    int i;
    Window *pWindows;
    Colormap *pColormaps;
    int colormapCount;
    XWindowAttributes wAttributes;
    ClientData *pcd;
    XSetWindowAttributes sAttributes;


    /*
     * pCD->clientCmapCount and pCD->clientCmapIndex are initialized in
     * WmWinInfo.c.
     */

    /*
     * Read the WM_COLORMAP_WINDOWS property.
     */

    rValue = XGetWindowProperty (DISPLAY, pCD->client,
		 wmGD.xa_WM_COLORMAP_WINDOWS, 0L,
		 (long)MAX_COLORMAP_WINDOWS_COUNT, False, AnyPropertyType,
		 &actualType, &actualFormat, &nitems, &leftover,
		 (unsigned char **)&property);


    if ((rValue == Success) && (actualType != None) && (actualFormat == 32) &&
	(nitems > 0))
    {
	/*
	 * WM_COLORMAP_WINDOWS exists and is a valid type.
	 */

        if (!(pWindows = (Window *)malloc ((nitems * sizeof (Window)) + 1)) ||
            !(pColormaps = (Colormap *)malloc ((nitems*sizeof(Colormap)) + 1)))
        {
	    /* unable to allocate space */
	    Warning ("Insufficient memory for window management data");
	    if (pWindows)
	    {
		free ((char *)pWindows);
	    }
        }
	else
	{
	    /*
	     * Check to see if the top-level client window is in the list.
	     * If it is not then add it to the head of the list.
	     */

    	    for (i = 0; i < nitems; i++)
	    {
		if (property[i] == pCD->client)
		{
		    break;
		}
	    }

	    colormapCount = 0;
	    if (i == nitems)
	    {
		/* add the client window to the colormap window list */
		pWindows[0] = pCD->client;
		pColormaps[0] = FindColormap (pCD, pCD->client);
		colormapCount++;
	    }

	    sAttributes.event_mask = (ColormapChangeMask);
    	    for (i = 0; i < nitems; i++)
    	    {
		if (pColormaps[colormapCount] = FindColormap (pCD, property[i]))
		{
		    pWindows[colormapCount] = property[i];
		    colormapCount++;
		}
		else if (XFindContext (DISPLAY, property[i],
		             wmGD.windowContextType, (caddr_t *)&pcd))
		{
		    /*
		     * The window is not a top level window or a window that
		     * is already being tracked for colormap changes.
		     * Track colormap attribute changes.
		     */

		    XChangeWindowAttributes (DISPLAY, property[i], CWEventMask,
			&sAttributes);
		

		    if (XGetWindowAttributes (DISPLAY, property[i],
			    &wAttributes))
		    {
			pWindows[colormapCount] = property[i];
			pColormaps[colormapCount] = wAttributes.colormap;
		        colormapCount++;
		    }
		}
	    }

	    /*
	     * Free up the old colormap window data if it has been set.  Set
	     * new window contexts.
	     */

	    ResetColormapData (pCD, pWindows, colormapCount);
		

	    /*
	     * Set the colormap window data.
	     */

	    pCD->clientColormap = pColormaps[0];
	    if (colormapCount > 1)
	    {
		/*
		 * The top level window and at least one other window is in
		 * the colormap windows list.
		 */

		pCD->clientCmapCount = colormapCount;
		pCD->cmapWindows = pWindows;
		pCD->clientCmapList = pColormaps;
		pCD->clientCmapIndex = 0;
	    }
	    else
	    {
		/*
		 * Only the top level window is being tracked for colormap
		 * data.
		 */

		pCD->clientCmapCount = 0;
		free ((char *)pWindows);
		free ((char *)pColormaps);
	    }
	}
    }


    if (property)
    {
	XFree ((char *)property);
    }


} /* END OF FUNCTION ProcessWmColormapWindows */



/*************************************<->*************************************
 *
 *  FindColormap (pCD, window)
 *
 *
 *  Description:
 *  -----------
 *  This function checks colormap information that is currently saved in
 *  the client data for the colormap of the specified window.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 *  window = get the colormap id for this window
 *
 * 
 *  Outputs:
 *  -------
 *  RETURN = colormap id for window (NULL if no colormap information)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Colormap FindColormap (pCD, window)
    ClientData *pCD;
    Window window;

#else /* _NO_PROTO */
Colormap FindColormap (ClientData *pCD, Window window)
#endif /* _NO_PROTO */
{
    Colormap colormap = NULL;
    int i;


    if (pCD->clientCmapCount == 0)
    {
	/*
	 * If the colormap count is 0 there is no list of colormaps and
	 * clientColormap is the colormap of the top-level window.
	 */

	if (window == pCD->client)
	{
	    colormap = pCD->clientColormap;
	}
    }
    else
    {
	for (i = 0; i < pCD->clientCmapCount; i++)
	{
	    if (pCD->cmapWindows[i] == window)
	    {
		colormap = pCD->clientCmapList[i];
		break;
	    }
	}
    }

    return (colormap);

} /* END OF FUNCTION FindColormap */



/*************************************<->*************************************
 *
 *  GetMwmMenuItems (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function reads and processes any _MWM_MENU property that is
 *  associated with a client window and returns a list of MenuItem structures
 *  specified by the property, or NULL.
 *
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data
 *
 * 
 *  Outputs:
 *  -------
 *  Return = MenuItem list or NULL.
 *
 *************************************<->***********************************/

MenuItem *GetMwmMenuItems (pCD)

    ClientData *pCD;

{
    int           rValue;
    char          *property = NULL;
    Atom          actualType;
    int           actualFormat;
    unsigned long leftover;
    unsigned long nitems;
    MenuItem     *menuItems;


    /*
     * Read the _MWM_MENU property.
     */

    rValue = XGetWindowProperty (DISPLAY, pCD->client, wmGD.xa_MWM_MENU, 0L,
		 (long)1000000, False, AnyPropertyType,
		 &actualType, &actualFormat, &nitems, &leftover,
		 (unsigned char **)&property);


    if ((rValue != Success) || (actualType == None) || (actualFormat != 8))
    /* _MWM_MENU does not exist or it is an invalid type.  */
    {
	menuItems = NULL;
    }

    else
    /* parse the property string */
    {
	menuItems = ParseMwmMenuStr (PSD_FOR_CLIENT(pCD), 
				     (unsigned char *)property);
    }

    if (property)
    {
	XFree ((char *)property);
    }

    return (menuItems);

} /* END OF FUNCTION GetMwmMenuItems */




