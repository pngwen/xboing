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
 * $Id: ball.c,v 1.1.1.1 1994/12/16 01:36:42 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/ball.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:42 $
 *
 * $Log: ball.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:42  jck
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
#include <math.h>
#include <values.h>

/*
// TODO: Remove instances of X11-associated data types
#include <xpm.h>
#include <X11/Xos.h>

//TODO: Replace references to bitmaps with png textures
#include "bitmaps/balls/ball1.png"
#include "bitmaps/balls/ball2.png"
#include "bitmaps/balls/ball3.png"
#include "bitmaps/balls/ball4.png"
#include "bitmaps/balls/killer.png"

#include "bitmaps/guides/guide1.png"
#include "bitmaps/guides/guide2.png"
#include "bitmaps/guides/guide3.png"
#include "bitmaps/guides/guide4.png"
#include "bitmaps/guides/guide5.png"
#include "bitmaps/guides/guide6.png"
#include "bitmaps/guides/guide7.png"
#include "bitmaps/guides/guide8.png"
#include "bitmaps/guides/guide9.png"
#include "bitmaps/guides/guide10.png"
#include "bitmaps/guides/guide11.png"

#include "bitmaps/balls/bbirth1.png"
#include "bitmaps/balls/bbirth2.png"
#include "bitmaps/balls/bbirth3.png"
#include "bitmaps/balls/bbirth4.png"
#include "bitmaps/balls/bbirth5.png"
#include "bitmaps/balls/bbirth6.png"
#include "bitmaps/balls/bbirth7.png"
#include "bitmaps/balls/bbirth8.png"
*/


#include "include/audio.h"
#include "include/error.h"
#include "include/score.h"
#include "include/sfx.h"
#include "include/eyedude.h"
#include "include/init.h"
#include "include/main.h"
#include "include/stage.h"
#include "include/blocks.h"
#include "include/paddle.h"
#include "include/misc.h"
#include "include/level.h"
#include "include/mess.h"
#include "include/special.h"
#include "include/ball.h"
#include "include/faketypes.h"

/*
 *  Internal macro definitions:
 */

#define X2COL(col, x) (col = x / colWidth)
#define Y2ROW(row, y) (row = y / rowHeight)

#define COL2X(x, col) (x = col * colWidth)
#define ROW2Y(y, row) (y = row * rowHeight)

/* MIN returns the smallest at a and b */
#ifndef MIN
#define MIN(a,b) ((a)<(b) ? (a):(b))
#endif

/* MAX returns the largest of a and b */
#ifndef MAX
#define MAX(a,b) ((a)>(b) ? (a):(b))
#endif

/* SQR returns the square of x */
#ifndef SQR
#define SQR(x) ((x)*(x))
#endif

#ifndef MINFLOAT
#define MINFLOAT ((float)1.40129846432481707e-45)
#endif


/*
 *  Internal type declarations:
 */

static void MoveBall(Display *display, Window window, int x, int y, int replace,
	int i);
static void MoveBallBirth(Display *display, Window window, int x, int y, 
	int slide, int replace, int i);
static void TeleportBall(Display *display, Window window, int i);
static int BallHitPaddle(Display *display, Window window, int *hit, int i,
	int *x, int *y);
static void UpdateABall(Display *display, Window window, int i);
static int CheckRegions(Display *display, Window window, int row, int col,
	int x, int y, int i);
static int CheckForCollision(Display *display, Window window, int x, int y, 
	int *r, int *c, int i);
static void updateBallVariables(int i);
static void SetBallWait(enum BallStates newMode, int waitFrame, int i);
static void DoBallWait(int i);
static void EraseTheBall(Display *display, Window window, int x, int y);
static void ChangeBallDirectionToGuide(int i);
static void Ball2BallCollision(BALL *ball1, BALL *ball2);
static int WhenBallsCollide(BALL *ball1, BALL *ball2, float *time);

typedef struct
{
   float x, y;
} vector_t;

/*
 *  Internal variable declarations:
 */

static Pixmap ballsPixmap[BALL_SLIDES];
static Pixmap ballsMask[BALL_SLIDES];
static Pixmap ballBirthPixmap[BIRTH_SLIDES];
static Pixmap ballBirthMask[BIRTH_SLIDES];
static Pixmap guides[11];
static Pixmap guidesM[11];
BALL balls[MAX_BALLS];
static int guidePos = 6;	 /* Start in middle of guider */

/* global constant machine epsilon */
float MACHINE_EPS;

void InitialiseBall(Display *display, Window window, Colormap colormap)
{
    XpmAttributes attributes;
    int XpmErrorStatus;

    attributes.valuemask = XpmColormap;
    attributes.colormap = colormap;

    /* --- BALL FRAMES --- */
    static char **ballFrames[] = {
        ball1_xpm, ball2_xpm, ball3_xpm, ball4_xpm, killer_xpm
    };

    for (int i = 0; i < BALL_SLIDES; i++) {
        XpmErrorStatus = XpmCreatePixmapFromData(display, window, ballFrames[i],
            &ballsPixmap[i], &ballsMask[i], &attributes);
        HandleXPMError(display, XpmErrorStatus, "InitialiseBall(ball)");
    }

    /* --- BALL BIRTH FRAMES --- */
    static char **birthFrames[] = {
        ballbirth1_xpm, ballbirth2_xpm, ballbirth3_xpm, ballbirth4_xpm,
        ballbirth5_xpm, ballbirth6_xpm, ballbirth7_xpm, ballbirth8_xpm
    };

    for (int i = 0; i < BIRTH_SLIDES; i++) {
        XpmErrorStatus = XpmCreatePixmapFromData(display, window, birthFrames[i],
            &ballBirthPixmap[i], &ballBirthMask[i], &attributes);
        HandleXPMError(display, XpmErrorStatus, "InitialiseBall(ballbirth)");
    }

    /* --- GUIDE FRAMES --- */
    static char **guideFrames[] = {
        guide1_xpm, guide2_xpm, guide3_xpm, guide4_xpm, guide5_xpm,
        guide6_xpm, guide7_xpm, guide8_xpm, guide9_xpm, guide10_xpm, guide11_xpm
    };

    for (int i = 0; i < 11; i++) {
        XpmErrorStatus = XpmCreatePixmapFromData(display, window, guideFrames[i],
            &guides[i], &guidesM[i], &attributes);
        HandleXPMError(display, XpmErrorStatus, "InitialiseBall(guide)");
    }

    /* Free the xpm pixmap attributes */
    XpmFreeAttributes(&attributes);

    MACHINE_EPS = sqrt(MINFLOAT);

    /* Make sure that all the balls are initialised */
    ClearAllBalls();
}

