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
 * $Id: gun.c,v 1.1.1.1 1994/12/16 01:36:44 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/gun.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:44 $
 *
 * $Log: gun.c,v $
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

//#include <xpm.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
//#include <X11/Xos.h>

//#include "bitmaps/guns/bullet.xpm"
//#include "bitmaps/guns/tink.xpm"

#include "include/faketypes.h"
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

#include "include/gun.h"

/*
 *  Internal macro definitions:
 */

#define BULLET_DY			-7

#define BULLET_WIDTH		7
#define BULLET_HEIGHT		16

#define BULLET_START_Y		(PLAY_HEIGHT - 40)

#define BULLET_WC			(BULLET_WIDTH / 2)
#define BULLET_HC			(BULLET_HEIGHT / 2)

#define TINK_WIDTH			10
#define TINK_HEIGHT			5

#define TINK_WC				(TINK_WIDTH / 2)
#define TINK_HC				(TINK_HEIGHT / 2)

#define X2COL(col, x) 		(col = x / colWidth)
#define Y2ROW(row, y) 		(row = y / rowHeight)

#define BULLET_FRAME_RATE	3

/* Should be the same */
#define MAX_MOVING_BULLETS	40	
#define MAX_TINKS			40	

#define TINK_DELAY			100	

/*
 *  Internal type declarations:
 */

static int 	ResetBulletStart(Display *display, Window window);
static void CheckTinks(Display *display, Window window);
static void AddTink(Display *display, Window window, int xpos);
static void ClearTinks(void);
static void UpdateBullet(Display *display, Window window);
static int	StartABullet(Display *display, Window window, int xpos);
static void ClearBullet(int i);
static void DrawTheTink(Display *display, Window window, int x, int y);
static void EraseTheTink(Display *display, Window window, int x, int y);
static void DrawBullet(Display *display, Window window, int i);
static int CheckForBulletCollision(Display *display, Window window, 
	int x, int y);
static int CheckBallBulletCollision(Display *display, Window window, 
	int bx, int by, int j);
static int CheckEyeDudeBulletCollision(Display *display, Window window, 
	int bx, int by);

static struct 
{
	int xpos;			/* x position of tink centre */
	int	clearFrame;		/* Last frame to clear it */
} tinks[MAX_TINKS];

static struct 
{
	int xpos;			/* x position of bullet */
	int ypos;			/* y position of bullet */
	int oldypos;		/* previous y position */
	int dy;				/* Change in y positoon */
} bullets[MAX_MOVING_BULLETS];

/*
 *  Internal variable declarations:
 */

static Pixmap bulletPixmap, bulletMask;
static Pixmap tinkPixmap, tinkMask;
static int numBullets, unlimitedBullets;

