/**
 * @file stage.h
 * @author Gerardo Gonzalez
 * @date 2024-10-01
 * @brief Handles the creation and management of various in-game xboing windows.
 * 
 * This file contains functions for initializing, creating, clearing, and mapping windows in the xboing, as well as managing pixmaps and window manager hints.
 */


#ifndef _STAGE_H_
#define _STAGE_H_

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
 * $Id: stage.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/stage.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: stage.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:53  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Dependencies on other include files:
 */

// TODO: Remove instances of X11-associated data types
#include "faketypes.h"
// Remove X11 #include <X11/Xlib.h>

/*
 *  Constants and macros:
 */

#define MAIN_WIDTH  70
#define MAIN_HEIGHT 130

#define PLAY_WIDTH  495
#define PLAY_HEIGHT 580

#define TOTAL_WIDTH  (MAIN_WIDTH + PLAY_WIDTH)
#define TOTAL_HEIGHT (MAIN_HEIGHT + PLAY_HEIGHT)

#define MESS_HEIGHT 30
#define TYPE_HEIGHT (MESS_HEIGHT + 5)

#define BACKGROUND_WHITE   -2
#define BACKGROUND_BLACK   -1
#define BACKGROUND_0    	0
#define BACKGROUND_1    	1
#define BACKGROUND_2    	2
#define BACKGROUND_3    	3
#define BACKGROUND_4    	4
#define BACKGROUND_5    	5
#define BACKGROUND_SEE_THRU 10
#define BACKGROUND_SPACE 	11

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

extern Window mainWindow;
extern Window scoreWindow;
extern Window levelWindow;
extern Window playWindow, bufferWindow;
extern Window messWindow;
extern Window specialWindow;
extern Window timeWindow;
extern Window inputWindow;
extern Window blockWindow, typeWindow;

/**
 * Creates all the game windows (score, level, gameplay, etc)
 *
 * @param Display *display display connection to x server
 * @param Colormap colormap map of colors used ingame
 *
 */
void CreateAllWindows(Display *display, Colormap colormap, char **argv, 
	int argc);

/**
 * Redraws play area
 *
 * @param Display *display Display connection to x server
 * @param Window window The window itself
 * 
 */
void RedrawPlayWindow(Display *display, Window window);

/**
 * Makes the windows visible
 *
 * @param Display *display Display connection to x server
 * 
 */
void MapAllWindows(Display *display);

/**
 * Clears the main window and returns it to its blank form
 *
 * @param Display *display Display connection to x server
 * @param Window window The window itself
 * 
 */
void ClearMainWindow(Display *display, Window window);

/**
 * Actually draws the background to whatever specification
 *
 * @param Display *display Display connection to x server
 * @param Window window The window itself
 * @param int stageType What color/type of background (used in switch case)
 * 
 */
void DrawStageBackground(Display *display, Window window, int stageType,
	int clear);

/**
 * Sets/preps the backgrounds for the game
 *
 * @param Display *display Display connection to x server
 * @param Colormap colormap Map of colors used for the game
 * 
 */
void SetBackgrounds(Display *display, Colormap colormap);

/**
 * Makes the devil eyes blink
 *
 * @param Display *display Display connection to x server
 * @param Window window The window itself
 * 
 */
int BlinkDevilEyes(Display *display, Window window);

/**
 * Sets the window size
 *
 * @param Display *display Display connection to x server
 * @param int w Window width
 * @param int h Window height
 * 
 */
void SetWindowSizeHints(Display *display, int w, int h);


#endif
