#ifndef _MAIN_H_
#define _MAIN_H_

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
 * $Id: main.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/main.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: main.h,v $
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

/*
 *  Constants and macros:
 */

#define PADDLE_ANIMATE_DELAY    5
#define BONUS_SEED              2000

#define CONTROL_KEYS		0	
#define CONTROL_MOUSE		1	

#define MODE_NONE			0	
#define MODE_HIGHSCORE		1
#define MODE_INTRO			2
#define MODE_GAME			3
#define MODE_PAUSE			4
#define MODE_BALL_WAIT		5
#define MODE_WAIT			6
#define MODE_BONUS			7
#define MODE_INSTRUCT		8
#define MODE_KEYS			9
#define MODE_PRESENTS		10
#define MODE_DEMO			11
#define MODE_PREVIEW		12
#define MODE_DIALOGUE		13
#define MODE_EDIT			14
#define MODE_KEYSEDIT		15

#define FAST_SPEED         	1
#define MEDIUM_SPEED        5
#define SLOW_SPEED          9

#define MAX_TILTS			3

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

void SetGameSpeed(int delay);
int GetWarpSpeed(void);
int paddleIsMoving(void);
void SetUserSpeed(int delay);
void SetPaddleControlMode(int type);
int GetPaddleControlMode(void);
void SelectiveRedraw(Display *display);
void handlePaddleMoving(Display *display);
void SetTiltsZero(void);

extern int frame, mode, oldMode, modeSfx, gameActive;
extern time_t pausedTime;

#endif