void FreeBall(Display *display)
{
	/*
	 * Free all the animation frames for the balls and guides etc.
	 */

	int i;

	/* Free all animation frames for the ball */
	for (i = 0; i < BALL_SLIDES; i++)
	{
		if (ballsPixmap[i]) 	XFreePixmap(display, ballsPixmap[i]);
	 	if (ballsMask[i]) 		XFreePixmap(display, ballsMask[i]);
	}

	/* Free all animation frames for the guides */
	for (i = 0; i < 11; i++)
	{
		if (guides[i]) 	XFreePixmap(display, guides[i]);
	 	if (guidesM[i]) XFreePixmap(display, guidesM[i]);
	}

	/* Free all animation frames for the ball birth */
	for (i = 0; i < BIRTH_SLIDES; i++)
	{
		/* Free the ball birth animation pixmaps */
		if (ballBirthPixmap[i]) 	XFreePixmap(display, ballBirthPixmap[i]);
	 	if (ballBirthMask[i]) 		XFreePixmap(display, ballBirthMask[i]);
	}
}

void RedrawBall(Display *display, Window window)
{
	/* not hard - STILL TO BE IMPLEMENTED */
}

static void EraseTheBall(Display *display, Window window, int x, int y)
{
	/* 
	 * Clear the ball area! The x, y coordinates are the centre of ball 
	 */

    XClearArea(display, window, x - BALL_WC, y - BALL_HC, 
		BALL_WIDTH, BALL_HEIGHT, False);
}

void DrawTheBall(Display *display, Window window, int x, int y, int slide)
{
	/* 
	 * Draw the ball using the slide variable as the index into the frames
	 * of the ball animation. The x,y are the centre of the ball.
	 */

    RenderShape(display, window, ballsPixmap[slide], ballsMask[slide],
		x - BALL_WC, y - BALL_HC, BALL_WIDTH, BALL_HEIGHT, False);
}

void DrawTheBallBirth(Display *display, Window window, int x, int y, int slide)
{
	/* 
	 * Draw the ball using the slide variable as the index into the frames
	 * of the ball animation. The x,y are the centre of the ball birth anim.
	 */

    RenderShape(display, window, ballBirthPixmap[slide], ballBirthMask[slide],
		x - BALL_WC, y - BALL_HC, BALL_WIDTH, BALL_HEIGHT, False);
}

static void MoveBallBirth(Display *display, Window window, int x, int y, 
	int slide, int replace, int i)
{
	/* 
	 * Remove any debris under ball first by clearing it 
	 */

	if (replace)
	{
		XClearArea(display, window, 
			balls[i].oldx - BALL_WC, balls[i].oldy - BALL_HC, 
			BALL_WIDTH, BALL_HEIGHT, False);
	}	

	balls[i].oldx = x;
	balls[i].oldy = y;

    /* If slide is -1 then clear the ball area */
    if (slide == -1)
        XClearArea(display, window,
            x - BALL_WC, y - BALL_HC, BALL_WIDTH, BALL_HEIGHT, False);
    else
        DrawTheBallBirth(display, window, x, y, slide);
}

static void MoveBall(Display *display, Window window, int x, int y, int replace,
	int i)
{
	/*
	 * Move the ball from one position to the next and also update the
	 * balls old positions. Will only move if the frame is correct for
	 * this move, ie: framrate. Ball animates as well - hard to see though.
	 */

	if (replace)
		EraseTheBall(display, window, balls[i].oldx, balls[i].oldy);

	/* Update the old position of this ball */
	balls[i].oldx = x;
	balls[i].oldy = y;

	if (Killer == True)
	{
		/* Render the killer ball now ie: red ball */
		DrawTheBall(display, window, x, y, BALL_SLIDES-1);
	}
	else
	{
		/* Render the ball now */
		DrawTheBall(display, window, x, y, balls[i].slide);
	}	

	/* Change slide for ball every n frames of animation */
	if ((frame % BALL_ANIM_RATE) == 0)
		balls[i].slide++;
	
	/* wrap around slides */
	if (balls[i].slide == BALL_SLIDES-1) balls[i].slide = 0;
}

static void MoveGuides(Display *display, Window window, int i, int remove)
{
	/*
	 * The guide hangs out above a READY ball when waiting to be sent into
	 * action. It has a rotating yellow dot that indicates which direction
	 * the ball will be heading off in. This code animates and also clears
	 * the guide.
	 */

	static int oldgx = 0;
	static int oldgy = 0;
	static int inc = 1;

	/* Clear the old slide */
    XClearArea(display, window, oldgx - 14, oldgy - 6, 29, 12, False);

	if (remove == False)
	{
		/* Update its old positions */
		oldgx = balls[i].oldx;
		oldgy = balls[i].oldy - 16;

		/* Check for any silly errors */
		if (guidePos < 0 || guidePos > 10)
			ErrorMessage("Guidepos out of range.");

		/* draw the guide pixmap */
    	RenderShape(display, window, guides[guidePos], guidesM[guidePos],
			oldgx - 14, oldgy - 6, 29, 12, False);

		/* Don't draw it ever frame */
		if ((frame % (BALL_FRAME_RATE*8)) == 0)
			guidePos += inc;

		/* wrap around slides */
		if (guidePos == 10) inc = -1;
		if (guidePos == 0) inc = 1;
	}
	else
		guidePos = 6;
}

void RandomiseBallVelocity(int i)
{
	balls[i].dx = balls[i].dy = 0;

	/* Loop until values are random */
	while (balls[i].dx == 0 || balls[i].dy == 0)
	{
		/* Randomise the ball btwn [3, MAX_VEL] */
   	 	balls[i].dx = (rand() % (MAX_X_VEL - 3)) + 3;
   	 	balls[i].dy = (rand() % (MAX_Y_VEL - 3)) + 3;

		/* Make it possible for negative numbers */
		if ((rand() % 10) < 5)
			balls[i].dx *= -1;
		if ((rand() % 10) < 5)
			balls[i].dy *= -1;

		balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;
	}
}

void DoBoardTilt(Display *display, int i)
{
	/*
	 * In the event of a ball loop bounce then this function will fiddle
	 * with the velocity and it may just jump out of the loop.
	 */

	/* Only worry about active balls */
	if (balls[i].ballState == BALL_ACTIVE)
	{
		DEBUG("Auto Tilt activated.");

		/* Tilt the board to remove any endless loops */
		SetCurrentMessage(display, messWindow, 
			"Auto Tilt Activated", True);
	
		RandomiseBallVelocity(i);
	}
}

