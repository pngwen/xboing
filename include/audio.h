/**
 * @file audio.h
 * @author Gerardo Gonzalez
 * @date 2024-10-01
 * @brief The header file for the audio of xboing
 * 
 * This header file contains the functions for the various audio sounds that xboing supported as well as the controls for raising and lowering the audio in-game
 */


#ifndef _AUDIO_H_
#define _AUDIO_H_

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
 * $Id: audio.h,v 1.1.1.1 1994/12/16 01:36:54 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/audio.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:54 $
 *
 * $Log: audio.h,v $
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

#define SBUF_SIZE	32

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */


/**
 * Initialize the audio system.
 *
 * @param Display *display The display of the X11 window
 * 
 */
int SetUpAudioSystem(Display *display);


/**
 * Deallocate memory and resources used by the audio system.
 *
 * 
 */
void FreeAudioSystem(void);


/**
 * Play a sound effect from a file
 *
 * @param char *filename The path to the sound file to be played.
 * @param int volume The volume level.
 * 
 */
void playSoundFile(char *filename, int volume);


/**
 * Process audio device events
 *
 * 
 */
void audioDeviceEvents(void);

/**
 * set the master volume to the maximum value.
 *
 * @param int Volume The maximum volume level to set.
 * 
 */
void SetMaximumVolume(int Volume);


/**
 * Get the current maximum volume
 *
 * @return The current maximum volume level.
 */
int GetMaximumVolume(void);

#endif
