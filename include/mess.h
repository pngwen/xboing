#ifndef _MESS_H_
#define _MESS_H_

/**
 * @file mess.h
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Manages the message system for xboing
 * @version 1.1.1.1
 * @date 16 Dec 1994
 * 
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved 
 * @copyright (see COPYRIGHT file for full text)
 * 
 * mess.h/mess.c manage the message system for xboing. Deals with setting up the message system.
 * The files also display messages and free message memory
 *  
 */

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
 * $Id: mess.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/mess.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: mess.h,v $
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

/**
 *
 * @brief Includes the X11 library for the window graphics.
 *
 * @todo Remove this dependency with a new graphics system.
 *
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
 *  @brief Initializes the message system.
 *
 * @param Display *display Pointer to the X11 display struct
 * @todo Update to remove X11 dependency
 * @param Window window This is the X Identifier for the window that will display the messages.
 * @todo Update to use window management from a new graphics system
 * @param Colormap colormap This is the colormap for the window.  
 * 
 * @todo Implement new graphics system management 
 * @todo Check if the message system can handle multiple windows if needed.
 * @todo Add initialization checks.
 *
 * @post This function sets up and prepares the message system. 
 */
void InitialiseMessageSystem(Display *display, Window window, 
	Colormap colormap);

/**
 * @brief Frees memory used by the message system.
 *
 * @param Display *display Pointer to the X11 display struct
 * @todo Update to remove X11 dependency
 *
 * @todo Add some resource management for future message enhancements
 * @todo Add checks to make sure that only allocated memory is being freed.
 * 
 * @post This function cleans and releases all memory that was allocated for the message system. 
 * 
 */
void FreeMessageSystem(Display *display);

/**
 * @brief Sets a new message to be displayed.
 *
 * @param Display *display Pointer to the X11 display struct
 * @todo Update to remove X11 dependency  
 * @param Window window This is the X Identifier for the window that will display the messages. 
 * @todo Update to use window management from a new graphics system 
 * @param *newMessage The new message to be displayed.
 * @param clear If true, the display will be cleared before displaying the new message. 
 * @todo Change the type of the clear parameter from int to boolean.
 *
 * @todo Make sure the new message is not NULL or empty before use.
 * @todo Add some sort of strategy to handle messages that are larger than the buffer size allows.
 * 
 * @post This function copies the new message into the buffer and updates the display based on the parameters defined.
 * 
 */
void SetCurrentMessage(Display *display, Window window, 
	char *newMessage, int clear);

/**
 * @brief Displays the current message. 
 *
 * @param Display *display Points to the X11 display struct.
 * @todo Update to remove X11 dependency 
 * @param Window window This is the X Identifier for the window that will display the messages. 
 * @todo Update to use window management from a new graphics system 
 * 
 * @todo Implement new graphics system 
 * @todo Add checks to ensure the message is displayed as expected.
 * 
 * @post This function takes the current message stored to the screen to be displayed.
 * 
 */
void DisplayCurrentMessage(Display *display, Window window);

#endif
