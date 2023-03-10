#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)WmWinList.c	3.17.1.2 91/03/12";
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


/*
 * include extern functions
 */
#include "WmWinList.h"
#include "WmCEvent.h"
#include "WmFunction.h"
#include "WmKeyFocus.h"
#include "WmResource.h"
#include "WmWinInfo.h"




/*
 * Global Variables:
 */


/*************************************<->*************************************
 *
 *  AddClientToList (pWS, pCD, onTop)
 *
 *
 *  Description:
 *  -----------
 *  This function adds a client window to the client window list.  If it is
 *  a transient window then it is added to the transient window tree that
 *  contains its transient leader.  The window stacking order is also
 *  maintained for the cases where there is a system modal window active
 *  or the window is a transient window.  If a system modal window is being
 *  added then the system modal "input screen" window is setup.
 *
 *
 *  Inputs:
 *  ------
 *  pCD	= pointer to client data for the window to be added to the list
 *
 *  pWS	= pointer to workspace data
 *
 *  onTop = if True then the window is displayed on top of the window
 *      stack and is added to the beginning of the window list, otherwise
 *      it is added to the end of the window list.
 *
 * 
 *  Outputs:
 *  -------
 *  pWS = (clientList, lastClient)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void AddClientToList (pWS, pCD, onTop)
	ClientData *pCD;
	WmWorkspaceData *pWS;
	Boolean onTop;

#else /* _NO_PROTO */
void AddClientToList (WmWorkspaceData *pWS, ClientData *pCD, Boolean onTop)
#endif /* _NO_PROTO */
{
    Boolean belowSystemModal = False;
    XWindowChanges windowChanges;
    WmScreenData *pSD = pWS->pSD;


    if (pCD->inputMode == MWM_INPUT_SYSTEM_MODAL)
    {
	/*
	 * Set up the system modal input screen window just below the
	 * system modal window.
	 */

	SetupSystemModalState (pCD);
    }
    else if (wmGD.systemModalActive &&
	     ((FindTransientTreeLeader (pCD))->inputMode !=
	      MWM_INPUT_SYSTEM_MODAL))
    {
	/*
	 * If a system modal window is active then place the window below
	 * the system modal input screen window if the window is not a
	 * descendant of the system modal window.
	 */

	windowChanges.sibling = pSD->inputScreenWindow;
	windowChanges.stack_mode = Below;
	XConfigureWindow (DISPLAY, pCD->clientFrameWin,
	    CWSibling | CWStackMode, &windowChanges);
	belowSystemModal = True;
    }

    if (pCD->transientLeader)
    {
	AddTransient (pWS, pCD);
    }
    else
    {
	pCD->clientEntry.type = NORMAL_STATE;
	pCD->clientEntry.pCD = pCD;

	if (belowSystemModal && wmGD.systemModalClient)
	{
	    AddEntryToList (pWS, &pCD->clientEntry, False /*below*/,
			    pSD->clientList);
	}
	else if (onTop)
	{
	    AddEntryToList (pWS, &pCD->clientEntry, True /*on top*/, NULL);
	}
	else
	{
	    AddEntryToList (pWS, &pCD->clientEntry, False /*on bottom*/, NULL);
	}


	if (!pCD->pIconBox && pCD->iconFrameWin)
	{
	    /*
	     * Put the icon on the bottom of the stack.
	     */

	    if (pSD->lastClient->type == MINIMIZED_STATE)
	    {
		windowChanges.sibling = pSD->lastClient->pCD->iconFrameWin;
	    }
	    else
	    {
		windowChanges.sibling = pSD->lastClient->pCD->clientFrameWin;
	    }
	    windowChanges.stack_mode = Below;
	    XConfigureWindow (DISPLAY, pCD->iconFrameWin,
	        CWSibling | CWStackMode, &windowChanges);

	    pCD->iconEntry.type = MINIMIZED_STATE;
	    pCD->iconEntry.pCD = pCD;
	    pCD->iconEntry.nextSibling = NULL;
	    pCD->iconEntry.prevSibling = pSD->lastClient;
	    pSD->lastClient->nextSibling = &pCD->iconEntry;
	    pSD->lastClient = &pCD->iconEntry;
	}
    }

} /* END OF FUNCTION AddClientToList */



/*************************************<->*************************************
 *
 *  AddEntryToList (pWS, pEntry, onTop, pStackEntry)
 *
 *
 *  Description:
 *  -----------
 *  This function adds a client list entry to the client window list.
 *  This is usually done as part of the process of changing the ordering
 *  of the window list.
 *
 *
 *  Inputs:
 *  ------
 *  pWS  = pointer to workspace data
 *  pEntry = pointer to a client list entry to be added to the client list
 *
 *  onTop = if True then the client list entry is added on top of the 
 *	specified client list stack entry (if the stack entry is not
 *	specified then the entry is added to the front of the list);
 *	otherwise the entry is added after the specified stacking entry
 *	(or to the end of the list if the stacking entry is not specified).
 *
 *  pStackEntry = pointer to a client list entry to be used as a reference
 *	in adding an entry to the client list.
 * 
 *  Outputs:
 *  -------
 *  pWS = (clientList, lastClient)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void AddEntryToList (pWS, pEntry, onTop, pStackEntry)
	WmWorkspaceData *pWS;
	ClientListEntry *pEntry;
	Boolean onTop;
	ClientListEntry *pStackEntry;

#else /* _NO_PROTO */
void AddEntryToList (WmWorkspaceData *pWS, ClientListEntry *pEntry, Boolean onTop, ClientListEntry *pStackEntry)
#endif /* _NO_PROTO */
{
    WmScreenData *pSD = pWS->pSD;

    if (onTop)
    {
	if (pStackEntry)
	{
	    if (pEntry != pStackEntry)
	    {
		pEntry->nextSibling = pStackEntry;
		pEntry->prevSibling = pStackEntry->prevSibling;
		pStackEntry->prevSibling = pEntry;
		if (pEntry->prevSibling)
		{
		    pEntry->prevSibling->nextSibling = pEntry;
		}
		else
		{
		    pSD->clientList = pEntry;
		}
	    }
	}
	else
	{
	    if (pSD->clientList != pEntry)
	    {
	        pEntry->nextSibling = pSD->clientList;
	        pEntry->prevSibling = NULL;
		if (pSD->clientList)
		{
		    pSD->clientList->prevSibling = pEntry;
		}
		else
		{
		    pSD->lastClient = pEntry;
		}
	        pSD->clientList = pEntry;
	    }
	}
    }
    else
    {
	if (pStackEntry)
	{
	    if (pEntry != pStackEntry)
	    {
		pEntry->nextSibling = pStackEntry->nextSibling;
		pEntry->prevSibling = pStackEntry;
		pStackEntry->nextSibling = pEntry;
		if (pEntry->nextSibling)
		{
		    pEntry->nextSibling->prevSibling = pEntry;
		}
		else
		{
		    pSD->lastClient = pEntry;
		}
	    }
	}
	else
	{
	    if (pSD->lastClient != pEntry)
	    {
	        pEntry->nextSibling = NULL;
	        pEntry->prevSibling = pSD->lastClient;
		if (pSD->clientList)
		{
		    pSD->lastClient->nextSibling = pEntry;
		}
		else
		{
		    pSD->clientList = pEntry;
		}
	        pSD->lastClient = pEntry;
	    }
	}
    }

} /* END OF FUNCTION AddEntryToList */



