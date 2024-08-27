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
 * $Id: stage.c,v 1.1.1.1 1994/12/16 01:36:45 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/stage.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:45 $
 *
 * $Log: stage.c,v $
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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <xpm.h>

#include "bitmaps/bgrnds/mnbgrnd.xpm"
#include "bitmaps/bgrnds/space.xpm"
#include "bitmaps/bgrnds/bgrnd.xpm"
#include "bitmaps/bgrnds/bgrnd2.xpm"
#include "bitmaps/bgrnds/bgrnd3.xpm"
#include "bitmaps/bgrnds/bgrnd4.xpm"
#include "bitmaps/bgrnds/bgrnd5.xpm"
#include "bitmaps/icon.xpm"
#include "bitmaps/eyes/deveyes.xpm"
#include "bitmaps/eyes/deveyes1.xpm"
#include "bitmaps/eyes/deveyes2.xpm"
#include "bitmaps/eyes/deveyes3.xpm"
#include "bitmaps/eyes/deveyes4.xpm"
#include "bitmaps/eyes/deveyes5.xpm"

#include "error.h"
#include "blocks.h"
#include "editor.h"
#include "sfx.h"
#include "ball.h"
#include "misc.h"
#include "paddle.h"
#include "dialogue.h"
#include "version.h"
#include "init.h"

#include "stage.h"

/*
 *  Internal macro definitions:
 */

#define LEFT_OFFSET	    10
#define RIGHT_OFFSET    10
#define TOP_OFFSET      10
#define MIDDLE_OFFSET   10

#define	DEVILEYE_WIDTH		57
#define	DEVILEYE_HEIGHT		16
#define	DEVILEYE_WC			(DEVILEYE_WIDTH / 2)
#define	DEVILEYE_HC			(DEVILEYE_HEIGHT / 2)

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
static Window SetWMIcon(Display *display);
#else
static Window SetWMIcon();
#endif

/*
 *  Internal variable declarations:
 */

Window 	iconWindow;
Window 	inputWindow;
Window 	mainWindow;
Window 	scoreWindow;
Window 	levelWindow;
Window 	playWindow;
Window 	bufferWindow;
Window 	messWindow;
Window 	specialWindow;
Window 	timeWindow;
Window 	blockWindow;
Window 	typeWindow;
Pixmap	mainBackPixmap, iconPixmap, spacePixmap;
Pixmap  back1Pixmap, back2Pixmap, back3Pixmap, back4Pixmap, back5Pixmap;
Pixmap  devilblink[6], devilblinkM[6];
int 	devilx, devily;
int 	blinkslides[] = { 0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0, 0, 0, 
					      0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0 };


#if NeedFunctionPrototypes
void InitialiseMainBackPixmap(Display *display, Window window, 
	Colormap colormap)
#else
void InitialiseMainBackPixmap(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	XpmAttributes   attributes;
	int		    XpmErrorStatus;

	attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

    /* Create the playfield background pixmaps */

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, 
		mainbackground_xpm, &mainBackPixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseMainBackPixmap()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, 
		space_xpm, &spacePixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseMainBackPixmap(space)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, background_xpm,
		&back1Pixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseMainBackPixmap()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, background2_xpm,
		&back2Pixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseMainBackPixmap()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, background3_xpm,
		&back3Pixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseMainBackPixmap()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, background4_xpm,
		&back4Pixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseMainBackPixmap()");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, background5_xpm,
		&back5Pixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseMainBackPixmap()");

	/* Devil blink animation */

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, devileyes_xpm,
		&devilblink[0], &devilblinkM[0], &attributes);
	HandleXPMError(display, XpmErrorStatus, 
		"InitialiseMainBackPixmap(devilblink[0])");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, devileyes1_xpm,
		&devilblink[1], &devilblinkM[1], &attributes);
	HandleXPMError(display, XpmErrorStatus, 
		"InitialiseMainBackPixmap(devilblink[1])");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, devileyes2_xpm,
		&devilblink[2], &devilblinkM[2], &attributes);
	HandleXPMError(display, XpmErrorStatus, 
		"InitialiseMainBackPixmap(devilblink[2])");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, devileyes3_xpm,
		&devilblink[3], &devilblinkM[3], &attributes);
	HandleXPMError(display, XpmErrorStatus, 
		"InitialiseMainBackPixmap(devilblink[3])");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, devileyes4_xpm,
		&devilblink[4], &devilblinkM[4], &attributes);
	HandleXPMError(display, XpmErrorStatus, 
		"InitialiseMainBackPixmap(devilblink[4])");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, devileyes5_xpm,
		&devilblink[5], &devilblinkM[5], &attributes);
	HandleXPMError(display, XpmErrorStatus, 
		"InitialiseMainBackPixmap(devilblink[5])");

	/* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);
}

#if NeedFunctionPrototypes
void ClearMainWindow(Display *display, Window window)
#else
void ClearMainWindow(display, window)
	Display *display;
	Window window;
#endif
{
	/* Make sure that it is drawn */
	XSetWindowBackgroundPixmap(display, mainWindow, spacePixmap);
	XClearWindow(display, mainWindow);
}

