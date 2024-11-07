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

// TODO: Remove instances of X11-associated data types
#include "faketypes.h"

// #include <X11/Xlib.h>

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
 * Draws text that tells the player what special powerup they have
 *
 * @param Display *display display pointer
 * @pre in a game
 * @return void 
 * @post displays powerup name
 * 
 */
void DrawSpecials(Display *display);

/**
 * toggles state of fast gun powerup
 *
 * @param Display *display pointer display
 * @param int state boolean: state of gun
 * @pre in a game
 * @return void 
 * @post toggles powerup 
 * 
 */
void ToggleFastGun(Display *display, int state);

/**
 * toggles window walls
 *
 * @param Display *display display pointer
 * @param int state boolean: state of window border'
 * @pre 
 * @return void 
 * @post toggle window border
 * 
 */
void ToggleWallsOn(Display *display, int state);

/**
 * turns off all specials
 *
 * @param Display *display display pointer
 * @pre 
 * @return void 
 * @post turns specials off 
 * 
 */
void TurnSpecialsOff(Display *display);

/**
 * toggtles Sticky Bat powerup
 *
 * @param Display *display display pointer
 * @param int state Boolean: either on or off
 * @pre in a game
 * @return void 
 * @post  toggles stickybat
 * 
 */
void ToggleStickyBat(Display *display, int state);

/**
 * toggles whether game can be saved or not
 *
 * @param Display *display display pointer
 * @param int state boolean: either can save or can't
 * @pre in a game
 * @return void 
 * @post toggles save feature
 * 
 */
void ToggleSaving(Display *display, int state);

/**
 * toggles 2x score bonus
 *
 * @param Display *display display pointer
 * @param int state Boolean: bonus either on or off
 * @pre in a game
 * @return void 
 * @post toggles score bonus
 * 
 */
void Togglex2Bonus(Display *display, int state);

/**
 * toggles 4x score bonus
 *
 * @param Display *display diplay pointer
 * @param int state boolean: either on or off
 * @pre in a game
 * @return void 
 * @post toggles score bonus
 * 
 */
void Togglex4Bonus(Display *display, int state);

/**
 * toggles killer mode
 *
 * @param Display *display display pointer
 * @param int state Boolean: either on or off
 * @pre in a game
 * @return void 
 * @post killer mode is toggled
 * 
 */
void ToggleKiller(Display *display, int state);

/**
 * draws random special
 *
 * @param Display *display display pointer
 * @pre in a game
 * @return void 
 * @post randomly spawns powerup
 * 
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
