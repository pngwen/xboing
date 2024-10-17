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
 *  Dependencies on other include files:
 */

#include <X11/Xlib.h>

/*
 *  Constants and macros:
 */

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */



/**
 * displays text
 *
 * @param Display *display display pointer
 * @param Window window window used to display
 * @param int x x coordinatre
 * @param int y y coordinate
 * @param XFontStruct *font font of the text
 * @param int colour text color
 * @param char *text text being displayed
 * @param int numChar the number of characters
 * @pre 
 * @return void 
 * @post displays text
 * 
 */
void 	DrawText(Display *display, Window window, int x, int y, XFontStruct *font, int colour, char *text, int numChar);


/**
 * draws text
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param int x x coordinate
 * @param int y y coordinate
 * @param XFontStruct *font font for text
 * @param int colour color of text
 * @param char *text text being displayed
 * @param int numChar number of character
 * @pre 
 * @return void 
 * @post dipalys text
 * 
 */
void 	DrawTextFast(Display *display, Window window, int x, int y, XFontStruct *font, int colour, char *text, int numChar);


/**
 * renders a line
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param int x x coordinate of one end 
 * @param int y y coordinate of one end
 * @param int x2 x coordinate of other end
 * @param int y2 y coordinate of other end
 * @param int colour color of line
 * @param int width width of line
 * @pre 
 * @return void 
 * @post draws line
 * 
 */
void 	DrawLine(Display *display, Window window, int x, int y, int x2, int y2,int colour, int width);


/**
 * renders a shape
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param Pixmap pixmap pixmap art
 * @param Pixmap mask color mask for the pixmap
 * @param int x x coordinate
 * @param int y y coordinate
 * @param int w width
 * @param int h height
 * @param int clear 
 * @pre 
 * @return void 
 * @post displays a graphic
 * 
 */
void 	RenderShape(Display *display, Window window, Pixmap pixmap,Pixmap mask, int x, int y, int w, int h, int clear);


/**
 * draws centered text with a shadow
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param XFontStruct *font font for text
 * @param char *string text being displayed
 * @param int y y coordinate of text
 * @param int colour text color
 * @param int width text window width
 * @pre 
 * @return void 
 * @post displays text in the center of the screen
 * 
 */
void 	DrawShadowCentredText(Display *display, Window window, XFontStruct *font, char *string, int y, int colour, int width);

/**
 * draws text with a shadow under it
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param XFontStruct *font text font
 * @param char *string text being displayed
 * @param int x x coordinate
 * @param int y y coordinate
 * @param int colour color
 * @pre 
 * @return void 
 * @post draws text
 * 
 */
void 	DrawShadowText(Display *display, Window window, XFontStruct *font,char *string, int x, int y, int colour);

/**
 * return color id of pixel
 *
 * @param Display *display display pointer
 * @param Colormap colormap colormap for graphic
 * @param char *colourName name of the color
 * @pre 
 * @return int color id
 * @post returns color id
 * 
 */
int 	ColourNameToPixel(Display *display, Colormap colormap, char *colourName);

/**
 * frees memory in display pointer
 *
 * @param Display *display display pointer
 * @pre 
 * @return void 
 * @post frees data
 * 
 */
void 	DrawText(Display *display, Window window, int x, int y, 
			XFontStruct *font, int colour, char *text, int numChar);
void 	DrawTextFast(Display *display, Window window, int x, int y, 
			XFontStruct *font, int colour, char *text, int numChar);
void 	DrawLine(Display *display, Window window, int x, int y, int x2, int y2,
			int colour, int width);
void 	RenderShape(Display *display, Window window, Pixmap pixmap,
			Pixmap mask, int x, int y, int w, int h, int clear);
void 	DrawShadowCentredText(Display *display, Window window, 
			XFontStruct *font, char *string, int y, int colour, int width);
void 	DrawShadowText(Display *display, Window window, XFontStruct *font,
			char *string, int x, int y, int colour);
