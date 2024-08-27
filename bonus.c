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
 * $Id: bonus.c,v 1.1.1.1 1994/12/16 01:36:44 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/bonus.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:44 $
 *
 * $Log: bonus.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:44  jck
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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <xpm.h>

#include "bitmaps/presents/titleSml.xpm"

#include "error.h"
#include "audio.h"
#include "highscore.h"
#include "misc.h"
#include "special.h"
#include "main.h"
#include "gun.h"
#include "init.h"
#include "file.h"
#include "dialogue.h"
#include "stage.h"
#include "blocks.h"
#include "sfx.h"
#include "ball.h"
#include "score.h"
#include "paddle.h"
#include "level.h"
#include "mess.h"
#include "intro.h"

#include "bonus.h"

/*
 *  Internal macro definitions:
 */

#define GAP					30
#define KEY_TYPE_VOL		50
#define LINE_DELAY			100
#define SAVE_LEVEL			5

#define BONUS_COIN_SCORE	3000
#define SUPER_BONUS_SCORE	50000
#define BULLET_SCORE		500
#define LEVEL_SCORE			100
#define TIME_BONUS			100

#define BORDER_LEFT			55
#define BORDER_RIGHT		((PLAY_WIDTH + MAIN_WIDTH) - 50)
#define BORDER_TOP			73
#define BORDER_BOTTOM		((PLAY_HEIGHT + MAIN_HEIGHT) - 85)

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
static void DoBullets(Display *display, Window window);
static void DoTimeBonus(Display *display, Window window);
#else
static void DoTimeBonus();
static void DoBullets();
#endif

/*
 *  Internal variable declarations:
 */

static int 		numBonus;
enum BonusStates 	BonusState;
static Pixmap 	titlePixmap, titlePixmapM;
static int 		ypos;
static int 		waitingFrame;
enum BonusStates		waitMode;
static char 	string[80];
static u_long 	bonusScore;
static int 		firstTime = True;


