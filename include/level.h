#ifndef _LEVEL_H_
#define _LEVEL_H_

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
 * $Id: level.h,v 1.1.1.1 1994/12/16 01:36:52 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/level.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:52 $
 *
 * $Log: level.h,v $
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

//#include <X11/Xlib.h>
#include <time.h>
#include "faketypes.h"

/*
 *  Constants and macros:
 */

#define SHOTS_TO_KILL_SPECIAL   3
#define BUF_SIZE 				1024
#define MAX_NUM_LEVELS  		80

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

void InitialiseLevelInfo(Display *display, Window window, Colormap colormap);
void FreeLevelInfo(Display *display);
void DisplayLevelInfo(Display *display, Window window, u_long level);
void CheckGameRules(Display *display, Window window);
void DeadBall(Display *display, Window window);
void DeleteABullet(Display *display);
void AddABullet(Display *display);
void ReDrawBulletsLeft(Display *display);
void RedrawLevelInfo(Display *display, Window window);
void SetLevelNumber(int levelNum);
void SetStartingLevel(int levelNum);
int GetStartingLevel(void);
char *GetLevelName(void);
void DecLevelTimeBonus(Display *display, Window window);
void SetLevelTimeBonus(Display *display, Window window, int seconds);
int GetLevelTimeBonus(void);
void UpdateHighScores(Display *display);
void AddExtraLife(Display *display);
void EndTheGame(Display *display, Window window);
void HandleGameTimer(Display *display, Window window);
void DecExtraLife(Display *display);
int GetNumberLife(void);
void AddToLevelTimeBonus(Display *display, Window window, int seconds);
void ChangeStartingLevel(Display *display);
void SetLivesLeft(int new);

extern int bonus, livesLeft, bonusBlock;
extern time_t gameTime;
extern u_long level;
extern char levelTitle[BUF_SIZE];


#endif
