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
// #include <raylib.h>

// TODO: Remove functional reliance on X11
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xos.h>
// #include <xpm.h>

//TODO: Replace references to bitmaps with png textures
// #include "bitmaps/bgrnds/mnbgrnd.xpm"
// #include "bitmaps/bgrnds/space.xpm"
// #include "bitmaps/bgrnds/bgrnd.xpm"
// #include "bitmaps/bgrnds/bgrnd2.xpm"
// #include "bitmaps/bgrnds/bgrnd3.xpm"
// #include "bitmaps/bgrnds/bgrnd4.xpm"
// #include "bitmaps/bgrnds/bgrnd5.xpm"
// #include "bitmaps/icon.xpm"
// #include "bitmaps/eyes/deveyes.xpm"
// #include "bitmaps/eyes/deveyes1.xpm"
// #include "bitmaps/eyes/deveyes2.xpm"
// #include "bitmaps/eyes/deveyes3.xpm"
// #include "bitmaps/eyes/deveyes4.xpm"
// #include "bitmaps/eyes/deveyes5.xpm"

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
// Unknown texture coordinates
#define UKTC 0

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

static Window SetWMIcon(Display *display);

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
Texture2D mainBackPixmap, iconPixmap, spacePixmap;
Texture2D back1Pixmap, back2Pixmap, back3Pixmap, back4Pixmap, back5Pixmap;
Texture2D devilblink[6];
int 	devilx, devily;
int 	blinkslides[] = { 0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0, 0, 0, 
					      0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0 };


/**
 * @brief Initializes the main background texture and various other textures.
 * 
 * Creates the main background texture along with several other background textures,
 * including the devil blink animation frames.
 */
void InitialiseMainBackPixmap(){

	/* Create the playfield background textures */

	mainBackPixmap = LoadTexture("bitmaps/bgrnds/mnbgrnd.png");
	HandleXPMError(mainBackPixmap, "mainBackPixmap");

	spacePixmap = LoadTexture("bitmaps/bgrnds/space.png");
	HandleXPMError(spacePixmap, "spacePixmap");
	
	back1Pixmap = LoadTexture("bitmaps/bgrnds/bgrnd.png");
	HandleXPMError(back1Pixmap, "back1Pixmap");

	back2Pixmap = LoadTexture("bitmaps/bgrnds/bgrnd2.png");
	HandleXPMError(back2Pixmap, "back2Pixmap");

	back3Pixmap = LoadTexture("bitmaps/bgrnds/bgrnd3.png");
	HandleXPMError(back3Pixmap, "back3Pixmap");

	back4Pixmap = LoadTexture("bitmaps/bgrnds/bgrnd4.png");
	HandleXPMError(back4Pixmap, "back4Pixmap");

	back5Pixmap = LoadTexture("bitmaps/bgrnds/bgrnd5.png");
	HandleXPMError(back5Pixmap, "back5Pixmap");

	/* Devil blink animation */

	devilblink[0] = LoadTexture("bitmaps/eyes/deveyes.png");
	HandleXPMError(devilblink[0], "devilblink[0]");

	devilblink[1] = LoadTexture("bitmaps/eyes/deveyes1.png");
	HandleXPMError(devilblink[1], "devilblink[1]");

	devilblink[2] = LoadTexture("bitmaps/eyes/deveyes2.png");
	HandleXPMError(devilblink[2], "devilblink[2]");

	devilblink[3] = LoadTexture("bitmaps/eyes/deveyes3.png");
	HandleXPMError(devilblink[3], "devilblink[3]");

	devilblink[4] = LoadTexture("bitmaps/eyes/deveyes4.png");
	HandleXPMError(devilblink[4], "devilblink[4]");

	devilblink[5] = LoadTexture("bitmaps/eyes/deveyes5.png");
	HandleXPMError(devilblink[5], "devilblink[5]");
}

/**
 * @brief Clears the main game window.
 * 
 * Sets the main window's background texture to `spacePixmap` and clears it.
 *
 * @param display The X11 display connection.
 * @param window The main game window.
 */
void ClearMainWindow(Display *display, Window window){

	/* Make sure that it is drawn */
	XSetWindowBackgroundPixmap(display, mainWindow, spacePixmap);
	XClearWindow(display, mainWindow);
}

/**
 * @brief Sets window size hints for the main window.
 * 
 * Defines the minimum and maximum sizes for the main game window.
 *
 * @param display The X11 display connection.
 * @param w The width of the window.
 * @param h The height of the window.
 */
void SetWindowSizeHints(Display *display, int w, int h)
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

/**
 * @brief Creates all the windows needed for the game.
 * 
 * This function creates the main game window, the score window, the play window,
 * and other sub-windows used by the game. It also sets window manager properties.
 *
 * @param display The X11 display connection.
 * @param colormap The colormap to be used for the windows.
 * @param argv The arguments used to run the program.
 * @param argc The number of arguments.
 */
