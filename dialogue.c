/**
 * @file dialogue.c
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Manages the dialogue for XBoing including taking user input
 * @version 1.1.1.1
 * @date 16 Dec 1994
 * 
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 *  
 * Manages the dialogue process and draws and redraws dialogue to the screen to match certain parameters
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
 * $Id: dialogue.c,v 1.1.1.1 1994/12/16 01:36:43 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/dialogue.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:43 $
 *
 * $Log: dialogue.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:43  jck
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
//#include <X11/Xlib.h>	//Replace this with RayLib
//#include <X11/Xutil.h>	//Replace this with RayLib
//#include <X11/Xos.h>	//Replace this with RayLib
//#include <X11/keysym.h>	//Replace this with RayLib
//#include <xpm.h>

#include "error.h"
#include "level.h"
#include "init.h"
#include "sfx.h"
#include "audio.h"
#include "stage.h"
#include "intro.h"
#include "main.h"
#include "misc.h"

#include "bitmaps/floppy.xpm"
#include "bitmaps/question.xpm"
#include "bitmaps/text.xpm"

#include "dialogue.h"

/*
 *  Internal macro definitions:
 */

/*
 *  Internal type declarations:
 */

static void redrawCurrentInput(Display *display);
static void handleDialogueEventLoop(Display *display);

/*
 *  Internal variable declarations:
 */

enum 	DialogueStates dialogueState;
int 	dialogueType, validation;
char 	theMessage[80];
char 	currentInput[1024];
Pixmap 	question, questionM;
Pixmap 	floppy, floppyM;
Pixmap 	text, textM;

/**
 * @brief Sets the Pixmap files for the dialogue for xboing
 * 
 * @param Display display X11 system display
 * @param Window window X11 ID for window
 * @pre Need to have X11 system
 * @post The pixmap files for the dialogue have been initialized
 * 
 * @todo Need to rewrite code so that it no longer relies on X11 system
 *
 */
void InitialiseDialoguePixmaps(Display *display, Window window,
    Colormap colormap)
{
    XpmAttributes   attributes;
    int         XpmErrorStatus;

    attributes.valuemask = XpmColormap;
    attributes.colormap = colormap;

    /* Create the playfield background pixmaps */

    XpmErrorStatus = XpmCreatePixmapFromData(display, window,
        floppy_xpm, &floppy, &floppyM, &attributes);
    HandleXPMError(display, XpmErrorStatus, 
		"InitialiseDialoguePixmaps(floppy)");

    XpmErrorStatus = XpmCreatePixmapFromData(display, window,
        question_xpm, &question, &questionM, &attributes);
    HandleXPMError(display, XpmErrorStatus, 
		"InitialiseDialoguePixmaps(question)");

    XpmErrorStatus = XpmCreatePixmapFromData(display, window,
        text_xpm, &text, &textM, &attributes);
    HandleXPMError(display, XpmErrorStatus, 
		"InitialiseDialoguePixmaps(text)");
}


/**
 * @brief Frees memory used by dialogue pixmap files
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11 system
 *
 */
void FreeDialoguePixmaps(Display *display)
{
    /* Free all the Dialogue Pixmaps */
    if (floppy)    		XFreePixmap(display, floppy);
    if (floppyM)    	XFreePixmap(display, floppyM);
    if (question)    	XFreePixmap(display, question);
    if (questionM)    	XFreePixmap(display, questionM);
    if (text)    		XFreePixmap(display, text);
    if (textM)    		XFreePixmap(display, textM);
}


/**
 * @brief Takes a user's input message for dialogue
 *
 * @param Display display X11 display
 * @param char message The user's input message
 * @param int type The type of dialogue
 * @param int entryValidation User's entry confirmation
 * 
 * @todo remove reliance on X11 systems
 *
 */ 
