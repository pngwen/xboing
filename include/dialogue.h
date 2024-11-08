/**
 * @file dialogue.h
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Manages the dialogue for XBoing including taking user input
 * @version 1.1.1.1
 * @date 16 Dec 1994
 * 
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 *  
 * Manages the dialogue process and draws and redraws dialogue to the screen to match certain parameters
 */

#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

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
 * $Id: dialogue.h,v 1.1.1.1 1994/12/16 01:36:55 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/dialogue.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:55 $
 *
 * $Log: dialogue.h,v $
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

#define DIALOGUE_WIDTH 		((int) (PLAY_WIDTH / 1.3))
#define DIALOGUE_HEIGHT 	120

#define DISK_ICON			1
#define TEXT_ICON			2

#define TEXT_ENTRY_ONLY		1
#define NUMERIC_ENTRY_ONLY	2
#define ALL_ENTRY			3
#define YES_NO_ENTRY		4

/*
 *  Type declarations:
 */

enum DialogueStates
{
    DIALOGUE_MAP,
    DIALOGUE_UNMAP,
    DIALOGUE_TEXT,
    DIALOGUE_NONE,
    DIALOGUE_FINISHED
};

/*
 *  Function prototypes:
 */


/**
 * @brief Processes dialogue and carries out different functions in different situations
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11 system
 *
 */
void ProcessDialogue(Display *display);

/**
 * @brief Checks what symbol is being pressed and carries out an action based on that
 *
 * @param Display display X11 display
 * @param XEvent event The symbil that has been pressed
 *
 * @todo Remove reliance on X11 system
 *
 */
void handleDialogueKeys(Display *display, XEvent event);

/**
 * @brief Takes a user's input message for dialogue
 *
 * @param Display display X11 display
 * @param char message The user's input message
 * @param int type The type of dialogue
 * @param int entryValidation User's entry confirmation
 * 
 * @todo remove reliance on X11 systems
 *
 */ 
char *UserInputDialogueMessage(Display *display, char *message, int type,
	int entryValidation);

/**
 * @brief Frees memory used by dialogue pixmap files
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11 system
 *
 */
void FreeDialoguePixmaps(Display *display);

/**
 * @brief Sets the Pixmap files for the dialogue for xboing
 * 
 * @param Display display X11 system display
 * @param Window window X11 ID for window
 * @pre Need to have X11 system
 * @post The pixmap files for the dialogue have been initialized
 * 
 * @todo Need to rewrite code so that it no longer relies on X11 system
 *
 */
void InitialiseDialoguePixmaps(Display *display, Window window, 
		Colormap colormap);

extern Pixmap question, questionM;
extern Pixmap floppy, floppyM;
extern Pixmap text, textM;

#endif
