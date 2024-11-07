/**
 * @file sfx.h
 * @author Gerardo Gonzalez
 * @date 2024-10-01
 * @brief Special effects implementation for xboing
 * 
 * This header file houses the functions for the implementation of various visual effects used in xboing including shaking, blinding, and fading effects for game elements
 */


#ifndef _SFX_H_
#define _SFX_H_

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
 * $Id: sfx.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/sfx.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: sfx.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:53  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Dependencies on other include files:
 */

// TODO: Remove instances of X11-associated data types
#include "faketypes.h"
// #include <X11/Xlib.h>

/*
 *  Constants and macros:
 */

// [DOXYGEN] Could replace list with enum
#define SFX_NONE        0
#define SFX_SHAKE      	1
#define SFX_FADE  		2
#define SFX_BLIND  		3
#define SFX_SHATTER  	4
#define SFX_STATIC  	5

/*
 *  Function prototypes:
 */

/**
 * Accessor for modifying the value of the sfxEndFrame variable.
 *
 * @param int endFrame The frame where the special effects should end.
 *
 *
 */
void SetSfxEndFrame(int endFrame);

/**
 * Accessor for modifying the value of the modeSfx variable
 *
 * @param int newMode The new mode for the special effects.
 *
 */
void changeSfxMode(int newMode);

// [DOXYGEN] Undefined function. Should be removed
int oldSfxMode(void);

/**
 * Accessor for the current state of modeSfx variable
 *
 * @return int The current special effects mode
 *
 */
int currentSfxMode(void);

/**
 * Creates a window shake effect.
 *
 * @param Display *display The display of the X11 window
 * @param Window window The X11 window to draw on
 *
 * @return int True while the effect is going, False when the effect is finished.
 *
 */
int WindowShakeEffect(Display *display, Window window);

/**
 * Creates a window shattering effect.
 *
 * @param Display *display The display of the x11 window
 * @param Window window The X11 window to draw on.
 *
 * @return int False when done.
 *
 */
int WindowShatterEffect(Display *display, Window window);

/**
 * Creates a window blind closing effect on the screen.
 *
 * @param Display *display The display of the X11 window
 * @param Window window The X11 window to draw on
 *
 */
int WindowBlindEffect(Display *display, Window window);

/**
 * Creates a window fade effect using horizontal and vertical bars.
 *
 * @param Display *display The displat to the X11 window
 * @param Window window The X11 window to draw on
 * @param int w The width of the window.
 * @param int h The height of the window.
 *
 * @return int True while the effect is going, False when the effect is finished.
 *
 */
int WindowFadeEffect(Display *display, Window window, int w, int h);

/**
 * Creates a static effect on the screen.
 *
 * @param Display *display The display of the X11 window
 * @param Window window The X11 window to draw on
 * @param int w The width of the window.
 * @param int h The height of the window.
 * @return int True while the effect is going, False when the effect is done.
 */
int WindowStaticEffect(Display *display, Window window, int w, int h);

/**
 * Accessor for useSfx variable (return value)
 *
 * @param Display *display The display of the X11 window
 *
 */
int getSpecialEffects(Display *display);

/**
 * Accesor used for modyfying the state of special effects.
 *
 * @param int state True to enable special effects, False to disable.
 *                                                                                                                          */
void useSpecialEffects(int state);

/**
 * Resets the window border.
 *
 * @param Display *display The display of the X11 window
 * @param Window window The X11 window to draw on
 *
 */
void ResetBorderGlow(Display *display, Window window);

/**
 * Creates a glowing border effect.
 *
 * @param Display *display The display of the X11 window
 * @param Window window The X11 window to draw on
 *
 */
void BorderGlow(Display *display, Window window);

/**
 * Creates a fade-away effect in specified area.
 *
 * @param Display *display The display of the X11 window
 * @param Window window The X11 window to draw on
 * @param int x The x coordinate of the area.
 * @param int y The y coordinate of the area
 * @param int w The width of the area.
 * @param int h The height of hte area.
 *
 */
void FadeAwayArea(Display *display, Window window, int x, int y, int w, int h);

#endif
