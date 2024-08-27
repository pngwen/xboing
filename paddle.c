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
 * $Id: paddle.c,v 1.1.1.1 1994/12/16 01:36:45 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/paddle.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:45 $
 *
 * $Log: paddle.c,v $
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
#include <xpm.h>

#include "bitmaps/paddle/padsml.xpm"
#include "bitmaps/paddle/padmed.xpm"
#include "bitmaps/paddle/padhuge.xpm"

#include "error.h"
#include "init.h"
#include "stage.h"
#include "blocks.h"
#include "misc.h"
#include "main.h"
#include "special.h"

#include "paddle.h"

/*
 *  Internal macro definitions:
 */

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

static Pixmap paddleSmallPixmap, paddleMediumPixmap, paddleHugePixmap;
static Pixmap paddleSmallMask, paddleMediumMask, paddleHugeMask;

int	paddlePos;
int	currentPaddleSize;
static int	oldX;
static int xpos_old;
int reverseOn, stickyOn;

#if NeedFunctionPrototypes
void DrawPaddle(Display *display, Window window, int x, int y, int size)
#else
void DrawPaddle(display, window, x, y, size)
	Display *display;
	Window window;
	int x;
	int y;
	int size;
#endif
{
	/* Switch on the paddle size */
	switch (size)
	{
		case PADDLE_SMALL:
			RenderShape(display, window, paddleSmallPixmap, paddleSmallMask,
				x - 20, y, 40, 15, True);
			break;

		case PADDLE_MEDIUM:
			RenderShape(display, window, paddleMediumPixmap, paddleMediumMask,
				x - 25, y, 50, 15, True);
			break;

		case PADDLE_HUGE:
			RenderShape(display, window, paddleHugePixmap, paddleHugeMask,
				x - 35, y, 70, 15, True);
			break;
	}
}

