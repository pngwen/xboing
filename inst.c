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
 * $Id: inst.c,v 1.1.1.1 1994/12/16 01:36:46 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/inst.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:46 $
 *
 * $Log: inst.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:46  jck
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
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <xpm.h>

#include "error.h"
#include "highscore.h"
#include "misc.h"
#include "main.h"
#include "init.h"
#include "stage.h"
#include "bonus.h"
#include "special.h"
#include "blocks.h"
#include "ball.h"
#include "score.h"
#include "paddle.h"
#include "level.h"
#include "mess.h"
#include "sfx.h"
#include "version.h"
#include "intro.h"
#include "audio.h"
#include "keys.h"
#include "demo.h"

#include "inst.h"

/*
 *  Internal macro definitions:
 */

#define GAP		5

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
void SetInstructWait(enum InstructStates newMode, int waitFrame);
static void DoSparkle(Display *display, Window window);
void DoInstructWait(void);
#else
static void DoSparkle();
void SetInstructWait();
void DoInstructWait();
#endif

/*
 *  Internal variable declarations:
 */

static int endFrame = 0;
static int nextFrame = 0;
enum InstructStates InstructState;
static int waitingFrame;
enum InstructStates waitMode;

#if NeedFunctionPrototypes
void SetUpInstructions(Display *display, Window window, Colormap colormap)
#else
void SetUpInstructions(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	/* Umm. Reset the instructions to default state */
	ResetInstructions();
}

char *instructionText[] =
{
	"XBoing is a blockout game where you use a paddle to bounce",
	"a proton ball around an arena full of nasties while keeping",
	"the ball from leaving the arena via the bottom rebound wall.",
	NULL,
	"Each block has a point value associated with it. Some blocks",
	"may award you more points for hitting them a number of times.",
	"Some blocks may toggle extra time/points or even special effects",
	"such as no walls, machine gun, sticky paddle, reverse controls, etc.",
	NULL,
	"Your paddle is equipped with special bullets that can disintegrate",
	"a block. You only have a limited supply of bullets so use them wisely.",
	NULL,
	"The multiple ball block will give you an extra ball to play with in",
	"the arena. This ball will act like any other ball except that when",
	"it dies it will not force a new ball to start. You can shoot your",
	"own ball so watch out. The death symbol is not too healthy either.",
	NULL,
	"Sometimes a special block may appear or be added to another block",
	"that will effect the gameplay if hit. They also disappear randomly.",
	NULL,
	"Please read the manual for more information on how to play."
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
	int y, i = 0, j = 0;

	SetCurrentMessage(display, messWindow, "Save the rainforests", False);

	DrawShadowCentredText(display, window, titleFont, 
		"- Instructions -", 110, red, PLAY_WIDTH);

	y = 150;

	/* Loop through all text printing it centred */
	for (i = 0; i < (sizeof(instructionText) / sizeof(char *)); i++)
	{
		/* If the text is not null then print it */
		if (instructionText[i] != '\0')
		{
			strcpy(string, instructionText[i]); 
			DrawShadowCentredText(display, window, dataFont, 
				string, y, j % 2 ? greens[0] : greens[2], PLAY_WIDTH);
			y += dataFont->ascent + GAP;
			j++;
		}
		else
			y += dataFont->ascent + GAP;
	}

	y += dataFont->ascent + GAP/2;

	/* Draw the standard message */
	strcpy(string, "Insert coin to start the game");
	DrawShadowCentredText(display, window, textFont, string, 
		PLAY_HEIGHT - 40, tann, PLAY_WIDTH);
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
        InstructState = INSTRUCT_FINISH;

    if (!store)
    {
        store = XCreatePixmap(display, window, 20, 20,
            DefaultDepth(display, XDefaultScreen(display)));
    }

    if (in == 0)
        XCopyArea(display, window, store, gc, x, y, 20, 20, 0, 0);

    if (frame == nextFrame)
    {
        XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);
        RenderShape(display, window, stars[in], starsM[in],
            x, y, 20, 20, False);

        in++;
        nextFrame = frame + 15;

        if (in == 11)
        {
            XCopyArea(display, store, window, gc, 0, 0, 20, 20, x, y);
            in = 0;
            nextFrame = frame + 500;
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
	ResetDemonstration();
	mode = MODE_DEMO;

    if (noSound == False)
		playSoundFile("shark", 50);
}


#if NeedFunctionPrototypes
void Instructions(Display *display, Window window)
#else
void Instructions(display, window)
	Display *display;
	Window window;
#endif
{
	switch (InstructState)
	{
		case INSTRUCT_TITLE:
			if (getSpecialEffects(display) == True)
				DoIntroTitle(display, bufferWindow);
			else
				DoIntroTitle(display, window);
			InstructState = INSTRUCT_TEXT;
			break;

		case INSTRUCT_TEXT:
			if (getSpecialEffects(display) == True)
			{
				DoText(display, bufferWindow);
				while (WindowShatterEffect(display, window));
			}
			else
				DoText(display, window);
			InstructState = INSTRUCT_SPARKLE;
			break;

		case INSTRUCT_SPARKLE:
			DoSparkle(display, window);
			BorderGlow(display, window);
			if ((frame % FLASH) == 0)
				RandomDrawSpecials(display);
			break;

		case INSTRUCT_FINISH:
			DoFinish(display, window);
			break;

		case INSTRUCT_WAIT:
			DoInstructWait();
			break;

		default:
			break;
	}
}

#if NeedFunctionPrototypes
void RedrawInstructions(Display *display, Window window)
#else
void RedrawInstructions(display, window)
	Display *display; 
	Window window;
#endif
{
	DoIntroTitle(display, window);
	DoText(display, window);
}

#if NeedFunctionPrototypes
void FreeInstructions(Display *display)
#else
void FreeInstructions(display)
	Display *display;
#endif
{
}

#if NeedFunctionPrototypes
void ResetInstructions(void)
#else
void ResetInstructions()
#endif
{
	InstructState = INSTRUCT_TITLE;
	nextFrame 	= frame + 100;
	endFrame 	= frame + 7000;

	DEBUG("Reset Instruction mode.")
}

#if NeedFunctionPrototypes
void SetInstructWait(enum InstructStates newMode, int waitFrame)
#else
void SetInstructWait(newMode, waitFrame)
	enum InstructStates newMode;
	int waitFrame;
#endif
{
	waitingFrame 	= waitFrame;
	waitMode 		= newMode;
	InstructState 	= INSTRUCT_WAIT;
}

#if NeedFunctionPrototypes
void DoInstructWait(void)
#else
void DoInstructWait()
#endif
{
	if (frame == waitingFrame)
		InstructState = waitMode;
}