static void TeleportBall(Display *display, Window window, int i)
{
	/* 
	 * This function will teleport the ball to some other space not occupied
	 * and start off there.
	 */

	int r1, c1, s1, r2, c2, s2, r3, c3, s3, r4, c4, s4;
    int r, c, x, y;
    struct aBlock *blockP, *bP;
	int done = False;
	int count = 0;

	/* Loop until we find a block to move to */
	while (done == False && count <= 20)
	{
		/* Stop this going on forever */
		count++;

		/* Give me a new random block position */
		r = (rand() % (MAX_ROW - 6)) + 1;
		c = (rand() % MAX_COL) + 1;

    	if (r < 0 || r >= MAX_ROW) continue;
    	if (c < 0 || c >= MAX_COL) continue;

		/* Pointer to the correct block we need - speed things up */
		blockP = &blocks[r][c];

		/* Check that the block is not occupied and not exploding */
		if ((blockP->occupied == False) && (blockP->exploding == False))
		{
        	/* Check that the block is not a closed in position */

            r1 = r;     c1 = c - 1;  s1 = 0;
            if (r1 < 0 || r1 >= MAX_ROW) s1 = 1;
            if (c1 < 0 || c1 >= MAX_COL) s1 = 1;
            if (s1 == 0) 
			{
            	bP = &blocks[r1][c1];
                if (bP->blockType == BLACK_BLK)
                s1 = 1;
            }

            r2 = r - 1; c2 = c;      s2 = 0;
            if (r2 < 0 || r2 >= MAX_ROW) s2 = 1;
            if (c2 < 0 || c2 >= MAX_COL) s2 = 1;
            if (s2 == 0) 
			{
                bP = &blocks[r2][c2];
                if (bP->blockType == BLACK_BLK)
                    s2 = 1;
            }

            r3 = r;     c3 = c + 1;  s3 = 0;
            if (r3 < 0 || r3 >= MAX_ROW) s3 = 1;
            if (c3 < 0 || c3 >= MAX_COL) s3 = 1;
            if (s3 == 0) 
			{
                bP = &blocks[r3][c3];
                if (bP->blockType == BLACK_BLK)
                    s3 = 1;
            }

            r4 = r + 1; c4 = c;      s4 = 0;
            if (r4 < 0 || r4 >= MAX_ROW) s4 = 1;
            if (c4 < 0 || c4 >= MAX_COL) s4 = 1;
            if (s4 == 0) 
			{
                bP = &blocks[r4][c4];
                if (bP->blockType == BLACK_BLK)
                    s4 = 1;
            }

			/* If it isn't ok to go here then keep searching */
            if ((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1))
			{
				DEBUG("Trying new spot for teleport.");
                continue;
			}

			/* Calculate the new ball coordinates */
			COL2X(x, c);
			ROW2Y(y, r);

			balls[i].ballx = x;
			balls[i].bally = y;

			/* Move the ball to the new position */
			MoveBall(display, window, x, y, True, i);

			balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

			/* Ok jump out now thanks. */
			done = True;

			DEBUG("Ball was Teleported.");

			return;
		}
		else
		{
			DEBUG("Looping in teleport function.");
		}
	}

	/* Ok - give up add erase ball and start it from paddle!! */
	EraseTheBall(display, window, balls[i].oldx, balls[i].oldy);
	updateBallVariables(i);

	DEBUG("Ball was NOT Teleported.");
}

void SplitBallInTwo(Display *display, Window window)
{
	/*
	 * If a multiball block was hit then start another ball from somewhere.
	 * Start it somewhere random and also randomise the velocity.
	 */

	int j;

	j = AddANewBall(display, 0, 0, 3, 3);
	if (j > 0)
	{
		/* Make this new ball move straight away */
		ChangeBallMode(BALL_ACTIVE, j);
		TeleportBall(display, window, j);
		RandomiseBallVelocity(j);

		SetCurrentMessage(display, messWindow, "Another ball!", True);
	}
	else
		SetCurrentMessage(display, messWindow,
			"Cannot add ball!", True);
}

void ClearBallNow(Display *display, Window window, int i)
{
	/*
	 * Terminate and clear this ball.
	 */

	EraseTheBall(display, window, balls[i].oldx, balls[i].oldy);
	ClearBall(i);
	DeadBall(display, window);

	DEBUG("Clear ball now called.");
}

void KillBallNow(Display *display, Window window, int i)
{
	/*
	 * Set the ball to die by popping.
	 */

	ChangeBallMode(BALL_POP, i);

	DEBUG("Ball mode now BALL_POP.");
}

void GetBallPosition(int *ballX, int *ballY, int i)
{
	/*
	 * Get the position of ball i.
	 */

	*ballX = balls[i].ballx;
	*ballY = balls[i].bally;
}

static int BallHitPaddle(Display *display, Window window, int *hit, int i,
	int *x, int *y)
{
	/*
	 * Handle the ball hitting the paddle if it is. The bounce it back
	 * at an angle that is described below.
	 */

	float x1, x2, y1, y2, alpha, beta, xP1, xP2, xH, yH;
    int paddleLine;

	/***********************************************************************

                        A1 (x1,y1)
                        *
                       .
                      .
         P1 =========.=========== P2   <----   paddle (x, y pos is known )
        (xP1,yP1)   . H (xH, yH)    (xP2,yP2)
                   .
                  .
                 .
                *
               A2 (x2,y2)

   		Given the line A1A2, is the intersecting point H (xH, yH) in the paddle 
		segment ? (i.e xH in [xP1,xP2])

   		A1A2 is :  y = alpha * x + beta

   		A1 and A2 are in A1A2 than beta = [(y1 + y2) - alpha*(x1+x2)] / 2

   		yH = yP1 = yP2

   		so xH = (yP1 - beta) / alpha

	**********************************************************************/

    paddleLine = (PLAY_HEIGHT - DIST_BASE - 2);

    if (balls[i].bally + BALL_HC > paddleLine)
   	{
   		xP1 = (float)(paddlePos - (GetPaddleSize() / 2) - BALL_WC);
   		xP2 = (float)(paddlePos + (GetPaddleSize() / 2) + BALL_WC);

   		if (balls[i].dx == 0)
      	{
			/* process the vertical moving balls */
      		if (((float)balls[i].ballx > xP1) && ((float)balls[i].ballx < xP2))
         	{
         		/* the ball hit the paddle */
         		*hit = balls[i].ballx - paddlePos;
         		*x 	 = balls[i].ballx;
         		*y 	 = paddleLine - BALL_HC;

         		return True;
         	}
      		else
        		return False;
      	}
   		else
      	{
 			/* compute the line coefficients of the ball */

			alpha 	= (float) balls[i].dy;
    		x1 		= (float) (balls[i].ballx - balls[i].dx);
    		y1 		= (float) (balls[i].bally - balls[i].dy);
    		x2 		= (float) (balls[i].ballx);
    		y2 		= (float) (balls[i].bally);
    		beta 	= ((y1 + y2) - alpha * (x1 + x2)) / 2.0;

    		yH = (float) paddleLine;
    		xH = (yH - beta) / alpha;

			if ((xH > xP1) && (xH < xP2))
    		{
    			/* the ball hit the paddle */
        		*hit 	= (int) (xH + 0.5) - paddlePos;
        		*x 		= (int) (xH + 0.5);
        		*y 		= paddleLine - BALL_HC;

        		return True;
    		}
    		else
    			return False;
		}
	}

   	/* We didn't hit the paddle */
   	return False;
}

