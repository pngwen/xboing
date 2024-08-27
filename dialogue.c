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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <xpm.h>

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

#if NeedFunctionPrototypes
static void redrawCurrentInput(Display *display);
static void handleDialogueEventLoop(Display *display);
#else
static void handleDialogueEventLoop();
static void redrawCurrentInput();
#endif

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

#if NeedFunctionPrototypes
void InitialiseDialoguePixmaps(Display *display, Window window,
    Colormap colormap)
#else
void InitialiseDialoguePixmaps(display, window, colormap)
    Display *display;
    Window window;
    Colormap colormap;
#endif
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

#if NeedFunctionPrototypes
void FreeDialoguePixmaps(Display *display)
#else
void FreeDialoguePixmaps(display)
    Display *display;
#endif
{
    /* Free all the Dialogue Pixmaps */
    if (floppy)    		XFreePixmap(display, floppy);
    if (floppyM)    	XFreePixmap(display, floppyM);
    if (question)    	XFreePixmap(display, question);
    if (questionM)    	XFreePixmap(display, questionM);
    if (text)    		XFreePixmap(display, text);
    if (textM)    		XFreePixmap(display, textM);
}

#if NeedFunctionPrototypes
char *UserInputDialogueMessage(Display *display, char *message, int type,
	int entryValidation)
#else
char *UserInputDialogueMessage(display, message, type, entryValidation)
	Display *display;
	char *message;
	int type;
	int entryValidation;
#endif
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

#if NeedFunctionPrototypes
static void ShowUserInputDialogue(Display *display)
#else
static void ShowUserInputDialogue(display)
	Display *display;
#endif
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

#if NeedFunctionPrototypes
static void RemoveUserInputDialogue(Display *display)
#else
static void RemoveUserInputDialogue(display)
	Display *display;
#endif
{
	while (WindowFadeEffect(display, inputWindow, 
		DIALOGUE_WIDTH, DIALOGUE_HEIGHT));

	DEBUG("RemoveUserInputDialogue() unmapping dialogue.");

	/* Unmap the user input dialogue */
	XUnmapWindow(display, inputWindow);
	XFlush(display);
}

#if NeedFunctionPrototypes
void ProcessDialogue(Display *display)
#else
void ProcessDialogue(display)
	Display *display;
#endif
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

#if NeedFunctionPrototypes
static void redrawCurrentInput(Display *display)
#else
static void redrawCurrentInput(display)
    Display *display;
#endif
{
	XClearArea(display, inputWindow, 0, 50, DIALOGUE_WIDTH, 50, False);
	if (strlen(currentInput) > 0)
		DrawShadowCentredText(display, inputWindow, textFont, currentInput, 
			70, yellow, DIALOGUE_WIDTH);
	else
		RenderShape(display, inputWindow, question, questionM, 
			(DIALOGUE_WIDTH / 2) - 16, 70, 
			32, 32, False);
}

#if NeedFunctionPrototypes
static void validateDialogueKeys(Display *display, KeySym keysym, char *str)
#else
static void validateDialogueKeys(display, keysym, str)
    Display *display;
	KeySym keysym;
	char *str;
#endif
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

#if NeedFunctionPrototypes
void handleDialogueKeys(Display *display, XEvent event)
#else
void handleDialogueKeys(display, event)
    Display *display;
	XEvent event;
#endif
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

#if NeedFunctionPrototypes
static void handleDialogueEventLoop(Display *display)
#else
static void handleDialogueEventLoop(display)
    Display *display;
#endif
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

