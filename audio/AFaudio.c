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
 * $Id: AFaudio.c,v 1.1.1.1 1994/12/16 01:36:56 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/audio/AFaudio.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:56 $
 *
 * $Log: AFaudio.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:56  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 * Audiofile format -	
 *
 * Original code from Andrew Leahy <A.Leahy@st.nepean.uws.edu.au> 
 *
 * Revised by Wes Whitnah <wesw@orca.wv.tek.com>
 *
 *    - File now compiles!
 *    - Correctly determine audio server connection per
 *      the AudioFile spec.  Checks for AUDIOFILE environment
 *      variable, but defaults to the X display if none set.
 *      AUDIOFILE environment variable is *NOT* a requirement.
 *    - Correctly determine which AudioFile device to connect to.
 *      Looks for a uLaw device, which may not be device zero.
 *    - Verify sound files and strip header before sending
 *      sound clip to the audio server.  Reduces clicks.
 *    - Clean up error checking and recovery in various places.
 *    - Removed unused variables and dead code.
 * 
 * Revised again by Wes Whitnah <wesw@orca.wv.tek.com> - Sep 1994
 */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <fcntl.h>
#include <X11/Xlib.h>
#include "include/error.h"
#include "AFlib.h"

/* 
 * Internal macro definitions: 
 */

#define BUFFER_SIZE				(1024 * 4)

/*
 *  Internal type declarations:
 */

typedef struct 
{
	unsigned	magic;				/* Magic number		  */
	unsigned	sample_rate;		/* Samples per second	  */
	unsigned	samples_per_unit;	/* Samples per unit	  */
	unsigned	bytes_per_unit;		/* Bytes per sample unit  */
	unsigned	channels;			/* # interleaved channels */
	unsigned	encoding;			/* Date encoding format	  */
} Sun_Audio_Hdr;

/* 
 * Internal variable declarations: 
 */

static char			*buf;
static char			errorString[255];
static AC			ac;
static AFAudioConn	*aud;


static int FindPlayDevice(AFAudioConn *aud, int rate,
	AEncodeType type, int nchan)
{
    AFDeviceDescriptor	*aDev;
    int			ndevices;
    int			i;

    ndevices = ANumberOfAudioDevices(aud);

    /*
     * Iterate through all of the devices until we find a match.
     * Make sure device is not connected to the phone.
     */

    for (i = 0; i < ndevices; i++)
    {
		aDev = AAudioDeviceDescriptor(aud, i);
		if (aDev->inputsFromPhone == 0
	    	&& aDev->outputsToPhone == 0
	    	&& aDev->playSampleFreq == rate
	    	&& aDev->playBufType == type
	    	&& aDev->playNchannels == nchan)
		{
			/* match found */
	    	return i;
		}
    }

    /* No match found. */
    return -1;
}

int SetUpAudioSystem(Display *display)
{
    AFSetACAttributes 	attributes;
    int 		device;
    char 		*server;
    int			audiofile_set = True;

    /*
     * Determine which audio server should be used.
     * AudioFile uses the following search sequence:
     *   1) AUDIOFILE environment variable, if set
     *   2) DISPLAY environment variable, if set
     */

    if ((server = (char *) getenv("AUDIOFILE")) == NULL)
    {
		audiofile_set = False;

		/*
	 	 * Use XDisplayString() in case user added the
	 	 * -display switch to the command line.
	 	 */
		if ((server = XDisplayString(display)) == NULL)
		{
		    sprintf(errorString, "Cannot find an audio server name.");
		    NormalMessage(errorString);
		    sprintf(errorString,
			    "Try setting the AUDIOFILE environment variable.");
		    NormalMessage(errorString);
		    return False;
		}
    }

    /*
     * Establish a connection to the audio server.
     */
    if ((aud = AFOpenAudioConn(server)) == NULL) 
    {
		sprintf(errorString,
			"Cannot open a connection to audio server %s.", server);
		NormalMessage(errorString);

		if (!audiofile_set)
		{
	   		sprintf(errorString,
		    	"Try setting the AUDIOFILE environment variable.");
	   		NormalMessage(errorString);
		}

		return False;
    }

    /*
     * Try to find a device that can play ulaw samples.
     */

    if ((device = FindPlayDevice(aud, 8000, MU255, 1)) == -1)
    {
		sprintf(errorString, "Couldn't find a ulaw audio device.");
		ErrorMessage(errorString);
		AFCloseAudioConn(aud);
		return False;
    }
    
    /*
     * Initialize player.
     */

    attributes.preempt 			= Mix;
    attributes.start_timeout 	= 0;
    attributes.end_silence 		= 0;
    attributes.play_gain 		= 0;
    attributes.rec_gain 		= 0;

    ac = AFCreateAC(aud, device, ACPlayGain, &attributes);

    if ((buf = (char *) malloc(BUFFER_SIZE)) == NULL) 
    {
		sprintf(errorString, "Couldn't allocate a play buffer.");
		ErrorMessage(errorString);
		AFFreeAC(ac);
		AFCloseAudioConn(aud);
		return False;
    }
 
    /*
     * Success in opening the audio device.
     */
    return True;
}

static void flushAudioDevice(void)
{
    /* Not needed */
}

void FreeAudioSystem(void)
{
    /* Close connection to the audiofile sound server */
    AFFreeAC(ac);
    AFCloseAudioConn(aud);

    /* Free memory buffer */
    if (buf != (char *) NULL) free(buf);
}

void SetMaximumVolume(int Volume)
{
    /* Not needed */
}

int GetMaximumVolume(void)
{
    return 0;
}

void playSoundFile(char *filename, int volume)
{
    int			fd;
    char		soundfile[1024];
    char		*str;
    int			nbytes;
    ATime		t, act, nact;
    Sun_Audio_Hdr	header;

    /*
     * Construct the sounds file path.
     */
    if ((str = (char *)getenv("XBOING_SOUND_DIR")) != NULL)
		sprintf(soundfile, "%s/%s.au", str, filename);
    else            
		sprintf(soundfile, "%s/%s.au", SOUNDS_DIR, filename);

    /*
     * Open the sound file for reading.
     */
    if ((fd = open(soundfile, O_RDONLY, 0)) < 0) 
    {
		sprintf(errorString, "Unable to open sound file %s .", soundfile);
		WarningMessage(errorString);
		return;
    }

    /*
     * Assume we have a Sun audio file, and strip the header.
     */
    if (read(fd, (char *)&header, sizeof(Sun_Audio_Hdr)) 
		< sizeof(Sun_Audio_Hdr))
    {
		sprintf(errorString, "Unable to play sound file %s .", soundfile);
		WarningMessage(errorString);
		(void) close(fd);
		return;
    }

    /*
     * Send the audio clip data to the audio server.
     */
    if ((nbytes = read(fd, (char *) buf, BUFFER_SIZE)) <= 0)
    {
		(void) close(fd);
		return;
    }

    t = AFGetTime(ac); 
	
    do
    {
		nact = AFPlaySamples(ac, t, nbytes, buf);
		act = nact;
		t += nbytes;
    } while ((nbytes = read(fd, buf, BUFFER_SIZE)) != 0);

    /*
     * Close the sound file
     */
    (void) close(fd);
}

void audioDeviceEvents(void)
{
    /* None to do */
}
