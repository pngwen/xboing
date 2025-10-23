/**
 * @file error.h
 * @author Gerardo Gonzalez
 * @date 2024-09-30
 * @brief deals with displaying error messages 
 * 
 * This is the header file for the funtions that are dealing with displaying messages in xboing 
 */


#ifndef _ERROR_H_
#define _ERROR_H_

//TODO: Remove the need for these typedefs

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
 * $Id: error.h,v 1.1.1.1 1994/12/16 01:36:51 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/error.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:51 $
 *
 * $Log: error.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:51  jck
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
 *  Constants and macros:
 */

/*
 *  Type declarations:
 */

/*
 *  Function prototypes:
 */

/**
 * Checks if a texture has been loaded, shuts down program if texture is blank
 *
 * @param Texture2D texture The texture to check if empty
 * @param char *texture_name The name of the texture (printed in error message)
 *
 */
void HandleXPMError(Texture2D texture, char* texture_name);

/**
 * @brief Prints a message to the console in the "Error" format & refers the user to the readme file
 *
 * @param message The string to be printed in the message.
 */
void ErrorMessage(char *message);

/**
 * Prints a message to the console in the in the "warning" format.
 *
 * @param char *message The string to be printed in the message
 *
 */
void WarningMessage(char *message);

/**
 * @brief Prints a message to the console in the "normal" format
 *
 * @param message The string to be printed in the message.
 */
void NormalMessage(char *message);

#endif
