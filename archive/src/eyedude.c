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
 * $Id: eyedude.c,v 1.1.1.1 1994/12/16 01:36:43 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/eyedude.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:43 $
 *
 * $Log: eyedude.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:43  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Include file dependencies:
 */

/*
// TODO: Remove instances of X11-associated data types
#include <xpm.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <X11/Xos.h>

//TODO: Replace references to bitmaps with png textures
#include "bitmaps/eyes/guyl1.png"
#include "bitmaps/eyes/guyl2.png"
#include "bitmaps/eyes/guyl3.png"
#include "bitmaps/eyes/guyl4.png"
#include "bitmaps/eyes/guyl5.png"
#include "bitmaps/eyes/guyl6.png"
#include "bitmaps/eyes/guyr1.png"
#include "bitmaps/eyes/guyr2.png"
#include "bitmaps/eyes/guyr3.png"
#include "bitmaps/eyes/guyr4.png"
#include "bitmaps/eyes/guyr5.png"
#include "bitmaps/eyes/guyr6.png"
#include "bitmaps/eyes/guydead.png"
*/

#include "include/error.h"
#include "include/audio.h"
#include "include/score.h"
#include "include/init.h"
#include "include/mess.h"
#include "include/main.h"
#include "include/stage.h"
#include "include/blocks.h"
#include "include/paddle.h"
#include "include/misc.h"
#include "include/level.h"
#include "include/ball.h"
#include "include/special.h"
#include "include/eyedude.h"
#include "include/faketypes.h"

/*
 *  Internal macro definitions:
 */

#define X2COL(col, x) 		(col = x / colWidth)
#define Y2ROW(row, y) 		(row = y / rowHeight)

/*
 *  Internal type declarations:
 */

static int CheckEyeDudeClearPath(Display *display, Window window);

/*
 *  Internal variable declarations:
 */

static Pixmap eyesLeft[6], eyesLeftM[6];
static Pixmap eyesRight[6], eyesRightM[6];
static Pixmap eyesDead, eyesDeadM;
static int	x, y, oldx, oldy, s, direction, inc, turn;
static eyeDudeStates eyeDudeState;

void InitialiseEyeDudes(Display *display, Window window, Colormap colormap)
{
    XpmAttributes   attributes;
	int		    XpmErrorStatus;

    attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_l1_xpm,
		&eyesLeft[0], &eyesLeftM[0], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye L1)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_l2_xpm,
		&eyesLeft[1], &eyesLeftM[1], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye L2)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_l3_xpm,
		&eyesLeft[2], &eyesLeftM[2], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye L3)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_l4_xpm,
		&eyesLeft[3], &eyesLeftM[3], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye L4)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_l5_xpm,
		&eyesLeft[4], &eyesLeftM[4], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye L5)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_l6_xpm,
		&eyesLeft[5], &eyesLeftM[5], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye L6)");


	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_r1_xpm,
		&eyesRight[0], &eyesRightM[0], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye R1)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_r2_xpm,
		&eyesRight[1], &eyesRightM[1], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye R2)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_r3_xpm,
		&eyesRight[2], &eyesRightM[2], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye R3)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_r4_xpm,
		&eyesRight[3], &eyesRightM[3], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye R4)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_r5_xpm,
		&eyesRight[4], &eyesRightM[4], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye R5)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_r6_xpm,
		&eyesRight[5], &eyesRightM[5], &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye R6)");

	XpmErrorStatus = XpmCreatePixmapFromData(display, window, eyeguy_dead_xpm,
		&eyesDead, &eyesDeadM, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(eye dead)");

	XpmFreeAttributes(&attributes);
}

void FreeEyeDudes(Display *display)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		/* Free all the pixmaps */
		if (eyesLeft[i])	XFreePixmap(display, eyesLeft[i]);
 		if (eyesLeftM[i]) 	XFreePixmap(display, eyesLeftM[i]);
		if (eyesRight[i])	XFreePixmap(display, eyesRight[i]);
 		if (eyesRightM[i]) 	XFreePixmap(display, eyesRightM[i]);
	}

	if (eyesDead)	XFreePixmap(display, eyesDead);
	if (eyesDeadM)	XFreePixmap(display, eyesDeadM);
}

static void DrawTheEyeDude(Display *display, Window window, int x, int y, 
	int slide, int direction)
{
	/* Draw the eyedude pixmap into the window */
	switch (direction)
	{
		case WALK_LEFT: 
    		RenderShape(display, window, eyesLeft[slide], eyesLeftM[slide],
				x - EYEDUDE_WC, y - EYEDUDE_HC, EYEDUDE_WIDTH, EYEDUDE_HEIGHT, 
				False);
			break;

		case WALK_RIGHT: 
    		RenderShape(display, window, eyesRight[slide], eyesRightM[slide],
				x - EYEDUDE_WC, y - EYEDUDE_HC, EYEDUDE_WIDTH, EYEDUDE_HEIGHT, 
				False);
			break;

		case WALK_DEAD: 
    		RenderShape(display, window, eyesDead, eyesDeadM,
				x - EYEDUDE_WC, y - EYEDUDE_HC, EYEDUDE_WIDTH, EYEDUDE_HEIGHT, 
				False);
			break;
	}
}

