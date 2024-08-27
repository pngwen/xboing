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
 * $Id: level.c,v 1.1.1.1 1994/12/16 01:36:47 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/level.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:47 $
 *
 * $Log: level.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:47  jck
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
#include <time.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <xpm.h>

#include "bitmaps/balls/life.xpm"

#include "error.h"
#include "audio.h"
#include "dialogue.h"
#include "special.h"
#include "intro.h"
#include "gun.h"
#include "init.h"
#include "stage.h"
#include "sfx.h"
#include "score.h"
#include "paddle.h"
#include "blocks.h"
#include "bonus.h"
#include "highscore.h"
#include "ball.h"
#include "main.h"
#include "mess.h"
#include "misc.h"
#include "file.h"

#include "level.h"

/*
 *  Internal macro definitions:
 */

#define MAX_LIVES			6
#define START_LIVES			3
#define NEW_LIVE_SCORE_INC	100000L

#define MIN(S, T) ((S) > (T) ? (T) : (S))

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
static void DrawLevelTimeBonus(Display *display, Window window, int timebonus);
#else
static void DrawLevelTimeBonus();
#endif

/*
 *  Internal variable declarations:
 */

Pixmap		lifePixmap, lifeMask;	
int 		livesLeft = 3;
u_long		level;
u_long		startlevel;
time_t		gameTime;
int			bonus = 1;
char 		levelTitle[BUF_SIZE];
int 		bonusBlock = False;
static int 	bulletPos;
static int 	timeBonus;

#if NeedFunctionPrototypes
void InitialiseLevelInfo(Display *display, Window window, Colormap colormap)
#else
void InitialiseLevelInfo(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	XpmAttributes   attributes;
	int 			XpmErrorStatus;

	attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

	/* Create xpm pixmap for the life */
	XpmErrorStatus = XpmCreatePixmapFromData(display, window, life_xpm, 
		&lifePixmap, &lifeMask, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseLevelInfo()");

	/* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);
}

#if NeedFunctionPrototypes
void DecLevelTimeBonus(Display *display, Window window)
#else
void DecLevelTimeBonus(display, window)
	Display *display;
	Window window;
#endif
{
	if (timeBonus > 0) 
	{
		/* Decrement the time bonus */
		timeBonus--;	

		/* Draw the time bonus */
		DrawLevelTimeBonus(display, window, timeBonus);

		/* Is the time run out */
		if (timeBonus == 0)
		{
			/* Times up buzzer sound */
			if (noSound == False) playSoundFile("buzzer", 70);
		}
	}
}

#if NeedFunctionPrototypes
int GetLevelTimeBonus(void)
#else
int GetLevelTimeBonus()
#endif
{
	/* return the time bonus */
	return timeBonus;	
}

#if NeedFunctionPrototypes
void AddToLevelTimeBonus(Display *display, Window window, int seconds)
#else
void AddToLevelTimeBonus(display, window, seconds)
	Display *display; 
	Window window;
	int seconds;
#endif
{
	/* add to the time bonus */
	SetLevelTimeBonus(display, window, timeBonus + seconds);
}

#if NeedFunctionPrototypes
void SetLevelTimeBonus(Display *display, Window window, int seconds)
#else
void SetLevelTimeBonus(display, window, seconds)
	Display *display;
	Window window;
	int seconds;
#endif
{
	assert(timeBonus >= 0);

	/* Reset the time bonus to desired time */
	timeBonus = seconds;	

	/* Draw the time bonus */
	DrawLevelTimeBonus(display, window, timeBonus);
}

#if NeedFunctionPrototypes
static void DrawLevelTimeBonus(Display *display, Window window, int timebonus)
#else
static void DrawLevelTimeBonus(display, window, timebonus)
	Display *display;
	Window window;
	int timebonus;
