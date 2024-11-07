/**
 * @file inst.h
 * @author Gerardo Gonzalez
 * @date 2024-10-01
 * @brief This file deals with the instructions
 * 
 * This is the header file for displaying the instructions of the game to the player
 */


#ifndef _INST_H_
#define _INST_H_

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
 * $Id: inst.h,v 1.1.1.1 1994/12/16 01:36:52 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/inst.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:52 $
 *
 * $Log: inst.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:52  jck
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

/*
 *  Type declarations:
 */

/**
 * @brief Enumeration for instruction states.
 *
 * This enumeration defines the different states of instructions in the game.
 */
enum InstructStates 
{ 
	INSTRUCT_TITLE, 
	INSTRUCT_TEXT, 
	INSTRUCT_SPARKLE, 
	INSTRUCT_WAIT, 
	INSTRUCT_FINISH 
};

/*
 *  Function prototypes:
 */

/**
 * @brief Draws parts of the instructions using the current instruct state. Also updates state so that next call will complete the next instruction part.
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to print the instructions to
 */
void Instructions(Display *display, Window window);

/**
 * @brief Used to draw instructions on the screen after void Instruction() has already been called
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to print the instructions to
 */
void RedrawInstructions(Display *display, Window window);

/**
 * @brief Resets InstructState (prepares for displaying instructions)
 * 
 */
void ResetInstructions(void);

// Used to hold the current state of printing instructions to the screen. Used for determining next step.
extern enum InstructStates InstructState;

#endif
