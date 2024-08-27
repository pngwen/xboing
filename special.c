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
 * $Id: special.c,v 1.1.1.1 1994/12/16 01:36:48 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/special.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:48 $
 *
 * $Log: special.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:48  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Include file dependencies:
 */

#include <stdlib.h>
#include <X11/Xos.h>

#include "init.h"
#include "misc.h"
#include "stage.h"
#include "level.h"
#include "score.h"
#include "paddle.h"

#include "special.h"

/*
 *  Internal macro definitions:
 */

#define GAP			5

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

int saving;
int stickyBat;
int fastGun;
int noWalls;
int Killer;
int x2Bonus;
int x4Bonus;

#if NeedFunctionPrototypes
void TurnSpecialsOff(Display *display)
#else
void TurnSpecialsOff(display)
	Display *display;
#endif
{
	/* Turn all specials off */

	ToggleFastGun(display, False);
	ToggleWallsOn(display, False);
	/*ToggleSaving(display, False);*/
	ToggleStickyBat(display, False);
	Togglex2Bonus(display, False);
	Togglex4Bonus(display, False);
	ToggleKiller(display, False);
}

#if NeedFunctionPrototypes
void Togglex2Bonus(Display *display, int state)
#else
void Togglex2Bonus(display, state)
	Display *display;
	int state;
#endif
{
	/* Set the x2Bonus flag to true or false */
	x2Bonus = state;
}

#if NeedFunctionPrototypes
void ToggleKiller(Display *display, int state)
#else
void ToggleKiller(display, state)
	Display *display;
	int state;
#endif
{
	/* Set the Killer flag to true or false */
	Killer = state;
}

#if NeedFunctionPrototypes
void Togglex4Bonus(Display *display, int state)
#else
void Togglex4Bonus(display, state)
	Display *display;
	int state;
#endif
{
	/* Set the x4Bonus flag to true or false */
	x4Bonus = state;
}

#if NeedFunctionPrototypes
void ToggleStickyBat(Display *display, int state)
#else
void ToggleStickyBat(display, state)
	Display *display;
	int state;
#endif
{
	/* Set the stickyBat flag to true or false */
	stickyBat = state;
}

#if NeedFunctionPrototypes
void ToggleSaving(Display *display, int state)
#else
void ToggleSaving(display, state)
	Display *display;
	int state;
#endif
{
	/* Set the Saving flag to true or false */
	saving = state;
}

#if NeedFunctionPrototypes
void ToggleFastGun(Display *display, int state)
#else
void ToggleFastGun(display, state)
	Display *display;
	int state;
#endif
{
	/* Set the fastgun flag to true or false */
	fastGun = state;
}

#if NeedFunctionPrototypes
void ToggleWallsOn(Display *display, int state)
#else
void ToggleWallsOn(display, state)
	Display *display;
	int state;
#endif
{
	/* Set the noWalls flag to true or false */
	noWalls = state;

	if (noWalls == True)
	{
		/* Turn the wall colour to green */
		XSetWindowBorder(display, playWindow, green);
	}
	else
	{
		/* Turn the wall colour to red */
		XSetWindowBorder(display, playWindow, red);
	}
}

#if NeedFunctionPrototypes
void DrawSpecials(Display *display)
#else
void DrawSpecials(display)
	Display *display;
#endif
{
	int y = 3;
	int x = 5;
	char string[80];

	/* Clear the special window */
	/*XClearWindow(display, specialWindow);*/

	strcpy(string, "Reverse");
	if (reverseOn == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);

	y += copyFont->ascent + GAP;

	strcpy(string, "Sticky");
	if (stickyBat == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);

	x = 55; y = 3;

	strcpy(string, "Save");
	if (saving == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);

	y += copyFont->ascent + GAP;

	strcpy(string, "FastGun");
	if (fastGun == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);

	x = 110; y = 3;

	strcpy(string, "NoWall");
	if (noWalls == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);

	y += copyFont->ascent + GAP;

	strcpy(string, "Killer");
	if (Killer == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);

	x = 155; y = 3;

	strcpy(string, "x2");
	if (x2Bonus == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);

	y += copyFont->ascent + GAP;

	strcpy(string, "x4");
	if (x4Bonus == True)
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, yellow);
	else
		DrawShadowText(display, specialWindow, copyFont, 
			string, x, y, white);
}


#if NeedFunctionPrototypes
void RandomDrawSpecials(Display *display)
#else
void RandomDrawSpecials(display)
	Display *display;
#endif
{
	int n;
	static u_long temp_score = 0;

	/* Randomly change the specials to flash on and off */
	n = rand() % 100;
	saving = n > 50 ? True : False;
	n = rand() % 100;
	stickyBat = n > 50 ? True : False;
	n = rand() % 100;
	Killer = n > 50 ? True : False;
	n = rand() % 100;
	x2Bonus = n > 50 ? True : False;
	n = rand() % 100;
	x4Bonus = n > 50 ? True : False;
	n = rand() % 100;
	fastGun = n > 50 ? True : False;
	n = rand() % 100;
	noWalls = n > 50 ? True : False;
	n = rand() % 100;
	reverseOn = n > 50 ? True : False;

	/* Make the score and level number change every now and then too */
	DisplayLevelNumber(display, levelWindow, (rand() % MAX_NUM_LEVELS) + 1);
	DisplayScore(display, scoreWindow, (u_long) temp_score++);

	DrawSpecials(display);
}
