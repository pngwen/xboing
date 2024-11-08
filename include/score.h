/**
 * @file score.h
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Manages the user's score including drawing the digits on the screen.
 * @version 1.1.1.1
 * @date 16 Dec 1994
 *
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 *
 * Calculates the user's score and keeps track of it throughout the game.
 */

#ifndef _SCORE_H_
#define _SCORE_H_

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
 * $Id: score.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/score.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: score.h,v $
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

//#include <X11/Xlib.h>
#include "faketypes.h"

/*
 *  Constants and macros:
 */

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */


/**
 * @brief Intializes the score on the screen
 *
 * @param Display display X11 display
 * @param Window window X11 window size
 * @param Colormap colormap The color that the score needs to be
 * @pre The display, window, and color need to be selected
 * @post The score counter is displayed to the screen with the selected size and color
 *
 */
void InitialiseScoreDigits(Display *display, Window window, Colormap colormap);

/**
 * @brief Frees the memory for the score
 *
 *@param Display display X11 display
 *@pre
 *@post Memory taken up by the score is freed
 *
 */
void FreeScoreDigits(Display *display);

/**
 * @brief Clear the old score display and creates a new one with new score digits
 *
 * @param Display display X11 display
 * @param Window window X11 window size
 * @param u_long score The current score
 * @pre The old score must have an increment added to it
 * @post The old score is erased while the new score is displayed
 *
 */
void DisplayScore(Display *display, Window window, u_long score);

/**
 * @brief Checks if the current score is divisible by 10 and sets the last digit to zero.
  Checks the score it needs to draw then calls the Drawdigit function.
 *
 * @param Display display X11 display
 * @param Window window X11 window size
 * @param u_long score The current score
 * @param int x The x position of the number
 * @param int y The y position of the number
 * @pre Must have selected window and display size
 * @post The number has been drawn
 */
void DrawOutNumber(Display *display, Window window, u_long score, int x, int y);

/**
 *Adds an increment to the score
 *
 * @param u_long inc The increment that will be added to the score
 * @pre The score must be declared
 * @post The score has had an increment added to it
 *
 */
void AddToScore(u_long inc);

/**
 * Calculates the score and takes any possible bonuses into account
 *
 * @param u_long inc The increment that will be added to the score
 * @pre The score must be declared as well as the increment
 * @post The new score is now computed
 *
 */
u_long ComputeScore(u_long inc);

/**
 * Sets the new score
 *
 * @param u_long new The new score
 * @pre The score must exist
 * @post The new score has been set
 *
 */
void SetTheScore(u_long new);

extern u_long score;

#endif
