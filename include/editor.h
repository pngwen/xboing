#ifndef _EDITOR_H_
#define _EDITOR_H_

/**
 * @file editor.h
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Controls functions for editor and deals with clicks and button presses
 * @version 1.1.1.1
 * @date 16 Dec 1994
 *
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 * @copyright (see COPYRIGHT file for full text)
 *
 * Editor.c/h handles the editor and manages functions for saving and loading levels, building the editor, and managing the
 * clicks and button presses
 *
 */

/*
 * XBoing - An X11 blockout style computer game
 *
 * (c) Copyright 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 *
 * In no event shall the author be liable to any party for direct, indirect,
 * special, incidental, or consequential damages arising out of the use of
 * this software and its documentation, even if the author has been advised
 * of the possibility of such damage.
 *
 * The author specifically disclaims any warranties, including, but not limited
 * to, the implied warranties of merchantability and fitness for a particular
 * purpose.  The software provided hereunder is on an "AS IS" basis, and the
 * author has no obligation to provide maintenance, support, updates,
 * enhancements, or modifications.
 */

/* 
 * =========================================================================
 *
 * $Id: editor.h,v 1.1.1.1 1994/12/16 01:36:55 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/editor.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:55 $
 *
 * $Log: editor.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:55  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Dependencies on other include files:
 */

//#include <X11/Xlib.h>
#include "faketypes.h"

/*
 *  Constants and macros:
 */

#define EDITOR_TOOL_WIDTH 	120

/*
 *  Type declarations:
 */

enum EditStates 
{ 
	EDIT_LEVEL, 
	EDIT_NONE, 
	EDIT_TEST, 
	EDIT_WAIT, 
	EDIT_FINISH 
};

/*
 *  Function prototypes:
 */

/**
 * @brief Calls ResetEditor in order to set up the editor for XBoing
 *
 * @param Display display X11 display
 * @param Window window window X11 ID for window
 * @param Colormap colormap Colormap for X11 system
 *
 * @todo remove reliance on X11
 *
 */
void SetUpEditor(Display *display, Window window, Colormap colormap);

/**
 * @brief Handles the editor state
 *
 * @param Display display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
void Editor(Display *display, Window window);


/**
 * @brief Redraws editor
 *
 * @param Display display X11 display
 * @param Window window X11 window id
 *
 * @todo Remove reliance on X11
 * @todo Possibly delete this function
 *
 */
void RedrawEditor(Display *display, Window window);

/**
 * @brief Free editor action
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
void FreeEditor(Display *display);

/**
 * @brief Sets edit state back to editor mode
 *
 */
void ResetEditor(void);

/**
 * @brief Calls the handleEditorkeys function to deal with all button presses in the editor
 *
 * @param Display display X11 display
 * @param KeySym keysym The symbol pressed
 *
 * @todo remove reliance on x11
 *
 */
void handleEditorKeys(Display *display, KeySym keysym);

/**
 * @brief Carries out actions based on mouse and button presses
 *
 * @param Display Display X11 Display
 * @param XEvent event the button event
 * @param int Down tells if the button is pressed or not
 *
 * @todo remove reliance on X11
 *
 */
void HandleEditorMouseButtons(Display *display, XEvent event, int Down);

/**
 * @brief Deals with all mouse and button events
 *
 * @param Display display X11 display ID
 * @param XEvent events The button or motion event
 *
 * @todo Remove Reliance on X11
 * @todo Possibly split this function into smaller functions
 *
 */
void HandleButtonMotion(Display *display, XEvent event);

extern 	enum EditStates EditState;


#endif
