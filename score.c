/**
 * @file score.c
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Manages the user's score including drawing the digits on the screen.
 * @version 1.1.1.1
 * @date 16 Dec 1994
 * 
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 *  
 * Calculates the user's score and keeps track of it throughout the game.
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
 * $Id: score.c,v 1.1.1.1 1994/12/16 01:36:45 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/score.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:45 $
 *
 * $Log: score.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:45  jck
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
//#include <xpm.h>

#include "error.h"
#include "init.h"
#include "special.h"
#include "misc.h"
#include "main.h"

#include "score.h"

#include "bitmaps/digits/digit0.xpm"
#include "bitmaps/digits/digit1.xpm"
#include "bitmaps/digits/digit2.xpm"
#include "bitmaps/digits/digit3.xpm"
#include "bitmaps/digits/digit4.xpm"
#include "bitmaps/digits/digit5.xpm"
#include "bitmaps/digits/digit6.xpm"
#include "bitmaps/digits/digit7.xpm"
#include "bitmaps/digits/digit8.xpm"
#include "bitmaps/digits/digit9.xpm"

/*
 *  Internal macro definitions:
 */

#define NUM_DIGITS	10

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

Pixmap	digitPixmaps[NUM_DIGITS];
Pixmap	digitPixmapsM[NUM_DIGITS];

u_long score = 0L;


/**
 * @brief Intializes the score on the screen
 * 
 * @param Display display X11 display
 * @param Window window X11 window
 * @param Colormap colormap X11 colormap
 * @pre The display, window, and color need to be selected
 * @post The score counter is displayed to the screen with the selected size and color
 *
 *@todo Remove reliance on X11
 *
 */
void InitialiseScoreDigits(Display *display, Window window, Colormap colormap)
{
	XpmAttributes   attributes;
	int 			XpmErrorStatus;

	attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

	/* Create all xpm pixmap digits from the files */
	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit0_xpm, 
		&digitPixmaps[0], &digitPixmapsM[0], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit1_xpm, 
		&digitPixmaps[1], &digitPixmapsM[1], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit2_xpm, 
		&digitPixmaps[2], &digitPixmapsM[2], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit3_xpm, 
		&digitPixmaps[3], &digitPixmapsM[3], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit4_xpm, 
		&digitPixmaps[4], &digitPixmapsM[4], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit5_xpm, 
		&digitPixmaps[5], &digitPixmapsM[5], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit6_xpm, 
		&digitPixmaps[6], &digitPixmapsM[6], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit7_xpm, 
		&digitPixmaps[7], &digitPixmapsM[7], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit8_xpm, 
		&digitPixmaps[8], &digitPixmapsM[8], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, digit9_xpm, 
		&digitPixmaps[9], &digitPixmapsM[9], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseScoreDigits()");

	/* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);
}

/**
 * @brief Draws the digits that are used to count the score
 *
 * @param Display display X11 display
 * @param Window window X11 window
 * @param int digit The current score
 * @param int x The x position of the digit
 * @param int y The y position of the digit
 * @pre The score must be displayed
 * @post The digits are drawn to the score counter
 *
 * @todo remove reliance on X11
 *
 */
static void DrawDigit(Display *display, Window window, int digit, int x, int y)
{
	/* Draw the digit in the window */
	RenderShape(display, window, 
		digitPixmaps[digit], digitPixmapsM[digit], x, y, 30, 40, True);
}

/**
 * @brief Checks if the current score is divisible by 10 and sets the last digit to zero.
 Checks the score it needs to draw then calls the Drawdigit function.
 *
 * @param Display display X11 display
 * @param Window window X11 window
 * @param u_long score The current score
 * @param int x The x position of the number
 * @param int y The y position of the number
 * @pre Must have selected window and display size
 * @post The number has been drawn
 *
 * @todo remove reliance on X11
 *
 */
void DrawOutNumber(Display *display, Window window, u_long score, int x, int y)
{
	int digit;

	/* Get the digit that we want from the score */
	if (score / 10)
		DrawOutNumber(display, window, (score / 10), x - 32, y);

	/* Work out the digit needed to draw */
	digit = (int) (score % 10);

	DrawDigit(display, window, digit, x - 32, y);
}

/**
 * @brief Sets the new score
 *
 * @param u_long new The new score
 * @pre The score must exist
 * @post The new score has been set
 *
 */
void SetTheScore(u_long new)
{
	/* Set the score */
	score = new;
}

/**
 * @brief Adds an increment to the score
 *
 * @param u_long inc The increment that will be added to the score
 * @pre The score must be declared
 * @post The score has had an increment added to it
 *
 */
void AddToScore(u_long inc)
{
	/* Compute the score */
	score += ComputeScore(inc);
}

/**
 * @brief Calculates the score and takes any possible bonuses into account
 *
 * @param u_long inc The increment that will be added to the score
 * @pre The score must be declared as well as the increment
 * @post The new score is now computed
 *
 */
u_long ComputeScore(u_long inc)
{
    /* Take into account any score bonuses */
    if (x2Bonus == True)
        inc *= 2;
    else if (x4Bonus == True)
        inc *= 4;

    /* return the score */
    return (u_long) (inc);
}

/**
 * @brief Clear the old score display and creates a new one with new score digits
 *
 * @param Display display X11 display
 * @param Window window X11 window size
 * @param u_long score The current score
 * @pre The old score must have an increment added to it
 * @post The old score is erased while the new score is displayed
 *
 * @todo remove reliance on X11
 *
 */
void DisplayScore(Display *display, Window window, u_long score)
{
	/* Erase the old score in the window */
	XClearWindow(display, window);

	/* Draw a zero if no score */
	if (score == 0L)
		RenderShape(display, window, 
			digitPixmaps[0], digitPixmapsM[0], 192, 0, 30, 40, True);
	else
		/* Draw the score digits rescursively */
		DrawOutNumber(display, window, score, 224, 0);
}

/**
 * @brief Frees the memory for the score
 *
 *@param Display display X11 display
 *@pre
 *@post Memory taken up by the score is freed
 *
 */
void FreeScoreDigits(Display *display)
{
	int i;

	/* Free the memory associated with the digit pixmaps */
	for (i = 0; i < NUM_DIGITS; i++)
	{
		/* Free the digits pixmap and mask */
		if (digitPixmaps[i])	XFreePixmap(display, digitPixmaps[i]);
		if (digitPixmapsM[i])	XFreePixmap(display, digitPixmapsM[i]);
	}
}
