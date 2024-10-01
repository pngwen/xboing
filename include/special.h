#ifndef _SPECIAL_H_
#define _SPECIAL_H_

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
 * $Id: special.h,v 1.1.1.1 1994/12/16 01:36:54 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/special.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:54 $
 *
 * $Log: special.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:54  jck
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

/*
 *  Constants and macros:
 */

#define REVERSE     1
#define STICKY      2

#define FLASH		500

/*
 *  Function prototypes:
 */

/**
 * @brief Draws graphical elements of specials
 * 
 * @param display The current display of the X11 window
 */
void DrawSpecials(Display *display);

/**
 * @brief Accessor for modifying fastGun
 * 
 * @param display The current display of the X11 window
 * @param state The state to set fastGun to
 * 
 * @todo Remove function and replace calls with contents
 */
void ToggleFastGun(Display *display, int state);

/**
 * @brief Turns the walls on or off (also updates state of noWalls)
 * 
 * @param display The current display of the X11 window
 * @param state The state to set noWalls to (turn walls on/off)
 */
void ToggleWallsOn(Display *display, int state);

/**
 * @brief Turns off / disables special characteristics and bonuses
 * 
 * @param display The current display of the X11 window
 */
void TurnSpecialsOff(Display *display);

/**
 * @brief Accessor for modifying stickyBat
 * 
 * @param display The current display of the X11 window
 * @param state The state to set stickyBat to
 * 
 * @todo Remove function and replace calls with contents
 */
void ToggleStickyBat(Display *display, int state);

/**
 * @brief Accessor for modifying saving
 * 
 * @param display The current display of the X11 window
 * @param state The state to set saving to
 * 
 * @todo Remove function and replace calls with contents
 */
void ToggleSaving(Display *display, int state);

/**
 * @brief Accessor for modifying x2Bonus (set x2 bonus)
 * 
 * @param display The current display of the X11 window
 * @param state The state to set x2Bonus to
 * @todo Remove function and replace calls with contents
 */
void Togglex2Bonus(Display *display, int state);

/**
 * @brief Accessor for modifying x4Bonus (set x4 bonus)
 * 
 * @param display The current display of the X11 window
 * @param state The state to set x4Bonus to
 * 
 * @todo Remove function and replace calls with contents
 */
void Togglex4Bonus(Display *display, int state);

/**
 * @brief Accessor for modifying Killer
 * 
 * @param display The current display of the X11 window
 * @param state The state to set Killer to
 * 
 * @todo Remove function and replace calls with contents
 */
void ToggleKiller(Display *display, int state);

/**
 * @brief Randomizes the state of specials& changes score and level number
 * 
 * @param display The current display of the X11 window
 */
void RandomDrawSpecials(Display *display);

/**
 * @brief The state (true/false) of various specials
 * 
 */
extern int saving;
extern int stickyBat;
extern int fastGun;
extern int noWalls;
extern int Killer;
extern int x2Bonus;
extern int x4Bonus;

#endif