#if NeedFunctionPrototypes
void SetWindowSizeHints(Display *display, int w, int h)
#else
void SetWindowSizeHints(display, w, h)
	Display *display;
	int w, h;
#endif
{
	XSizeHints 		sizehints;
	
	/* Setup the max and minimum size that the window will be */
	sizehints.flags 		= PPosition | PSize | PMinSize | PMaxSize;
	sizehints.min_width 	= w;
	sizehints.min_height	= h;
	sizehints.max_width 	= w;
	sizehints.max_height	= h;

	/* Now set the window manager size hints properties */
	XSetWMNormalHints(display, mainWindow, &sizehints);
}

#if NeedFunctionPrototypes
void CreateAllWindows(Display *display, Colormap colormap,
	char **argv, int argc)
#else
void CreateAllWindows(display, colormap, argv, argc)
	Display *display;
	Colormap colormap;
	char **argv;
	int argc;
#endif
{
    char 			title[80];
	int 			offsetX, offsetY, scoreWidth;
	XWMHints 		wmhints;
	XClassHint 		classhints;
	XSizeHints 		sizehints;
	XTextProperty 	windowName, iconName;
	XSetWindowAttributes winattr;
	unsigned long 	valuemask;
	int temp;

	char *window_Name 	= "- XBoing II -";
	char *icon_Name 	= "XBoing II";

	offsetX = MAIN_WIDTH / 2;
	offsetY = MAIN_HEIGHT / 2;
	scoreWidth = 224;

	DEBUG("Creating windows ....");

	/* Create the main window */
    mainWindow = XCreateSimpleWindow(display,
		RootWindow(display, DefaultScreen(display)), 0, 0,
		PLAY_WIDTH + MAIN_WIDTH + 10, PLAY_HEIGHT + MAIN_HEIGHT + 10, 2, 
		red, black);

	/* Create the score window */
    scoreWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX, 10, scoreWidth, 42, 0, white, black);

	/* Create the level window */
    levelWindow = XCreateSimpleWindow(display, mainWindow, 
		scoreWidth + offsetX + 25, 5, 
			PLAY_WIDTH + offsetX - 20 - scoreWidth, 52, 0, white, black);

	/* Create the playing area window */
    playWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX, 60, PLAY_WIDTH, PLAY_HEIGHT, 2, red, black);

	/* Create the buffer playing area window NON VISIBLE */
    bufferWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX, 60, PLAY_WIDTH, PLAY_HEIGHT, 2, red, black);

	/* Create the message area window */
    messWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX, 65 + PLAY_HEIGHT + 10, PLAY_WIDTH / 2, MESS_HEIGHT, 0, 
		white, black);

	/* Create the special bonus area window */
    specialWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX + PLAY_WIDTH / 2 + 10, 65 + PLAY_HEIGHT + 10, 
		180, MESS_HEIGHT + 5, 0, white, black);

	/* Create the timer area window */
    timeWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX + PLAY_WIDTH / 2 + 10 + 180 + 5, 
		65 + PLAY_HEIGHT + 10, PLAY_WIDTH / 8, MESS_HEIGHT + 5, 
		0, white, black);

	/* Create the blocks window for editor */
    blockWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX + PLAY_WIDTH + 15, 60, EDITOR_TOOL_WIDTH, PLAY_HEIGHT, 
		2, red, black);

	/* Create the type of blocks currently active window for editor */
    typeWindow = XCreateSimpleWindow(display, mainWindow, 
		offsetX + PLAY_WIDTH + 15, 65 + PLAY_HEIGHT + 5, 
		EDITOR_TOOL_WIDTH, TYPE_HEIGHT, 
		2, red, black);

	/* Create the input dialogue window */
    inputWindow = XCreateSimpleWindow(display, mainWindow, 
		((PLAY_WIDTH + MAIN_WIDTH) / 2) - (DIALOGUE_WIDTH / 2), 
		((PLAY_HEIGHT + MAIN_HEIGHT) / 2) - (DIALOGUE_HEIGHT / 2), 
		DIALOGUE_WIDTH, DIALOGUE_HEIGHT, 
		4, red, black);

	DEBUG("Creating windows finished.");

    /* Set window manager properties */
	sprintf(title, "XBoing V%d.%d", VERSION, REVNUM + buildNum);

	if (XStringListToTextProperty(&window_Name, 1, &windowName) == 0)
		ShutDown(display, 1, "Cannot create window name resource.");

	if (XStringListToTextProperty(&icon_Name, 1, &iconName) == 0) 
		ShutDown(display, 1, "Cannot create icon name resource.");

	if (noicon == False)
		iconWindow = SetWMIcon(display);

	/* Various window manager settings */
    wmhints.initial_state 	= NormalState;
	wmhints.input 			= True;

	if (noicon == False)
	{
		/* Don't create an icon if the user doesn't want it */
		wmhints.icon_pixmap 	= iconPixmap;
		wmhints.icon_window 	= iconWindow;
		wmhints.flags = StateHint | InputHint | IconPixmapHint | IconWindowHint;
	}
	else
		wmhints.flags = StateHint | InputHint;

	/* Set the class for XBoing */
	classhints.res_name		= "XBoing";
	classhints.res_class 	= "XBoing";

	/* Setup the max and minimum size that the window will be */
	sizehints.flags 		= PPosition | PSize | PMinSize | PMaxSize;
	sizehints.min_width 	= PLAY_WIDTH + MAIN_WIDTH + 10;
	sizehints.min_height	= PLAY_HEIGHT + MAIN_HEIGHT + 10;
	sizehints.max_width 	= PLAY_WIDTH + MAIN_WIDTH + 10;
	sizehints.max_height	= PLAY_HEIGHT + MAIN_HEIGHT + 10;

	/* Now set the window manager properties */
	XSetWMProperties(display, mainWindow, &windowName, &iconName,
		argv, argc, &sizehints, &wmhints, &classhints);

	DEBUG("Setting WM properties.");

	if (noicon == False)
	{
		/* Set the current icon as the window's background pixmap */
		XSetWindowBackgroundPixmap(display, iconWindow, iconPixmap);
		XClearWindow(display, iconWindow);
	}

	valuemask = CWColormap;
	winattr.colormap = colormap;

	/* Check if the server allows backing store */
    if (DoesBackingStore(XDefaultScreenOfDisplay(display)) == Always)
	{
		/* Ok we want backing store as it is very useful */
		valuemask |= CWBackingStore;
		winattr.backing_store = Always;
	}

	DEBUG("Changing window attributes.");

	XChangeWindowAttributes(display, mainWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, playWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, bufferWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, levelWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, scoreWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, messWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, specialWindow, valuemask, &winattr);
	XChangeWindowAttributes(display, timeWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, inputWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, blockWindow, 	valuemask, &winattr);
	XChangeWindowAttributes(display, typeWindow, 	valuemask, &winattr);
}