static int HandleTheBlocks(Display *display, Window window, int row, int col,
	int i)
{
	/*
	 * When a ball hits a block it calls this routine and this routine
	 * will perform the function relevant to that block. eg: teleport
	 * will teleport the ball. "i" is the ball number.
	 *
	 * Will return TRUE if the ball is not to bounce off the block.
	 */

	struct aBlock *blockP;

	/* Pointer to the block the ball is in */
	blockP = &blocks[row][col];

	/* There has been a collision so handle it */
	if (blockP->exploding == False)
	{
		switch (blockP->blockType)
		{
			case COUNTER_BLK:
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True || blockP->explodeAll == True)
				{
					/* Ok kill the block outright */
					DrawBlock(display, window, row, col, KILL_BLK);
					return True;
				}
	
				/* Special case for counter - reduce count on block */
				if (blockP->counterSlide == 0)
					DrawBlock(display, window, row, col, KILL_BLK);
				else
				{
					/* Draw the counter block minus one number */
					blockP->counterSlide--;
					DrawBlock(display, window, row, col, COUNTER_BLK);
				}

				break;

			case MGUN_BLK:
				/* Turn the machine gun on */
				ToggleFastGun(display, True);
				DrawSpecials(display);
				SetCurrentMessage(display, messWindow, "Machine Gun", True);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case DEATH_BLK:
				/* Ha ha - hit death block so die */

				/* Kill the ball now */
				ClearBallNow(display, window, i);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case HYPERSPACE_BLK:
				/* Teleport to somewhere else */
				TeleportBall(display, window, i);
				RandomiseBallVelocity(i);

				/* Redraw it just in case */
				DrawBlock(display, window, row, col, HYPERSPACE_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				PlaySoundForBlock(HYPERSPACE_BLK);

				return True;

				break;

			case WALLOFF_BLK:
				/* Walls are now turned off */
				ToggleWallsOn(display, True);
				DrawSpecials(display);
				SetCurrentMessage(display, messWindow,
					"Walls off", True);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case REVERSE_BLK:
				/* Paddle control now reverse */
				ToggleReverse(display);
				SetCurrentMessage(display, messWindow,
					"Paddle control reversed", True);
				DrawSpecials(display);

				/* Move the paddle to reflect reversed paddle */
				handlePaddleMoving(display);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case PAD_SHRINK_BLK:
				/* Paddle shrinking block */
				ChangePaddleSize(display, window, PAD_SHRINK_BLK);
				SetCurrentMessage(display, messWindow,
					"Shrink Paddle", True);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case PAD_EXPAND_BLK:
				/* Paddle expanding block */
				ChangePaddleSize(display, window, PAD_EXPAND_BLK);
				SetCurrentMessage(display, messWindow,
					"Expand Paddle", True);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case EXTRABALL_BLK:
				/* Extra ball */
				AddExtraLife(display);
				SetCurrentMessage(display, messWindow,
					"Extra ball", True);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case STICKY_BLK:
				ToggleStickyBat(display, True);
				DrawSpecials(display);
				SetCurrentMessage(display, messWindow,
					"Sticky Bat", True);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case MULTIBALL_BLK:
				SplitBallInTwo(display, window);

				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;

			case BLACK_BLK:
				if (frame <= blockP->nextFrame)
				{
					/* Set the block up to be blown away */
					DrawBlock(display, window, row, col, KILL_BLK);
					balls[i].lastPaddleHitFrame = frame + 
						PADDLE_BALL_FRAME_TILT;

					/* If in killer mode then don't bounce off block */
					if (Killer == True) return True;
				}
				else
				{
					/* Redraw the solid wall block to make sure */
					DrawBlock(display, window, row, col, BLACKHIT_BLK);
					blockP->nextFrame = frame + 30;
				}
					
				break;

			default:
				/* Not a wall so explode the block */
				DrawBlock(display, window, row, col, KILL_BLK);
				balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

				/* If in killer mode then don't bounce off block */
				if (Killer == True) return True;

				break;
		}
	}

	/* Don't return after returning */
	return False;
}