static void EraseTheEyeDude(Display *display, Window window, int x, int y)
{
	/* Erase the eye dude pixmap from the window */
    XClearArea(display, window, x - EYEDUDE_WC, y - EYEDUDE_HC, 
		EYEDUDE_WIDTH, EYEDUDE_HEIGHT, False);
}

int CheckBallEyeDudeCollision(Display *display, Window window, int j)
{
    int ballX, ballY;

    GetBallPosition(&ballX, &ballY, j);

    /* Check if any part of the bullets coords is inside the balls box */
    if (((x + EYEDUDE_WC) >= (ballX - BALL_WC)) &&
        ((x - EYEDUDE_WC) <= (ballX + BALL_WC)) &&
        ((y + EYEDUDE_HC) >= (ballY - BALL_HC)) &&
        ((y - EYEDUDE_HC) <= (ballY + BALL_HC)))
        return True;
    else
        return False;
}

static void ResetEyeDude(Display *display, Window window)
{
	s = 0;
	turn = False;

	/* Check if the dude has a clear path then walk */
	if (CheckEyeDudeClearPath(display, window) == False)
	{
		/* Ok - so I'll go walking some other time */
		ChangeEyeDudeMode(EYEDUDE_NONE);
		return;
	}

	if ((rand() % 100) < 30)
		turn = True;

	/* Setup initial positions based on direction given */
	switch (rand() % 2)
	{
		case 1: /* Walk left */
			x = oldx = PLAY_WIDTH + EYEDUDE_WC;
			y = oldy = EYEDUDE_HC;
			direction = WALK_LEFT;
			break;

		case 0:	/* Walk right */
			x = oldx = -EYEDUDE_WC;
			y = oldy = EYEDUDE_HC;
			direction = WALK_RIGHT;
			break;

		default:
			WarningMessage("Incorrect eyedude direction.");
			break;
	}

	ChangeEyeDudeMode(EYEDUDE_WALK);

	if (noSound == False) playSoundFile("hithere", 100);
}

static int CheckEyeDudeClearPath(Display *display, Window window)
{
	int col;

	/* Check the top row for occupied blocks */
	for (col = 0; col < MAX_COL; col++)
	{
		/* Check if the block is occupied and if so then return false */
		if (blocks[0][col].occupied == True)
			return False;
	}

	/* Path clear for takeoff */
	return True;
}

void GetEyeDudePosition(int *x, int *y)
{
	*x = oldx;
	*y = oldy;
}

static void HandleEyeDudeWalk(Display *display, Window window)
{
	/* Update the eyedude that may be moving */
	if ((frame % EYEDUDE_FRAME_RATE) == 0)
	{
		/* Erase and draw our new dude */
		EraseTheEyeDude(display, window, oldx, oldy);
		DrawTheEyeDude(display, window, x, y, s, direction);
		oldx = x; oldy = y;

		/* Update the frame of animation for dude */
		s++;
		if (s == 6) s = 0;

		switch (direction)
		{
			case WALK_LEFT:
				if ((x <= (PLAY_WIDTH / 2)) && turn)
				{
					/* Turn the other way now */
					direction = WALK_RIGHT;	
					turn = False;
					break;
				}

				if (x < -EYEDUDE_WIDTH)
					ChangeEyeDudeMode(EYEDUDE_NONE);
				inc = -5;
				break;

			case WALK_RIGHT:
				if ((x >= (PLAY_WIDTH / 2)) && turn)
				{
					/* Turn the other way now */
					direction = WALK_LEFT;	
					turn = False;
					break;
				}

				if (x > (PLAY_WIDTH + EYEDUDE_WC))
					ChangeEyeDudeMode(EYEDUDE_NONE);
				inc = 5;
				break;
		}

		/* Move our little dude along */
		x += inc;
	}
}

void HandleEyeDudeMode(Display *display, Window window)
{
	switch (getEyeDudeMode())
	{
		case EYEDUDE_RESET:
			ResetEyeDude(display, window);
			break;

		case EYEDUDE_WALK:
			HandleEyeDudeWalk(display, window);
			break;

		case EYEDUDE_DIE:
			EraseTheEyeDude(display, window, oldx, oldy);
			ChangeEyeDudeMode(EYEDUDE_NONE);

            SetCurrentMessage(display, messWindow,
            	"- Eye Dude Bonus 10000 -", False);

            /* Add the bonus to the score */
            AddToScore((u_long) EYEDUDE_HIT_BONUS);
            DisplayScore(display, scoreWindow, score);

            if (noSound == False) playSoundFile("supbons", 80);
			break;

		case EYEDUDE_TURN:
		case EYEDUDE_NONE:
		case EYEDUDE_WAIT:
			break;
	}
}

void ChangeEyeDudeMode(eyeDudeStates state)
{
	/* Maybe start our little eyedude dude on his way! */
	eyeDudeState = state;
}

eyeDudeStates getEyeDudeMode(void)
{
	return eyeDudeState;
}
