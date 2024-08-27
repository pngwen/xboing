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
 * $Id: preview.c,v 1.1.1.1 1994/12/16 01:36:48 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/preview.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:48 $
 *
 * $Log: preview.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:48  jck
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

#include "error.h"
#include "misc.h"
#include "gun.h"
#include "main.h"
#include "init.h"
#include "file.h"
#include "inst.h"
#include "stage.h"
#include "blocks.h"
#include "sfx.h"
#include "special.h"
#include "ball.h"
#include "score.h"
#include "paddle.h"
#include "level.h"
#include "mess.h"
#include "audio.h"
#include "intro.h"
#include "keys.h"
#include "version.h"

#include "preview.h"

/*
 *  Internal macro definitions:
 */

#define GAP 10

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
void SetPreviewWait(enum PreviewStates newMode, int waitFrame);
void DoPreviewWait(void);
#else
void SetPreviewWait();
void DoPreviewWait();
#endif

/*
 *  Internal variable declarations:
 */

enum PreviewStates PreviewState;
static int waitingFrame;
enum PreviewStates waitMode;

#if NeedFunctionPrototypes
void SetUpPreviewLevel(Display *display, Window window, Colormap colormap)
#else
void SetUpPreviewLevel(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	ResetPreviewLevel();
}

#if NeedFunctionPrototypes
static void DoLoadLevel(Display *display, Window window)
#else
static void DoLoadLevel(display, window)
	Display *display;
	Window window;
#endif
{
	int lnum = 1;
    char levelPath[1024];
    char str2[80];
    char *str;
    static int bgrnd = 1;

    bgrnd++;
    if (bgrnd == 6) bgrnd = 2;
    DrawStageBackground(display, window, bgrnd, True);

	/* choose a random level */
	lnum = (rand() % (MAX_NUM_LEVELS - 1)) + 1;

    /* Construct the Preview level filename */
    if ((str = getenv("XBOING_LEVELS_DIR")) != NULL)
        sprintf(levelPath, "%s/level%02d.data", str, lnum);
    else
        sprintf(levelPath, "%s/level%02d.data", LEVEL_INSTALL_DIR, lnum);

	/* Read in a Preview level */
    if (ReadNextLevel(display, window, levelPath, True) == False)
        ShutDown(display, 1, "Sorry, invalid level specified.");

	sprintf(str2, "- %s -", GetLevelName());
	DrawShadowCentredText(display, window, titleFont, 
		str2, PLAY_HEIGHT - 80, red, PLAY_WIDTH);

	/* Be very friendly */
	sprintf(str2, "Preview of level %d", lnum);
	SetCurrentMessage(display, messWindow, str2, False);

	DisplayLevelInfo(display, levelWindow, (u_long) lnum);
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

	y = PLAY_HEIGHT - 27;

	strcpy(string, "Insert coin to start the game");
	DrawShadowCentredText(display, window, textFont, 
		string, y, tann, PLAY_WIDTH);

	if (noSound == False) 
	{
		if ((rand() % 3) == 0)
			playSoundFile("looksbad", 80);
	}

	SetPreviewWait(PREVIEW_FINISH, frame + 5000);
}

#if NeedFunctionPrototypes
static void DoFinish(Display *display, Window window)
#else
static void DoFinish(display, window)
	Display *display;
	Window window;
#endif
{
    mode = MODE_INTRO;
    ResetIntroduction();

    if (noSound == False)
        playSoundFile("whizzo", 50);

    SetGameSpeed(FAST_SPEED);
}

#if NeedFunctionPrototypes
void PreviewLevel(Display *display, Window window)
#else
void PreviewLevel(display, window)
	Display *display;
	Window window;
#endif
{
	switch (PreviewState)
	{
		case PREVIEW_LEVEL:
			if (getSpecialEffects(display) == True)
				DoLoadLevel(display, bufferWindow);
			else
				DoLoadLevel(display, window);
			PreviewState = PREVIEW_TEXT;
			break;

		case PREVIEW_TEXT:
			if (getSpecialEffects(display) == True)
			{
				DoText(display, bufferWindow);
				while (WindowShatterEffect(display, window));
			}
			else
				DoText(display, window);
			break;

		case PREVIEW_FINISH:
			DoFinish(display, window);
			break;

		case PREVIEW_WAIT:
			BorderGlow(display, window);
            if ((frame % FLASH) == 0)
                RandomDrawSpecials(display);

			DoPreviewWait();
			break;

		default:
			break;
	}
}

#if NeedFunctionPrototypes
void RedrawPreviewLevel(Display *display, Window window)
#else
void RedrawPreviewLevel(display, window)
	Display *display;
	Window window;
#endif
{
	DoLoadLevel(display, window);
	DoText(display, window);
}

#if NeedFunctionPrototypes
void FreePreviewLevel(Display *display)
#else
void FreePreviewLevel(display)
	Display *display;
#endif
{
}

#if NeedFunctionPrototypes
void ResetPreviewLevel(void)
#else
void ResetPreviewLevel()
#endif
{
	PreviewState = PREVIEW_LEVEL;

	DEBUG("Reset PreviewLevel mode.")
}

#if NeedFunctionPrototypes
void SetPreviewWait(enum PreviewStates newMode, int waitFrame)
#else
void SetPreviewWait(newMode, waitFrame)
	enum PreviewStates newMode;
	int waitFrame;
#endif
{
	waitingFrame = waitFrame;
	waitMode = newMode;
	PreviewState = PREVIEW_WAIT;
}

#if NeedFunctionPrototypes
void DoPreviewWait(void)
#else
void DoPreviewWait()
#endif
{
	if (frame == waitingFrame)
		PreviewState = waitMode;
}