#if NeedFunctionPrototypes
void SetBackgrounds(Display *display, Colormap colormap)
#else
void SetBackgrounds(display, colormap)
	Display *display;
	Colormap colormap;
#endif
{
	InitialiseMainBackPixmap(display, mainWindow, colormap);

	ClearMainWindow(display, mainWindow);
	XSetWindowBackgroundPixmap(display, levelWindow, ParentRelative);
	XClearWindow(display, levelWindow);
	XSetWindowBackgroundPixmap(display, scoreWindow, ParentRelative);
	XClearWindow(display, scoreWindow);
	XSetWindowBackgroundPixmap(display, specialWindow, ParentRelative);
	XClearWindow(display, specialWindow);
	XSetWindowBackgroundPixmap(display, timeWindow, ParentRelative);
	XClearWindow(display, timeWindow);

	XClearWindow(display, inputWindow);
}

#if NeedFunctionPrototypes
void MapAllWindows(Display *display)
#else
void MapAllWindows(display)
	Display *display;
#endif
{
	/* Actually make everything visible */
  	XMapWindow(display, specialWindow);
  	XMapWindow(display, timeWindow);
  	XMapWindow(display, messWindow);
	XMapWindow(display, playWindow);
	XMapWindow(display, levelWindow);
	XMapWindow(display, scoreWindow);
	XMapWindow(display, mainWindow);
	XFlush(display);
}

