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
 * $Id: sfx.c,v 1.1.1.1 1994/12/16 01:36:45 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/sfx.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:45 $
 *
 * $Log: sfx.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:45  jck
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

// TODO: Remove functional reliance on X11
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xos.h>
// #include <xpm.h>

#include "error.h"
#include "init.h"
#include "stage.h"
#include "score.h"
#include "blocks.h"
#include "ball.h"
#include "main.h"
#include "mess.h"
#include "misc.h"
#include "intro.h"

#include "sfx.h"

/*
 *  Internal macro definitions:
 */

#define SHAKE_DELAY			5
#define NUM_SCAT 			10 
#define RANDY(range) 		(rand() % (range))

/*
 *  Internal variable declarations:
 */

static int sfxEndFrame, useSfx;
int modeSfx;
static int xscat[NUM_SCAT] = { 1, 9, 3, 6, 2, 4, 0, 7, 5, 8 };
static int yscat[NUM_SCAT] = { 2, 1, 0, 8, 6, 4, 9, 3, 7, 5 };

/**
 * @brief Enables or disables special effects.
 *
 * This function sets the state of special effects based on the given parameter.
 *
 * @param state A boolean value to enable (non-zero) or disable (zero) special effects.
 */
void useSpecialEffects(int state)
{
	/* Set the state of the special effects - True = use */
	/* Of course - if the sfx are not possible then no effect */
	useSfx = state;
}

/**
 * @brief Gets the current state of special effects.
 *
 * This function checks if the display supports backing store and returns
 * the current state of special effects.
 *
 * @param display Pointer to the X Display structure.
 * @return int Returns 1 if special effects are enabled, 0 if disabled, and -1 if not supported.
 */
int getSpecialEffects(Display *display)
{
	/* Only shake around if the server has backing store on */
	if (DoesBackingStore(XDefaultScreenOfDisplay(display)) != Always)
		return -1;

	/* Return special effects state - on or off */
	return useSfx;
}

/**
 * @brief Changes the current mode of special effects.
 *
 * This function sets the new special effects mode.
 *
 * @param newMode The new mode to set for special effects.
 */
void changeSfxMode(int newMode)
{
	modeSfx = newMode;
}


/**
 * @brief Retrieves the current special effects mode.
 *
 * @return int Returns the current special effects mode.
 */
int currentSfxMode(void)
{
	/* Return the current special effects mode */
	return modeSfx;
}

/**
 * @brief Resets the current special effect.
 *
 * This function re-centers the game window to its original position
 * and turns off the current special effect.
 *
 * @param display Pointer to the X Display structure.
 */
static void resetEffect(Display *display)
{
	/* Just re-centre window return */
	modeSfx = SFX_NONE;
	XMoveWindow(display, playWindow, 35, 60);
}

/**
 * @brief Creates a window blind closing effect.
 *
 * This function creates a visual effect where blinds appear to close over
 * the specified window.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to apply the effect.
 * @return int Returns 0 (False) to indicate completion of the effect.
 */
int WindowBlindEffect(Display *display, Window window)
{
	int x, i;

	/* Does the user want special effects */
	if (useSfx == False)
	{
		/* No - Just return out */
		resetEffect(display);
		return False;
	}

	XSetBackground(display, gcsfx, black);

	/* Draw a blinds effect where little doors close over screen */
	for (i = 0; i <= (PLAY_WIDTH / 8); i++)
		for (x = 0; x <= PLAY_WIDTH; x += (PLAY_WIDTH / 8))
			XCopyArea(display, bufferWindow, window, gc, 
				x+i, 0, 1, PLAY_HEIGHT, x+i, 0);

	/* End of special effect - reset off */
	resetEffect(display);
	return False;
}

/**
 * @brief Creates a static noise effect over the window.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to apply the effect.
 * @param w Width of the effect area.
 * @param h Height of the effect area.
 * @return int Returns 1 (True) while the effect is ongoing, 0 (False) when finished.
 */
int WindowStaticEffect(Display *display, Window window, int w, int h)
{
	static int start = True;

	if (start)
	{
		SetSfxEndFrame(frame + 50);
		start = False;
	}

	/* Do somehting in here */

	if (frame >= sfxEndFrame) 
	{
		/* End of special effect - reset off */
		resetEffect(display);
		start = True;
		return False;
	}

	return True;
}

/**
 * @brief Creates a window shattering effect.
 *
 * This effect simulates a window breaking into pieces and scattering.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to apply the effect.
 * @return int Returns 0 (False) after the effect is completed.
 */
int WindowShatterEffect(Display *display, Window window)
{
    int offx, offy, sizeWidth, sizeHeight;
    int srcx, srcy, destx, desty;

	/* Does the user want special effects */
	if (useSfx == False)
	{
		/* No - Just return out */
		resetEffect(display);
		return False;
	}


    offx = RANDY(NUM_SCAT);
    offy = RANDY(NUM_SCAT);
	sizeWidth = 200;
	sizeHeight = 200;

	/* Spend a bit of time scattering new pixmap into view */
	/* Original idea for this effect from xjewel */
    for (srcx = 0; srcx < NUM_SCAT; srcx++)
    {
    	for (srcy = 0; srcy < NUM_SCAT; srcy++)
        {
            for (destx = 0; destx <= 4; destx++)
			{
            	for (desty = 0; desty <= 5; desty++)
                {
					XCopyArea(display, bufferWindow, window, gc, 
                    	(destx * sizeWidth) + 
                        	xscat[(srcx + srcy + offx) % NUM_SCAT] 
							* (sizeWidth / NUM_SCAT),

                    	(desty * sizeHeight) + 
                        	yscat[(srcy + offy) % NUM_SCAT] 
							* (sizeHeight / NUM_SCAT),

                    	(sizeWidth  / NUM_SCAT), 
						(sizeHeight / NUM_SCAT),

                    	(destx * sizeWidth) + 
                        	xscat[(srcx + srcy + offx) % NUM_SCAT] 
							* (sizeWidth / NUM_SCAT),

                    	(desty * sizeHeight) + 
                        	yscat[(srcy + offy) % NUM_SCAT] 
							* (sizeHeight / NUM_SCAT));
               	}
			}
        }
    }

	/* End of special effect - reset off */
	resetEffect(display);
	return False;
}