/*************************************<->*************************************
 *
 *  MoveEntryInList (pWS, pEntry, onTop, pStackEntry)
 *
 *
 *  Description:
 *  -----------
 *  This function moves a client list entry in the client window list.
 *
 *
 *  Inputs:
 *  ------
 *  pWS = pointer to workspace data
 *
 *  pEntry = pointer to a client list entry to be moved in the client list
 *
 *  onTop = if True then the client list entry is moved on top of the 
 *	specified client list stack entry (if the stack entry is not
 *	specified then the entry is moved to the front of the list);
 *	otherwise the entry is moved after the specified stacking entry
 *	(or to the end of the list if the stacking entry is not specified).
 *
 *  pStackEntry = pointer to a client list entry to be used as a reference
 *	in moving an entry in the client list.
 * 
 *  Outputs:
 *  -------
 *  pWS = (clientList, lastClient)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void MoveEntryInList (pWS, pEntry, onTop, pStackEntry)
	WmWorkspaceData *pWS;
	ClientListEntry *pEntry;
	Boolean onTop;
	ClientListEntry *pStackEntry;

#else /* _NO_PROTO */
void MoveEntryInList (WmWorkspaceData *pWS, ClientListEntry *pEntry, Boolean onTop, ClientListEntry *pStackEntry)
#endif /* _NO_PROTO */
{
    DeleteEntryFromList (pWS, pEntry);
    AddEntryToList (pWS, pEntry, onTop, pStackEntry);

} /* END OF FUNCTION MoveEntryInList */



/*************************************<->*************************************
 *
 *  DeleteEntryFromList (pWS, pListEntry)
 *
 *
 *  Description:
 *  -----------
 *  This function deletes a client list entry from the client window list.
 *  This is usually done as part of the process of changing the ordering
 *  of the window list.
 *
 *
 *  Inputs:
 *  ------
 *  pWS = pointer to workspace data
 *  listEntry = pointer to a client list entry
 * 
 *  Outputs:
 *  -------
 *  pWS = (clientList, lastClient)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void DeleteEntryFromList (pWS, pListEntry)
	WmWorkspaceData *pWS;
	ClientListEntry *pListEntry;

#else /* _NO_PROTO */
void DeleteEntryFromList (WmWorkspaceData *pWS, ClientListEntry *pListEntry)
#endif /* _NO_PROTO */
{
    
    if (pListEntry->prevSibling)
    {
	pListEntry->prevSibling->nextSibling = pListEntry->nextSibling;
    }
    else
    {
	pWS->pSD->clientList = pListEntry->nextSibling;
    }

    if (pListEntry->nextSibling)
    {
	pListEntry->nextSibling->prevSibling = pListEntry->prevSibling;
    }
    else
    {
	pWS->pSD->lastClient = pListEntry->prevSibling;
    }

} /* END OF FUNCTION DeleteEntryFromList */



/*************************************<->*************************************
 *
 *  DeleteClientFromList (pWS, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function deletes a client from the client window list.  If this is
 *  a transient window then it is deleted from its transient window tree.
 *  If this is a system modal window then clean up the system modal state.
 *
 *
 *  Inputs:
 *  ------
 *  pCD	= pointer to client data for the window to be added to the list
 * 
 *  Outputs:
 *  -------
 *  pWS = (clientList, lastClient)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void DeleteClientFromList (pWS, pCD)
	WmWorkspaceData *pWS;
	ClientData *pCD;

#else /* _NO_PROTO */
void DeleteClientFromList (WmWorkspaceData *pWS, ClientData *pCD)
#endif /* _NO_PROTO */
{
    WmScreenData *pSD = pWS->pSD;

    if (pCD->transientLeader)
    {
	DeleteTransient (pCD);
    }
    else
    {
	/*
	 * If this is a system modal window then clean up the system modal
	 * state.
	 */

	if (pCD->inputMode == MWM_INPUT_SYSTEM_MODAL)
	{
	    UndoSystemModalState ();
	}

	/*
	 * Remove the client and icon entries from the window list.
	 */

	if (!pCD->pIconBox && pCD->iconFrameWin)
	{
	    if (pCD->iconEntry.prevSibling)
	    {
		pCD->iconEntry.prevSibling->nextSibling =
						pCD->iconEntry.nextSibling;
	    }
	    else
	    {
		pSD->clientList = pCD->iconEntry.nextSibling;
	    }
	    if (pCD->iconEntry.nextSibling)
	    {
		pCD->iconEntry.nextSibling->prevSibling =
						pCD->iconEntry.prevSibling;
	    }
	    else
	    {
		pSD->lastClient = pCD->iconEntry.prevSibling;
	    }
	}

	if (pCD->clientEntry.prevSibling)
	{
	    pCD->clientEntry.prevSibling->nextSibling =
						pCD->clientEntry.nextSibling;
	}
	else
	{
	    pSD->clientList = pCD->clientEntry.nextSibling;
	}

	if (pCD->clientEntry.nextSibling)
	{
	    pCD->clientEntry.nextSibling->prevSibling =
						pCD->clientEntry.prevSibling;
	}
	else
	{
	    pSD->lastClient = pCD->clientEntry.prevSibling;
	}
    }

} /* END OF FUNCTION DeleteClientFromList */



/*************************************<->*************************************
 *
 *  AddTransient (pWS, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function adds the transient window to the lead window's list of
 *  transients.
 *
 *
 *  Inputs:
 *  ------
 *  pWS	= pointer to workspace data
 *  pCD	= pointer to client data of a transient window
 *
 * 
 *  Outputs:
 *  -------
 *  pCD->transientLeader = (transientChildren, modalCount)
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void AddTransient (pWS, pCD)
	WmWorkspaceData *pWS;
	ClientData *pCD;

#else /* _NO_PROTO */
void AddTransient (WmWorkspaceData *pWS, ClientData *pCD)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader = pCD->transientLeader;
    ClientData *pcdTop = FindTransientTreeLeader (pCD);
    Boolean restackTransients;
    WmScreenData *pSD = pWS->pSD;


    pCD->transientSiblings = pcdLeader->transientChildren;
    pcdLeader->transientChildren = pCD;


    /*
     * Insure that the new transient window is on top of its siblings
     * and that the transient window tree is on top of the window
     * stack (this is the standard behavior for newly mapped and
     * managed windows).  If there is a system modal window that the
     * transient window is not associated with then don't raise the
     * transient tree.
     */

    restackTransients = PutTransientOnTop (pcdLeader);


    /*
     * Handle application modal transient windows
     */

    if (pCD->inputMode == MWM_INPUT_PRIMARY_APPLICATION_MODAL)
    {
	/*
	 * If this is a primary application modal window then increment 
	 * the modal count for transient leaders that are directly up 
	 * the transient tree.
	 *
	 * (This is the old MWM_INPUT_APPLICATION_MODAL behavior.)
	 */
        while (pcdLeader)
        {
	    MarkModalTransient (pcdLeader, pCD);
	    pcdLeader = pcdLeader->transientLeader;
        }
    }
    else if (pCD->inputMode == MWM_INPUT_FULL_APPLICATION_MODAL)
    {
	/*
	 * If this is a full application modal window then increment 
	 * the modal count for the rest of the transient tree.
	 */

	MarkModalSubtree (pcdTop, pCD);
    }
    else if (pcdTop->fullModalCount)
    {
	/*
	 * There is already a full application modal window in the tree
	 */
	pcdLeader = pCD->transientLeader;
    	if ((pcdLeader->inputMode != MWM_INPUT_FULL_APPLICATION_MODAL) ||
	    (IS_APP_MODALIZED(pcdLeader)))
	{
	    /*
	     * The immediate parent of this transient is not the
	     * current full application modal window.  Set the full
	     * modal count to the parent's so that they both become
	     * unmodalized at the same time. This allows a full
	     * app modal window to have active, non-modal transients.
	     */
	    pCD->fullModalCount = pcdLeader->fullModalCount;
	}
    }


    /*
     * Do the actual restacking in the X window stack if necessary.
     */

    if ((pSD->clientList != &pcdTop->clientEntry) && !wmGD.systemModalActive)
    {
	F_Raise (NULL, pCD, NULL);
    }
    else if (restackTransients)
    {
	RestackTransientsAtWindow (pCD);
    }
    else if (pCD != FindTransientOnTop (pcdTop))
    {
	StackTransientWindow (pCD);
    }


} /* END OF FUNCTION AddTransient */