static void UpdateABall(Display *display, Window window, int i)
{
	/*
	 * Main routine that will update the ball given and handle all collisions
	 * and also bouce off all walls and blocks.
	 */

	int row, col, hitPos, ret, t;
	int cx, cy, step, j, r, ddx, ddy, Hx, Hy;
	float incx, incy, x, y;
	float Vs, Vx, Vy, alpha, beta, gamma, padSize;
	float dummy;

	/* Update ball position using dx and dy values */	
	balls[i].ballx = balls[i].oldx + balls[i].dx;
	balls[i].bally = balls[i].oldy + balls[i].dy;

	/* Mark the ball to die as it is past the paddle */
	if (balls[i].bally > (PLAY_HEIGHT - DIST_BASE + BALL_HEIGHT))
		ChangeBallMode(BALL_DIE, i);

	/* Check if ball has hit left wall and bounce off */		
	if (balls[i].ballx < BALL_WC && noWalls == False)
	{
		balls[i].dx = abs(balls[i].dx);
		if (noSound == False) playSoundFile("boing", 10);

	} else if (noWalls == True && balls[i].ballx < BALL_WC)
	{
		/* If the no walls mode is on then wrap around onto right wall */
		balls[i].ballx = (PLAY_WIDTH - BALL_WC);

		/* Move the ball to the new position */
		MoveBall(display, window, balls[i].ballx, balls[i].bally, True, i);

		return;
	}

	/* Check if ball has hit right wall and bounce off */		
	if (balls[i].ballx > (PLAY_WIDTH - BALL_WC) && noWalls == False)
	{
		balls[i].dx = -(abs(balls[i].dx));
		if (noSound == False) playSoundFile("boing", 10);

	} else if (noWalls == True && balls[i].ballx > (PLAY_WIDTH - BALL_WC))
	{
		/* If the no walls mode is on then wrap around onto left wall */
		balls[i].ballx = BALL_WC;

		/* Move the ball to the new position */
		MoveBall(display, window, balls[i].ballx, balls[i].bally, True, i);

		return;
	}

	/* Check if ball has hit top wall and bounce off */		
	if (balls[i].bally < BALL_HC) 
	{
		balls[i].dy = abs(balls[i].dy);
		if (noSound == False) playSoundFile("boing", 10);
	}

	if (balls[i].ballState != BALL_DIE)
	{
		/* Check if the ball has hit the paddle */
		if (BallHitPaddle(display, window, &hitPos, i, &Hx, &Hy) == True)
		{
			/* Keep track of how long it was since the last paddle hit */
			balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;
			if (noSound == False) playSoundFile("paddle", 50);

			/* Add a paddle hit bonus score, I'm nice ;-) */
			AddToScore((u_long) PADDLE_HIT_SCORE);
			DisplayScore(display, scoreWindow, score);

			/* speed vector of the ball */
           	Vx = (float) balls[i].dx;    
           	Vy = (float) balls[i].dy;

			/* speed intensity of the ball */
           	Vs = sqrt(Vx * Vx + Vy * Vy );

           	alpha = atan(Vx / -Vy);

           	padSize = (float) (GetPaddleSize() + BALL_WC);
           	Vx = (float) hitPos;
           	Vy = (float) padSize / 1.0;

           	beta = atan(Vx / Vy);
           	gamma = 2.0 * beta - alpha;

           	Vx = Vs * sin(gamma);
           	Vy = -Vs * cos(gamma);

			/* take in account the horizontal speed of the paddle: 
			 * vectorial summ 
			 */
           	Vx += (float) (paddleDx / 10.0);

           	if (Vx > 0.0)
            	balls[i].dx = (int) (Vx + 0.5);
           	else
             	balls[i].dx = (int) (Vx - 0.5);

           	if (Vy < 0.0)
             	balls[i].dy = (int) (Vy - 0.5);
           	else
             	balls[i].dy = -MIN_DY_BALL;

           	if (balls[i].dy > -MIN_DY_BALL) 
				balls[i].dy = -MIN_DY_BALL;

           	balls[i].ballx = Hx;
           	balls[i].bally = Hy;

			/* handle the sticky paddle special by changing the ball mode
			 * to BALL_READY so it will need user to press space to start
			 * the ball moving again.
			 */
			if (stickyBat == True)	
			{
				ChangeBallMode(BALL_READY, i);

				/* Move the ball to the new position */
				MoveBall(display, window, 
					balls[i].ballx, balls[i].bally, True, i);

				/* So that it will auto shoot off if you wait too long */
				balls[i].nextFrame = frame + BALL_AUTO_ACTIVE_DELAY;

				return;
			}
		}
		else
		{
			/* Ball didn't hit the paddle so check if it time to autotilt the
			 * board as it is in an infinite loop most likely.
			 */
			if (balls[i].lastPaddleHitFrame <= frame)
				DoBoardTilt(display, i);
		}

       	Vx = (float) balls[i].dx;
       	Vy = (float) balls[i].dy;
       	Vs = sqrt(Vx * Vx + Vy * Vy);

       	alpha = sqrt((float)MAX_X_VEL*(float)MAX_X_VEL + (float)MAX_Y_VEL*
			(float)MAX_Y_VEL );
       	alpha /= 9.0; /* number of speed level */
       	alpha *= (float) speedLevel;
		if (Vs == 0.0) Vs = 1.0;
       	beta = alpha / Vs;

       	Vx *= beta;
       	Vy *= beta;

       	if (Vx > 0.0)
         	balls[i].dx = (int) (Vx + 0.5);
       	else
         	balls[i].dx = (int) (Vx - 0.5);

       	if (Vy > 0.0)
         	balls[i].dy = (int) (Vy + 0.5);
       	else
         	balls[i].dy = (int) (Vy - 0.5);

       	if (balls[i].dy == 0) 
			balls[i].dy = MIN_DY_BALL;

       	if (balls[i].dx == 0) 
			balls[i].dx = MIN_DX_BALL;
	}

	/* Has the player lost the ball of the bottom of the screen */
	if (balls[i].bally > (PLAY_HEIGHT + BALL_HEIGHT*2))
	{
		DEBUG("Ball lost off bottom.");

		/* Make ball start to die */
		ClearBallNow(display, window, i);
		return;
	}

	/* Convert the new ball positions to rows and cols for collision */
	X2COL(col, balls[i].ballx);
	Y2ROW(row, balls[i].bally);

	x = balls[i].oldx;
	y = balls[i].oldy;

	cx = balls[i].dx > 0 ? 1 : -1;
	cy = balls[i].dy > 0 ? 1 : -1;

	if (abs(balls[i].dx) == abs(balls[i].dy))
	{
		incx = (float) cx;
		incy = (float) cy;
		step = abs(balls[i].dx);
	} else if (abs(balls[i].dx) > abs(balls[i].dy))
	{
		incx = (float) cx;
		incy = ((float) abs(balls[i].dy) / (float) abs(balls[i].dx)) * cy;
		step = abs(balls[i].dx);
	} 
	else 
	{
		incy = (float) cy;
		incx = ((float) abs(balls[i].dx) / (float) abs(balls[i].dy)) * cx;
		step = abs(balls[i].dy);
	}

	for (j = 0; j < step; j++)
	{
		/* Check if the ball has hit a brick or something */
		if ((ret = CheckForCollision(display, window, 
			(int) x, (int) y, &row, &col, i)) != REGION_NONE)
		{
			if (HandleTheBlocks(display, window, row, col, i) == True)
				return;

			ddx = ddy = 0;
			r = (rand() >> 16) % 4;

			/* Find out which side the ball hit the brick */
			switch (ret)
			{
				case REGION_LEFT:
					ddx = -r/4;
					balls[i].dx = -(abs(balls[i].dx));
					break;

				case REGION_RIGHT:
					ddx = r/4;
					balls[i].dx = abs(balls[i].dx);
					break;

				case REGION_TOP:
					ddy = -r/4;
					balls[i].dy = -(abs(balls[i].dy));
					break;

				case REGION_BOTTOM:
					ddy = r/4;
					balls[i].dy = abs(balls[i].dy);
					break;

				case (REGION_BOTTOM | REGION_RIGHT):
					ddy = r;
					ddx = r;
					balls[i].dy = abs(balls[i].dy);
					balls[i].dx = abs(balls[i].dx);
					break;

				case (REGION_TOP | REGION_RIGHT):
					ddy = -r;
					ddx = r;
					balls[i].dy = -(abs(balls[i].dy));
					balls[i].dx = abs(balls[i].dx);
					break;

				case (REGION_BOTTOM | REGION_LEFT):
					ddy = r;
					ddx = -r;
					balls[i].dx = -(abs(balls[i].dx));
					balls[i].dy = abs(balls[i].dy);
					break;

				case (REGION_TOP | REGION_LEFT):
					ddy = -r;
					ddx = -r;
					balls[i].dx = -(abs(balls[i].dx));
					balls[i].dy = -(abs(balls[i].dy));
					break;
			}


			/* Update ball position using dx and dy values */	
			balls[i].ballx = (int) x + balls[i].dx + ddx + 1 - rand() % 3;
			balls[i].bally = (int) y + balls[i].dy + ddy + 1 - rand() % 3;

			break;
		}

		x += incx;
		y += incy;

	}	/* for */

	/* Move the ball to the new position */
	MoveBall(display, window, balls[i].ballx, balls[i].bally, True, i);

	/* Loop all the balls checking for ball2ball collisions */
	for (t = 0; t < MAX_BALLS; t++)
	{
		/* Online check out active balls */
		if (balls[t].ballState == BALL_ACTIVE)
		{
			/* Will they collide */
			if (WhenBallsCollide(&balls[i], &balls[t], &dummy))
			{
				DEBUG("Ball hit ball - rebound.");

				/* Ok collided - so rebound please */
				Ball2BallCollision(&balls[i], &balls[t]);

				if (noSound == False) playSoundFile("ball2ball", 90);
			}

			/* Check if the ball has hit an eye dude */
			if (getEyeDudeMode() == EYEDUDE_WALK)
			{
				if (CheckBallEyeDudeCollision(display, window, t) == True)
				{	
           	    	/* Ok so the eyedude has been hit - arrggh */
           	    	ChangeEyeDudeMode(EYEDUDE_DIE);
				}
			}
		}
	}
}

