#ifndef _INIT_H_
#define _INIT_H_

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
 * $Id: init.h,v 1.1.1.1 1994/12/16 01:36:52 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/init.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:52 $
 *
 * $Log: init.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:52  jck
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

#define DEBUG(x)    if (debug == True) NormalMessage(x);

#define CURSOR_WAIT		1
#define CURSOR_PLUS		2
#define CURSOR_NONE		3
#define CURSOR_POINT	4
#define CURSOR_SKULL	5

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

Display *InitialiseGame(char **argv, int argc);
void ShutDown(Display *display, int exitCode, char *message);
void GrabPointer(Display *display, Window window);
void UnGrabPointer(Display *display);
void ChangePointer(Display *display, Window window, int cursorState);

extern GC gccopy, gc, gcxor, gcand, gcor, gcsfx;
extern XFontStruct *titleFont, *copyFont, *textFont, *dataFont;
extern int red, tann, yellow, green, white, black, blue, purple;
extern int greens[7], reds[7];
extern Colormap colormap;
extern int noSound, debug, noicon;

#endif