/*************************************<->*************************************
 *
 *  MarkModalSubtree (pcdTree, pcdAvoid)
 *
 *
 *  Description:
 *  -----------
 *  This function marks the transient tree with pcdTree as its leader.
 *  If pcdAvoid is in the tree, it is not marked.
 *
 *  Inputs:
 *  ------
 *  pcdTree	= pointer to client data of the tree to mark
 *  pcdAvoid	= pointer to client data to not mark if in tree
 *
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void MarkModalSubtree (pcdTree, pcdAvoid)
	ClientData *pcdTree, *pcdAvoid;

#else /* _NO_PROTO */
void MarkModalSubtree (ClientData *pcdTree, ClientData *pcdAvoid)
#endif /* _NO_PROTO */
{
    /* Mark children, if any */

    if (pcdTree->transientChildren)
	MarkModalSubtree (pcdTree->transientChildren, pcdAvoid);

    /* Mark this node */

    if (pcdTree != pcdAvoid) 
    {
	MarkModalTransient (pcdTree, pcdAvoid);
    }

    /* Mark siblings */

    if (pcdTree->transientSiblings)
	MarkModalSubtree (pcdTree->transientSiblings, pcdAvoid);

}


/*************************************<->*************************************
 *
 *  MarkModalTransient (pcdLeader, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function marks a transient window for application modal processing.
 *  Grabs are done to eat up pointer button events.
 *
 *  Inputs:
 *  ------
 *  pcdLeader = pointer to client data to mark
 *  pCD	= pointer to client data of new transient
 *
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void MarkModalTransient (pcdLeader, pCD)
	ClientData *pcdLeader, *pCD;

#else /* _NO_PROTO */
void MarkModalTransient (ClientData *pcdLeader, ClientData *pCD)
#endif /* _NO_PROTO */
{
    if (!IS_APP_MODALIZED(pcdLeader))
    {
	/*
	 * Eat pointer button events while application modal.
	 */
	XGrabButton (DISPLAY, AnyButton, AnyModifier,
	    pcdLeader->clientBaseWin, True,
	    ButtonPressMask | ButtonMotionMask, GrabModeAsync,
	    GrabModeAsync, None, wmGD.workspaceCursor);
    }

    /* bump application modal count */
    if (pCD->inputMode == MWM_INPUT_FULL_APPLICATION_MODAL)
	pcdLeader->fullModalCount++;
    else 
	pcdLeader->primaryModalCount++;
}


/*************************************<->*************************************
 *
 *  DeleteTransient (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function deletes the transient window from the lead window's list
 *  of transients
 *
 *  Much of the complication of this code arises from trying to handle
 *  mixtures of both full- and primary-application modal transients.
 *  It also tries to handle the case where a sequence of application
 *  modal transients appear in different places in the transient tree
 *  (i.e. not as descendents of a previously existing full app modal 
 *  transient).
 *
 *  Inputs:
 *  ------
 *  pCD	= pointer to client data of transient.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void DeleteTransient (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void DeleteTransient (ClientData *pCD)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader;
    ClientData *pcdPrev; 
    int modalCount;


    /*
     * Handle primary application modality.
     * Reset the modal window counts for the leader windows up through the
     * transient window tree.
     */

    modalCount = pCD->primaryModalCount;
    if (pCD->inputMode == MWM_INPUT_PRIMARY_APPLICATION_MODAL)
    {
	modalCount += 1;
    }
    if (modalCount)
    {
	pcdLeader = pCD->transientLeader;
	while (pcdLeader)
	{
	    if (modalCount)
		UnMarkModalTransient (pcdLeader, modalCount, pCD);

	    pcdLeader = pcdLeader->transientLeader;
	}
    }

    /*
     * Handle full application modality.
     * Undo application modal windows in a depth first manner.
     */

    pcdLeader = FindTransientTreeLeader (pCD);

    if (pCD->transientChildren)
    {
	DeleteFullAppModalChildren (pcdLeader, pCD->transientChildren);
    }
    if (pCD->inputMode == MWM_INPUT_FULL_APPLICATION_MODAL)

    {
	/*
	 * If this is a full application modal window then decrement 
	 * the modal count for the rest of the transient tree.
	 */

	FixupFullAppModalCounts (pcdLeader, pCD);
    }


    /*
     * Delete this transient from its parent's list of transient windows.
     */

    pcdPrev = pCD->transientLeader->transientChildren;
    if(pcdPrev)
    {
	if (pcdPrev == pCD)
	{
	    pCD->transientLeader->transientChildren = pCD->transientSiblings;
	}
	else
	{
	    while (pcdPrev->transientSiblings != pCD)
	    {
		pcdPrev = pcdPrev->transientSiblings;
	    }
	    pcdPrev->transientSiblings = pCD->transientSiblings;
	}
    }

} /* END OF FUNCTION DeleteTransient */


/*************************************<->*************************************
 *
 *  DeleteFullAppModalChildren (pcdLeader, pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function handles the clean-up of nested full application modal
 *  windows. The deletion has to be handled carefully to keep a correct
 *  fullModalCount on the transients that remain in the tree.
 *
 *  The algorithm is to traverse the transient children depth first and
 *  fix up the tree's fullModalCount for each full application modal
 *  window that's found. 
 *
 *  Inputs:
 *  ------
 *  pcdLeader	= pointer to client data of transient tree root.
 *  pCD		= pointer to client data of transient subtree to delete.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void DeleteFullAppModalChildren (pcdLeader, pCD)
	ClientData *pcdLeader, *pCD;

#else /* _NO_PROTO */
void DeleteFullAppModalChildren (ClientData *pcdLeader, ClientData *pCD)
#endif /* _NO_PROTO */
{

    /* recursively do children first */
    if (pCD->transientChildren)
	DeleteFullAppModalChildren (pcdLeader, pCD->transientChildren);

    /* do fullAppModal fixup for this guy */
    FixupFullAppModalCounts (pcdLeader, pCD);

    /* do siblings of passed transient */
    if (pCD->transientSiblings)
	DeleteFullAppModalChildren (pcdLeader, pCD->transientSiblings);

    
} /* END OF FUNCTION DeleteFullAppModalChildren */


/*************************************<->*************************************
 *
 *  FixupFullAppModalCounts (pcdLeader, pcdDelete)
 *
 *
 *  Description:
 *  -----------
 *  This function traverses the entire transient tree (pointed to by 
 *  pcdLeader) and fixes up the fullModalCounts to reflect the removal 
 *  of pcdDelete. 
 *
 *  The fix-up consists of decrementing the count
 *  of the remaining full app modal windows in the tree iff the remaining
 *  window's fullModalCount is greater than the fullModalCount of the 
 *  transient being deleted.
 *
 *  Inputs:
 *  ------
 *  pcdLeader	= pointer to client data for head of transient tree.
 *  pcdDelet	= pointer to client data of transient being deleted.
 *
 *************************************<->***********************************/

void
FixupFullAppModalCounts (pcdLeader, pcdDelete)
ClientData *pcdLeader, *pcdDelete;

{

    /* fixup children */
    if (pcdLeader->transientChildren) 
    {
	FixupFullAppModalCounts (pcdLeader->transientChildren, pcdDelete);
    }

    /* 
     * fixup leader: decrement the count if it is greater than
     * the transient being deleted.
     *
     */
    if (pcdLeader->fullModalCount > pcdDelete->fullModalCount)
    {
	UnMarkModalTransient (pcdLeader, 1, pcdDelete);
    }

    /* fixup siblings */
    if (pcdLeader->transientSiblings) 
    {
	FixupFullAppModalCounts (pcdLeader->transientSiblings, pcdDelete);
    }
    
} /* END OF FUNCTION FixupFullAppModalCounts */