static int CheckRegions(Display *display, Window window, int row, int col,
	int x, int y, int i)
{
	/*
	 * Check each region and see if the ball has hit it.
	 *
	 * Returns the region hit or REGION_NONE.
	 */

    struct aBlock *blockP;
	struct aBlock *blockPleft;
	struct aBlock *blockPright;
	struct aBlock *blockPtop;
	struct aBlock *blockPbottom;

	int region = REGION_NONE;

    if (row < 0 || row >= MAX_ROW) return REGION_NONE;
    if (col < 0 || col >= MAX_COL) return REGION_NONE;

    blockP = &blocks[row][col];
    blockPleft = &blocks[row][col-1];
    blockPright = &blocks[row][col+1];
    blockPtop = &blocks[row-1][col];
    blockPbottom = &blocks[row+1][col];

    /* If blocks is occupied then check for collision */
    if (blockP->occupied == 1 && blockP->exploding == False)
    {
        /* Suss out if ball is moving more vertically than horizontally */
        if (abs(balls[i].dx) > abs(balls[i].dy))
        {
            /* Check left and right first as ball is moving more horizontal */

            /* See if the ball intersects with the block's left region */
            if (XRectInRegion(blockP->regionLeft, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPleft->occupied == False)
                	region |= REGION_LEFT;
			}

            /* See if the ball intersects with the block's right region */
            if (XRectInRegion(blockP->regionRight, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPright->occupied == False)
                	region |= REGION_RIGHT;
			}

            /* See if the ball intersects with the block's bottom region */
            if (XRectInRegion(blockP->regionBottom, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPbottom->occupied == False)
                	region |= REGION_BOTTOM;
			}

            /* See if the ball intersects with the block's top region */
            if (XRectInRegion(blockP->regionTop, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPtop->occupied == False)
                	region |= REGION_TOP;
			}
        }
        else
        {
            /* Check top and bottom first as ball is moving more vertical */

            /* See if the ball intersects with the block's bottom region */
            if (XRectInRegion(blockP->regionBottom, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPbottom->occupied == False)
                	region |= REGION_BOTTOM;
			}

            /* See if the ball intersects with the block's top region */
            if (XRectInRegion(blockP->regionTop, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPtop->occupied == False)
                	region |= REGION_TOP;
			}

            /* See if the ball intersects with the block's left region */
            if (XRectInRegion(blockP->regionLeft, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPleft->occupied == False)
                	region |= REGION_LEFT;
			}

            /* See if the ball intersects with the block's right region */
            if (XRectInRegion(blockP->regionRight, x - BALL_WC, y - BALL_HC,
                BALL_WIDTH, BALL_HEIGHT) != RectangleOut)
			{
            	if (blockPright->occupied == False)
                	region |= REGION_RIGHT;
			}
        }
    }

	/* Return the region combination */
	return region;
}

static int CheckForCollision(Display *display, Window window, int x, int y, 
	int *r, int *c, int i)
{
	/*
	 * Check each adjoining block and see if the ball has hit any region in
	 * it. Will return the hit region or REGION_NONE.
	 */
	int ret, row, col;

	row = *r;
	col = *c;

	/* Check all the regions around block */
	if ((ret = CheckRegions(display, window, row, col, x, y, i)) 
		!= REGION_NONE)	/*nothin*/;
	else if ((ret = CheckRegions(display, window, row+1, col, x, y, i)) 
		!= REGION_NONE)	row++;
	else if ((ret = CheckRegions(display, window, row-1, col, x, y, i)) 
		!= REGION_NONE)	row--;
	else if ((ret = CheckRegions(display, window, row, col+1, x, y, i)) 
		!= REGION_NONE)	col++;
	else if ((ret = CheckRegions(display, window, row, col-1, x, y, i)) 
		!= REGION_NONE)	col--;
	else if ((ret = CheckRegions(display, window, row+1, col+1, x, y, i)) 
		!= REGION_NONE)	{ row++; col++; }
	else if ((ret = CheckRegions(display, window, row-1, col-1, x, y, i)) 
		!= REGION_NONE)	{ row--; col--; }
	else if ((ret = CheckRegions(display, window, row+1, col-1, x, y, i)) 
		!= REGION_NONE) { row++; col--; }
	else if ((ret = CheckRegions(display, window, row-1, col+1, x, y, i)) 
		!= REGION_NONE)	
	{
		*r = row-1;
		*c = col+1;
		return REGION_NONE;
	}

	/* Return the row and column where the ball hit */
	*r = row;
	*c = col;

	return ret;
}

static int WhenBallsCollide(BALL *ball1, BALL *ball2, float *time)
{
    /*
     * Calculate when 2 balls will collide.
     * If the balls collide, a True status is returned, and when the
     * incident takes place is returned in time.
     * If the balls don't collide, a False status is returned.
     */

   	vector_t 	p, v;          /* deltas between the 2 balls */
   	float      	tmp1, tmp2, t1, t2, tmin, v2, r2;

   	p.x = ball1->ballx   -  ball2->ballx;
   	p.y = ball1->bally   -  ball2->bally;
   	v.x = ball1->dx  	-  ball2->dx;
    v.y = ball1->dy  	-  ball2->dy;

   	v2 = SQR(v.x) + SQR(v.y);
   	r2 = SQR(ball1->radius + ball2->radius);

   	/*
     * tmp2 >  0   Balls will collide, or are off from a collision direction
     * tmp2 == 0   Balls will touch or have already touched
     * tmp2 <  0   Balls will not be/have not been close to eachother.
     */

   	tmp2 = (v2 * r2) - SQR((v.x * p.y) - (v.y * p.x));

    /*
     * Check the magnitude of v2 to safeguard against numerical trouble.
     * The velocities must be scaled so that this is not a problem,
     * and rather change the time scale so that the travelled distance
     * v * t is constant.
     */

   	if (tmp2 >= 0.0 && v2 > MACHINE_EPS)
   	{
      	tmp2 = sqrt(tmp2) / v2;
      	tmp1 = -((p.x * v.x) + (p.y * v.y)) / v2;

      	t1 = tmp1 - tmp2;
      	t2 = tmp1 + tmp2;

      	/*
       	 * Choose the smallest of t1 and t2.
       	 * Note that both solutions t1 and t2 will (should) have the same sign.
       	 * If t1 and t2 are opposite sign, this means that the two ball centers
       	 * are closer to eachother than their combined radius.
       	 */
      	tmin = MIN(t1, t2);

      	if (tmin >= 0.0 && tmin <= 1.0)
	  	{
         	*time = tmin;
			return True;
		}
   	}

   	*time = 0.0;

   	return False;
}