int 	ColourNameToPixel(Display *display, Colormap colormap, 
			char *colourName);
void 	FreeMisc(Display *display);

/**
 * gets users username
 *
 * @param void 
 * @pre username and password
 * @return char return users username
 * @post user is logged in
 * 
 */
 char 	*getUsersFullName(void);

/**
 * gets users home directory
 *
 * @param void 
 * @pre username and password
 * @return char gets home directory
 * @post 
 * 
 */
char 	*GetHomeDir(void);

/**
 * resizes the window
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param int width new width
 * @param int height new height
 * @pre window is open and displaying
 * @return int bool
 * @post resizes window
 * 
 */
int 	ResizeMainWindow(Display *display, Window window, int width, int height);

/**
 * accessor for window width and height
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param int *width window width
 * @param int *height window height
 * @pre 
 * @return int no return
 * @post 
 * 
 */
int 	ObtainWindowWidthHeight(Display *display, Window window,int *width, int *height);

/**
 * controls framerate
 *
 * @param Display *display display pointer
 * @param unsigned long ms time between frames in milliseconds
 * @pre 
 * @return void 
 * @post 
 * 
 */
void 	sleepSync(Display *display, unsigned long ms);
#ifdef NEED_USLEEP

/**
 * controls framerate
 *
 * @param unsigned long usec time between frames in microseconds
 * @pre 
 * @return void 
 * @post 
 * 
 */
void 	usleep(unsigned long usec);
#endif

/**
 * gets mouse position
 *
 * @param Display *display diplay pointer
 * @param Window window display window
 * @param int *x x coordinate of mouse
 * @param int *y y coordinate of mouse
 * @pre 
 * @return int bool
 * @post 
 * 
 */
int 	ObtainMousePosition(Display *display, Window window, int *x, int *y);

/**
 * displays prompt for user to answer a yes or no question
 *
 * @param Display *display display pointer
 * @param char *message question prompt
 * @pre 
 * @return int bool
 * @post 
 * 
 */
int 	YesNoDialogue(Display *display, char *message);

/**
 * scales pixmap
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param Pixmap source pixmap source
 * @param int swidth numerator of scaling factor in the x coordinate
 * @param int sheight numerator of scaling factor in the y coordinate
 * @param int dwidth demoninator of the scaling in the x coordinate
 * @param int dheight demoninator of the scaling factor in the y coordinate
 * @pre window is opwn
 * @return Pixmap returns scaled pixmap
 * @post 
 * 
 */
Pixmap 	ScalePixmap(Display *display, Window window, Pixmap source,int swidth, int sheight, int dwidth, int dheight);

/**
 * draws a curve using 4 control points
 *
 * @param Display *display display pointer
 * @param Window window display window
 * @param XPoint *p control points
 * @param int num_steps number of steps
 * @pre 
 * @return void 
 * @post 
 * 
 */
void 	Draw4PointCurve(Display *display, Window window, XPoint *p, int num_steps);
#else
void 	Draw4PointCurve();
Pixmap 	ScalePixmap();
int 	YesNoDialogue();
int 	ObtainMousePosition();
void 	sleepSync();
#ifdef NEED_USLEEP
void 	usleep();
#endif
int 	ObtainWindowWidthHeight();
int 	ResizeMainWindow();
char 	*GetHomeDir();
char 	*getUsersFullName();
void 	FreeMisc();
int 	ColourNameToPixel();
void 	DrawText();
void 	DrawTextFast();
void 	DrawLine();
void 	RenderShape();
void 	DrawShadowCentredText();
void 	DrawShadowText();
#endif
Pixmap 	ScalePixmap(Display *display, Window window, Pixmap source,
			int swidth, int sheight, int dwidth, int dheight);
void 	Draw4PointCurve(Display *display, Window window, XPoint *p, 
			int num_steps);

#endif