/*************************************<->*************************************
 *
 *  UnMarkModalTransient (pcdModee, modalCount, pcdModal)
 *
 *
 *  Description:
 *  -----------
 *  This function unmarks a transient window for application modal processing.
 *  Original grabs are restored.
 *
 *  Inputs:
 *  ------
 *  pcdModee   = pointer to client data for transient to unmark
 *  pcdModal   = pointer to client data for modal transient
 *  modalCount = amount to decrement the client's modal count by
 *
 * 
 *************************************<->***********************************/

#ifdef _NO_PROTO
void UnMarkModalTransient (pcdModee, modalCount, pcdModal)
	ClientData *pcdModee, *pcdModal;
	int modalCount;

#else /* _NO_PROTO */
void UnMarkModalTransient (ClientData *pcdModee, int modalCount, ClientData *pcdModal)
#endif /* _NO_PROTO */
{
    /* decrement application modal count */
    if (pcdModal->inputMode == MWM_INPUT_FULL_APPLICATION_MODAL)
	pcdModee->fullModalCount -= modalCount;
    else 
	pcdModee->primaryModalCount -= modalCount;

    /*
     * Restore original button bindings/grabs if not modal anymore
     */
    if (!IS_APP_MODALIZED(pcdModee))
    {
	XUngrabButton (DISPLAY, AnyButton, AnyModifier, 
	    pcdModee->clientBaseWin);

	SetupCButtonBindings (pcdModee->clientBaseWin, BUTTON_SPECS(pcdModee));

	if ((wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_EXPLICIT) &&
	    (wmGD.keyboardFocus != pcdModee))
	{
	    DoExplicitSelectGrab (pcdModee->clientBaseWin);
	}
    }
}


/*************************************<->*************************************
 *
 *  PutTransientOnTop (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function changes the transient window list to insure that the
 *  specified transient window is on top of its siblings and its parent
 *  is on top of its siblings, etc.  The sibling list is ordered such
 *  that the first window in the list is on top of second window in the
 *  list, etc.
 *
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a transient window
 *
 * 
 *  Outputs:
 *  -------
 *  pcdLeader = (transientSiblings)
 *
 *  RETURN = True if the transient tree needs to be restacked
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean PutTransientOnTop (pcd)
	ClientData *pcd;

#else /* _NO_PROTO */
Boolean PutTransientOnTop (ClientData *pcd)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader;
    ClientData *pcdPrev;
    Boolean restack = False;


    pcdLeader = pcd->transientLeader;
    if (pcdLeader != NULL)
    {
	pcdPrev = pcdLeader->transientChildren;
	if (pcdPrev != pcd)
	{
	    while (pcdPrev->transientSiblings != pcd)
	    {
		pcdPrev = pcdPrev->transientSiblings;
	    }
	    pcdPrev->transientSiblings = pcd->transientSiblings;
	    pcd->transientSiblings = pcdLeader->transientChildren;
	    pcdLeader->transientChildren = pcd;
	    restack = True;
	}

	if (PutTransientOnTop (pcdLeader))
	{
	    restack = True;
	}
    }

    return (restack);

} /* END OF FUNCTION PutTransientOnTop */



/*************************************<->*************************************
 *
 *  PutTransientBelowSiblings (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function changes the transient window list to insure that the
 *  specified transient window is below its sibling windows.
 *
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a transient window
 *
 * 
 *  Outputs:
 *  -------
 *  pcdLeader = (transientSiblings)
 *
 *  RETURN = True if restacking has been done in the transient window tree.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean PutTransientBelowSiblings (pcd)
	ClientData *pcd;

#else /* _NO_PROTO */
Boolean PutTransientBelowSiblings (ClientData *pcd)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader;
    ClientData *pcdNext;
    Boolean restack = False;


    pcdLeader = pcd->transientLeader;
    if (pcdLeader)
    {
	if (pcd->transientSiblings || (pcdLeader->transientChildren != pcd))
	{
	    restack = True;
	    if (pcdLeader->transientChildren == pcd)
	    {
	        pcdLeader->transientChildren = pcd->transientSiblings;
	    }

	    pcdNext = pcdLeader->transientChildren;
	    while (pcdNext->transientSiblings)
	    {
		if (pcdNext->transientSiblings == pcd)
		{
		    pcdNext->transientSiblings = pcd->transientSiblings;
		}
		else
		{
		    pcdNext = pcdNext->transientSiblings;
		}
	    }
	    pcdNext->transientSiblings = pcd;
	}
	pcd->transientSiblings = NULL;
    }

    return (restack);

} /* END OF FUNCTION PutTransientBelowSiblings */



/*************************************<->*************************************
 *
 *  RestackTransients (pcd, doTop)
 *
 *
 *  Description:
 *  -----------
 *  This function restacks windows in a transient window tree.  Secondary
 *  (transient) windows are stacked on top of their associated primary
 *  windows and the first secondary window in the transientSiblings list
 *  is stacked on top of the second window in the list, etc.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a window in a transient tree
 *
 *  doTop = True, put transient on top before stacking
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void RestackTransients (pcd, doTop)
	ClientData *pcd;
	Boolean doTop;

#else /* _NO_PROTO */
void RestackTransients (ClientData *pcd, Boolean doTop)
#endif /* _NO_PROTO */
{
    ClientData *pcdLeader;
    int count;
    static int size = 0;
    static Window *windows = NULL;
    Window *nextWindow;


    /*
     * Build a restacking list and do the restacking.
     */

    pcdLeader = FindTransientTreeLeader (pcd);
    count = CountTransientChildren (pcdLeader);

    if (count > size)
    {
	/*
	 * Expand the (static) windows buffer that is used in restacking.
	 */

	if (!(windows =
		(Window *)WmMalloc ((char*)windows, (count + 5) * sizeof (Window))))
	{
	    /* cannot get memory space */
	    size = 0;
	    return;
	}
	size = count + 5;
    }

    nextWindow = MakeTransientWindowList (windows, pcdLeader);
    *nextWindow = pcdLeader->clientFrameWin;

    if (doTop && (count > 1))
    {
	XWindowChanges windowChanges;

	/*
	 * Force the transient we want on top to the top first
	 * before calling XRestackWindows to avoid spurious
	 * exposures and unnecessary repainting.
	 */

	windowChanges.sibling = windows[1];
	windowChanges.stack_mode = Above;
	XConfigureWindow (DISPLAY, windows[0],
	    CWSibling | CWStackMode, &windowChanges);
    }

    XRestackWindows (DISPLAY, windows, count);


} /* END OF FUNCTION RestackTransients */



/*************************************<->*************************************
 *
 *  RestackTransientsAtWindow (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function restacks windows in a transient window tree.  The
 *  "anchor point" in the stack for the transient window tree is the
 *  specified window.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a window in a transient tree
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void RestackTransientsAtWindow (pcd)
     ClientData *pcd;

#else /* _NO_PROTO */
void RestackTransientsAtWindow (ClientData *pcd)
#endif /* _NO_PROTO */
{
    ClientData *pcdTop;
    XWindowChanges windowChanges;


    if ((pcdTop = FindTransientOnTop (pcd)) != pcd)
    {
	windowChanges.sibling = pcd->clientFrameWin;
	windowChanges.stack_mode = Above;
	XConfigureWindow (DISPLAY, pcdTop->clientFrameWin,
	    CWSibling | CWStackMode, &windowChanges);
    }

    RestackTransients (pcd, False);

} /* END OF FUNCTION RestackTransientsAtWindow */



/*************************************<->*************************************
 *
 *  FindTransientTreeLeader (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function identifies the leader of the transient tree that
 *  contains the specified client.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a window in a transient tree.
 *
 *  Outputs:
 *  -------
 *  RETURN = pointer to the client data for the transient tree leader.
 *
 *************************************<->***********************************/

ClientData * FindTransientTreeLeader (pcd)
	ClientData *pcd;

{

    /*
     * Find the head of the transient window tree.
     */

    while (pcd->transientLeader)
    {
	pcd = pcd->transientLeader;
    }

    return (pcd);

} /* END OF FUNCTION FindTransientTreeLeader */