#if NeedFunctionPrototypes
void SetUpBonus(Display *display, Window window, Colormap colormap)
#else
void SetUpBonus(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
{
	XpmAttributes   attributes;
	int             XpmErrorStatus;

	attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

	/* Create the small title pixmap */
	XpmErrorStatus = XpmCreatePixmapFromData(display, window, titleSmall_xpm,
		&titlePixmap, &titlePixmapM, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBonus()");

    /* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);
}

#if NeedFunctionPrototypes
void DecNumberBonus(void)
#else
void DecNumberBonus()
#endif
{
	/* bump down the number of bonuses. sic */
	numBonus--;
}

#if NeedFunctionPrototypes
void IncNumberBonus(void)
#else
void IncNumberBonus()
#endif
{
	/* bump up the number of bonuses. sic */
	numBonus++;
}

#if NeedFunctionPrototypes
int GetNumberBonus(void)
#else
int GetNumberBonus()
#endif
{
	/* Umm - return the number of bonuses */
	return numBonus;
}

#if NeedFunctionPrototypes
void ResetNumberBonus(void)
#else
void ResetNumberBonus()
#endif
{
	/* No more bonuses thanks */
	numBonus = 0;
}

#if NeedFunctionPrototypes
void DrawBallBorder(Display *display, Window window)
#else
void DrawBallBorder(display, window)
	Display *display;
	Window window;
#endif
{
	int x, y;
	static int slide = 0;

	DEBUG("Drawing ball borders.")

	/* Draw a row of balls along the top and bottom of screen */
	for (x = BORDER_LEFT; x < BORDER_RIGHT; x += 22)
	{
		DrawTheBall(display, window, x, BORDER_TOP, slide);
		DrawTheBall(display, window, x, BORDER_BOTTOM, slide);

		/* Next frame of ball animation */
		slide++;

		/* Wrap around animation */
		if (slide == BALL_SLIDES) slide = 0;
	}

	/* Draw a row of balls along both sides of the screen */
	for (y = BORDER_TOP; y < BORDER_BOTTOM; y += 22)
	{
		DrawTheBall(display, window, BORDER_LEFT, y, slide);
		DrawTheBall(display, window, BORDER_RIGHT, y, slide);

		/* Next frame of ball animation */
		slide++;

		/* Wrap around animation */
		if (slide == BALL_SLIDES) slide = 0;
	}

}

#if NeedFunctionPrototypes
void DrawSmallIntroTitle(Display *display, Window window, int x, int y)
#else
void DrawSmallIntroTitle(display, window, x, y)
	Display *display;
	Window window;
	int x;
	int y;
#endif
{
	DEBUG("Drawing small intro title.")

	/* Draw the small title pixmap */
	RenderShape(display, window, titlePixmap, titlePixmapM,
		x - SMALL_TITLE_WC, y - SMALL_TITLE_HC, 
		SMALL_TITLE_WIDTH, SMALL_TITLE_HEIGHT, False);
}

#if NeedFunctionPrototypes
void SetupBonusScreen(Display *display, Window window)
#else
void SetupBonusScreen(display, window)
	Display *display;
	Window window;
#endif
{
	/* Clear the background again */
	ClearMainWindow(display, window);

	/* Draw the rectangular border of balls */
	DrawBallBorder(display, window);

	/* Draw the main title pixmap */
	DrawSmallIntroTitle(display, window, TOTAL_WIDTH / 2, 120);

	/* The new state will be the text */
	ResetBonus();

	/* Remove the window to show bonus stuff */
	while (WindowFadeEffect(display, playWindow, PLAY_WIDTH, PLAY_HEIGHT));
	XUnmapWindow(display, playWindow);
	XFlush(display);
}

#if NeedFunctionPrototypes
void DrawTitleText(Display *display, Window window)
#else
void DrawTitleText(display, window)
	Display *display;
	Window window;
#endif
{
	SetCurrentMessage(display, messWindow, "- Bonus Tally -", True);
	
	/* Indicate which level the bonus is for */
	sprintf(string, "- Level %ld -", level);
	DrawShadowCentredText(display, window, titleFont, 
		string, ypos, red, TOTAL_WIDTH);
	ypos += (titleFont->ascent + GAP);

	strcpy(string, "Press space for next level");
	DrawShadowCentredText(display, window, textFont, 
		string, PLAY_HEIGHT - 12, tann, TOTAL_WIDTH);

    /* Adjust the level so that the starting level is taken into account */
    if ((((int) (level - (u_long) GetStartingLevel() + 1L)) % SAVE_LEVEL) == 0)
	{
		/* Toggle the saving of levels */
		ToggleSaving(display, True);
		DrawSpecials(display);

		RenderShape(display, window,
        	floppy, floppyM, TOTAL_WIDTH - 100, PLAY_HEIGHT, 32, 32, False);

		DEBUG("Saving activated.")
	}

	XFlush(display);

	SetBonusWait(BONUS_SCORE, frame + 5);
	SetGameSpeed(SLOW_SPEED);

	DEBUG("set bonus mode to BONUS_SCORE.")
}

#if NeedFunctionPrototypes
static void DoScore(Display *display, Window window)
#else
static void DoScore(display, window)
	Display *display;
	Window window;
#endif
{
	DEBUG("in function DoScore() in bonus.c")

	SetGameSpeed(SLOW_SPEED);

	/* Nice message rewarding you for your efforts */
	strcpy(string, "Congratulations on finishing this level.");
	DrawShadowCentredText(display, window, textFont, 
		string, ypos, white, TOTAL_WIDTH);
	XFlush(display);

	ypos += (35 + GAP);
	SetBonusWait(BONUS_BONUS, frame + LINE_DELAY);
	SetGameSpeed(SLOW_SPEED);
	DEBUG("set bonus mode to BONUS_BONUS.")
}

#if NeedFunctionPrototypes
static void DoBonuses(Display *display, Window window)
#else
static void DoBonuses(display, window)
	Display *display;
	Window window;
#endif
{
	int x, plen, secs;
	static int maxLen;

	/* Get the number of seconds left on the clock */
	secs = GetLevelTimeBonus();

	if (secs == 0)
	{
		/* Play the sound for the super bonus */
		if (noSound == False) playSoundFile("Doh4", 80);

		strcpy(string, "Bonus coins void - Timer ran out!");
		DrawShadowCentredText(display, window, textFont, 
			string, ypos, blue, TOTAL_WIDTH);

		SetGameSpeed(SLOW_SPEED);

		/* Now skip to the next sequence */
		SetBonusWait(BONUS_LEVEL, frame + LINE_DELAY);
		firstTime = True;
		ypos += (textFont->ascent + GAP * 2);
		return;
	}

	if (firstTime)
	{
		/* Set up the bonus coin sequence */
		firstTime = False;

		if (numBonus == 0)
		{
			/* Play the sound for the super bonus */
			if (noSound == False) playSoundFile("Doh1", 80);

			/* No bonus coins - so tell user */
			strcpy(string, "Sorry, no bonus coins collected.");
			DrawShadowCentredText(display, window, textFont, 
				string, ypos, blue, TOTAL_WIDTH);

			SetGameSpeed(SLOW_SPEED);

			/* Now skip to the next sequence */
			SetBonusWait(BONUS_LEVEL, frame + LINE_DELAY);
			firstTime = True;
			ypos += (textFont->ascent + GAP * 2);
			return;
		}

		if (numBonus > MAX_BONUS)
		{
			/* Play the sound for the super bonus */
			if (noSound == False) playSoundFile("supbons", 80);

			/* More than 10 coins collected - super bonus reward */
			sprintf(string, "Super Bonus - %ld", 
				ComputeScore(SUPER_BONUS_SCORE));
			DrawShadowCentredText(display, window, titleFont, 
					string, ypos, blue, TOTAL_WIDTH);

			/* Update the score with more points */
			bonusScore += ComputeScore(SUPER_BONUS_SCORE);
			DisplayScore(display, scoreWindow, bonusScore);
	
			SetGameSpeed(SLOW_SPEED);

			/* Now skip to the next sequence */
			SetBonusWait(BONUS_LEVEL, frame + LINE_DELAY);
			firstTime = True;
			ypos += (textFont->ascent + GAP * 2);
			return;
		}

		/* Calculate where to draw these coins centred */
		maxLen = ((numBonus * 27) + (10 * numBonus) + 5);
	}

	/* Find out where the next bonus coin will go next */
	plen = ((numBonus * 27) + (10 * numBonus));
	x = (((PLAY_WIDTH + MAIN_WIDTH) / 2) + (maxLen / 2)) - plen;

	/* Draw the bonus coin shape */
	DrawTheBlock(display, window, x, ypos, BONUS_BLK, 0, 0, 0);

	/* Play the sound for the bonus */
	if (noSound == False) 
		playSoundFile("bonus", 50);

	/* Increment the score by the value of the bonus */
	bonusScore += ComputeScore(BONUS_COIN_SCORE);
	DisplayScore(display, scoreWindow, bonusScore);

	/* Reduce number of bonuses */
	DecNumberBonus();

	if (numBonus <= 0)
	{
		/* Set up bonus state for next sequence */
		SetBonusWait(BONUS_LEVEL, frame + LINE_DELAY);
		DEBUG("set bonus mode to BONUS_LEVEL.")

		/* Make sure bonus is reset */
		ResetNumberBonus();

		ypos += (int) (textFont->ascent + GAP * 1.5);
		firstTime = True;

		SetGameSpeed(SLOW_SPEED);
	}
}

#if NeedFunctionPrototypes
static void DoLevel(Display *display, Window window)
#else
static void DoLevel(display, window)
	Display *display;
	Window window;
#endif
{
	int secs, theLevel;

	SetGameSpeed(SLOW_SPEED);

	/* Get the number of seconds left on the clock */
	secs = GetLevelTimeBonus();

	if (secs > 0)
	{
    	/* Adjust the level so that the starting level is taken into account */
    	theLevel = (int) level - GetStartingLevel() + 1;

		/* Draw level bonus text */
		sprintf(string, "Level bonus - level %d x %ld = %ld points", 
			theLevel, ComputeScore(LEVEL_SCORE), 
			theLevel * ComputeScore(LEVEL_SCORE));
		DrawShadowCentredText(display, window, textFont, 
			string, ypos, yellow, TOTAL_WIDTH);

		/* Increment the score by the value of the level bonus */
		bonusScore += ComputeScore((LEVEL_SCORE * theLevel));
		DisplayScore(display, scoreWindow, bonusScore);
	}
	else
	{
		strcpy(string, "No level bonus - Timer ran out.");
		DrawShadowCentredText(display, window, textFont, 
			string, ypos, yellow, TOTAL_WIDTH);

		/* Play the sound for the super bonus */
		if (noSound == False) playSoundFile("Doh2", 80);
	}

	/* Next section setup */
	ypos += (int) (textFont->ascent + GAP * 1.5);
	SetBonusWait(BONUS_BULLET, frame + LINE_DELAY);

	DEBUG("set bonus mode to BONUS_BULLET.")
}

#if NeedFunctionPrototypes
static void DoBullets(Display *display, Window window)
#else
static void DoBullets(display, window)
	Display *display;
	Window window;
#endif
{
	int x, plen;
	static int maxLen;

	if (firstTime)
	{
		/* For the first time setup the bullet bonus seq. */
		firstTime = False;
		SetUnlimitedBullets(False);

		if (GetNumberBullets() == 0)
		{
			/* No bullets - say so */
			strcpy(string, "You have used all your bullets. No bonus!");
			DrawShadowCentredText(display, window, textFont, 
				string, ypos, blue, TOTAL_WIDTH);

			/* Play the sound for the super bonus */
			if (noSound == False) playSoundFile("Doh3", 80);
			
			SetGameSpeed(SLOW_SPEED);

			/* Get ready for the next sequence */
			SetBonusWait(BONUS_TIME, frame + LINE_DELAY);
			firstTime = True;
			ypos += (textFont->ascent + GAP/2);
			return;
		}

		/* Position where the first bullet will be drawn */
		maxLen = ((GetNumberBullets() * 7) + (3 * GetNumberBullets()));
	}

	/* Find out where the next bonus bullet will go next */
	plen = ((GetNumberBullets() * 7) + (3 * GetNumberBullets()));
	x = (((PLAY_WIDTH + MAIN_WIDTH) / 2) + (maxLen / 2)) - plen;

	DrawTheBullet(display, window, x, ypos);

	/* Play the sound for the bullets */
	if (noSound == False) 
		playSoundFile("key", 50);

	/* Increment the score by the value of the bullet bonus */
	bonusScore += ComputeScore(BULLET_SCORE);
	DisplayScore(display, scoreWindow, bonusScore);

	/* Ummm. Draw a bullet */
	DeleteABullet(display);	

	if (GetNumberBullets() == 0)
	{
		/* Reset bullets and get ready for next sequence */
		SetBonusWait(BONUS_TIME, frame + LINE_DELAY);
		ypos += (textFont->ascent + GAP/2);
		firstTime = True;
		SetGameSpeed(SLOW_SPEED);
	}
}

#if NeedFunctionPrototypes
static void DoTimeBonus(Display *display, Window window)
#else
static void DoTimeBonus(display, window)
	Display *display;
	Window window;
#endif
{
	int secs = 0;

	SetGameSpeed(SLOW_SPEED);

	/* Get the number of seconds left on the clock */
	secs = GetLevelTimeBonus();

	if (secs > 0)
	{
		/* Draw time bonus text */
		sprintf(string, "Time bonus - %d seconds x %ld = %ld points", 
			secs, ComputeScore(TIME_BONUS), secs * ComputeScore(TIME_BONUS));
		DrawShadowCentredText(display, window, textFont, 
			string, ypos, yellow, TOTAL_WIDTH);

		/* Increment the score by the value of the time bonus */
		bonusScore += ComputeScore(TIME_BONUS * secs);
		DisplayScore(display, scoreWindow, bonusScore);
	}
	else
	{
		/* Draw no time bonus text */
		strcpy(string, "No time bonus - not quick enough!");
		DrawShadowCentredText(display, window, textFont, 
			string, ypos, yellow, TOTAL_WIDTH);

		/* Play the sound for the super bonus */
		if (noSound == False) playSoundFile("Doh4", 80);
	}

	/* Next section setup */
	ypos += (textFont->ascent + GAP/2);
	SetBonusWait(BONUS_HSCORE, frame + LINE_DELAY);
}

#if NeedFunctionPrototypes
static void DoHighScore(Display *display, Window window)
#else
static void DoHighScore(display, window)
	Display *display;
	Window window;
#endif
{
	int myrank = 0;
	char str[5];

	SetGameSpeed(SLOW_SPEED);

	/* Obtain current ranking for this score */
	myrank = GetHighScoreRanking(score);

	if (myrank > 0)
	{
		/* Special case for first place */
		if (myrank == 1)
			sprintf(string, "You are ranked 1st. Well done!");
		else
		{
			/* Add the correct grammer for the sentence */
			switch (myrank)
			{
				case 1: strcpy(str, "st"); break;
				case 2: strcpy(str, "nd"); break;
				case 3: strcpy(str, "rd"); break;

				case 4: case 5: case 6: case 7: case 8: case 9: case 10: 
					strcpy(str, "th"); 
					break;
				
				default: strcpy(str, ""); break;
			}

			/* Construct beautiful sentence */
			sprintf(string, "You are currently ranked %d%s.", 
				myrank, str);
		}
	}
	else
		/* What a loser ;-) */
		strcpy(string, "Keep on trying!");

	/* Draw the text for the game ranking */
	DrawShadowCentredText(display, window, textFont, 
		string, ypos, red, TOTAL_WIDTH);
	ypos += (textFont->ascent + GAP/2);

	SetBonusWait(BONUS_END_TEXT, frame + LINE_DELAY);
}

#if NeedFunctionPrototypes
static void DoEndText(Display *display, Window window)
#else
static void DoEndText(display, window)
	Display *display;
	Window window;
#endif
{
	DEBUG("DoEndText in bonus screen.")

	SetGameSpeed(SLOW_SPEED);

	/* Finishing sentence - so you know what level to do */
	sprintf(string, "Prepare for level %ld", level+1);
	DrawShadowCentredText(display, window, textFont, 
		string, ypos, yellow, TOTAL_WIDTH);
	XFlush(display);

	if (noSound == False) playSoundFile("applause", 80);

	SetBonusWait(BONUS_FINISH, frame + LINE_DELAY * 2);
}

#if NeedFunctionPrototypes
static void DoFinish(Display *display, Window window)
#else
static void DoFinish(display, window)
	Display *display;
	Window window;
#endif
{
	DEBUG("DoFinish in bonus screen.")

	/* Setup game window for the next level */
	level++;
	SetupStage(display, playWindow);

	BonusState = BONUS_TEXT;
	SetGameSpeed(FAST_SPEED);

	mode = MODE_GAME;

	XSetWindowBorder(display, playWindow, red);
	XMapWindow(display, playWindow);

	/* Only redraw if the server hasn't backing store on */
	if (DoesBackingStore(XDefaultScreenOfDisplay(display)) != Always)
		SelectiveRedraw(display);
}

#if NeedFunctionPrototypes
void SetBonusWait(enum BonusStates newMode, int waitFrame)
#else
void SetBonusWait(newMode, waitFrame)
	enum BonusStates newMode;
	int waitFrame;
#endif
{
	waitingFrame = waitFrame;
	waitMode = newMode;
	BonusState = BONUS_WAIT;
}

#if NeedFunctionPrototypes
void DoBonusWait(void)
#else
void DoBonusWait()
#endif
{
	/* Wait for the frame we want to come along - then change modes */
	if (frame == waitingFrame)
		BonusState = waitMode;
}

#if NeedFunctionPrototypes
void DoBonus(Display *display, Window window)
#else
void DoBonus(display, window)
	Display *display;
	Window window;
#endif
{
	/* The states within the bonus mode */
	switch (BonusState)
	{
		case BONUS_TEXT:
			DEBUG("BONUS: DrawTitleText()")
			DrawTitleText(display, window);
			break;

		case BONUS_SCORE:
			DEBUG("BONUS: DoScore()")
			DoScore(display, window);
			break;

		case BONUS_BONUS:
			DEBUG("BONUS: DoBonuses()")
			DoBonuses(display, window);
			break;

		case BONUS_LEVEL:
			DEBUG("BONUS: DoLevel()")
			DoLevel(display, window);
			break;

		case BONUS_BULLET:
			DEBUG("BONUS: DoBullets()")
			DoBullets(display, window);
			break;

		case BONUS_TIME:
			DEBUG("BONUS: DoTimeBonus()")
			DoTimeBonus(display, window);
			break;

		case BONUS_HSCORE:
			DEBUG("BONUS: DoHighScore()")
			DoHighScore(display, window);
			break;

		case BONUS_END_TEXT:
			DEBUG("BONUS: DoEndText()")
			DoEndText(display, window);
			break;

		case BONUS_FINISH:
			DEBUG("BONUS: DoFinish()")
			DoFinish(display, window);
			break;

		case BONUS_WAIT:
			DoBonusWait();
			break;

		default:
			break;
	}
}

#if NeedFunctionPrototypes
void RedrawBonus(Display *display, Window window)
#else
void RedrawBonus(display, window)
	Display *display;
	Window window;
#endif
{
	/* This will redraw the entire screen */

	/* Took this out as some people were cheating!!! */
}

#if NeedFunctionPrototypes
void FreeBonus(Display *display)
#else
void FreeBonus(display)
	Display *display;
#endif
{
	/* Free all the hungry memory leaks */
	if (titlePixmap)	XFreePixmap(display, titlePixmap);
	if (titlePixmapM)	XFreePixmap(display, titlePixmapM);
}

#if NeedFunctionPrototypes
void ComputeAndAddBonusScore(void)
#else
void ComputeAndAddBonusScore()
#endif
{
	/* This function will pre compute the bonus score and add it to the
	 * score. The bonus screen will only change the score by updating the
	 * score display and not the score value. This means the user can hit
	 * space and go to the next screen without delay. Cool.
	 */
	int secs = 0;
	int theLevel = 0;

	DEBUG("computing and adding bonus score.")

	secs = GetLevelTimeBonus();
	if (secs > 0)
	{
		/* Compute bonus coin bonus */
		if (numBonus > MAX_BONUS)
		{
			/* More than MAX_BONUS bonus so give super bonus */
			AddToScore((u_long) SUPER_BONUS_SCORE);
		}
		else
		{
			/* Less than MAX_BONUS so work out bonus score */
			AddToScore((u_long) (numBonus * BONUS_COIN_SCORE));
		}

   		/* Adjust the level so that the starting level is taken into account */
   		theLevel = (int) level - GetStartingLevel() + 1;

		/* Increment the score by the value of the level bonus */
		AddToScore((u_long) (LEVEL_SCORE * theLevel));
	}

	/* Increment the score by the value of the bullet bonus */
	if (GetNumberBullets() != 0)
		AddToScore((u_long) (GetNumberBullets() * BULLET_SCORE));

    /* Get the number of seconds left on the clock */
    secs = GetLevelTimeBonus();
    if (secs > 0)
    {
        /* Increment the score by the value of the time bonus */
        AddToScore((u_long) (TIME_BONUS * secs));
	}

	DEBUG("finished computing and adding bonus score.")
}

#if NeedFunctionPrototypes
void ResetBonus(void)
#else
void ResetBonus()
#endif
{
	DEBUG("Reseting bonus screen.")

	/* Setup for bonus screen from start */
	BonusState = BONUS_TEXT;
	firstTime = True;
	bonusScore = score;
	ComputeAndAddBonusScore();
	ypos = 180;
	SetGameSpeed(FAST_SPEED);
}