char *UserInputDialogueMessage(Display *display, char *message, int type,
	int entryValidation)
{
	/* Change the message for the dialogue */
	strcpy(theMessage, message);

	dialogueState 	= DIALOGUE_MAP;
	oldMode 		= mode;
	dialogueType 	= type;
	validation 		= entryValidation;
	mode 			= MODE_DIALOGUE;

	/* Null the current input string */
	strcpy(currentInput, "");

	handleDialogueEventLoop(display);

	DEBUG("UserInputDialogueMessage() returning input value.");

	return (currentInput);
}

/**
 * @brief Draw the input dialogue for the user
 *
 * @param Display display X11 display
 *
 * @todo remove reliance on X11 system
 *
 */
static void ShowUserInputDialogue(Display *display)
{
	int dw = DIALOGUE_WIDTH;

	XMapWindow(display, inputWindow);

	/* Change the background to a pattern */
	DrawStageBackground(display, inputWindow, BACKGROUND_1, True);
	XSetWindowBorder(display, inputWindow, red);

	/* Draw the message string */
	DrawShadowCentredText(display, inputWindow, textFont, 
		theMessage, 10, green, dw);

	DEBUG("ShowUserInputDialogue() setting icon for dialogue.");

	/**
	*
	* @todo Impliment the C Boolean Library
	*
	*/
	switch (dialogueType)
	{
		case DISK_ICON:
			RenderShape(display, inputWindow, 
				floppy, floppyM, 2, 4, 32, 32, False);
			break;
			
		case TEXT_ICON:
			RenderShape(display, inputWindow, 
				text, textM, 2, 4, 32, 32, False);
			break;
	}

	DrawLine(display, inputWindow, 10, 45, dw - 10, 45, white, 2);

	redrawCurrentInput(display);

	DEBUG("ShowUserInputDialogue() mapping dialogue now.");

	/*XMapWindow(display, inputWindow);*/
	XFlush(display);

	DEBUG("ShowUserInputDialogue() mapping complete.");
}


/**
 * @brief Removes user's input dialogue
 *
 * @param Display display X11 display
 *
 * @todo remove reliance on X11 system
 *
 */
static void RemoveUserInputDialogue(Display *display)
{
	while (WindowFadeEffect(display, inputWindow, 
		DIALOGUE_WIDTH, DIALOGUE_HEIGHT));

	DEBUG("RemoveUserInputDialogue() unmapping dialogue.");

	/* Unmap the user input dialogue */
	XUnmapWindow(display, inputWindow);
	XFlush(display);
}

/**
 * @brief Processes dialogue and carries out different functions in different situations
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11 system
 *
 */
void ProcessDialogue(Display *display)
{
    /* The states within the dialogue mode */
    switch (dialogueState)
    {
        case DIALOGUE_MAP:
			ShowUserInputDialogue(display);
			dialogueState = DIALOGUE_TEXT;
            break;

        case DIALOGUE_UNMAP:
			RemoveUserInputDialogue(display);
			dialogueState = DIALOGUE_FINISHED;
			mode = oldMode;
            break;

        case DIALOGUE_TEXT:
			break;

        case DIALOGUE_FINISHED:
			break;

		default:
			break;
	}
}

/**
 * @brief Redraws current input with new parameters
 *
 * @param Display display X11 display
 *
 * @todo remove X11 system
 *
 */
static void redrawCurrentInput(Display *display)
{
	/**
	*
	* @todo: Improve readability of this code
	* @todo: Reference the constant ints, possibly make them const vars
	*
	*/
	XClearArea(display, inputWindow, 0, 50, DIALOGUE_WIDTH, 50, False);
	if (strlen(currentInput) > 0)
		DrawShadowCentredText(display, inputWindow, textFont, currentInput, 
			70, yellow, DIALOGUE_WIDTH);
	else
		RenderShape(display, inputWindow, question, questionM, 
			(DIALOGUE_WIDTH / 2) - 16, 70, 
			32, 32, False);
}

/**
 * @brief Checks what keys are bein entered such as spacebar. The function will play sound based on certain actions.
 *
 * @param Display display X11 display
 * @param KeySym keysym The key being entered
 * @param char str The user input
 *
 * @todo Remove reliance on X11
 * @todo Choose new sound libraries
 *
 */