#endif
{
	int len, minutes, seconds;
	char str[10];

	/* Work out the minutes and seconds using time bonus */
	minutes = timebonus / 60;
	seconds = timebonus % 60;

	/* Construct a string with the time bonus and draw it */
	sprintf(str, "%02d:%02d", minutes, seconds);
	len = strlen(str);

	/* Draw the text now thanks  - using title font for big numbers */
	XClearWindow(display, window);
	DrawText(display, window, 2, 7, titleFont, black, str, len);

	/* The less time you have the more drastic the colour comes */
	if (timebonus <= 10)
		DrawText(display, window, 0, 5, titleFont, red, str, len);
	else if (timebonus <= 60)
		DrawText(display, window, 0, 5, titleFont, yellow, str, len);
	else 
		DrawText(display, window, 0, 5, titleFont, green, str, len);
}

#if NeedFunctionPrototypes
void DrawLife(Display *display, Window window, int x, int y)
#else
void DrawLife(display, window, x, y)
	Display *display;
	Window window;
	int x;
	int y;
#endif
{
	/* Draw the life pixmap */
	RenderShape(display, window, lifePixmap, lifeMask, 
		x-12, y-12, 25, 24, True);
}

#if NeedFunctionPrototypes
void DisplayLevelNumber(Display *display, Window window, u_long level)
#else
void DisplayLevelNumber(display, window, level)
	Display *display;
	Window window;
	u_long level;
#endif
{
	/* Put the level number up */
	DrawOutNumber(display, levelWindow, level, 260, 5);
}

#if NeedFunctionPrototypes
void DisplayLevelInfo(Display *display, Window window, u_long level)
#else
void DisplayLevelInfo(display, window, level)
	Display *display;
	Window window;
	u_long level;
#endif
{
	int i;

	/* Clear the window for level information */
	XClearWindow(display, levelWindow);

	DisplayLevelNumber(display, levelWindow, level);

	/* Draw out the lives left pixmaps - only MAX_LIVES will be displayed */
	for (i = 0; i < MIN(livesLeft, MAX_LIVES); i++)
		DrawLife(display, window, 175 - (i * 30), 21);

	/* Draw the score in the score window */
	DisplayScore(display, scoreWindow, score);

	/* Draw all the bullets in the ammo pouch ;-) */
	ReDrawBulletsLeft(display);

	XFlush(display);
}

#if NeedFunctionPrototypes
void SetLevelNumber(int levelNum)
#else
void SetLevelNumber(levelNum)
	int levelNum;
#endif
{
	level = (u_long) levelNum;
}

#if NeedFunctionPrototypes
void SetStartingLevel(int levelNum)
#else
void SetStartingLevel(levelNum)
	int levelNum;
#endif
{
	startlevel = (u_long) levelNum;
}

#if NeedFunctionPrototypes
void ChangeStartingLevel(Display *display)
#else
void ChangeStartingLevel(display)
	Display *display;
#endif
{
	/*
	 * This function will display the user input dialogue and ask the
	 * user for a starting level. It is validated here and if valid it
	 * will set the new starting level.
	 */
	char str[80];
	int num;


    sprintf(str, "Level range is [1-%d]", MAX_NUM_LEVELS);
	SetCurrentMessage(display, messWindow, str, False);

	str[0] = '\0';

	/* Obtain a string from the user. Should contain NULL or numbers */
	strcpy(str, UserInputDialogueMessage(display,
    	"Input game starting level number.", TEXT_ICON, NUMERIC_ENTRY_ONLY));

	/* Nothing input so just return */
	if (str[0] == '\0') return;

    /* Obtain the start level setting */
    num = atoi(str);
    if ((num > 0) && (num <= MAX_NUM_LEVELS))
	{
		/* Ok now set the new starting level number */
    	SetStartingLevel(num);
		sprintf(str, "Starting level set to %d", num);
		SetCurrentMessage(display, messWindow, str, True);
	}
	else
	{
		/* Value out of range so let them know the range. */
        sprintf(str, "Invalid - level range [1-%d]", MAX_NUM_LEVELS);
		SetCurrentMessage(display, messWindow, str, True);
	}
}

