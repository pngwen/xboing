#ifndef _PADDLE_H_
#define _PADDLE_H_

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
 * $Id: paddle.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/paddle.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: paddle.h,v $
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

#define PADDLE_LEFT		1
#define PADDLE_SHOOT	2
#define PADDLE_RIGHT	3
#define PADDLE_NONE		0

#define PADDLE_SMALL	4
#define PADDLE_MEDIUM	5
#define PADDLE_HUGE		6

#define DIST_BASE   	30
#define PADDLE_VEL  	10

#define PADDLE_HC  		4
#define PADDLE_HEIGHT 	9

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void InitialisePaddle(Display *display, Window window, Colormap colormap);
void FreePaddle(Display *display);
void DrawPaddle(Display *display, Window window, int x, int y, int size);
void MovePaddle(Display *display, Window window, int direction, 
	int size, int xpos);
void ResetPaddleStart(Display *display, Window window);
int GetPaddleSize(void);
void RedrawPaddle(Display *display, Window window);
void FlushPaddleBackingStore(Display *display, Window window);
void ToggleReverse(Display *display);
void SetReverseOff(void);
void ChangePaddleSize(Display *display, Window window, int type);
#else
void ChangePaddleSize();
void SetReverseOff();
void ToggleReverse();
void InitialisePaddle();
void FreePaddle();
void DrawPaddle();
void MovePaddle();
void ResetPaddleStart();
int GetPaddleSize();
void RedrawPaddle();
void FlushPaddleBackingStore();
#endif

extern int currentPaddleSize, paddlePos, reverseOn, stickyOn;

#endif
