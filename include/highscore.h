#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

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
 * $Id: highscore.h,v 1.1.1.1 1994/12/16 01:36:51 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/highscore.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:51 $
 *
 * $Log: highscore.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:51  jck
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
#include <sys/time.h>
#include <sys/types.h>

/*
 *  Constants and macros:
 */

#define PERSONAL 		1
#define GLOBAL 			2

#define SCORE_VERSION	2

/*
 *  Type declarations:
 */

enum HighScoreStates 
{ 
	HIGHSCORE_TITLE, 
	HIGHSCORE_SHOW, 
	HIGHSCORE_WAIT, 
	HIGHSCORE_SPARKLE, 
	HIGHSCORE_FINISH
};

typedef struct 
{
	u_long 	version;			/* Highscore File version */
	char 	masterText[80];		/* Boing Masters words of wisdom */
} highScoreHeader;

typedef struct 
{
	u_long 	score;		/* Score */
	u_long 	level;		/* delta Level reached */
	time_t 	gameTime;	/* Time taken to complete game */
	time_t 	time;		/* Date when played */
	char 	name[40];	/* Full user name */
	uid_t	userId;		/* Real user id of player */
} highScoreEntry;

/*
 *  Function prototypes:
 */

#if NeedFunctionPrototypes
void SetUpHighScore(Display *display, Window window, Colormap colormap);
void HighScore(Display *display, Window window);
void RedrawHighScore(Display *display, Window window);
void FreeHighScore(Display *display);
void ResetHighScore(int type);
int ReadHighScoreTable(int type);
int WriteHighScoreTable(int type);
int CheckAndAddScoreToHighScore(u_long score, u_long level, time_t gameTime, 
	int type, char *message);
int GetHighScoreRanking(u_long score);
void CommandlineHighscorePrint(void);
void SetNickName(char *nick);
char *GetNickName(void);
void SetBoingMasterText(char *message);
#else
void SetBoingMasterText();
char *GetNickName();
void SetNickName();
void CommandlineHighscorePrint();
void SetUpHighScore();
void HighScore();
void RedrawHighScore();
void FreeHighScore();
void ResetHighScore();
int ReadHighScoreTable();
int WriteHighScoreTable();
int CheckAndAddScoreToHighScore();
int GetHighScoreRanking();
#endif

extern enum HighScoreStates HighScoreState;

#endif