#if NeedFunctionPrototypes
int GetStartingLevel(void)
#else
int GetStartingLevel()
#endif
{
	return ((int) startlevel);
}

#if NeedFunctionPrototypes
void RedrawLevelInfo(Display *display, Window window)
#else
void RedrawLevelInfo(display, window)
	Display *display;
	Window	window;
#endif
{
	DisplayLevelInfo(display, window, level);
}

#if NeedFunctionPrototypes
void FreeLevelInfo(Display *display)
#else
void FreeLevelInfo(display)
	Display *display;
#endif
{
	/* Free the life pixmap  */
	if (lifePixmap)		XFreePixmap(display, lifePixmap);
	if (lifeMask)		XFreePixmap(display, lifeMask);
}

#if NeedFunctionPrototypes
void DeleteABullet(Display *display)
#else
void DeleteABullet(display)
	Display *display;
#endif
{
	bulletPos = 192 - (GetNumberBullets() * 9);

	/* Take a bullet away from ammo belt */
	EraseTheBullet(display, levelWindow, bulletPos, 43);

	DecNumberBullets();
}

#if NeedFunctionPrototypes
void AddABullet(Display *display)
#else
void AddABullet(display)
	Display *display;
#endif
{
	IncNumberBullets();

	bulletPos = 192 - (GetNumberBullets() * 9);

	/* Add a bullet to the ammo belt */
	DrawTheBullet(display, levelWindow, bulletPos, 43);
}

#if NeedFunctionPrototypes
void ReDrawBulletsLeft(Display *display)
#else
void ReDrawBulletsLeft(display)
	Display *display;
#endif
{
	int x, i;

	/* Draw the bullets in the ammo belt */
	for (i = 0; i < GetNumberBullets(); i++)
	{
		x = 192 - ((i+1) * 9);
		DrawTheBullet(display, levelWindow, x, 43);
	} 
}

#if NeedFunctionPrototypes
int GetNumberLife(void)
#else
int GetNumberLife()
#endif
{
	return livesLeft;
}

#if NeedFunctionPrototypes
void SetLivesLeft(int new)
#else
void SetLivesLeft(new)
	int new;
#endif
{
	livesLeft = new;
}

#if NeedFunctionPrototypes
void DecExtraLife(Display *display)
#else
void DecExtraLife(display)
	Display *display;
#endif
{
	/* Take a life */
	if (mode != MODE_EDIT)
		livesLeft--;

	if (livesLeft < 0) 
		livesLeft = 0;

	/* redraw the level info */
	DisplayLevelInfo(display, levelWindow, level);
}

#if NeedFunctionPrototypes
void AddExtraLife(Display *display)
#else
void AddExtraLife(display)
	Display *display;
#endif
{
	/* Add a new life */
	livesLeft++;

	SetCurrentMessage(display, messWindow, "Extra ball", True);

	/* redraw the level info */
	DisplayLevelInfo(display, levelWindow, level);
}

#if NeedFunctionPrototypes
void CheckAndAddExtraLife(Display *display, long score)
#else
void CheckAndAddExtraLife(display, score)
	Display *display;
	long score;
#endif
{
	static int ballInc = 0;

	/* Add a new life? */
	if ((score) && ((score / NEW_LIVE_SCORE_INC) != ballInc)) 
	{
		/* Add a new life */
		AddExtraLife(display);
	}

	/* Next inc before adding a new life */
	ballInc = score / NEW_LIVE_SCORE_INC;
}

#if NeedFunctionPrototypes
void HandleGameTimer(Display *display, Window window)
#else
void HandleGameTimer(display, window)
	Display *display;
	Window window;
#endif
{
	static time_t oldTime = 0;

	/* Time to decrement the timer */
	if (time(NULL) > oldTime)
	{
		/* Decrement the timer bonus */
		DecLevelTimeBonus(display, timeWindow);
		oldTime = time(NULL);
	}
}

