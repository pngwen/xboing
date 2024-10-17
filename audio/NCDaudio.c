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
 * $Id: NCDaudio.c,v 1.1.1.1 1994/12/16 01:36:57 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/audio/NCDaudio.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:57 $
 *
 * $Log: NCDaudio.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:57  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/* NCD Audio format - original code by Dave Lemke <lemke@verbosa.ncd.com> */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include "include/error.h"
#include "include/audio.h"
#include "include/init.h"

#include <audio/audiolib.h>
#include <audio/soundlib.h>

/*
 *  Internal variable declarations:
 */

#define	MAX_SOUNDS	64

AuServer   *aud = NULL;
AuDeviceID  device;
static int	audio_on = False;
static int  num_sounds = 0;
static char	errorString[255];

static struct 
{
    char       *name;
    char       *filename;
    void       *private;
} sound_table[MAX_SOUNDS];

typedef struct 
{
    int        playing;
    AuBucketID  bucket;
} audioRec, *audioPtr;

int SetUpAudioSystem(Display *display)
{
    int         i;
    char       *displayname = DisplayString(display);

    if (audio_on)
		return True;

	/* try and connect to the NCD audio server */
    if (!(aud = AuOpenServer(displayname, 0, NULL, 0, NULL, NULL)))
	{
		ErrorMessage("Cannot connect to NCD audio server.");
		return False;
	}

	/* Look for an audio device that we can use */
    for (i = 0; i < AuServerNumDevices(aud); i++)
	{
		if ((AuDeviceKind(AuServerDevice(aud, i)) == 
			AuComponentKindPhysicalOutput) && 
			AuDeviceNumTracks(AuServerDevice(aud, i)) == 1) 
		{
	    	device = AuDeviceIdentifier(AuServerDevice(aud, i));
	    	break;
		}
	}

	/* Well we didn't get a device - all busy? */
    if (!device) 
	{
		ErrorMessage("Cannot obtain NCD audio device.");
		AuCloseServer(aud);
		return False;
    }

#if defined(SOUNDLIB_VERSION) && SOUNDLIB_VERSION >= 2
    AuSoundRestartHardwarePauses = AuFalse;
#endif		

	/* Success - we have an audio device */
    audio_on = True;
    return True;
}

void FreeAudioSystem(void)
{
    /* Turn off the connection to the au server */
	AuCloseServer(aud);
    audio_on = False;
}

void setNewVolume(unsigned int Volume)
{
    /* Do nothing here as we don't need too */
}

static void doneCB(AuServer *aud, AuEventHandlerRec *handler, AuEvent *event,		audioPtr info)
{
    info->playing = False;
}

void audioDevicePlay(char *filename, int volume, void **private)
{
    audioPtr   *info = (audioPtr *) private;
	char str[1024];

    if (!*info) 
	{
		if (!(*info = (audioPtr) malloc(sizeof(audioRec))))
		{
			sprintf(errorString, "Cannot malloc memory for sound file %s.", 
				filename);
			WarningMessage(errorString);
	    	return;
		}

		(*info)->playing = 0;
		(*info)->bucket = AuSoundCreateBucketFromFile(aud, filename, 
			AuAccessAllMasks, NULL, NULL);
    }
	
	sprintf(str, "Playing soundfile: %s\n", filename);
	DEBUG(str)

	/* Flush sound */
	AuFlush(aud);

    if ((*info)->bucket && (!(*info)->playing)) 
	{
		(*info)->playing = True;
		AuSoundPlayFromBucket(aud, (*info)->bucket, device,
	    	AuFixedPointFromFraction(volume, 100),
		    (void (*)) doneCB, (AuPointer) * info, 1, NULL, NULL, NULL, NULL);
    }
}

void audioDeviceEvents(void)
{
    if (aud) AuHandleEvents(aud);
}

void playSoundFile(char *filename, int volume)
{
    int         i;
    char        fbuf[1024];
    char        *str;

	/* Loop through the sound table looking for sound */
    for (i = 0; i < num_sounds; i++) 
	{
		if (!strcmp(sound_table[i].name, filename)) 
		{
			/* Yeah - already in sound table */
	    	break;
		}
    }

	/* Ok - not found so add it to the sound table */
    if (i == num_sounds) 
	{	
		/* new one - so add it to the table */
		sound_table[num_sounds].name = strdup(filename);

		/* Use the environment variable if it exists */
        if ((str = getenv("XBOING_SOUND_DIR")) != NULL)
            sprintf(fbuf, "%s/%s.au", str, filename);
        else            
            sprintf(fbuf, "%s/%s.au", SOUNDS_DIR, filename);

		sound_table[num_sounds].filename = strdup(fbuf);
		num_sounds++;
    }

    audioDevicePlay(sound_table[i].filename, volume, &sound_table[i].private);
}

void SetMaximumVolume(int Volume)
{
}

int GetMaximumVolume(void)
{
    return 0;
}