void CreateAllWindows(Display *display, Colormap colormap, char **argv, int argc){
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
		// XSetWindowBackgroundPixmap(display, iconWindow, iconPixmap); #Replaced with drawTexture
		// XClearWindow(display, iconWindow); #Replaced with clearBackground() for now
		ClearBackground(BLACK);
		DrawTexture(iconPixmap, UKTC, UKTC, WHITE); // UKTC = Unknown Texture Coordinates
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

/**
 * @brief Sets the backgrounds for the different game windows.
 * 
 * Initializes pixmaps for the background of the game windows and clears the windows.
 *
 * @param display The X11 display connection.
 * @param colormap The colormap used for the pixmaps.
 */
void SetBackgrounds(Display *display, Colormap colormap)
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

/**
 * @brief Maps all game windows to make them visible.
 * 
 * Maps all the game windows, making them visible on the screen.
 *
 * @param display The X11 display connection.
 */
void MapAllWindows(Display *display)
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

/**
 * @brief Redraws the play window.
 * 
 * Clears the play window and redraws the game components, such as blocks, paddle,
 * and ball.
 *
 * @param display The X11 display connection.
 * @param window The play window.
 */
void RedrawPlayWindow(Display *display, Window window)
{
	/* Redraw the main playfield */
	XClearWindow(display, playWindow);
	RedrawAllBlocks(display, window);
	RedrawPaddle(display, window);
	RedrawBall(display, window);
}

/**
 * @brief Frees all background pixmaps.
 * 
 * Frees all pixmaps that were allocated for backgrounds, the icon, and devil blink
 * animations.
 *
 * @param display The X11 display connection.
 */
void FreeBackgroundPixmaps(Display *display)
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
		//if (devilblinkM[i])	XFreePixmap(display, devilblinkM[i]); 
	}
}

/**
 * @brief Sets the window manager icon.
 * 
 * Creates an icon window and sets it as the window manager icon for the main game
 * window.
 *
 * @param display The X11 display connection.
 * @return The created icon window.
 */
static Window SetWMIcon(Display *display){

	Window win, root;
							
	/* Suss out the root window */
	root = RootWindow(display, DefaultScreen(display));

	if (!(win = XCreateSimpleWindow(display, root,
		0, 0, 50, 50, 0, CopyFromParent, CopyFromParent)))
	{
		/* Well, what a bummer. Just use default icon then. */
		ErrorMessage("Cannot create icon pixmap.");
		return ((Window) NULL);
	}

	/* Load icon texture */

	iconPixmap = LoadTexture("bitmaps/icon.png");
	HandleXPMError(iconPixmap, "iconPixmap");

	return win;
}

/**
 * @brief Draws the background for the current game stage.
 * 
 * Draws the appropriate background for the current stage of the game.
 *
 * @param display The X11 display connection.
 * @param window The window in which to draw the stage background.
 * @param stageType The type of stage background to draw.
 * @param clear Whether to clear the window before drawing.
 */
void DrawStageBackground(Display *display, Window window, int stageType,
	int clear)
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

/**
 * @brief Clear the devil eyes from the current location on the screen.
 * 
 * This function erases the drawn devil eyes from their previous position.
 * 
 * @param display The display connection.
 * @param window The window from which the devil eyes will be cleared.
 */
void ClearDevilEyes(Display *display, Window window)
{
	/* Clear the devil eyes from current location */
	XClearArea(display, window, 
		devilx - DEVILEYE_WC, devily - DEVILEYE_HC, 
		DEVILEYE_WIDTH, DEVILEYE_HEIGHT, False);
}

/**
 * @brief Draw a frame of the devil eyes at the specified position.
 * 
 * This function draws one frame of the devil eye animation at the given
 * coordinates, applying the specified frame (slide) of the animation.
 * 
 * @param x The x-coordinate where the devil eye will be drawn.
 * @param y The y-coordinate where the devil eye will be drawn.
 * @param slide The animation frame index of the devil eye.
 */
static void DrawTheDevilEye(int x, int y, int slide){
	
	/* Draw a frame of the devil eyes */
	DrawTexture(devilblink[slide], x - DEVILEYE_WC, y - DEVILEYE_HC, WHITE);
}

/**
 * @brief Perform the devil eye blinking animation.
 * 
 * This function animates the blinking of the devil eyes by cycling through
 * frames. The devil eyes are drawn at a fixed position on the play area.
 * 
 * @param display The display connection.
 * @param window The window where the devil eyes will be animated.
 * @return True if the animation should continue, False if it is finished.
 */
int BlinkDevilEyes(Display *display, Window window){

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
	DrawTheDevilEye(devilx, devily, blinkslides[slide]);

	slide++;
	if (slide == 26)
	{
		first = True;
		slide = 0;
		return False;
	}

	return True;
}