static void Ball2BallCollision(BALL *ball1, BALL *ball2)
{
    /*
     * Calclulate the new velocity (direction) of the balls after a collision.
     * On entry, the balls positions and velocities are set to those values
     * when the collision takes place.  On exit, the balls velocities are set
     * to their new directions, whilst the position info remains unchanged.
     */

   vector_t   p, v;          /* deltas between the 2 balls */
   float      k, plen, massrate;

   p.x  = ball1->ballx  -  ball2->ballx;
   p.y  = ball1->ballx  -  ball2->bally;
   v.x  = ball1->dx  	-  ball2->dx;
   v.y  = ball1->dy  	-  ball2->dy;

   /*
    * p is the direction between the 2 balls centers, and will
    * have the langth of ball1->radius + ball2->radius
    */
   plen = sqrt(SQR(p.x) + SQR(p.y));
   p.x /= plen;
   p.y /= plen;

   massrate = ball1->mass / ball2->mass;

   k = -2.0 * ((v.x * p.x) + (v.y * p.y)) / (1.0 + massrate);
   ball1->dx += (int) (k * p.x);
   ball1->dy += (int) (k * p.y);

   /* New k for ball 2 */
   k *= -massrate;
   ball2->dx += (int) (k * p.x);
   ball2->dy += (int) (k * p.y);
}

static void updateBallVariables(int i)
{
	/*
	 * Update the balls x and old pos for a ball moving on the paddle 
	 * waiting to be shot off. Also ball birth.
	 */

	balls[i].ballx 	= paddlePos;
	balls[i].bally 	= PLAY_HEIGHT - DIST_BALL_OF_PADDLE;
	balls[i].oldx 	= balls[i].ballx;
	balls[i].oldy 	= balls[i].bally;
}

int GetNumberOfActiveBalls(void)
{
	/*
	 * Return the number of balls active in the arena.
	 */

	int i;
	int t;

	/* Zap through the list of balls */
	for (i = 0, t = 0; i < MAX_BALLS; i++)
	{
		/* Found an active abll - add to total */
		if (balls[i].ballState == BALL_ACTIVE)
			t++;
	}

	/* Return the number of active balls */
	return t;
}

int GetAnActiveBall(void)
{
	/*
	 * 
	 */
	int i;

	/* Zap through the list of balls */
	for (i = 0; i < MAX_BALLS; i++)
	{
		/* Return this ball */
		if (balls[i].ballState == BALL_ACTIVE)
			return i;
	}

	return -1;
}

int IsBallWaiting(void)
{
	/*
	 * Check to see if any ball is ready to be actiavted.
	 */
	int i;

	/* Zap through the list of balls */
	for (i = 0; i < MAX_BALLS; i++)
	{
		/* Ok it must be on the paddle so shoot it off */
		if (balls[i].ballState == BALL_READY)
			return True;
	}

	return False;
}

static void ChangeBallDirectionToGuide(int i)
{
	/* 
	 * Change the direction vector of the ball to that of the guide
	 * marker so that it will shoot off in the direction you want.
	 */
	int dx, dy;

	dx = dy = 0;

	switch (guidePos)
	{
		/* Left to middle to right */
		case 0: dx = -5; dy = -1; break;

		case 1: dx = -4; dy = -2; break;

		case 2: dx = -3; dy = -3; break;

		case 3: dx = -2; dy = -4; break;

		case 4: dx = -1; dy = -5; break;

		case 5:	dx = 0; dy = -5; break;

		case 6: dx = 1; dy = -5; break;

		case 7: dx = 2; dy = -4; break;

		case 8: dx = 3; dy = -3; break;

		case 9: dx = 4; dy = -2; break;

		case 10: dx = 5; dy = -1; break;
	}

	/* Make the ball go off in the direction of the guide */
	balls[i].dx	= dx; balls[i].dy	= dy;

	DEBUG("Changed ball start direction to guide.");
}

int ActivateWaitingBall(Display *display, Window window)
{
	/* 
	 * Loop through all balls and find the first one that is ready to
	 * be activated and activate it. Also erase the guide marker.
	 */
	int i;

	/* Zap through the list of balls */
	for (i = 0; i < MAX_BALLS; i++)
	{
		/* Ok it must be on the paddle so shoot it off */
		if (balls[i].ballState == BALL_READY)
		{
			/* Change the balls mode so that it shoots off */
			ChangeBallMode(BALL_ACTIVE, i);
			balls[i].lastPaddleHitFrame = frame + PADDLE_BALL_FRAME_TILT;

			ChangeBallDirectionToGuide(i);
			MoveGuides(display, window, i, True);

			return True;
		}
	}

	return False;
}

void ResetBallStart(Display *display, Window window)
{
	/*
	 * Add a new ball and create it on the paddle. It will then wait to be
	 * activated and have guides above it until activated.
	 */

	int i;

	i = AddANewBall(display, 0, 0, 3, -3);
	if (i >= 0)
	{
		/* Make sure that all variables are updated */
		updateBallVariables(i);

		/* Add 2 bullets every ball death or creation as it happens */
		AddABullet(display);
		AddABullet(display);

		/* Set up animation for ball creation */
		SetBallWait(BALL_CREATE, frame + 1, i);

		DEBUG("Reset ball start and create.");
	}
}

static void AnimateBallPop(Display *display, Window window, int i)
{
	/*
	 * Animate a ball popping and then kill it off.
	 */

	static int slide = BIRTH_SLIDES + 1;

	if (frame == balls[i].nextFrame)
	{
		/* We are imploding so go backwards through slides */
		slide--;

		/* Wait for the next frame */
		balls[i].nextFrame += BIRTH_FRAME_RATE;

		/* First frame is to clear the ball away */
		if (slide == BIRTH_SLIDES)
		{
			/* Clear the ball area */
    		EraseTheBall(display, window, balls[i].oldx, balls[i].oldy);
			slide--;
		}

		if (slide < 0)
		{
			/* Erase the ball birth image */
			MoveBallBirth(display, window, 
				balls[i].oldx, balls[i].oldy, -1, True, i);

			slide = BIRTH_SLIDES + 1;

			/* Stop the ball by killing it! */
			ClearBall(i);
			ResetBallStart(display, window);
			DeadBall(display, window);
		}
		else
			/* Draw ball birth - handles ball moving as well */
			MoveBallBirth(display, window, 
				balls[i].oldx, balls[i].oldy, slide, True, i);
	}
}

