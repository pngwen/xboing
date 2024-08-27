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
#include <xpm.h>

#include "init.h"
#include "error.h"

/*
 *  Internal macro definitions:
 */

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

#if NeedFunctionPrototypes
void NormalMessage(char *message)
#else
void NormalMessage(message)
	char *message;
#endif
{
	/* Print a message to standard out flush it */
	fprintf(stdout, "XBoing - %s\n", message);
	fflush(stdout);
}

#if NeedFunctionPrototypes
void ErrorMessage(char *message)
#else
void ErrorMessage(message)
	char *message;
#endif
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

#if NeedFunctionPrototypes
void WarningMessage(char *message)
#else
void WarningMessage(message)
	char *message;
#endif
{
	/* Print a standard warning message to stdout and flush it */
	fprintf(stdout, "XBoing - Warning: %s\n", message);
	fflush(stdout);
}

#if NeedFunctionPrototypes
void HandleXPMError(Display *display, int ErrorStatus, char *tag)
#else
void HandleXPMError(display, ErrorStatus, tag)
	Display *display;
	int ErrorStatus;
	char *tag;
#endif
{
    char *error = NULL;
	char *warning = NULL;

	/* Switch on the type of error returned by xpm library */
	switch (ErrorStatus) 
	{
		case XpmSuccess:
			return;

		case XpmColorError:
			/* The colour name passed was bung */
			warning = "Could not parse or alloc requested colour";
			break;

		case XpmNoMemory:
			/* Not enough memory for pixmap */
			error = "Not enough memory for pixmap creation";
			break;

		case XpmColorFailed:
			/* No more entries available in colourmap */
			error = "Colourmap is full - cannot allocate a colour";
			break;

		case XpmOpenFailed:
			/* Xpm could not open the pixmap file */
			error = "Unable to open pixmap file";
			break;

		case XpmFileInvalid:
			/* XPM file contains invalid or corrupt data */
			error = "XPM file contains invalid or corrupt data";
			break;

		default:
			/* Unexpected xpm error code */
			error = "Unexpected xpm error code";
			break;
	}

	/* If there is to be a warning then issue it */
    if (warning)
		fprintf(stdout, "%s - Warning: %s.\n", tag, warning);

	if (error) 
	{
		/* Argg. An error so tell everyone */
		fprintf(stderr, "%s - Error: %s.\n", tag, error);
		ShutDown(display, 1, "Fatal error.");
	}
}
