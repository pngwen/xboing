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

#if NeedFunctionPrototypes
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
char 	*getUsersFullName(void);
char 	*GetHomeDir(void);
int 	ResizeMainWindow(Display *display, Window window, int width, 
			int height);
int 	ObtainWindowWidthHeight(Display *display, Window window,
			int *width, int *height);
void 	sleepSync(Display *display, unsigned long ms);
#ifdef NEED_USLEEP
void 	usleep(unsigned long usec);
#endif
int 	ObtainMousePosition(Display *display, Window window, int *x, int *y);
int 	YesNoDialogue(Display *display, char *message);
Pixmap 	ScalePixmap(Display *display, Window window, Pixmap source,
			int swidth, int sheight, int dwidth, int dheight);
void 	Draw4PointCurve(Display *display, Window window, XPoint *p, 
			int num_steps);
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

#endif
