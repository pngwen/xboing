#ifndef _MISC_H_
#define _MISC_H_

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
 * $Id: misc.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/misc.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: misc.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:53  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Dependencies on other includes files:
 */

// TODO: Remove instances of X11-associated data types
#include "faketypes.h"

// #include <X11/Xlib.h>

/*
 *  Function prototypes:
 */

// Removed definition and call of empty function "FreeMisc()"

/**
 * @brief Draws text (multi-layer, smoother)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param x The x-coordinate of the text
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 * @param numChar The number of characters of "string" to draw
 */
void 	DrawText(Display *display, Window window, int x, int y, 
			XFontStruct *font, int colour, char *text, int numChar);

/**
 * @brief Draws text (single layer)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param x The x-coordinate of the text
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 * @param numChar The number of characters of "string" to draw
 */
void 	DrawTextFast(Display *display, Window window, int x, int y, 
			XFontStruct *font, int colour, char *text, int numChar);

/**
 * @brief Draws a formatted line between 2 points using X11 library calls
 * 
 * @param display The display of the current X11 window
 * @param window  The X11 window to draw on
 * @param x The x-coordinate of the first point of the line
 * @param y The y-coordinate of the first point of the line
 * @param x2 The x-coordinate of the second point of the line
 * @param y2 The y-coordinate of the second point of the line
 * @param colour The color of the line
 * @param width The width of the line (in px)
 */
void 	DrawLine(Display *display, Window window, int x, int y, int x2, int y2,
			int colour, int width);

/**
 * @brief Draws a pixmap (and can also clear background)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param pixmap The pixmap to draw
 * @param mask The clip-mask of the pixmap
 * @param x The x-coordinate of the pixmap
 * @param y The y-coordinate of the pixmap
 * @param w The width of the pixmap
 * @param h The height of the pixmap
 * @param clear Clear the background before drawing?
 */
void 	RenderShape(Display *display, Window window, Pixmap pixmap,
			Pixmap mask, int x, int y, int w, int h, int clear);

/**
 * @brief Draws text centered with a 2px offset shadow
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 * @param width The width of the area to draw the centered text in
 */
void 	DrawShadowCentredText(Display *display, Window window, 
			XFontStruct *font, char *string, int y, int colour, int width);

/**
 * @brief Draws text with a 2px offset shadow
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param x The x-coordinate of the text
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 */
void 	DrawShadowText(Display *display, Window window, XFontStruct *font,
			char *string, int x, int y, int colour);

/**
 * @brief Obtains color data from X11 library (Used for initialization)
 * 
 * @param window The X11 window to draw on
 * @param pixmap The pixmap to draw
 * @param colourName The name of the color to find color data for
 * @return int The color-data of the given color name
 */
int 	ColourNameToPixel(Display *display, Colormap colormap, 
			char *colourName);

/**
 * @brief Parses the user's name from their password file
 * 
 * @return char* The user's name 
 */
char 	*getUsersFullName(void);

/**
 * @brief Returns the path of the home directory
 * 
 * @return char* The path of the Home directory
 */
char 	*GetHomeDir(void);

/**
 * @brief Resizes the main window (returns the success of resize)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param width The width of the window
 * @param height The height of the window
 * @return int Whether the window could be resized or not. (0/1)
 */
int 	ResizeMainWindow(Display *display, Window window, int width, 
			int height);

/**
 * @brief Obtains the current window size & propogates it (width&height) to parameter variables.
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param width The variable to propogate the window width to
 * @param height The variable to propogate the window height to
 * @return int (nothing)
 * 
 * @todo Add return values for function or make return type void
 */
int 	ObtainWindowWidthHeight(Display *display, Window window,
			int *width, int *height);

/**
 * @brief Stops program execution (usleep()) if needed to meet target framerate.
 * 
 * @param display The display of the current X11 window
 * @param ms The number of miliseconds to wait between each frame
 */
void 	sleepSync(Display *display, unsigned long ms);

#ifdef NEED_USLEEP
/**
 * @brief Halts program execution for a specified number of microseconds
 * 
 * @param usec The number of microseconds to sleep
 */
void 	usleep(unsigned long usec);
#endif

/**
 * @brief Updates parameter varibles with mouse position (set to 0 if cursor is off-window)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param x The variable to propogate the mouse x-position to
 * @param y The variable to propogate the mouse y-position to
 * @return int Whether or not the mouse is on the window
 */
int 	ObtainMousePosition(Display *display, Window window, int *x, int *y);

/**
 * @brief Prints a message to the user and prompts them with y/n. (Returns user answer)
 * 
 * @param display The display of the current X11 window
 * @param message The message to prompt the user with
 * @return int The user's response
 */
int 	YesNoDialogue(Display *display, char *message);

/**
 * @brief Creates a scaled copy of the source Pixmap
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param source The source pixmap
 * @param swidth The width of the source pixmap
 * @param sheight The height of the source pixmap
 * @param dwidth The width of the output pixmap
 * @param dheight The height of the output pixmap
 * @return Pixmap The new scaled-copy of the source pixmap
 */
Pixmap 	ScalePixmap(Display *display, Window window, Pixmap source,
			int swidth, int sheight, int dwidth, int dheight);

/**
 * @brief Generates a list of points for a 4-point curve and draws lines connecting them
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param p The array of points of the 4-point curve
 * @param num_steps The number of steps of the curve (more steps = higher quality)
 */
void 	Draw4PointCurve(Display *display, Window window, XPoint *p, 
			int num_steps);

#endif