static void AnimateBallCreate(Display *display, Window window, int i)
{
	/*
	 * Animate a ball being created and then make it READY on the paddle.
	 */

	static int slide = 0;

	/* Draw the ball birth at the new position */
	MoveBallBirth(display, window, paddlePos, 
		PLAY_HEIGHT - DIST_BALL_OF_PADDLE, slide, True, i);

	if (frame == balls[i].nextFrame)
	{
		/* Next slide thanks */
		slide++;

		/* Frame that will trigger the new slide */
		balls[i].nextFrame += BIRTH_FRAME_RATE;

		if (slide == BIRTH_SLIDES)
		{
			/* Erase the ball birth image */
			MoveBallBirth(display, window, 
				paddlePos, PLAY_HEIGHT - DIST_BALL_OF_PADDLE, -1, 
				True, i);

			slide = 0;

			updateBallVariables(i);

			MoveBall(display, window, paddlePos, 
				PLAY_HEIGHT - DIST_BALL_OF_PADDLE, True, i);

			ChangeBallMode(BALL_READY, i);

			/* This frame will trigger the auto shoot off the ball if you
			 * don't press space within a specified time 
			 */
			balls[i].nextFrame = frame + BALL_AUTO_ACTIVE_DELAY;
		}
		else
			MoveBallBirth(display, window, 
				paddlePos, PLAY_HEIGHT - DIST_BALL_OF_PADDLE, 
				slide, True, i);

		if (paddleIsMoving())
			updateBallVariables(i);
	}
}

void ChangeBallMode(enum BallStates newMode, int i)
{
	/*
	 * Change the mode of a ball to a new one.
	 */

	/* Change the ball mode */
	balls[i].ballState = newMode;
}

static void SetBallWait(enum BallStates newMode, int waitFrame, int i)
{
	/*
	 * While the ball is waiting stay in ball wait mode.
	 */

	/* Set up the ball waiting loop */
	balls[i].waitingFrame	= waitFrame;
	balls[i].waitMode 		= newMode;
	balls[i].ballState 		= BALL_WAIT;
}

static void DoBallWait(int i)
{
	/*
	 * Do the waiting for a ball that is in BALL_WAIT mode.
	 */

	/* Once the waiting frame is reached then activate new state */
	if (frame == balls[i].waitingFrame)
	{
		balls[i].nextFrame = frame + 10;
		balls[i].ballState = balls[i].waitMode;
	}
}

int AddANewBall(Display *display, int x, int y, int dx, int dy)
{
	/*
	 * Function that adds a new ball.
	 *
	 * If one exists to be added it sets up the structure and returns its
	 * index into the ball array. If it fails then -1 is returned.
	 */

	int i;

	/* Add a new ball to the balls array */
	for (i = 0; i < MAX_BALLS; i++)
	{
		/* Is the ball free for us to use? */
		if (balls[i].active == False && GetNumberLife() >= 0)
		{
			/* Make sure that it is clear */
			ClearBall(i);

			/* We have found a new ball spot so setup the ball */
			balls[i].active 	= True;
			balls[i].ballx 		= x;
			balls[i].bally 		= y;
			balls[i].oldx 		= balls[i].ballx;
			balls[i].oldy 		= balls[i].bally;
			balls[i].dx 		= dx;
			balls[i].dy 		= dy;
			balls[i].ballState 	= BALL_CREATE;
			balls[i].mass 		= (rand() % (int)MAX_BALL_MASS) + MIN_BALL_MASS;
			balls[i].slide 		= 0;
			balls[i].nextFrame 	= frame + BIRTH_FRAME_RATE;

			DEBUG("Added new ball to arena.");
			return i;
		}
	}

	/* No more free balls available */
	WarningMessage("Cannot create a new ball - all slots full.");

	DEBUG("Cannot create a new ball as all slots are full.");

	return -1;
}

void ClearBall(int i)
{
	/*
	 * Initialise all the ball structure to default values.
	 */

	balls[i].waitMode 			= BALL_NONE;
	balls[i].waitingFrame 		= 0;
	balls[i].lastPaddleHitFrame = 0;
	balls[i].nextFrame 			= 0;
	balls[i].newMode 			= BALL_NONE;
	balls[i].active 			= False;
	balls[i].oldx 				= 0;
	balls[i].oldy 				= 0;
	balls[i].ballx 				= 0;
	balls[i].bally 				= 0;
	balls[i].dx 				= 0;
	balls[i].dy 				= 0;
	balls[i].slide 				= 0;
	balls[i].radius 			= BALL_WC;
	balls[i].mass 			 	= MIN_BALL_MASS;
	balls[i].ballState 			= BALL_CREATE;
}

void ClearAllBalls(void)
{
	/*
	 * Clear all ball data so we can start afresh next time.
	 */

	int i;

	DEBUG("Clearing all balls from slots.");

	/* Clear all the balls in the balls array */
	for (i = 0; i < MAX_BALLS; i++)
	{
		/* "Clear the ball" - in an American accent */
		ClearBall(i);
	}
}

void HandleBallMode(Display *display, Window window)
{
	/*
	 * Handle each active ball and handle each of their modes.
	 *
	 * This function is called very heavily. ;-(
	 */

	int i;

	/* Loop through all the balls */
	for (i = 0; i < MAX_BALLS; i++)
	{
		/* Only handle active balls - sounds disgusting! :-) */
		if (balls[i].active == True)
		{
			/* Switch on the state of the ball */
			switch (balls[i].ballState)
			{
				case BALL_POP:		/* Ball pop animation */
					AnimateBallPop(display, window, i);
					break;

				case BALL_ACTIVE:	/* Animate the ball normally */
					if ((frame % BALL_FRAME_RATE) == 0)
						UpdateABall(display, window, i);
					break;

				case BALL_READY:	/* ball created and waiting to move */
					if (paddleIsMoving())
					{
						balls[i].ballx = paddlePos;
						balls[i].bally = PLAY_HEIGHT - DIST_BALL_OF_PADDLE;

						MoveBall(display, window, balls[i].ballx, 
							balls[i].bally, True, i);
					}

					if ((frame % (BALL_FRAME_RATE)) == 0)
						MoveGuides(display, window, i, False);

					/* After a certain number of seconds fire off anyway */
					if (frame == balls[i].nextFrame)
					{
						ChangeBallMode(BALL_ACTIVE, i);
						ChangeBallDirectionToGuide(i);
						MoveGuides(display, window, i, True);
					}
					break;

				case BALL_STOP:		/* Ball dead and stopped */
					break;

				case BALL_CREATE:	/* Create ball animation */
					AnimateBallCreate(display, window, i);
					break;

				case BALL_WAIT:		/* In wait mode waiting to change state */
					DoBallWait(i);
					break;

				case BALL_DIE:		/* Ball is going to die */
					if ((frame % BALL_FRAME_RATE) == 0)
						UpdateABall(display, window, i);
					break;

				case BALL_NONE:		/* Really cool mode ;-) */
				default:
					break;

			}	/* Ball modes */
		}	/* If active */	
	}	/* For loop */
}