/**
 * @brief Creates a window fade effect.
 *
 * This function fades a window in and out by drawing vertical and horizontal lines
 * over the window area, creating a fading effect.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to apply the effect.
 * @param w Width of the window.
 * @param h Height of the window.
 * @return int Returns 1 (True) while the effect is ongoing, 0 (False) when finished.
 */
int WindowFadeEffect(Display *display, Window window, int w, int h)
{
	static int done = False;
	static int first = True;
	int y;
	int x;
	static int i = 0;

	/* Does the user want special effects */
	if (useSfx == False)
	{
		/* No - Just return out */
		resetEffect(display);
		return False;
	}

	if (first == True)
	{
		first = False;
		XSetForeground(display, gcsfx, black);
		XSetBackground(display, gcsfx, black);
		XSetWindowBorder(display, window, red);
	}

	/* Draw vertical lines */
	for (x = i; x <= w; x += 12)
		XDrawLine(display, window, gcsfx, x, 0, x, h);

	/* Draw horizontal lines */
	for (y = i; y <= h; y += 12)
		XDrawLine(display, window, gcsfx, 0, y, w, y);

	/* Fill in grid slowly */
	i++;
	if (i > 12) done = True;

	if (done == True)
	{
		/* End of special effect - reset off */
		done = False;
		first = True;
		i = 0;
		resetEffect(display);
		return False;
	}

	/* Keep efect going please */
	return True;
}

/**
 * @brief Creates a window shake effect.
 *
 * This function shakes a window by periodically changing its position slightly.
 * The shake stops after a certain number of frames.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to apply the effect.
 * @return int Returns 1 (True) while the effect is ongoing, 0 (False) when finished.
 */
int WindowShakeEffect(Display *display, Window window)
{
	static int x = 35;
	static int y = 60;
	int xi, yi;

	/* Does the user want special effects */
	if (useSfx == False)
	{
		/* No - Just return out */
		resetEffect(display);
		return False;
	}

	/* Only shake around if the server has backing store on */
	if (DoesBackingStore(XDefaultScreenOfDisplay(display)) != Always)
	{
		resetEffect(display);
		return False;
	}

	if (frame >= sfxEndFrame) 
	{
		/* End of special effect - reset off */
		resetEffect(display);
		return False;
	}
	
	if ((frame % SHAKE_DELAY) != 0) return True;

	XMoveWindow(display, window, x, y);
	XFlush(display);

	xi = (rand() % 6) - 3; 
	yi = (rand() % 6) - 3; 
	x = xi + 35; y = yi + 60;

	return True;
}

/**
 * @brief Sets the frame at which the special effects end.
 *
 * This function sets the value for `sfxEndFrame`, which determines when
 * special effects should stop.
 *
 * @param endFrame The frame number to end the special effects.
 */
void SetSfxEndFrame(int endFrame)
{
	sfxEndFrame = endFrame;
}

/**
 * @brief Creates a border glow effect.
 *
 * This function alternates the border color of a window between red and green,
 * giving a glowing effect.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to apply the effect.
 */
void BorderGlow(Display *display, Window window)
{
    static int i = 0;
    static int d = 1;
    static int t = 1;

	/* Does the user want special effects */
	if (useSfx == False)
	{
		/* No - Just return out */
		return;
 	}

	/* Only update every n frames */
    if ((frame % 40) == 0)
    {
		/* Alternate between the red and the green ranges */
        if (t > 0)
            XSetWindowBorder(display, playWindow, reds[i]);
        else
            XSetWindowBorder(display, playWindow, greens[i]);

		/* Ok change range or fade down again */
        if (i == 6)
        {
            d = -1;
            t *= -1;
        }

        if (i == 0)
            d = 1;

		/* Next range index */
        i += d;
     }
}

/**
 * @brief Resets the border glow effect.
 *
 * This function resets the border of the window to its original color.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to reset.
 */
void ResetBorderGlow(Display *display, Window window)
{
    XSetWindowBorder(display, playWindow, red);
}

/**
 * @brief Fades away an area of the window.
 *
 * This function fades out an area of the window by slowly clearing the pixels
 * in a grid pattern.
 *
 * @param display Pointer to the X Display structure.
 * @param window The X Window to apply the effect.
 * @param x X-coordinate of the area to fade.
 * @param y Y-coordinate of the area to fade.
 * @param w Width of the area to fade.
 * @param h Height of the area to fade.
 */
void FadeAwayArea(Display *display, Window window, int x, int y, int w, int h)
{
	int i, x1, y1, step;

	step = w / 15;

	/* Fade away an area slowly so it looks good */
	for (i = 0; i <= step; i++)
	{
		/* Draw vertical lines */
		for (x1 = i; x1 <= w; x1 += 15)
			XClearArea(display, window, x + x1, y, 1, h, False);
	
		/* Draw horizontal lines */
		for (y1 = i; y1 <= h; y1 += 15)
			XClearArea(display, window,  x, y + y1, w, 1, False);
	}
}