/*************************************<->*************************************
 *
 *  CountTransientChildren (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function returns a count of the number of children in the 
 *  transient window tree headed by the specified client window.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a window in a transient tree
 *
 *  Outputs:
 *  -------
 *  RETURN = count of transient windows in the transient window tree
 *
 *************************************<->***********************************/

int
CountTransientChildren (pcd)
ClientData *pcd;

{
    ClientData *pcdNext;
    int count = 1;


    pcdNext = pcd->transientChildren;
    while (pcdNext)
    {
	if (pcdNext->transientChildren)
	{
	    count += CountTransientChildren (pcdNext);
	}
	else
	{
	    count++;
	}
	pcdNext = pcdNext->transientSiblings;
    }

    return (count);

} /* END OF FUNCTION CountTransientChildren */



/*************************************<->*************************************
 *
 *  MakeTransientWindowList (windows, pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function makes a transient window list of windows in the 
 *  transient window tree headed by the specified client.  This list is
 *  to be passed to XRestackWindows.
 *  
 *
 *  Inputs:
 *  ------
 *  windows = pointer to the windows list to be filled out
 *
 *  pcd	= pointer to client data of a window in a transient tree
 *
 *  Outputs:
 *  -------
 *  RETURN = pointer to the next entry in the windows list
 *
 *************************************<->***********************************/

Window * MakeTransientWindowList (windows, pcd)
	Window *windows;
	ClientData *pcd;

{
    ClientData *pcdNext;


    pcdNext = pcd->transientChildren;
    while (pcdNext)
    {
	if (pcdNext->transientChildren)
	{
	    windows = MakeTransientWindowList (windows, pcdNext);
	}
	*windows = pcdNext->clientFrameWin;
	windows++;
	pcdNext = pcdNext->transientSiblings;
    }

    return (windows);


} /* END OF FUNCTION MakeTransientWindowList */



/*************************************<->*************************************
 *
 *  FindTransientFocus (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function identifies a window in the transient tree that is headed
 *  by the specified client that can accept the keyboard input.  The
 *  effect of application modal windows is taken into account.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a window in a transient tree.
 *
 *  Outputs:
 *  -------
 *  RETURN = pointer to the client data for a window that can accept the
 *      keyboard input focus.
 *
 *************************************<->***********************************/

ClientData * FindTransientFocus (pcd)
	ClientData *pcd;

{

    ClientData *pcdFocus;

    /*
     * Find a window that does not have an application modal subordinate.
     * First, search descendents
     */

    pcdFocus = pcd;
    while (pcdFocus->transientChildren && IS_APP_MODALIZED(pcdFocus))
    {
	pcdFocus = pcdFocus->transientChildren;
    }

    /*
     * If (search of descendents FAILS) then search siblings.
     */
    
    if (IS_APP_MODALIZED(pcdFocus))
    {
	ClientData *pcdSibling;

	pcdFocus = pcd;
	while (pcdFocus && IS_APP_MODALIZED(pcdFocus))
	{
	    pcdSibling = pcdFocus;
	    while (pcdSibling->transientSiblings && IS_APP_MODALIZED(pcdFocus))
	    {
		pcdSibling = pcdSibling->transientSiblings;
	    }
	    if (IS_APP_MODALIZED(pcdSibling))
	    {
		pcdFocus = pcdFocus->transientChildren;
	    }
	    else
	    {
		pcdFocus = pcdSibling;
		break;
	    }
	}
    }

    return (pcdFocus);

} /* END OF FUNCTION FindTransientFocus */



/*************************************<->*************************************
 *
 *  FindTransientOnTop (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function identifies the top-most transient window in the
 *  transient window tree that contains the specified client.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a window in a transient tree.
 *
 *  Outputs:
 *  -------
 *  RETURN = pointer to the client data for the top-most transient window.
 *
 *************************************<->***********************************/

ClientData * FindTransientOnTop (pcd)
	ClientData *pcd;

{

    /*
     * Find the head of the transient window tree.
     */

    pcd = FindTransientTreeLeader (pcd);


    /*
     * Find the top-most transient window (the window in the transient tree
     * that is highest in the window stack).
     */

    while (pcd->transientChildren)
    {
	pcd = pcd->transientChildren;
    }

    return (pcd);

} /* END OF FUNCTION FindTransientOnTop */



/*************************************<->*************************************
 *
 *  StackWindow (pWS, pEntry, onTop, pStackEntry)
 *
 *
 *  Description:
 *  -----------
 *  This function stacks a window of a particular type (normal or icon)
 *  to the top or botton of the window stack on the screen.
 *  
 *
 *  Inputs:
 *  ------
 *  pWS = pointer to workspace data
 *
 *  pEntry = pointer to the client list entry for the window to be restacked.
 *
 *  onTop = if True then the window is to be restacked on top of the
 *	specified stack window (if the stack window is not specified then
 *	the entry is added to the top of the window stack)
 *	otherwise the window is stacked below the specified stack window
 *	(or at the bottom of the window stack if the stack window is not
 *	specified).
 *
 *  pStackEntry = pointer to a client list entry for a window in the window
 *	stack that is to be used as a reference in restacking.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void StackWindow (pWS, pEntry, onTop, pStackEntry)
	WmWorkspaceData *pWS;
	ClientListEntry *pEntry;
	Boolean onTop;
	ClientListEntry *pStackEntry;

#else /* _NO_PROTO */
void StackWindow (WmWorkspaceData *pWS, ClientListEntry *pEntry, Boolean onTop, ClientListEntry *pStackEntry)
#endif /* _NO_PROTO */
{
    Window stackWindow;
    Boolean stackTransientTreeWindows = False;
    Window activeIconWindow;
    Window window;
    XWindowChanges changes;
    WmScreenData *pSD = pWS->pSD;


    if (pStackEntry)
    {
	if (pStackEntry->type == MINIMIZED_STATE)
	{
	    stackWindow = ICON_FRAME_WIN(pStackEntry->pCD);
	}
	else
	{
	    stackWindow = pStackEntry->pCD->clientFrameWin;
	}
    }
    else
    {
	stackWindow = NULL;
    }

    if (pEntry->type == MINIMIZED_STATE)
    {
	window = ICON_FRAME_WIN(pEntry->pCD);
    }
    else
    {
	/*
	 * Restack the transient tree if appropriate.
	 */

	if (pEntry->pCD->transientLeader || pEntry->pCD->transientChildren)
	{
	    stackTransientTreeWindows = True;
	    window = (FindTransientOnTop (pEntry->pCD))->clientFrameWin;
	}
	else
	{
	    window = pEntry->pCD->clientFrameWin;
	}
    }


    /*
     * The active icon text label must be restacked along with the associated
     * icon.
     */

    if ((pEntry->type == MINIMIZED_STATE) &&
	(pEntry->pCD == wmGD.keyboardFocus) &&
	(ICON_DECORATION(pEntry->pCD) & ICON_ACTIVE_LABEL_PART) &&
	(ACTIVE_ICON_TEXT_WIN))
    {
	activeIconWindow = ACTIVE_ICON_TEXT_WIN;
    }
    else
    {
	activeIconWindow = NULL;
    }

    if (onTop)
    {
	if (stackWindow == NULL)
	{
	    if (pSD->clientList->type == MINIMIZED_STATE)
	    {
		stackWindow = ICON_FRAME_WIN(pSD->clientList->pCD);
	    }
	    else
	    {
		if (pSD->clientList->pCD->transientChildren)
		{
		    stackWindow =
		     (FindTransientOnTop(pSD->clientList->pCD))->clientFrameWin;
		}
		else
		{
		    stackWindow = pSD->clientList->pCD->clientFrameWin;
		}
	    }
	}

	if (activeIconWindow)
	{
	    changes.sibling = stackWindow;
	    changes.stack_mode = Above;
	    XConfigureWindow (DISPLAY, activeIconWindow,
	        (CWSibling | CWStackMode), &changes);
	    changes.sibling = activeIconWindow;
	    changes.stack_mode = Below;
	    XConfigureWindow (DISPLAY, window, (CWSibling | CWStackMode),
	        &changes);
	}
	else
	{
	    changes.sibling = stackWindow;
	    changes.stack_mode = Above;
	    XConfigureWindow (DISPLAY, window, (CWSibling | CWStackMode),
	        &changes);
	    if (stackTransientTreeWindows)
	    {
	        RestackTransients (pEntry->pCD, False);
	    }
	}
    }
    else
    {
	if (stackWindow == NULL)
	{
	    if (pSD->lastClient->type == MINIMIZED_STATE)
	    {
		stackWindow = ICON_FRAME_WIN(pSD->lastClient->pCD);
	    }
	    else
	    {
		stackWindow = pSD->lastClient->pCD->clientFrameWin;
	    }
	}

	if (activeIconWindow)
	{
	    changes.sibling = stackWindow;
	    changes.stack_mode = Below;
	    XConfigureWindow (DISPLAY, activeIconWindow,
			      (CWSibling | CWStackMode), &changes);
	    changes.sibling = activeIconWindow;
	    changes.stack_mode = Below;
	    XConfigureWindow (DISPLAY, window, (CWSibling | CWStackMode),
			      &changes);
	}
	else
	{
	    changes.sibling = stackWindow;
	    changes.stack_mode = Below;
	    XConfigureWindow (DISPLAY, window, (CWSibling | CWStackMode),
			      &changes);
	    if (stackTransientTreeWindows)
	    {
	        RestackTransients (pEntry->pCD, False);
	    }
	}
    }

} /* END OF FUNCTION StackWindow */



