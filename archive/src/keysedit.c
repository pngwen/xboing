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
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <xpm.h>

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

static void DoKeysEditWait(void);


/*
 *  Internal variable declarations:
 */

static int endFrame = 0;
static int startFrame = 0;
enum KeysEditStates KeysEditState;
static int waitingFrame;
enum KeysEditStates waitMode;

void SetUpKeysEdit(Display *display, Window window, Colormap colormap)
{
	ResetKeysEdit();
}

/* Lines of text shown in the instruction section */
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

/*
* Draws all of the instruction text.
* This is called once after the title and before the sparkle animation.
*/
static void DoText(Display *display, Window window)
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

	/* Leave space before the key bindings list */
    y += dataFont->ascent;
	y1 = y;

	/* Left column of key commands */
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


/*
* Handles the little sparkle animation that plays on the help screen.
* The animation moves a sparkle to random positions and animates it frame-by-frame.
* This uses global frame to time events.
*/
static void DoSparkle(Display *display, Window window)
{
	static Pixmap store;
	static int x = 100;
	static int y = 20;
	static int in = 0;

	/* When the animation reaches its end frame, finish the state */
	if (frame >= endFrame)
		KeysEditState = KEYSEDIT_FINISH;

	/* Create a small pixmap (an off-screen image) the first time */
	if (!store)
	{
		store = XCreatePixmap(display, window, 20, 20,
			DefaultDepth(display, XDefaultScreen(display)));
	}

	if (in == 0) 
		XCopyArea(display, window, store, gc, x, y, 20, 20, 0, 0);

	/* If it's time to advance the sparkle frame */
	if (frame == startFrame)
	{
		/* Restore background first */
		XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);

		/* Draw next sparkle frame */
		RenderShape(display, window, stars[in], starsM[in],
			x, y, 20, 20, False);

		/* Advance frame and set time for next frame */
	 	in++;
		startFrame = frame + 15;

		if (in == 11) 
		{
			/* Restore background */
			XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);
			in = 0;
			startFrame = frame + 500;
			x = (rand() % 474) + 5;
			y = (rand() % 74) + 5;
		}	
	}
}

/*
* Called once when the sparkle animation ends.
* Switches modes and plays a sound. :D
*/
static void DoFinish(Display *display, Window window)
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



/*
* The main update function for the keys editor.
* This is a state machine. Each part of the screen (title, text, sparkle, finish) happens in order.
* 
*/
void KeysEdit(Display *display, Window window)
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

/* Called when the screen needs to be redrawn fully */
void RedrawKeysEdit(Display *display, Window window)
{
	DoIntroTitle(display, window);
	DoText(display, window);
}

void FreeKeyEditControl(Display *display)
{
}

/*
* Resets all timing and goes back to the very first state.
*/
void ResetKeysEdit(void)
{
	KeysEditState = KEYSEDIT_TITLE;
	startFrame 	= frame + 100; /* Wait 100 frames before first sparkle */
	endFrame 	= frame + 4000; /* Sparkle section lasts 4000 frames */

	DEBUG("Reset KeysEdit mode.")
}

/*
* Helper function used when the state machine places itself in WAIT mode.
* It resumes operation when the global frame counter reaches waitingFrame.
*/
static void DoKeysEditWait(void)
{
	if (frame == waitingFrame)
		KeysEditState = waitMode;
}