#if NeedFunctionPrototypes
void RedrawPlayWindow(Display *display, Window window)
#else
void RedrawPlayWindow(display, window)
	Display *display;
	Window window;
#endif
{
	/* Redraw the main playfield */
	XClearWindow(display, playWindow);
	RedrawAllBlocks(display, window);
	RedrawPaddle(display, window);
	RedrawBall(display, window);
}

#if NeedFunctionPrototypes
void FreeBackgroundPixmaps(Display *display)
#else
void FreeBackgroundPixmaps(display)
	Display *display;
#endif
{
	int i;

	/* Free all the backgound pixmaps */
    if (back1Pixmap)	XFreePixmap(display, back1Pixmap); 
	if (back2Pixmap)	XFreePixmap(display, back2Pixmap);
	if (back3Pixmap)	XFreePixmap(display, back3Pixmap); 
	if (back4Pixmap)	XFreePixmap(display, back4Pixmap);
	if (back5Pixmap)	XFreePixmap(display, back5Pixmap); 

	/* Free the icon and main background pixmaps */
	if (iconPixmap)		XFreePixmap(display, iconPixmap);
    if (mainBackPixmap)	XFreePixmap(display, mainBackPixmap); 
    if (spacePixmap)	XFreePixmap(display, spacePixmap); 

	for (i = 0; i < 6; i++)
	{
		/* Free all devil blink eyes */
		if (devilblink[i])	XFreePixmap(display, devilblink[i]); 
		if (devilblinkM[i])	XFreePixmap(display, devilblinkM[i]); 
	}
}

#if NeedFunctionPrototypes
static Window SetWMIcon(Display *display)
#else
static Window SetWMIcon(display)
	Display *display;