/*************************************<->*************************************
 *
 *  StackTransientWindow (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function stacks a transient window within its transient window
 *  tree on the screen.  The transient window tree should indicate the
 *  intended stacking position.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd	= pointer to client data of a window in a transient tree
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void StackTransientWindow (pcd)
	ClientData *pcd;

#else /* _NO_PROTO */
void StackTransientWindow (ClientData *pcd)
#endif /* _NO_PROTO */
{
    XWindowChanges changes;
    ClientData *pcdPrev;


    if (pcd->transientLeader->transientChildren == pcd)
    {
	if (pcd->transientSiblings)
	{
	    changes.sibling = pcd->transientSiblings->clientFrameWin;
	}
	else
	{
	    changes.sibling = pcd->transientLeader->clientFrameWin;
	}
	changes.stack_mode = Above;
    }
    else
    {
	pcdPrev = pcd->transientLeader;
	while (pcdPrev->transientSiblings != pcd)
	{
	    pcdPrev = pcdPrev->transientSiblings;
	}
	changes.sibling = pcdPrev->clientFrameWin;
	changes.stack_mode = Below;
    }

    XConfigureWindow (DISPLAY, pcd->clientFrameWin, (CWSibling | CWStackMode),
	&changes);


} /* END OF FUNCTION StackTransientWindow */



/*************************************<->*************************************
 *
 *  CheckIfClientObscuring (pcdTop, pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function determines whether a window or a transient window tree
 *  is obscuring (at least partially) a window or a transient window tree
 *  that is below it in the window stack.
 *  
 *
 *  Inputs:
 *  ------
 *  pcdTop = pointer to client data for a window (it may be the leader of
 *	a transient tree; this window is the higher in the window stack
 *	than the window it is be checked against.
 *
 *  pcd = pointer to client data for a window (it may be the leader of
 *	a transient tree.
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if the top window(s) overlap the lower window(s)
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean CheckIfClientObscuring (pcdTop, pcd)
	ClientData *pcdTop;
	ClientData *pcd;

#else /* _NO_PROTO */
Boolean CheckIfClientObscuring (ClientData *pcdTop, ClientData *pcd)
#endif /* _NO_PROTO */
{
    Boolean obscuring = False;
    ClientData *pcdNext;


    /*
     * Check only if the top window is visible onscreen.
     */

    if (pcdTop->transientChildren && (pcdTop->clientState != MINIMIZED_STATE))
    {
	pcdNext = pcdTop->transientChildren;
	while (pcdNext && !obscuring)
	{
	    obscuring = CheckIfClientObscuring (pcdNext, pcd);
	    pcdNext = pcdNext->transientSiblings;
	}
    }

    if (!obscuring && pcd->transientChildren &&
	(pcd->clientState != MINIMIZED_STATE))
    {
	pcdNext = pcd->transientChildren;
	while (pcdNext && !obscuring)
	{
	    obscuring = CheckIfClientObscuring (pcdTop, pcdNext);
	    pcdNext = pcdNext->transientSiblings;
	}
    }

    if (!obscuring)
    {
	obscuring = CheckIfObscuring (pcdTop, pcd);
    }

    return (obscuring);

} /* END OF FUNCTION CheckIfClientObscuring */



/*************************************<->*************************************
 *
 *  CheckIfObscuring (pcdA, pcdB)
 *
 *
 *  Description:
 *  -----------
 *  This function determines whether a window (not a transient tree)
 *  is obscuring (at least partially) a window (not a transient tree)
 *  that is below it in the window stack.
 *  
 *
 *  Inputs:
 *  ------
 *  pcdA = pointer to client data for a window; this window is higher in
 *	the window stack than the window it is be checked against.
 *
 *  pcdB = pointer to client data for a window.
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if the top window overlaps the lower window
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean CheckIfObscuring (pcdA, pcdB)
	ClientData *pcdA;
	ClientData *pcdB;

#else /* _NO_PROTO */
Boolean CheckIfObscuring (ClientData *pcdA, ClientData *pcdB)
#endif /* _NO_PROTO */
{
    Boolean obscuring = False;
    int aX1;
    int aX2;
    int aY1;
    int aY2;
    int bX1;
    int bX2;
    int bY1;
    int bY2;


    if (pcdA->clientState == NORMAL_STATE)
    {
	aX1 = pcdA->clientX - pcdA->clientOffset.x;
	aY1 = pcdA->clientY - pcdA->clientOffset.y;
	aX2 = aX1 + pcdA->clientWidth + (2 * pcdA->clientOffset.x) - 1;
	aY2 = aY1 + pcdA->clientHeight + pcdA->clientOffset.y +
	      pcdA->clientOffset.x - 1;
    }
    else if (pcdA->clientState == MINIMIZED_STATE)
    {
	aX1 = ICON_X(pcdA);
	aY1 = ICON_Y(pcdA);
	aX2 = aX1 + ICON_WIDTH(pcdA) - 1; 
	aY2 = aY1 + ICON_HEIGHT(pcdA) - 1;
    }
    else /* (pcdA->clientState == MAXIMIZED_STATE) */
    {
	aX1 = pcdA->maxX - pcdA->clientOffset.x;
	aY1 = pcdA->maxY - pcdA->clientOffset.y;
	aX2 = aX1 + pcdA->maxWidth + (2 * pcdA->clientOffset.x) - 1;
	aY2 = aY1 + pcdA->maxHeight + pcdA->clientOffset.y +
	      pcdA->clientOffset.x - 1;
    }

    if (pcdB->clientState == NORMAL_STATE)
    {
	bX1 = pcdB->clientX - pcdB->clientOffset.x;
	bY1 = pcdB->clientY - pcdB->clientOffset.y;
	bX2 = bX1 + pcdB->clientWidth + (2 * pcdB->clientOffset.x) - 1;
	bY2 = bY1 + pcdB->clientHeight + pcdB->clientOffset.y +
	      pcdB->clientOffset.x - 1;
    }
    else if (pcdB->clientState == MINIMIZED_STATE)
    {
	bX1 = ICON_X(pcdB);
	bY1 = ICON_Y(pcdB);
	bX2 = bX1 + ICON_WIDTH(pcdB) - 1; 
	bY2 = bY1 + ICON_HEIGHT(pcdB) - 1;
    }
    else /* (pcdB->clientState == MAXIMIZED_STATE) */
    {
	bX1 = pcdB->maxX - pcdB->clientOffset.x;
	bY1 = pcdB->maxY - pcdB->clientOffset.y;
	bX2 = bX1 + pcdB->maxWidth + (2 * pcdB->clientOffset.x) - 1;
	bY2 = bY1 + pcdB->maxHeight + pcdB->clientOffset.y +
	      pcdB->clientOffset.x - 1;
    }

    /*
     * Check if there is overlap in both dimensions.
     */

    if (((aX1 >= bX1) && (aX1 <= bX2)) || ((aX2 >= bX1) && (aX2 <= bX2)) ||
	((bX1 >= aX1) && (bX1 <= aX2)) || ((bX2 >= aX1) && (bX2 <= aX2)))
    {
	if (((aY1 >= bY1) && (aY1 <= bY2)) || ((aY2 >= bY1) && (aY2 <= bY2)) ||
	    ((bY1 >= aY1) && (bY1 <= aY2)) || ((bY2 >= aY1) && (bY2 <= aY2)))
	{
	    obscuring = True;
	}
    }

    return (obscuring);


} /* END OF FUNCTION CheckIfObscuring */



