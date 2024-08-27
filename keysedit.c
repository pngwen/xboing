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
 * $Id: keysedit.c,v 1.1.1.1 1994/12/16 01:36:48 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/keysedit.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:48 $
 *
 * $Log: keysedit.c,v $
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

/*
 *  Internal macro definitions:
 */

#define GAP 12

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
static void DoKeysEditWait(void);
#else
static void DoKeysEditWait();
#endif


/*
 *  Internal variable declarations:
 */

static int endFrame = 0;
static int startFrame = 0;
enum KeysEditStates KeysEditState;
static int waitingFrame;
enum KeysEditStates waitMode;

#if NeedFunctionPrototypes
void SetUpKeysEdit(Display *display, Window window, Colormap colormap)
#else
void SetUpKeysEdit(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	ResetKeysEdit();
}

char *infoText[] =
{
    "The level editor will allow you to edit any of the levels and",
    "create your own ones. You use the left button for drawing blocks",
	"and the middle button for erasing. Below lists the keys that can",
	"be used while in the level editor.",
	"The bottom four rows are reserved so that your paddle and ball",
	"has room to start. Design your levels so that it is possible",
	"to finish. Remember that random specials will appear.",
};

#if NeedFunctionPrototypes
static void DoText(Display *display, Window window)
#else
static void DoText(display, window)
	Display *display;
	Window window;
#endif
{
	char string[80];
	int y, i, j, x, y1;

	SetCurrentMessage(display, messWindow, "Be happy!", False);

	y = 120;

	DrawShadowCentredText(display, window, titleFont, 
		"- Level Editor Controls -", y, red, PLAY_WIDTH);

	y = 160;

    DrawLine(display, window, 32, y+2, PLAY_WIDTH - 28, y+2, black, 3);
    DrawLine(display, window, 30, y, PLAY_WIDTH - 30, y, white, 3);
	y += textFont->ascent;

    /* Loop through all text printing it centred */
    for (i = 0, j = 0; i < (sizeof(infoText) / sizeof(char *)); i++)
    {
        /* If the text is not null then print it */
        if (infoText[i] != '\0')
        {
            strcpy(string, infoText[i]);
            DrawShadowCentredText(display, window, dataFont,
                string, y, j % 2 ? greens[0] : greens[2], PLAY_WIDTH);
            y += dataFont->ascent + GAP;
            j++;
        }
        else
            y += dataFont->ascent + GAP;
    }

    y += dataFont->ascent;

	y1 = y;
	x = 30;

	strcpy(string, "<r> = Redraw level");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<c> = Clear Level");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<q> = Quit editor");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<h/v> = Flip horz/vert");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<H/V> = Scroll horz/vert");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	y = y1;
	x = 270;

	strcpy(string, "<s> = Save level");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<l> = load level");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<t> = Set time limit");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<n> = Change level name");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;

	strcpy(string, "<p> = Play test");
	DrawShadowText(display, window, textFont, string, x, y, yellow);
	y += textFont->ascent + GAP;


	y += textFont->ascent + GAP / 2;

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
		KeysEditState = KEYSEDIT_FINISH;

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
	static int toggle = GLOBAL;

	ResetHighScore(toggle);
	mode = MODE_HIGHSCORE;

	/* Switch between the global highscores and personal version */
	if (toggle == GLOBAL)
		toggle = PERSONAL;
	else
		toggle = GLOBAL;

    if (noSound == False)
		playSoundFile("warp", 50);
}


#if NeedFunctionPrototypes
void KeysEdit(Display *display, Window window)
#else
void KeysEdit(display, window)
	Display *display;
	Window window;
#endif
{
	switch (KeysEditState)
	{
		case KEYSEDIT_TITLE:
			if (getSpecialEffects(display) == True)
				DoIntroTitle(display, bufferWindow);
			else
				DoIntroTitle(display, window);
			KeysEditState = KEYSEDIT_TEXT;
			break;

		case KEYSEDIT_TEXT:
			if (getSpecialEffects(display) == True)
			{
				DoText(display, bufferWindow);
				while (WindowShatterEffect(display, window));
			}
			else
				DoText(display, window);
			KeysEditState = KEYSEDIT_SPARKLE;
			break;

		case KEYSEDIT_SPARKLE:
			DoSparkle(display, window);
			BorderGlow(display, window);
			if ((frame % FLASH) == 0)
				RandomDrawSpecials(display);
			break;

		case KEYSEDIT_FINISH:
			DoFinish(display, window);
			break;

		case KEYSEDIT_WAIT:
			DoKeysEditWait();
			break;

		default:
			break;
	}
}

#if NeedFunctionPrototypes
void RedrawKeysEdit(Display *display, Window window)
#else
void RedrawKeysEdit(display, window)
	Display *display;
	Window window;
#endif
{
	DoIntroTitle(display, window);
	DoText(display, window);
}

#if NeedFunctionPrototypes
void FreeKeyEditControl(Display *display)
#else
void FreeKeyEditControl(display)
	Display *display;
#endif
{
}

#if NeedFunctionPrototypes
void ResetKeysEdit(void)
#else
void ResetKeysEdit()
#endif
{
	KeysEditState = KEYSEDIT_TITLE;
	startFrame 	= frame + 100;
	endFrame 	= frame + 4000;

	DEBUG("Reset KeysEdit mode.")
}

#if NeedFunctionPrototypes
static void DoKeysEditWait(void)
#else
static void DoKeysEditWait()
#endif
{
	if (frame == waitingFrame)
		KeysEditState = waitMode;
}
