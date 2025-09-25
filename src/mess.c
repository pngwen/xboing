/**
 * @file mess.c
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Manages the message system for xboing
 * @version 1.1.1.1
 * @date 16 Dec 1994
 *
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 * @copyright (see COPYRIGHT file for full text)
 *
 * mess.h/mess.c manage the message system for xboing. Deals with setting up the message system.
 * The files also display messages and free message memory
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
 * $Id: mess.c,v 1.1.1.1 1994/12/16 01:36:49 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/mess.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:49 $
 *
 * $Log: mess.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:49  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>

#include "error.h"
#include "level.h"
#include "init.h"
#include "stage.h"
#include "intro.h"
#include "main.h"
#include "misc.h"

#include "mess.h"

/*
 *  Internal macro definitions:
 */

#define CLEAR_DELAY		2000
#define FADE_DELAY		20

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

char 	currentMessage[1024];
int		clearFrame;
int 	newMessageStart;

/**
 *  @brief Initializes the message system.
 *
 * @param Display *display Pointer to the X11 display struct
 * @todo Update to remove X11 dependency
 * @param Window window This is the X Identifier for the window that will display the messages.
 * @todo Update to use window management from a new graphics system
 * @param Colormap colormap This is the colormap for the window.
 *
 * @todo Implement new graphics system management
 * @todo Check if the message system can handle multiple windows if needed.
 * @todo Add initialization checks.
 *
 * @post This function sets up and prepares the message system.
 */
void InitialiseMessageSystem(Display *display, Window window, Colormap colormap)
{
	/* Frame to clear message area */
	clearFrame = 0;
}

/**
 * @brief Frees memory used by the message system.
 *
 * @param Display *display Pointer to the X11 display struct
 * @todo Update to remove X11 dependency
 *                                                                                                                      * @todo Add some resource management for future message enhancements
 * @todo Add checks to make sure that only allocated memory is being freed.
 *
 * @post This function cleans and releases all memory that was allocated for the message system.
 *
 */
void FreeMessageSystem(Display *display)
{
	/* Not much to free yet - maybe one day .... */
}

/**
 * @brief Draws the message to the screen
 *
 * @param Display *display X11 display
 * @param Window window X11 window ID
 * @param char *message message that will be drawn
 * @param int clear checks if the display is clear
 *
 * @todo remove reliance on x11
 * @todo use booleans instead of 0 or 1
 *
 */
void DrawMessage(Display *display, Window window, char *message, int clear,
	int ci)
{
	int len = strlen(message);
	int plen;

	/* Clear the message window */
	XClearWindow(display, window);

	/* Obtain the text width so it can be centered */
    plen = XTextWidth(textFont, message, len);

	/* Draw the string in the message window */
	DrawTextFast(display, window, ((PLAY_WIDTH/2) - plen) / 2, 5,
		textFont, ci, message, len);

	/* Just to be sure, flush the display */
	XFlush(display);
}

/**
 * @brief Sets a new message to be displayed.
 *
 * @param Display *display Pointer to the X11 display struct
 * @todo Update to remove X11 dependency
 * @param Window window This is the X Identifier for the window that will display the messages.
 * @todo Update to use window management from a new graphics system
 * @param *newMessage The new message to be displayed.
 * @param clear If true, the display will be cleared before displaying the new message.
 * @todo Change the type of the clear parameter from int to boolean.
 *
 * @todo Make sure the new message is not NULL or empty before use.
 * @todo Add some sort of strategy to handle messages that are larger than the buffer size allows.
 *
 * @post This function copies the new message into the buffer and updates the display based on the parameters defined.
 *
 */
void SetCurrentMessage(Display *display, Window window, char *newMessage, 
	int clear)
{
	/* Draw out new message */
	strcpy(currentMessage, newMessage);
	newMessageStart = True;

	if (clear)
		clearFrame = frame + CLEAR_DELAY;
	else
		clearFrame = frame - 1;

	DisplayCurrentMessage(display, window);
}

/**
 * @brief Displays the current message.
 *
 * @param Display *display Points to the X11 display struct.
 * @todo Update to remove X11 dependency
 * @param Window window This is the X Identifier for the window that will display the messages.
 * @todo Update to use window management from a new graphics system
 *
 * @todo Implement new graphics system
 * @todo Add checks to ensure the message is displayed as expected.
 *
 * @post This function takes the current message stored to the screen to be displayed.
 *
 */
void DisplayCurrentMessage(Display *display, Window window)
{
	char str[80];
	char str2[80];

	if (newMessageStart == True)
	{
		newMessageStart = False;
		DrawMessage(display, window, currentMessage, True, green);
	}

	/* Clear the frame when it's time */
	if (frame == clearFrame)
	{
		/* Effectively erases message */
		if (mode == MODE_GAME || mode == MODE_EDIT)
			strcpy(str2, GetLevelName());
		
		if (str2[0] != '\0')
		{
			/* Set the message to the name of the level */
			sprintf(str, "- %s -", str2);
			SetCurrentMessage(display, window, str, False);
		}
		else
			DrawMessage(display, window, "", False, green);

		/* To be sure to be sure */
		XFlush(display);
	}
}
