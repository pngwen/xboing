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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

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

#if NeedFunctionPrototypes
void InitialiseMessageSystem(Display *display, Window window, Colormap colormap)
#else
void InitialiseMessageSystem(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	/* Frame to clear message area */
	clearFrame = 0;
}

#if NeedFunctionPrototypes
void FreeMessageSystem(Display *display)
#else
void FreeMessageSystem(display)
	Display *display;
#endif
{
	/* Not much to free yet - maybe one day .... */
}

#if NeedFunctionPrototypes
void DrawMessage(Display *display, Window window, char *message, int clear,
	int ci)
#else
void DrawMessage(display, window, message, clear, ci)
	Display *display; 
	Window window;
	char *message;
	int clear;
	int ci;
#endif
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

#if NeedFunctionPrototypes
void SetCurrentMessage(Display *display, Window window, char *newMessage, 
	int clear)
#else
void SetCurrentMessage(display, window, newMessage, clear)
	Display *display;
	Window window;
	char *newMessage;
	int clear;
#endif
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

#if NeedFunctionPrototypes
void DisplayCurrentMessage(Display *display, Window window)
#else
void DisplayCurrentMessage(display, window)
	Display *display;
	Window window;
#endif
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
