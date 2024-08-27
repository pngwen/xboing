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

#include <xpm.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <X11/Xos.h>

#include "bitmaps/eyes/guyl1.xpm"
#include "bitmaps/eyes/guyl2.xpm"
#include "bitmaps/eyes/guyl3.xpm"
#include "bitmaps/eyes/guyl4.xpm"
#include "bitmaps/eyes/guyl5.xpm"
#include "bitmaps/eyes/guyl6.xpm"
#include "bitmaps/eyes/guyr1.xpm"
#include "bitmaps/eyes/guyr2.xpm"
#include "bitmaps/eyes/guyr3.xpm"
#include "bitmaps/eyes/guyr4.xpm"
#include "bitmaps/eyes/guyr5.xpm"
#include "bitmaps/eyes/guyr6.xpm"
#include "bitmaps/eyes/guydead.xpm"

#include "error.h"
#include "audio.h"
#include "score.h"
#include "init.h"
#include "mess.h"
#include "main.h"
#include "stage.h"
#include "blocks.h"
#include "paddle.h"
#include "misc.h"
#include "level.h"
#include "ball.h"
#include "special.h"

#include "eyedude.h"

/*
 *  Internal macro definitions:
 */

#define X2COL(col, x) 		(col = x / colWidth)
#define Y2ROW(row, y) 		(row = y / rowHeight)

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
static int CheckEyeDudeClearPath(Display *display, Window window);
#else
static int CheckEyeDudeClearPath();
#endif

/*
 *  Internal variable declarations:
 */

static Pixmap eyesLeft[6], eyesLeftM[6];
static Pixmap eyesRight[6], eyesRightM[6];
static Pixmap eyesDead, eyesDeadM;
static int	x, y, oldx, oldy, s, direction, inc, turn;
static eyeDudeStates eyeDudeState;

#if NeedFunctionPrototypes
void InitialiseEyeDudes(Display *display, Window window, Colormap colormap)
#else
void InitialiseEyeDudes(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
#endif
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

#if NeedFunctionPrototypes
void FreeEyeDudes(Display *display)
#else
void FreeEyeDudes(display)
	Display *display;
#endif
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

#if NeedFunctionPrototypes
static void DrawTheEyeDude(Display *display, Window window, int x, int y, 
	int slide, int direction)
#else
static void DrawTheEyeDude(display, window, x, y, slide, direction)
	Display *display;
	Window window;
	int x;
	int y;
	int slide;
	int direction;
#endif
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

#if NeedFunctionPrototypes
static void EraseTheEyeDude(Display *display, Window window, int x, int y)
#else
static void EraseTheEyeDude(display, window, x, y)
	Display *display;
	Window window;
	int x;
	int y;
#endif
{
	/* Erase the eye dude pixmap from the window */
    XClearArea(display, window, x - EYEDUDE_WC, y - EYEDUDE_HC, 
		EYEDUDE_WIDTH, EYEDUDE_HEIGHT, False);
}

#if NeedFunctionPrototypes
int CheckBallEyeDudeCollision(Display *display, Window window, int j)
#else
int CheckBallEyeDudeCollision(display, window, j)
    Display *display;
    Window window;
    int j;
#endif
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

#if NeedFunctionPrototypes
static void ResetEyeDude(Display *display, Window window)
#else
static void ResetEyeDude(display, window)
	Display *display;
	Window window;
#endif
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

#if NeedFunctionPrototypes
static int CheckEyeDudeClearPath(Display *display, Window window)
#else
static int CheckEyeDudeClearPath(display, window)
	Display *display;
	Window window;
#endif
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

#if NeedFunctionPrototypes
void GetEyeDudePosition(int *x, int *y)
#else
void GetEyeDudePosition(x, y)
	int *x;
	int *y;
#endif
{
	*x = oldx;
	*y = oldy;
}

#if NeedFunctionPrototypes
static void HandleEyeDudeWalk(Display *display, Window window)
#else
static void HandleEyeDudeWalk(display, window)
	Display *display;
	Window window;
#endif
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

#if NeedFunctionPrototypes
void HandleEyeDudeMode(Display *display, Window window)
#else
void HandleEyeDudeMode(display, window)
	Display *display;
	Window window;
#endif
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

#if NeedFunctionPrototypes
void ChangeEyeDudeMode(eyeDudeStates state)
#else
void ChangeEyeDudeMode(state)
	eyeDudeStates state;
#endif
{
	/* Maybe start our little eyedude dude on his way! */
	eyeDudeState = state;
}

#if NeedFunctionPrototypes
eyeDudeStates getEyeDudeMode(void)
#else
eyeDudeStates getEyeDudeMode()
#endif
{
	return eyeDudeState;
}