/*************************************<->*************************************
 *
 *  CheckIfClientObscuredByAny (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function determines whether a window or a transient window tree
 *  is obscured (at least partially) by any other window.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd = pointer to client data for a window (it may be the leader of
 *	a transient tree.
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if the window(s) are overlapped.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean CheckIfClientObscuredByAny (pcd)
	ClientData *pcd;

#else /* _NO_PROTO */
Boolean CheckIfClientObscuredByAny (ClientData *pcd)
#endif /* _NO_PROTO */
{
    Boolean obscured = False;
    ClientListEntry *pListEntry;


    pListEntry = ACTIVE_PSD->clientList;
    while (pListEntry && !obscured)
    {
	if (pListEntry->pCD == pcd)
	{
	    if (((pListEntry->type == MINIMIZED_STATE) &&
		 (pListEntry->pCD->clientState == MINIMIZED_STATE)) ||
		((pListEntry->type != MINIMIZED_STATE) &&
		 (pListEntry->pCD->clientState != MINIMIZED_STATE)))
	    {
		pListEntry = NULL;
	    }
	}
	else if (((pListEntry->type == MINIMIZED_STATE) &&
		   (pListEntry->pCD->clientState == MINIMIZED_STATE)) ||
		 ((pListEntry->type != MINIMIZED_STATE) &&
		  (pListEntry->pCD->clientState != MINIMIZED_STATE)))
	{
	    /*
	     * The window for the entry is visible on screen.  See if it
	     * obscures the indicated window.
	     */

	    obscured = CheckIfClientObscuring (pListEntry->pCD, pcd);
	}

	if (pListEntry)
	{
	    pListEntry = pListEntry->nextSibling;
	}
    }

    return (obscured);

} /* END OF FUNCTION CheckIfClientObscuredByAny */



/*************************************<->*************************************
 *
 *  CheckIfClientObscuringAny (pcd)
 *
 *
 *  Description:
 *  -----------
 *  This function determines whether a window or a transient window tree
 *  is obscuring another window.
 *  
 *
 *  Inputs:
 *  ------
 *  pcd = pointer to client data for a window (it may be the leader of
 *	a transient tree.
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = True if the window(s) overlaps anther window.
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
Boolean CheckIfClientObscuringAny (pcd)
ClientData *pcd;

#else /* _NO_PROTO */
Boolean CheckIfClientObscuringAny (ClientData *pcd)
#endif /* _NO_PROTO */
{
    Boolean obscuring = False;
    ClientListEntry *pListEntry;


    pListEntry = (pcd->clientState == MINIMIZED_STATE) ?
					&pcd->iconEntry : &pcd->clientEntry;
    while (pListEntry && !obscuring)
    {
	if ((pListEntry->pCD != pcd) &&
	    (((pListEntry->type == MINIMIZED_STATE) &&
	      (pListEntry->pCD->clientState == MINIMIZED_STATE)) ||
	     ((pListEntry->type != MINIMIZED_STATE) &&
	      (pListEntry->pCD->clientState != MINIMIZED_STATE))))
	{
	    obscuring = CheckIfClientObscuring (pcd, pListEntry->pCD);
	}

	pListEntry = pListEntry->nextSibling;
    }

    return (obscuring);

} /* END OF FUNCTION CheckIfClientObscuringAny */



/*************************************<->*************************************
 *
 *  SetupSystemModalState (pCD)
 *
 *
 *  Description:
 *  -----------
 *  This function prepares for mapping a system modal window.  An input
 *  screen window is mapped below the system modal window to prevent input
 *  to the windows not related to the system modal window.
 *  
 *
 *  Inputs:
 *  ------
 *  pCD = pointer to client data for the system modal window; if NULL the
 *	system modal window is a special window manager dialog box
 *
 *
 *  Outputs:
 *  -------
 *  wmGD = changes to system modal state data
 *
 *************************************<->***********************************/

#ifdef LARGECURSORS

#define noenter32_width 32
#define noenter32_height 32
#define noenter32_x_hot 15
#define noenter32_y_hot 15

static char noenter32_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xfc, 0x3f, 0x00,
   0x00, 0xff, 0xff, 0x00, 0x80, 0xff, 0xff, 0x01, 0xc0, 0xff, 0xff, 0x03,
   0xe0, 0xff, 0xff, 0x07, 0xf0, 0xff, 0xff, 0x0f, 0xf8, 0xff, 0xff, 0x1f,
   0xf8, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0x3f,
   0xfc, 0xff, 0xff, 0x3f, 0x06, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x60,
   0x06, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x60,
   0x06, 0x00, 0x00, 0x60, 0xfc, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0x3f,
   0xfc, 0xff, 0xff, 0x3f, 0xf8, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0xff, 0x1f,
   0xf0, 0xff, 0xff, 0x0f, 0xe0, 0xff, 0xff, 0x07, 0xc0, 0xff, 0xff, 0x03,
   0x80, 0xff, 0xff, 0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0xfc, 0x3f, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00};

static char noenter32m_bits[] = {
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0xff, 0xff, 0x00,
   0x80, 0xff, 0xff, 0x01, 0xc0, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0x07,
   0xf0, 0xff, 0xff, 0x0f, 0xf8, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0x3f,
   0xfc, 0xff, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x7f,
   0xfe, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x7f,
   0xfe, 0xff, 0xff, 0x7f, 0xfc, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0x3f,
   0xf8, 0xff, 0xff, 0x1f, 0xf0, 0xff, 0xff, 0x0f, 0xe0, 0xff, 0xff, 0x07,
   0xc0, 0xff, 0xff, 0x03, 0x80, 0xff, 0xff, 0x01, 0x00, 0xff, 0xff, 0x00,
   0x00, 0xfc, 0x3f, 0x00, 0x00, 0xe0, 0x07, 0x00};

#endif /* LARGECURSORS */

#define noenter16_width 16
#define noenter16_height 16
#define noenter16_x_hot 7
#define noenter16_y_hot 7

static char noenter16_bits[] = {
   0x00, 0x00, 0xc0, 0x03, 0xf0, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfc, 0x3f,
   0xfe, 0x7f, 0x02, 0x40, 0x02, 0x40, 0xfe, 0x7f, 0xfc, 0x3f, 0xfc, 0x3f,
   0xf8, 0x1f, 0xf0, 0x0f, 0xc0, 0x03, 0x00, 0x00};

static char noenter16m_bits[] = {
   0xc0, 0x03, 0xf0, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe, 0x7f, 0xfe, 0x7f,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x7f,
   0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xc0, 0x03};

#ifdef _NO_PROTO
void SetupSystemModalState (pCD)
	ClientData *pCD;