static void validateDialogueKeys(Display *display, KeySym keysym, char *str)
{
	/* Check the type of keyboard input needed */
	switch (validation)
	{
		case TEXT_ENTRY_ONLY:	/* Strictly text only */
			if (keysym < XK_space || keysym > XK_z)
				return;
			break;

		case YES_NO_ENTRY:	/* Strictly text Yy/Nn only */
			if (keysym != XK_y && keysym != XK_Y 
				&& keysym != XK_n && keysym != XK_N)
				return;
			break;

		case NUMERIC_ENTRY_ONLY:	/* Strictly numbers only */
			if (keysym < XK_0 || keysym > XK_9)
				return;
			break;

		case ALL_ENTRY:	/* Anything goes */
			if (keysym < XK_space || keysym > XK_asciitilde)
				return;
			break;
	}

	/* Check the width of the text is not out of bounds */
	if (XTextWidth(textFont, currentInput, strlen(currentInput)) 
		< (DIALOGUE_WIDTH - 20))
	{
		if (validation == YES_NO_ENTRY)
		{
			if (strlen(currentInput) == 0)
			{
				/* Now add that key to the input string */
				strncat(currentInput, str, 1);
				redrawCurrentInput(display);
			}
		}
		else
		{
			/* Now add that key to the input string */
			strncat(currentInput, str, 1);
			redrawCurrentInput(display);
		}

		/* Play a bit of sound */
   		if (noSound == False) playSoundFile("click", 70);
	}
	else
	{
		/* Play a bit of sound */
   		if (noSound == False) playSoundFile("tone", 40);
	}
}

/**
 * @brief Checks what symbol is being pressed and carries out an action based on that
 *
 * @param Display display X11 display
 * @param XEvent event The symbil that has been pressed
 *
 * @todo Remove reliance on X11 system
 *
 */
void handleDialogueKeys(Display *display, XEvent event)
{
	char *str;
	int len;
    KeySym keysym;
    int count;
    XComposeStatus compose;

	/* Obtain a small buffer */
	str = (char *) malloc(5 * sizeof(char));

    /* Lookup a keysym using the event key */
    count = XLookupString(&event.xkey, str, 1, &keysym, &compose);

    /* Switch on the keysym */
    switch (keysym)
    {
        case XK_Escape:  
			dialogueState = DIALOGUE_UNMAP;
			break;

        case XK_Return:
			dialogueState = DIALOGUE_UNMAP;
			break;

        case XK_Delete:
        case XK_BackSpace:
			len = strlen(currentInput);
			if (len > 0)
			{
				/* Backspace the text */
				currentInput[len-1] = '\0';
				redrawCurrentInput(display);

				/* Play a bit of sound */
   				if (noSound == False) playSoundFile("key", 70);
			}
			break;

		default:
			validateDialogueKeys(display, keysym, str);
			break;
	}

	/* Free the temp string */
	free(str);
}

/**
 * @brief Manages the dialogue event loop. It deals with one event then moves on to another.
 *
 * @param Display display X11 display
 *
 * @todo remove reliance on X11
 *
 */
static void handleDialogueEventLoop(Display *display)
{
    XEvent event;
    int pending;

    pending = 0;

	ProcessDialogue(display);
	XFlush(display);
    XNextEvent(display, &event);

	/* Now handle all valid dialogue entry events */
    while (dialogueState != DIALOGUE_FINISHED)
    {
        /* handle and audio device events if supported */
        audioDeviceEvents();

        /* Wait here for an event and then get the number waiting */
        XPeekEvent(display, &event);
        pending = XPending(display);

        /* Handle any events pending */
        while (pending > 0)
        {
            /* Get the next X event thanks */
            XNextEvent(display, &event);

            switch(event.type)
            {
                case KeyPress:
                    handleDialogueKeys(display, event);
                    break;

                case Expose:
                    break;

                default:
                    break;
            }

            /* Decrement the number of pending events */
            pending--;
        }

		ProcessDialogue(display);
	}
}

