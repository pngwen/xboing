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
 * $Id: RPLAYaudio.c,v 1.1.1.1 1994/12/16 01:36:57 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/audio/RPLAYaudio.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:57 $
 *
 * $Log: RPLAYaudio.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:57  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/* RPLAYaudio.c - Use rplay to play sounds over network. Original source by
 *                Joel J. Fleck II - <joel@cc.bellcore.com> 
 * 				- I haven't the time to test this code. Should work though.
 */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include <rplay.h>

#include "include/error.h"
#include "include/main.h"
#include "include/audio.h"

/*
 *  Internal macro definitions:
 */

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

static RPLAY   	*rp;
static int 		rplay_fd;
static char 	errorString[255];

#if NeedFunctionPrototypes
int SetUpAudioSystem(Display *display)
#else
int SetUpAudioSystem(display)
	Display *display;
#endif
{
	char hostname[256];
   	char display_machine[256];
    char *displayname = XDisplayString(display);

	/* Obtain the systems hostname */
    gethostname(hostname, 256);

    if (strncmp(hostname, displayname, strlen(hostname)) != 0) 
	{
		/* Obtain the server name etc for rplay */
    	strncpy(display_machine, displayname, strcspn(displayname,":"));

		/* Connect to the rplay sound server */
        rplay_fd = rplay_open(display_machine);
    } 
	else 
	{
		/* Connect to the rplay sound server */
        rplay_fd = rplay_open("localhost");
    }

    if ( rplay_fd == -1 ) 
	{
		/* Error while trying to connect to rplay sound server */
    	rplay_perror("SetUpAudioSystem()");
		ErrorMessage("Error: Cannot connect to rplay sound server.");
        return False;
    } 
	else 
        return True;
}

#if NeedFunctionPrototypes
void FreeAudioSystem(void)
#else
void FreeAudioSystem()
#endif
{
	/* Close the rplay sound server */
    rplay_close(rplay_fd);
    rplay_destroy(rp);
}

#if NeedFunctionPrototypes
static void flushAudioDevice(void)
#else
static void flushAudioDevice()
#endif
{
	/* This is done when the sound is sent by setting the priority to highest
	 * for every sound - I hope this works. JCK
	 */
}

#if NeedFunctionPrototypes
void setNewVolume(unsigned int Volume)
#else
void setNewVolume(Volume)
	unsigned int Volume;
#endif
{
	/* How do you do thism with rplay? */
}

#if NeedFunctionPrototypes
void audioDeviceEvents(void)
#else
void audioDeviceEvents()
#endif
{
    /* None to do */
}

#if NeedFunctionPrototypes
void playSoundFile(char *filename, int volume)
#else
void playSoundFile(filename, volume)
	char *filename;
	int volume;
#endif
{
    char soundfile[1024];
    char *str;

    /* Clear any active audio for the new sound */
    flushAudioDevice();

    /* Construct the sounds file path and use env var if exists */
    if ((str = getenv("XBOING_SOUND_DIR")) != NULL)
        sprintf(soundfile, "%s/%s.au", str, filename);
    else
        sprintf(soundfile, "%s/%s.au", SOUNDS_DIR, filename);

	/* Create a connection to rplay server */
	if ((rp = rplay_create(RPLAY_PLAY)) == NULL) 
	{
		/* Cannot play sound for some reason */
    	rplay_perror("playSoundFile()");
		ErrorMessage(errorString);
    } 
	else 
	{
		/* Volume for rplay is 1 to 255 instead of 1 to 100 */
        volume += 50;

		/* Setup the sound for rplay */
        rplay_set(rp, RPLAY_APPEND, RPLAY_SOUND, soundfile, RPLAY_VOLUME, 
			volume, RPLAY_PRIORITY, 255, NULL);

		/* Send the rplay packet */
        rplay(rplay_fd, rp);
    }
}

#if NeedFunctionPrototypes
void SetMaximumVolume(int Volume)
#else
void SetMaximumVolume(Volume)
    int Volume;
#endif
{
}

#if NeedFunctionPrototypes
int GetMaximumVolume(void)
#else
int GetMaximumVolume()
#endif
{
    return 0;
}

