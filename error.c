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
 * $Id: error.c,v 1.1.1.1 1994/12/16 01:36:46 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/error.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:46 $
 *
 * $Log: error.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:46  jck
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

// TODO: Remove functional reliance on X11
// #include <xpm.h>

#include "init.h"
#include "error.h"

/*
 *  Internal variable declarations:
 */

/**
 * @brief Prints a normal message to the standard output.
 *
 * This function takes a message string and prints it to the standard output
 * with a prefix indicating the application name.
 *
 * @param message The message string to be printed.
 */
void NormalMessage(char *message)
{
	/* Print a message to standard out flush it */
	fprintf(stdout, "XBoing - %s\n", message);
	fflush(stdout);
}

/**
 * @brief Prints an error message to the standard output.
 *
 * This function takes an error message string, prints it to the standard output,
 * and suggests reading a specific README file for help.
 *
 * @param message The error message string to be printed.
 */
void ErrorMessage(char *message)
{

#ifndef READMEP_FILE
    /* Repeated to generate a message when compiling */
#define READMEP_FILE "README.problems"
#define READMEP_FILE "README.problems"
#endif
	/* Print a standard error message to stdout and flush it */
	fprintf(stdout, "XBoing - Error: %s\n", message);
	fprintf(stdout, "\nPlease read `%s' document for help.\n",
		READMEP_FILE);
	fflush(stdout);
}

/**
 * @brief Prints a warning message to the standard output.
 *
 * This function takes a warning message string and prints it to the standard output
 * with a prefix indicating it is a warning.
 *
 * @param message The warning message string to be printed.
 */
void WarningMessage(char *message)
{
	/* Print a standard warning message to stdout and flush it */
	fprintf(stdout, "XBoing - Warning: %s\n", message);
	fflush(stdout);
}

/**
 * Checks if a texture has been loaded, shuts down program if texture is blank
 *
 * @param Texture2D texture The texture to check if empty
 * @param char *texture_name The name of the texture (printed in error message)
 *
 */
void HandleXPMError(Texture2D texture, char* texture_name)
{
	Display* display;// Fake variable needed to compile to object code

	/* If the texture has not been loaded */
    if(!texture){
		/* Argg. An error so tell everyone */
		fprintf(stderr, "Error: failed to load texture '%s'.\n", texture_name);
		ShutDown(display, 1, "Fatal error.");
	}
}