#if NeedFunctionPrototypes
void CheckGameRules(Display *display, Window window)
#else
void CheckGameRules(display, window)
	Display *display;
	Window window;
#endif
{
	CheckAndAddExtraLife(display, score);

	HandleGameTimer(display, window);

	if (StillActiveBlocks() == False)
	{
		/* Turn off the x2 x4 bonuses so bonus screen is x2 or x4 */
		Togglex2Bonus(display, False);
		Togglex4Bonus(display, False);
		DrawSpecials(display);

		/* Give the play a big head with some applause */
		if (noSound == False) playSoundFile("applause", 70);

		/* Finished level now so set up bonus screen */
        mode = MODE_BONUS;
		SetupBonusScreen(display, mainWindow);
	}
}

#if NeedFunctionPrototypes
void UpdateHighScores(Display *display)
#else
void UpdateHighScores(display)
	Display *display;
#endif
{
	time_t endTime;
	u_long theLevel;
	char message[80];

	/* Obtain the game duration in seconds - taking account for pauses */
	endTime = time(NULL) - gameTime - pausedTime;

	/* Adjust the level so that the starting level is taken into account */
	theLevel = level - (u_long) GetStartingLevel() + 1L;

	/* If the user is the new boing master */
	if (GetHighScoreRanking(score) == 1)
	{
		if (noSound == False) playSoundFile("youagod", 99);

		/* Ask them for some words of wisdom */
		strcpy(message, UserInputDialogueMessage(display,
        	"Words of wisdom Boing Master?", TEXT_ICON, TEXT_ENTRY_ONLY));
	}

	/* Update the high score table */
	(void) CheckAndAddScoreToHighScore(score, theLevel, endTime, PERSONAL,
		message);
	if (CheckAndAddScoreToHighScore(score, theLevel, endTime, GLOBAL, 
	  message) == False)
		ResetHighScore(PERSONAL);
	else
		ResetHighScore(GLOBAL);
}


#if NeedFunctionPrototypes
void EndTheGame(Display *display, Window window)
#else
void EndTheGame(display, window)
	Display *display;
	Window window;
#endif
{
	/* Game over man! */
	SetCurrentMessage(display, messWindow, "- Game Over - ", True);

	if (noSound == False) playSoundFile("game_over", 99);

	TurnSpecialsOff(display);

	UpdateHighScores(display);

	/* redraw the level info */
	DisplayLevelInfo(display, levelWindow, level);

	/* Reset game and setup for high score table */
	gameActive = False;
	SetLevelNumber(GetStartingLevel());
	ResetIntroduction();
	mode = MODE_HIGHSCORE;

}

#if NeedFunctionPrototypes
void DeadBall(Display *display, Window window)
#else
void DeadBall(display, window)
	Display *display;
	Window window;
#endif
{
	if (noSound == False) playSoundFile("balllost", 99);

	/* More than one ball on screen - 1 died */
	SetCurrentMessage(display, messWindow, "Another one bites the dust!", True);

	if (livesLeft <= 0 && GetAnActiveBall() == -1)
		EndTheGame(display, window);
	else 
	{
		/* Start a new ball if some to spare */
		if (GetAnActiveBall() == -1 && livesLeft > 0)
		{
			/* Last ball on screen died so start a new one */
			SetCurrentMessage(display, messWindow, "Balls Terminated!", True);

			SetReverseOff();
			DrawSpecials(display);

			/* Make the paddle the maximum size */
			ChangePaddleSize(display, window, PAD_EXPAND_BLK);
			ChangePaddleSize(display, window, PAD_EXPAND_BLK);

			/* Decrement the number of lives left and display so */
			DecExtraLife(display);

			ResetBallStart(display, window);
		}
	}
}

#if NeedFunctionPrototypes
char *GetLevelName(void)
#else
char *GetLevelName()
#endif
{
	/* Return the name of the current level */
	return (levelTitle);
}
