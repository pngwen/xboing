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
 * $Id: keys.c,v 1.1.1.1 1994/12/16 01:36:44 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/keys.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:44 $
 *
 * $Log: keys.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:44  jck
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
#include <xpm.h>

#include "bitmaps/mouse.xpm"
#include "bitmaps/larrow.xpm"
#include "bitmaps/rarrow.xpm"

#include "error.h"
#include "highscore.h"
#include "special.h"
#include "misc.h"
#include "sfx.h"
#include "main.h"
#include "init.h"
#include "stage.h"
#include "blocks.h"
#include "ball.h"
#include "score.h"
#include "paddle.h"
#include "level.h"
#include "mess.h"
#include "version.h"
#include "audio.h"
#include "intro.h"
#include "keysedit.h"

#include "keys.h"

/*
 *  Internal macro definitions:
 */

#define GAP 12

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
static void DoKeysWait(void);
#else
static void DoKeysWait();
#endif


/*
 *  Internal variable declarations:
 */

static int endFrame = 0;
static int startFrame = 0;
enum KeysStates KeysState;
static int waitingFrame;
enum KeysStates waitMode;
Pixmap mouse, leftarrow, rightarrow;
Pixmap mouseM, leftarrowM, rightarrowM;

#if NeedFunctionPrototypes
void SetUpKeys(Display *display, Window window, Colormap colormap)
#else
void SetUpKeys(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
    XpmAttributes   attributes;
    int             XpmErrorStatus;

    attributes.valuemask = XpmColormap;
    attributes.colormap = colormap;

    XpmErrorStatus = XpmCreatePixmapFromData(display, window, mouse_xpm,
        &mouse, &mouseM, &attributes);
    HandleXPMError(display, XpmErrorStatus, "InitialiseKeys(mouse)");

    XpmErrorStatus = XpmCreatePixmapFromData(display, window, leftarrow_xpm,
        &leftarrow, &leftarrowM, &attributes);
    HandleXPMError(display, XpmErrorStatus, "InitialiseKeys(leftarrow)");

    XpmErrorStatus = XpmCreatePixmapFromData(display, window, rightarrow_xpm,
        &rightarrow, &rightarrowM, &attributes);
    HandleXPMError(display, XpmErrorStatus, "InitialiseKeys(rightarrow)");

    /* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);

	ResetKeys();
}

#if NeedFunctionPrototypes
static void DoText(Display *display, Window window)
#else
static void DoText(display, window)
	Display *display;
	Window window;
#endif
{
	char string[80];
	int y;
	int x;

	SetCurrentMessage(display, messWindow, "Drink driving kills!", False);

	y = 120;

	DrawShadowCentredText(display, window, titleFont, 
		"- Game Controls -", y, red, PLAY_WIDTH);

	y = 160;

    DrawLine(display, window, 32, y+2, PLAY_WIDTH - 28, y+2, black, 3);
    DrawLine(display, window, 30, y, PLAY_WIDTH - 30, y, white, 3);
	y += textFont->ascent;

	x = (PLAY_WIDTH / 2) - 17 - 10 - 35;
    RenderShape(display, window, leftarrow, leftarrowM,
        x, y + 28, 35, 19, True);

	DrawShadowText(display, window, textFont, "Paddle left", 
		x - 40 - 60, y + 28, green);

	x = (PLAY_WIDTH / 2) - 17;
    RenderShape(display, window, mouse, mouseM,
        x, y, 35, 57, True);

	x = (PLAY_WIDTH / 2) + 17 + 10;
    RenderShape(display, window, rightarrow, rightarrowM,
        x, y + 28, 35, 19, True);

	DrawShadowText(display, window, textFont, "Paddle right", 
		x + 40, y + 28, green);

	y = 250;
	x = 30;

	strcpy(string, "<s> = Sfx On/Off");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<P> = Pause/Resume");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<I> = Iconify Quickly");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<h> = Roll of Honour");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<H> = Personal scores");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<d> = Kill Ball");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<q> = Quit XBoing");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<+/-> = Inc/Dec Volume");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<z/x> = Save/Load game");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<w> = Set Starting level");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	y = 250;
	x = 280;

	strcpy(string, "<j> = Paddle left");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<k> = Shoot");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<l> = Paddle right");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<a> = Audio On/Off");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<c> = Cycle intros");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<g> = Toggle control");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<1-9> = Game speed");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<t> = Tilt board");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<e> = Level Editor");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<?> = spare");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP * 2;

    DrawLine(display, window, 32, y+2, PLAY_WIDTH - 28, y+2, black, 3);
    DrawLine(display, window, 30, y, PLAY_WIDTH - 30, y, white, 3);

	strcpy(string, "Insert coin to start the game");
	DrawShadowCentredText(display, window, textFont, string, 
		PLAY_HEIGHT - 30, tann, PLAY_WIDTH);
}

#if NeedFunctionPrototypes
static void DoSparkle(Display *display, Window window)
#else
static void DoSparkle(display, window)
	Display *display;
	Window window;
#endif
{
	static Pixmap store;
	static int x = 100;
	static int y = 20;
	static int in = 0;

	if (frame >= endFrame)
		KeysState = KEYS_FINISH;

	if (!store)
	{
		store = XCreatePixmap(display, window, 20, 20,
			DefaultDepth(display, XDefaultScreen(display)));
	}

	if (in == 0) 
		XCopyArea(display, window, store, gc, x, y, 20, 20, 0, 0);

	if (frame == startFrame)
	{
		XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);
		RenderShape(display, window, stars[in], starsM[in],
			x, y, 20, 20, False);

	 	in++;
		startFrame = frame + 15;

		if (in == 11) 
		{
			XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);
			in = 0;
			startFrame = frame + 500;
			x = (rand() % 474) + 5;
			y = (rand() % 74) + 5;
		}	
	}
}

#if NeedFunctionPrototypes
static void DoFinish(Display *display, Window window)
#else
static void DoFinish(display, window)
	Display *display;
	Window window;
#endif
{
	ResetKeysEdit();
	mode = MODE_KEYSEDIT;

    if (noSound == False) playSoundFile("boing", 50);
}


#if NeedFunctionPrototypes
void Keys(Display *display, Window window)
#else
void Keys(display, window)
	Display *display;
	Window window;
#endif
{
	switch (KeysState)
	{
		case KEYS_TITLE:
			if (getSpecialEffects(display) == True)
				DoIntroTitle(display, bufferWindow);
			else
				DoIntroTitle(display, window);
			KeysState = KEYS_TEXT;
			break;

		case KEYS_TEXT:
			if (getSpecialEffects(display) == True)
			{
				DoText(display, bufferWindow);
				while (WindowShatterEffect(display, window));
			}
			else
				DoText(display, window);
			KeysState = KEYS_SPARKLE;
			break;

		case KEYS_SPARKLE:
			DoSparkle(display, window);
			BorderGlow(display, window);
			if ((frame % FLASH) == 0)
				RandomDrawSpecials(display);
			HandleBlink(display, window);
			break;

		case KEYS_FINISH:
			DoFinish(display, window);
			break;

		case KEYS_WAIT:
			DoKeysWait();
			break;

		default:
			break;
	}
}

#if NeedFunctionPrototypes
void RedrawKeys(Display *display, Window window)
#else
void RedrawKeys(display, window)
	Display *display;
	Window window;
#endif
{
	DoIntroTitle(display, window);
	DoText(display, window);
}

#if NeedFunctionPrototypes
void FreeKeyControl(Display *display)
#else
void FreeKeyControl(display)
	Display *display;
#endif
{
    if (mouse)     		XFreePixmap(display, mouse);
    if (mouseM)     	XFreePixmap(display, mouseM);
    if (leftarrow)    	XFreePixmap(display, leftarrow);
    if (leftarrowM)    	XFreePixmap(display, leftarrowM);
    if (rightarrow)    	XFreePixmap(display, rightarrow);
    if (rightarrowM)   	XFreePixmap(display, rightarrowM);
}

#if NeedFunctionPrototypes
void ResetKeys(void)
#else
void ResetKeys()
#endif
{
	KeysState = KEYS_TITLE;
	startFrame 	= frame + 100;
	endFrame 	= frame + 4000;
	nextBlink = frame + 10;

	DEBUG("Reset keys mode.")
}

#if NeedFunctionPrototypes
static void DoKeysWait(void)
#else
static void DoKeysWait()
#endif
{
	if (frame == waitingFrame)
		KeysState = waitMode;
}
