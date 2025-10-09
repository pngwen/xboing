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
#include <string.h> // Added for implementation of strcpy()

// TODO: Remove functional reliance on X11
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xos.h>
// #include <xpm.h>

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

/**
 * @brief Sets instruction state & other properties to wait. Function-not-used, could be removed.
 * 
 * @param newMode The new instruction state to set
 * @param waitFrame The new waiting frame to set
 */
void SetInstructWait(enum InstructStates newMode, int waitFrame);

/**
 * @brief Draws sparkle animation
 * 
 * @param display The display of the X11 window
 * @param window The X11 window to draw on
 */
static void DoSparkle(Display *display, Window window);

/**
 * @brief Sets the instruction state to wait if the current frame is waiting. Single-use-function, could be removed
 * 
 */
void DoInstructWait(void);

/*
 *  Internal variable declarations:
 */

static int endFrame = 0;
static int nextFrame = 0;
enum InstructStates InstructState;
static int waitingFrame;
enum InstructStates waitMode;


/**
 * @brief Sets up the initial instruction display.
 *
 * This function resets the instruction display to its default state.
 *
 * @param display A pointer to the Display structure for graphics.
 * @param window The window where instructions will be displayed.
 * @param colormap The colormap used for rendering.
 */
void SetUpInstructions(Display *display, Window window, Colormap colormap)
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

/**
 * @brief Draws instruction text to the screen
 * 
 * @param display The display of the X11 window
 * @param window the X11 window to draw on
 */
static void DoText(Display *display, Window window)
{
	char string[80];// [DOXYGEN] Unneeded string used for unnecessary string copy
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
			strcpy(string, instructionText[i]);// [DOXYGEN] Unnecessary string copy, could be removed.
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

/**
 * @brief Creates a sparkle effect in the instruction display.
 *
 * This function animates a sparkle effect at a random position on the screen.
 *
 * @param display A pointer to the Display structure for graphics.
 * @param window The window where the sparkle effect will be drawn.
 */
static void DoSparkle(Display *display, Window window)
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


/**
 * @brief Finalizes the instruction display.
 *
 * This function resets the demonstration mode and plays a sound.
 *
 * @param display A pointer to the Display structure for graphics.
 * @param window The window where instructions will be displayed.
 */
static void DoFinish(Display *display, Window window)
{
	ResetDemonstration();
	mode = MODE_DEMO;

    if (noSound == False)
		playSoundFile("shark", 50);
}

/**
 * @brief Main function for handling instruction display states.
 *
 * This function updates the instruction display based on the current state.
 *
 * @param display A pointer to the Display structure for graphics.
 * @param window The window where instructions will be displayed.
 */

void Instructions(Display *display, Window window)
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

/**
 * @brief Redraws the current instructions.
 *
 * This function re-renders the title and text instructions in the specified window.
 *
 * @param display A pointer to the Display structure for graphics.
 * @param window The window where instructions will be redrawn.
 */
void RedrawInstructions(Display *display, Window window)
{
	DoIntroTitle(display, window);
	DoText(display, window);
}

/**
 * @brief Frees resources associated with instructions.
 *
 * This function currently does not perform any actions but is provided for future use.
 *
 * @param display A pointer to the Display structure for graphics.
 */
void FreeInstructions(Display *display)
{
}

/**
 * @brief Resets the instruction state.
 *
 * This function sets the instruction display back to its initial state.
 */
void ResetInstructions(void)
{
	InstructState = INSTRUCT_TITLE;
	nextFrame 	= frame + 100;
	endFrame 	= frame + 7000;

	DEBUG("Reset Instruction mode.")
}

/**
 * @brief Sets the state to wait before proceeding with instructions.
 *
 * This function updates the waiting frame and the next mode to proceed to.
 *
 * @param newMode The new instruction state to transition to.
 * @param waitFrame The frame at which to transition to the new mode.
 */
void SetInstructWait(enum InstructStates newMode, int waitFrame)
{
	waitingFrame 	= waitFrame;
	waitMode 		= newMode;
	InstructState 	= INSTRUCT_WAIT;
}

/**
 * @brief Handles the waiting state in instruction display.
 *
 * This function checks if the wait duration has elapsed to transition to the next state.
 */
void DoInstructWait(void)
{
	if (frame == waitingFrame)
		InstructState = waitMode;
}