#else /* _NO_PROTO */
void SetupSystemModalState (ClientData *pCD)
#endif /* _NO_PROTO */
{
    XWindowChanges windowChanges;
    unsigned int   width, height;
    unsigned int   x_hot, y_hot;
    char          *bits;
    char          *mask_bits;
    WmScreenData *pSD;
    int scr;

#ifdef LARGECURSORS

    if (wmGD.useLargeCursors)
    {
	width = noenter32_width;
	height = noenter32_height;
	x_hot = noenter32_x_hot;
	y_hot = noenter32_y_hot;
	bits = noenter32_bits;
	mask_bits = noenter32m_bits;
    }
    else

#endif /* LARGECURSORS */

    {
	width = noenter16_width;
	height = noenter16_height;
	x_hot = noenter16_x_hot;
	y_hot = noenter16_y_hot;
	bits = noenter16_bits;
	mask_bits = noenter16m_bits;
    }

    for (scr=0; scr<wmGD.numScreens; scr++)
    {
	pSD = &(wmGD.Screens[scr]);
	
	/*
	 * Make the system modal input screen window if necessary.
	 */
	
	if (pSD->managed && pSD->inputScreenWindow == NULL)
	{
	    XSetWindowAttributes windowAttributes;
	    Pixmap               pixmap;
	    Pixmap               maskPixmap;
	    XColor               xcolors[2];
	    
	    windowAttributes.event_mask = ButtonPressMask;
	    if (wmGD.keyboardFocusPolicy == KEYBOARD_FOCUS_POINTER)
	    {
		windowAttributes.event_mask |= EnterWindowMask;
	    }
	    windowAttributes.override_redirect = True;
	    
	    pixmap = XCreateBitmapFromData (DISPLAY, pSD->rootWindow, 
					    bits, width, height);
	    
	    maskPixmap = XCreateBitmapFromData (DISPLAY, pSD->rootWindow, 
						mask_bits, width, height);
	    
	    xcolors[0].pixel = BlackPixel (DISPLAY, pSD->screen);
	    xcolors[1].pixel = WhitePixel (DISPLAY, pSD->screen);
	    XQueryColors (DISPLAY, DefaultColormap (DISPLAY, pSD->screen), 
			  xcolors, 2);
	    windowAttributes.cursor =
		XCreatePixmapCursor (DISPLAY, pixmap, maskPixmap,
				     &(xcolors[0]), &(xcolors[1]), 
				     x_hot, y_hot);
	    XFreePixmap (DISPLAY, pixmap);
	    XFreePixmap (DISPLAY, maskPixmap);
	    
	    pSD->inputScreenWindow =
		XCreateWindow (DISPLAY, pSD->rootWindow, 0, 0,
			       DisplayWidth (DISPLAY, pSD->screen),
			       DisplayHeight (DISPLAY, pSD->screen),
			       0,
			       0,
			       InputOnly,
			       CopyFromParent,
			       CWEventMask | CWOverrideRedirect | CWCursor,
			       &windowAttributes);
	}
	if (pSD->managed && pSD != ACTIVE_PSD)
	{
	    XMapRaised (DISPLAY, pSD->inputScreenWindow);
	}
    }
    
    if (pCD)
    {
	wmGD.systemModalWindow = pCD->clientFrameWin;
    }
    else
    {
        /*
         * ELSE: the system modal window is a special window manager dialog
         * box and wmGD.systemModalWindow is set prior to the call to 
         * SetupSystemModalState.  Set the focus to the special window manager
	 * dialog box.
         */
	
	SetKeyboardFocus (NULL, REFRESH_LAST_FOCUS);
	XSetInputFocus (DISPLAY, wmGD.systemModalWindow, RevertToPointerRoot,
			CurrentTime);
    }
    
    
    /*
     * Map the system modal input screen window below the system modal
     * window.
     */
    
    windowChanges.sibling = wmGD.systemModalWindow;
    windowChanges.stack_mode = Below;
    XConfigureWindow (DISPLAY, ACTIVE_PSD->inputScreenWindow, 
		      CWSibling | CWStackMode, &windowChanges);
    
    XMapWindow (DISPLAY, ACTIVE_PSD->inputScreenWindow);
    
    
    /*
     * Setup the system modal global data.
     */
    
    wmGD.systemModalActive = True;
    wmGD.systemModalClient = pCD;
    
    
} /* END OF FUNCTION SetupSystemModalState */



/*************************************<->*************************************
 *
 *  UndoSystemModalState ()
 *
 *
 *  Description:
 *  -----------
 *  This function cleans up after a system modal window goes away.
 *  
 *
 *  Inputs:
 *  ------
 *  wmGD = (system modal state data)
 *
 *
 *  Outputs:
 *  -------
 *  wmGD = changes to system modal state data
 *
 *************************************<->***********************************/

#ifdef _NO_PROTO
void UndoSystemModalState ()

#else /* _NO_PROTO */
void UndoSystemModalState (void)
#endif /* _NO_PROTO */
{
    int scr;
    
    /*
     * Unmap the system modal input screen window.
     */

    for (scr = 0; scr < wmGD.numScreens; scr++)
    {
	if(wmGD.Screens[scr].managed)
	{
	    XUnmapWindow (DISPLAY, wmGD.Screens[scr].inputScreenWindow);
	}
    }

    /*
     * Reset the focus if a window manager system modal dialog box was
     * being displayed.
     */

    if (!wmGD.systemModalClient)
    {
	AutoResetKeyFocus (NULL, CurrentTime);
    }


    /*
     * Reset the system modal global data.
     */

    wmGD.systemModalActive = False;
    wmGD.systemModalClient = NULL;
    wmGD.systemModalWindow = NULL;

} /* END OF FUNCTION UndoSystemModalState */



/*************************************<->*************************************
 *
 *  FindClientNameMatch (pStartingEntry, toNext, clientName, types)
 *
 *
 *  Description:
 *  -----------
 *  This function searches for a client that has a particular name or class.
 *  A match will be indicated if the client with the name or class also
 *  is in a particular state.
 *  
 *
 *  Inputs:
 *  ------
 *  pEntry = pointer to the client list entry where the search is
 *	to begin.
 *
 *  toNext = if True then search client list from first to last; otherwise
 *	search the client list last to first.
 *
 *  clientName = string that indicates a client name or class.
 *
 *  type = types of objects (icon, window, ...) that are to be matched.
 *
 *
 *  Outputs:
 *  -------
 *  RETURN = pointer to client list entry for matched client.
 *
 *************************************<->***********************************/
#ifdef _NO_PROTO
ClientListEntry * FindClientNameMatch (pEntry, toNext, clientName, types)
	ClientListEntry *pEntry;
	Boolean toNext;
	String clientName;
	unsigned long types;
#else /* _NO_PROTO */
ClientListEntry * FindClientNameMatch (ClientListEntry *pEntry,
				       Boolean toNext,
				       String clientName,
				       unsigned long types)
#endif /* _NO_PROTO */

{
    Boolean foundMatch = False;
    Boolean checkEntry;
    ClientData *pCD;


    while (!foundMatch && pEntry)
    {
	checkEntry = False;
	pCD = pEntry->pCD;
	if (pEntry->type == MINIMIZED_STATE)
	{
	    if ((pCD->clientState == MINIMIZED_STATE) &&
		(types & F_GROUP_ICON))
	    {
		checkEntry = True;
	    }
	}
	else
	{
	    if ((pCD->clientState != MINIMIZED_STATE) &&
		(types & F_GROUP_WINDOW))
	    {
		checkEntry = True;
	    }
	}

	if (checkEntry &&
	    ((pCD->clientName && (strcmp (clientName,pCD->clientName) == 0)) ||
	     (pCD->clientClass && (strcmp (clientName,pCD->clientClass) == 0))))
	{
	    foundMatch = True;
	}
	else
	{
	    pEntry = (toNext) ? pEntry->nextSibling : pEntry->prevSibling;
	}
    }

    return (pEntry);

} /* END OF FUNCTION FindClientNameMatch */