#if NeedFunctionPrototypes
void InitialisePaddle(Display *display, Window window, Colormap colormap)
#else
void InitialisePaddle(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
    XpmAttributes   attributes;
	int		    XpmErrorStatus;

    attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

	/* Create the xpm pixmap paddles */
	XpmErrorStatus = XpmCreatePixmapFromData(display, window, paddlesmall_xpm,
		&paddleSmallPixmap, &paddleSmallMask, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialisePaddle()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, paddlemedium_xpm,
		&paddleMediumPixmap, &paddleMediumMask, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialisePaddle()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, paddlehuge_xpm,
		&paddleHugePixmap, &paddleHugeMask, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialisePaddle()");

	/* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);
}

#if NeedFunctionPrototypes
void SetReverseOff(void)
#else
void SetReverseOff()
#endif
{
	/* Set the reverse state off */
	reverseOn = False;
}

#if NeedFunctionPrototypes
void ToggleReverse(Display *display)
#else
void ToggleReverse(display)
	Display *display;
#endif
{
	/* Set the reverse state */
	if (reverseOn == True)
		reverseOn = False;
	else
		reverseOn = True;

	/* Update the display */
	DrawSpecials(display);
}

#if NeedFunctionPrototypes
void FreePaddle(Display *display)
#else
void FreePaddle(display)
	Display *display;
#endif
{
	/* Free the paddle pixmaps and masks */
	if (paddleSmallPixmap)	XFreePixmap(display, paddleSmallPixmap);
	if (paddleMediumPixmap)	XFreePixmap(display, paddleMediumPixmap);
	if (paddleHugePixmap)	XFreePixmap(display, paddleHugePixmap);

	if (paddleSmallMask)	XFreePixmap(display, paddleSmallMask);
	if (paddleMediumMask)	XFreePixmap(display, paddleMediumMask);
	if (paddleHugeMask)		XFreePixmap(display, paddleHugeMask);
}

#if NeedFunctionPrototypes
void MovePaddle(Display *display, Window window, int direction, int size, 
	int xpos)
#else
void MovePaddle(display, window, direction, size, xpos)
	Display *display;
	Window window;
	int direction;
	int size;
	int xpos;
#endif
{
	static int	y = (PLAY_HEIGHT - DIST_BASE);

	/* Handle the paddle moving either left or right */
	switch (direction)
	{
		case PADDLE_LEFT:
			/* Move to the left or if reverse to right*/
			if (reverseOn)
				paddlePos += PADDLE_VEL;
			else
				paddlePos -= PADDLE_VEL;
			break;

		case PADDLE_RIGHT:
			/* Move to the right or if reverse to left*/
			if (reverseOn)
				paddlePos -= PADDLE_VEL;
			else
				paddlePos += PADDLE_VEL;
			break;

		case PADDLE_NONE:
			if (reverseOn)
			{
				xpos = PLAY_WIDTH - xpos;
			}
			break;
	}

	/* Switch on the size of the paddle */
	switch (size)
	{
		case PADDLE_SMALL:
			/* Handle the small paddle */
			if (xpos > 0)
				paddlePos = xpos - (MAIN_WIDTH / 2) + 20;

			/* Stop the paddle from going past the walls */
			if (paddlePos < 20) paddlePos = 20;
			if (paddlePos > (PLAY_WIDTH - 20)) 
				paddlePos = (PLAY_WIDTH - 20);

			/* Clear the old position */
			XClearArea(display, window, oldX, y, 40, 15, False);
			oldX = paddlePos - 20; 

			DrawPaddle(display, window, paddlePos, PLAY_HEIGHT - DIST_BASE, 
				PADDLE_SMALL);
			break;

		case PADDLE_MEDIUM:
			/* Handle the medium paddle */
			if (xpos > 0)
				paddlePos = xpos - (MAIN_WIDTH / 2) + 25;

			/* Stop the paddle from going past the walls */
			if (paddlePos < 25) paddlePos = 25;
			if (paddlePos > (PLAY_WIDTH - 25)) 
				paddlePos = (PLAY_WIDTH - 25);

			/* Clear the old position */
			XClearArea(display, window, oldX, y, 50, 15, False);
			oldX = paddlePos - 25; 

			DrawPaddle(display, window, paddlePos, 
				PLAY_HEIGHT - DIST_BASE, PADDLE_MEDIUM);
			break;

		case PADDLE_HUGE:
			/* Handle the large paddle */
			if (xpos > 0)
				paddlePos = xpos - (MAIN_WIDTH / 2) + 35;

			/* Stop the paddle from going past the walls */
			if (paddlePos < 35) paddlePos = 35;
			if (paddlePos > (PLAY_WIDTH - 35)) 
				paddlePos = (PLAY_WIDTH - 35);

			/* Clear the old position */
			XClearArea(display, window, oldX, y, 70, 15, False);
			oldX = paddlePos - 35; 

			DrawPaddle(display, window, paddlePos, 
				PLAY_HEIGHT - DIST_BASE, PADDLE_HUGE);
			break;
	}
}

#if NeedFunctionPrototypes
void FlushPaddleBackingStore(Display *display, Window window)
#else
void FlushPaddleBackingStore(display, window)
	Display *display;
	Window window;
#endif
{
	static int y = (PLAY_HEIGHT - DIST_BASE);

	/* Clear the entire paddle area */
	XClearArea(display, window, 0, y, PLAY_WIDTH, 15, False);
}

#if NeedFunctionPrototypes
int GetPaddleSize(void)
#else
int GetPaddleSize()
#endif
{
	/* Switch on the current paddle size */
	switch (currentPaddleSize)
	{
		case PADDLE_SMALL:
			/* Return the size of the small paddle in pixels */
			return 40;

		case PADDLE_MEDIUM:
			/* Return the size of the medium paddle in pixels */
			return 50;

		case PADDLE_HUGE:
			/* Return the size of the huge paddle in pixels */
			return 70;
	}

	/* Bug if this happens */
	return 0;
}

#if NeedFunctionPrototypes
void ResetPaddleStart(Display *display, Window window)
#else
void ResetPaddleStart(display, window)
	Display *display;
	Window window;
#endif
{
	paddlePos = PLAY_WIDTH / 2;
	oldX = PLAY_WIDTH / 2;
	xpos_old = oldX;

	/* Get rid of old paddle image and start new one */
	FlushPaddleBackingStore(display, window);
	MovePaddle(display, window, PADDLE_NONE, currentPaddleSize, 0);
}

#if NeedFunctionPrototypes
void RedrawPaddle(Display *display, Window window)
#else
void RedrawPaddle(display, window)
	Display *display;
	Window window;
#endif
{
	MovePaddle(display, window, PADDLE_NONE, currentPaddleSize, 0);
}

#if NeedFunctionPrototypes
void ChangePaddleSize(Display *display, Window window, int type)
#else
void ChangePaddleSize(display, window, type)
	Display *display;
	Window window;
	int type;
#endif
{
	/* 
	 * This function will erase the paddle and then change its size and
	 * redraw it. If the paddle cannot grow as it is at the biggest state
	 * then it will stay the same. Likewise for the smallest paddle.
	 */

	FlushPaddleBackingStore(display, window);

	if (type == PAD_SHRINK_BLK)
	{
		if (currentPaddleSize == PADDLE_MEDIUM)
		{
			/* Shrink the paddle */
        	currentPaddleSize = PADDLE_SMALL;
		} else if (currentPaddleSize == PADDLE_HUGE)
		{
			/* Shrink the paddle */
        	currentPaddleSize = PADDLE_MEDIUM;
		}
	}
	else
	{
		if (currentPaddleSize == PADDLE_SMALL)
		{
			/* Grow the paddle */
        	currentPaddleSize = PADDLE_MEDIUM;
		} else if (currentPaddleSize == PADDLE_MEDIUM)
		{
			/* Grow the paddle */
        	currentPaddleSize = PADDLE_HUGE;
		}
	}

	/* Draw the new paddle in its new size */
	RedrawPaddle(display, window);
}