void InitialiseBullet(Display *display, Window window, Colormap colormap)
{
    XpmAttributes   attributes;
	int		    XpmErrorStatus;

    attributes.valuemask = XpmColormap;
	attributes.colormap = colormap;

	/* Create the xpm pixmap bullet */
	XpmErrorStatus = XpmCreatePixmapFromData(display, window, bullet_xpm,
		&bulletPixmap, &bulletMask, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(bullet)");

	/* Create the xpm pixmap tink for bullet */
	XpmErrorStatus = XpmCreatePixmapFromData(display, window, tink_xpm,
		&tinkPixmap, &tinkMask, &attributes);
	HandleXPMError(display, XpmErrorStatus, "InitialiseBullet(tink)");

    /* Free the xpm pixmap attributes */
	XpmFreeAttributes(&attributes);

	SetNumberBullets(4);
	ClearTinks();
	ClearBullets();
}

static void CheckTinks(Display *display, Window window)
{
	int i;

	/* Clear and tinks that need to be cleared */
	for (i = 0; i < MAX_TINKS; i++)
	{
		/* Is this tink active */
		if (tinks[i].xpos != -1)
		{
			/* Time to clear tink? */
			if (frame >= tinks[i].clearFrame)
			{
				/* Clear the tink! */
				EraseTheTink(display, window, tinks[i].xpos, 2);
					
				/* Free the tink up for another */
				tinks[i].xpos = -1;
				tinks[i].clearFrame = 0;
			}
		}
	}
}

static void AddTink(Display *display, Window window, int xpos)
{
	int i;

	/* Cycle through tinks and try to add one */
	for (i = 0; i < MAX_TINKS; i++)
	{
		/* Is this tink free? */
		if (tinks[i].xpos == -1)
		{
			/* Set the tink array position */
			tinks[i].xpos = xpos;
			tinks[i].clearFrame = frame + TINK_DELAY;

			/* Draw the new found tink! */
			DrawTheTink(display, window, xpos, 2);

			if (noSound == False) playSoundFile("shoot", 80);

			return;
		}
	}

	/* Full tink array - lots of shooting? */
	WarningMessage("Cannot draw tink - tink array full.");
}

static void ClearTinks(void)
{
	int i;

	/* Initialise tinks array to empty */
	for (i = 0; i < MAX_TINKS; i++)
	{
		tinks[i].xpos = -1;
		tinks[i].clearFrame = 0;
	}
}


static void UpdateBullet(Display *display, Window window)
{
	int i, j;
	int row, col;
	int ballX, ballY;
	struct aBlock *blockP;

	/* Obtain the position of the ball */
	GetBallPosition(&ballX, &ballY, 0);

	/* Draw all bullets that need updating */
	for (i = 0; i < MAX_MOVING_BULLETS; i++)
	{
		/* Is this bullet active */
		if (bullets[i].xpos != -1)
		{
			/* Update bullet position using dy value */	
			bullets[i].ypos = bullets[i].oldypos + bullets[i].dy;

			/* Has the bullet gone off the top edge */
			if (bullets[i].ypos < -BULLET_HC)
			{
				/* Clear the bullet from the screen */
				EraseTheBullet(display, window, 
					bullets[i].xpos, bullets[i].oldypos);

				/* Draw a tink on the top edge */
				AddTink(display, window, bullets[i].xpos);

				/* Free the bullet up for another */
				ClearBullet(i);

				continue;
			}

			for (j = 0; j < MAX_BALLS; j++)
			{
				if (balls[j].active == True)
				{
					/* Has the bullet killed the ball */
					if (CheckBallBulletCollision(display, window, 
						bullets[i].xpos, bullets[i].ypos, j))
					{
						/* Clear the bullet from the screen */
						EraseTheBullet(display, window, 
							bullets[i].xpos, bullets[i].oldypos);
						ClearBullet(i);

						/* Kill the ball off */
						ClearBallNow(display, window, j);

						/* Play the lovel ahhh pop sound for ball shot */
						if (noSound == False) playSoundFile("ballshot", 50);
						break;
					}
				}
			}

			/* Convert the new bullet pos to rows and cols for collision */
			X2COL(col, bullets[i].xpos);
			Y2ROW(row, bullets[i].ypos);

			/* Pointer to the correct block we need - speed things up */
			blockP = &blocks[row][col];

			if (getEyeDudeMode() == EYEDUDE_WALK)
			{
				/* See if the bullet has hit the active eyedude */
				if (CheckEyeDudeBulletCollision(display, window, 
					bullets[i].xpos, bullets[i].ypos) == True)
				{
					/* Clear the bullet from the screen */
					EraseTheBullet(display, window, 
						bullets[i].xpos, bullets[i].oldypos);

					/* Ok so the eyedude has been hit - arrggh */
					ChangeEyeDudeMode(EYEDUDE_DIE);

					/* Free the bullet up for another */
					ClearBullet(i);
				}
			}

			/* Check if the bullet has hit a brick or something */
			if (CheckForBulletCollision(display, window, 
				bullets[i].xpos, bullets[i].ypos) == True)
			{
				/* Clear the bullet from the screen */
				EraseTheBullet(display, window, 
					bullets[i].xpos, bullets[i].oldypos);

				/* Switch on the type of block hit */
				switch (blockP->blockType)
				{
					case COUNTER_BLK:
						if (blockP->counterSlide == 0)
						{
							/* Counter has counted down to 0 so kill off */
							DrawBlock(display, window, row, col, KILL_BLK);
						}
						else
						{
							/* Decrement counter block and draw new one */
							blockP->counterSlide--;
							DrawBlock(display, window, row, col, 
								COUNTER_BLK);
						}
						break;

					case HYPERSPACE_BLK:
						/* Do nothing - redraw the block */
						DrawBlock(display, window, row, col, HYPERSPACE_BLK);
						break;

					case BLACK_BLK:
						/* Do nothing - redraw the block */
						DrawBlock(display, window, row, col, BLACK_BLK);
						break;

					case REVERSE_BLK:		
					case MGUN_BLK:		
					case STICKY_BLK:		
					case WALLOFF_BLK:		
					case MULTIBALL_BLK:		
					case PAD_EXPAND_BLK:		
					case PAD_SHRINK_BLK:		
					case DEATH_BLK:		
						/* Shoot the block times to kill it */
						blockP->counterSlide--;

						if (blockP->counterSlide == 0)
						{
							/* Ok then a hit, explode that block */
							DrawBlock(display, window, row, col, KILL_BLK);
						}
						break;

					default:
						/* Ok then a hit, explode that block */
						DrawBlock(display, window, row, col, KILL_BLK);
						break;
				}

				/* Free the bullet up for another */
				ClearBullet(i);
			}
			else
				DrawBullet(display, window, i);

			/* Keep track of old position */
			bullets[i].oldypos = bullets[i].ypos;

		}	/* Bullet active? */
	}	/* For loop */
}


static int StartABullet(Display *display, Window window, int xpos)
{
	int i;

	/* Cycle through bullets and try to add one */
	for (i = 0; i < MAX_MOVING_BULLETS; i++)
	{
		/* Is this bullet free? */
		if (bullets[i].xpos == -1)
		{
			/* Set the bullet array position */
			bullets[i].xpos = xpos;

			/* Get out of here */
			return True;
		}
		
		/* Break out as the machine gun is not active */
		if (fastGun == False) return False;
	}

	/* Full moving bullet array - lots of shooting? */
	WarningMessage("Cannot draw bullet - bullet array full.");

	return False;
}

static void ClearBullet(int i)
{
	/* Setup the bullet entry */
	bullets[i].xpos 			= -1;
	bullets[i].ypos 			= BULLET_START_Y;
	bullets[i].oldypos 			= BULLET_START_Y;
	bullets[i].dy 				= BULLET_DY;
}

void ClearBullets(void)
{
	int i;

	/* Initialise bullets array to empty */
	for (i = 0; i < MAX_MOVING_BULLETS; i++)
		ClearBullet(i);
}

void FreeBullet(Display *display)
{
	if (bulletPixmap)	XFreePixmap(display, bulletPixmap);
 	if (bulletMask) 	XFreePixmap(display, bulletMask);

	if (tinkPixmap)	XFreePixmap(display, tinkPixmap);
 	if (tinkMask) 	XFreePixmap(display, tinkMask);
}

void SetNumberBullets(int num)
{
	/* Set the number of bullets available */
	numBullets = num;
}

void IncNumberBullets(void)
{
	/* Increment the number of bullets */
	numBullets++;

	/* But don't give to many */
	if (numBullets > MAX_BULLETS) 
		numBullets = MAX_BULLETS;
}

void SetUnlimitedBullets(int state)
{
	/* Set the unlimit bullets state */
	unlimitedBullets = state;
}

void DecNumberBullets(void)
{
	/* Only decrement number of bullets if the unlimited ammo is off */
	if (unlimitedBullets == False)
	{
		/* Decrement the number of bullets */
		numBullets--;

		/* But not to far */
		if (numBullets < 0) 
			numBullets = 0;
	}
}

int GetNumberBullets(void)
{
	assert(numBullets >= 0);

	/* How many bullets do I have */
	return numBullets;
}

void shootBullet(Display *display, Window window)
{
	/* Only shoot if no bullet is active and bullets and ball active */
	if ((GetNumberBullets() > 0) && (IsBallWaiting() == False))
	{
		/* Reset the bullet to the starting possy to go forward */
		if (ResetBulletStart(display, window) == True)
		{
			/* Remove a bullet from the ammunition */
			DeleteABullet(display);

			/* Play a shooting sound */
			if (noSound == False) playSoundFile("shotgun", 50);
		}
	}
	else if (GetNumberBullets() == 0)
	{
		/* Play an trigger clicking sound */
		if (noSound == False) playSoundFile("click", 99);
	}
}

void EraseTheBullet(Display *display, Window window, int x, int y)
{
	/* Erase the bullet pixmap from the window */
    XClearArea(display, window, x - BULLET_WC, y - BULLET_HC, 
		BULLET_WIDTH, BULLET_HEIGHT, False);
}

static void DrawTheTink(Display *display, Window window, int x, int y)
{
	/* Draw the tink pixmap into the window */
    RenderShape(display, window, tinkPixmap, tinkMask,
		x - TINK_WC, y - TINK_HC, TINK_WIDTH, TINK_HEIGHT, False);
}

static void EraseTheTink(Display *display, Window window, int x, int y)
{
	/* Erase the tink pixmap from the window */
    XClearArea(display, window, x - TINK_WC, y - TINK_HC, 
		TINK_WIDTH, TINK_HEIGHT, False);
}

void DrawTheBullet(Display *display, Window window, int x, int y)
{
	/* Draw the bullet pixmap into the window */
    RenderShape(display, window, bulletPixmap, bulletMask,
		x - BULLET_WC, y - BULLET_HC, BULLET_WIDTH, BULLET_HEIGHT, False);
}

static void DrawBullet(Display *display, Window window, int i)
{
	/* Clear the window of the bullet in the old position */
	XClearArea(display, window, bullets[i].xpos - BULLET_WC, 
		bullets[i].oldypos - BULLET_HC, BULLET_WIDTH, BULLET_HEIGHT, False);

	/* Now draw the new bullet in the new position */
	DrawTheBullet(display, window, bullets[i].xpos, bullets[i].ypos);
}

static int CheckEyeDudeBulletCollision(Display *display, Window window, 
	int bx, int by)
{
	/*
	 * Check if the bullet has hit an eye dude.
	 */

	int eyeX, eyeY;

	GetEyeDudePosition(&eyeX, &eyeY);

    /* Check if any part of the bullets coords is inside the eyedudes box */
    if (((bx + BULLET_WC) >= (eyeX - EYEDUDE_WC)) &&
   	    ((bx - BULLET_WC) <= (eyeX + EYEDUDE_WC)) &&
        ((by + BULLET_HC) >= (eyeY - EYEDUDE_WC)) &&
        ((by - BULLET_HC) <= (eyeY + EYEDUDE_WC)))
		return True;
	else
		return False;
}

static int CheckBallBulletCollision(Display *display, Window window, 
	int bx, int by, int j)
{
	int ballX, ballY;

	GetBallPosition(&ballX, &ballY, j);

    /* Check if any part of the bullets coords is inside the balls box */
    if (((bx + BULLET_WC) >= (ballX - BALL_WC)) &&
   	    ((bx - BULLET_WC) <= (ballX + BALL_WC)) &&
        ((by + BULLET_HC) >= (ballY - BALL_HC)) &&
        ((by - BULLET_HC) <= (ballY + BALL_HC)))
		return True;
	else
		return False;
}

static int CheckForBulletCollision(Display *display, Window window, 
	int x, int y)
{
    /* Check for bullet to block collision */
    int row, col;
    struct aBlock *blockP;

    /* Get the row and col for block where bullet is */
    X2COL(col, x);
    Y2ROW(row, y);

    blockP = &blocks[row][col];

    /* If blocks is occupied then check for collision */
    if (blockP->occupied == 1 && blockP->exploding == False)
    {
        /* Check if x adjusted for bullet width is in block region */
        if (((x + BULLET_WC) > blockP->x) &&
          ((x - BULLET_WC) < (blockP->x + blockP->width)))
        {
            if (((y + BULLET_HC) > blockP->y) &&
              ((y - BULLET_HC) < (blockP->y + blockP->height)))
            {
                /* Collision */
                return True;
            }
        }
    }

    /* No collision if reached here */
    return False;
}

static int ResetBulletStart(Display *display, Window window)
{
	int status, size;

	/* Start a bullet on the way if possible */
	if (fastGun == True) 
	{
		/* Obtain the size of the paddle */
		size = GetPaddleSize();

		/* Shoot 2 bullets - dual fire!!!!! Power */
		status = StartABullet(display, window, paddlePos - (size / 3));
		status = StartABullet(display, window, paddlePos + (size / 3));
	}
	else
		status = StartABullet(display, window, paddlePos);

	/* Return status of bullet */
	return status;
}

void HandleBulletMode(Display *display, Window window)
{
	/* Update all the bullets that may be moving */
	if ((frame % BULLET_FRAME_RATE) == 0)
		UpdateBullet(display, window);

	/* Clear any tinks that are due to be cleared */
	CheckTinks(display, window);
}