#endif
{
    XpmAttributes   attributes;
	Window	   		win, root;
	Colormap		iconcolormap;
	int		    	XpmErrorStatus;
							
	/* Suss out the root window */
	root = RootWindow(display, DefaultScreen(display));

	if (!(win = XCreateSimpleWindow(display, root,
		0, 0, 50, 50, 0, CopyFromParent, CopyFromParent)))
	{
		/* Well, what a bummer. Just use default icon then. */
		ErrorMessage("Cannot create icon pixmap.");
		return ((Window) NULL);
	}

	/* Create a new colourmap for the icon window */
	iconcolormap = XDefaultColormap(display, XDefaultScreen(display));

	/* Create all xpm pixmap blocks from the files */
	attributes.colormap = iconcolormap;
	attributes.valuemask = XpmColormap;
	XpmErrorStatus = XpmCreatePixmapFromData(display, win, 
		icon_xpm, &iconPixmap, NULL, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseWMIcon()");

	/* Make the new window have the new colourmap */
	XSetWindowColormap(display, win, iconcolormap);

	/* Free the background pixmap attributes */
	XpmFreeAttributes(&attributes);

	return win;
}

#if NeedFunctionPrototypes
void DrawStageBackground(Display *display, Window window, int stageType,
	int clear)
#else
void DrawStageBackground(display, window, stageType, clear)
	Display *display;
	Window window;
	int stageType;
	int clear;
#endif
{
	char type[80];

	if (debug == True)
	{
		sprintf(type, "Changing background to type %d.", stageType);
		DEBUG(type);
	}

	switch (stageType)
	{
		case BACKGROUND_SPACE:
			XSetWindowBackgroundPixmap(display, window, spacePixmap);
			break;

		case BACKGROUND_SEE_THRU:
			XSetWindowBackgroundPixmap(display, window, ParentRelative);
			break;

		case BACKGROUND_BLACK:
			XSetWindowBackground(display, window, black);
			break;

		case BACKGROUND_WHITE:
			XSetWindowBackground(display, window, white);
			break;

		case BACKGROUND_0:
			XSetWindowBackgroundPixmap(display, window, mainBackPixmap);
			break;

		case BACKGROUND_1:
			XSetWindowBackgroundPixmap(display, window, back1Pixmap);
			break;

		case BACKGROUND_2:
			XSetWindowBackgroundPixmap(display, window, back2Pixmap);
			break;

		case BACKGROUND_3:
			XSetWindowBackgroundPixmap(display, window, back3Pixmap);
			break;
																						case BACKGROUND_4:
			XSetWindowBackgroundPixmap(display, window, back4Pixmap);
			break;

		case BACKGROUND_5:
			XSetWindowBackgroundPixmap(display, window, back5Pixmap);
			break;

		default:
			ErrorMessage("Unknown background type requested.");
	}

	/* Flush the new background */
	if (clear == True)
		XClearWindow(display, window);
}

#if NeedFunctionPrototypes
void ClearDevilEyes(Display *display, Window window)
#else
void ClearDevilEyes(display, window)
	Display *display;
	Window window;
#endif
{
	/* Clear the devil eyes from current location */
	XClearArea(display, window, 
		devilx - DEVILEYE_WC, devily - DEVILEYE_HC, 
		DEVILEYE_WIDTH, DEVILEYE_HEIGHT, False);
}

#if NeedFunctionPrototypes
static void DrawTheDevilEye(Display *display, Window window, int x, int y,
    int slide)
#else
static void DrawTheDevilEye(display, window, x, y, slide)
    Display *display;
    Window window;
    int x;
    int y;
    int slide;
#endif
{
	/* Draw a frame of the devil eyes */
    RenderShape(display, window, devilblink[slide], devilblinkM[slide],
    	x - DEVILEYE_WC, y - DEVILEYE_HC, DEVILEYE_WIDTH, DEVILEYE_HEIGHT,
        False);
}

#if NeedFunctionPrototypes
int BlinkDevilEyes(Display *display, Window window)
#else
int BlinkDevilEyes(display, window)
	Display *display;
	Window window;
#endif
{
	static int slide = 0;
	static int first = True;

	if (first)
	{
		/* Setup the eyes */
		devilx = PLAY_WIDTH - DEVILEYE_WC - 5;
		devily = PLAY_HEIGHT - DEVILEYE_HC - 5;
		first = False;
		slide = 0;
	}

	/*ClearDevilEyes(display, playWindow);*/
	DrawTheDevilEye(display, playWindow, devilx, devily, blinkslides[slide]);

	slide++;
	if (slide == 26)
	{
		first = True;
		slide = 0;
		return False;
	}

	return True;
}
