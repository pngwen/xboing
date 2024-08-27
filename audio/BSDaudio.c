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
 * $Id: BSDaudio.c,v 1.1.1.1 1994/12/16 01:36:57 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/audio/BSDaudio.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:57 $
 *
 * $Log: BSDaudio.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:57  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/* SoundBlaster support for NetBSD */

/*
** Modified by Justin Kibell to suit xboing 30/09/93.
**
** Copyright (C) 1993 by Jordan Hubbard
**
** Ulaw code Copyright (C) 1989 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
**
*
*/

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sblast.h>

/*
 *  Internal macro definitions:
 */

#define BSIZE	4096

/*
** This macro converts from ulaw to 16 bit linear, faster.
**
** Jef Poskanzer
** 23 October 1989
**
** Input: 8 bit ulaw sample
** Output: signed 16 bit linear sample
*/

#define ulaw_to_linear(ulawbyte) ulaw_table[ulawbyte]

/* Define the magic number for audio files - just to check */
#define	SUN_AUDIO_MAGIC		((u_long)0x2e736e64)

/*
 *  Internal type declarations:
 */

/* What sun's .au format looks like */
struct _sun_audio 
{
	u_long	magic;
	u_long	hdr_size;
	u_long	data_size;
	u_long	data_encoding;
	u_long	sample_rate;
	u_long	nchannels;
};
typedef struct _sun_audio Sun_audio_hdr;

/*
 *  Internal variable declarations:
 */

static int ulaw_table[256] = 
{
    -32124, -31100, -30076, -29052, -28028, -27004, -25980, -24956,
    -23932, -22908, -21884, -20860, -19836, -18812, -17788, -16764,
    -15996, -15484, -14972, -14460, -13948, -13436, -12924, -12412,
    -11900, -11388, -10876, -10364,  -9852,  -9340,  -8828,  -8316,
     -7932,  -7676,  -7420,  -7164,  -6908,  -6652,  -6396,  -6140,
     -5884,  -5628,  -5372,  -5116,  -4860,  -4604,  -4348,  -4092,
     -3900,  -3772,  -3644,  -3516,  -3388,  -3260,  -3132,  -3004,
     -2876,  -2748,  -2620,  -2492,  -2364,  -2236,  -2108,  -1980,
     -1884,  -1820,  -1756,  -1692,  -1628,  -1564,  -1500,  -1436,
     -1372,  -1308,  -1244,  -1180,  -1116,  -1052,   -988,   -924,
      -876,   -844,   -812,   -780,   -748,   -716,   -684,   -652,
      -620,   -588,   -556,   -524,   -492,   -460,   -428,   -396,
      -372,   -356,   -340,   -324,   -308,   -292,   -276,   -260,
      -244,   -228,   -212,   -196,   -180,   -164,   -148,   -132,
      -120,   -112,   -104,    -96,    -88,    -80,    -72,    -64,
       -56,    -48,    -40,    -32,    -24,    -16,     -8,      0,
     32124,  31100,  30076,  29052,  28028,  27004,  25980,  24956,
     23932,  22908,  21884,  20860,  19836,  18812,  17788,  16764,
     15996,  15484,  14972,  14460,  13948,  13436,  12924,  12412,
     11900,  11388,  10876,  10364,   9852,   9340,   8828,   8316,
      7932,   7676,   7420,   7164,   6908,   6652,   6396,   6140,
      5884,   5628,   5372,   5116,   4860,   4604,   4348,   4092,
      3900,   3772,   3644,   3516,   3388,   3260,   3132,   3004,
      2876,   2748,   2620,   2492,   2364,   2236,   2108,   1980,
      1884,   1820,   1756,   1692,   1628,   1564,   1500,   1436,
      1372,   1308,   1244,   1180,   1116,   1052,    988,    924,
       876,    844,    812,    780,    748,    716,    684,    652,
       620,    588,    556,    524,    492,    460,    428,    396,
       372,    356,    340,    324,    308,    292,    276,    260,
       244,    228,    212,    196,    180,    164,    148,    132,
       120,    112,    104,     96,     88,     80,     72,     64,
		56,     48,     40,     32,     24,     16,      8,      0 
};


/* The audio device */
int 	Audio_fd;
char	errorString[255];


/* Slurp in a .au file */
#if NeedFunctionPrototypes
static int read_audio_header(int fd, Sun_audio_hdr *buf)
#else
static int read_audio_header(fd, buf)
	int fd;
	Sun_audio_hdr *buf;
#endif
{
    read(fd, buf, sizeof(Sun_audio_hdr));

    buf->magic 			= ntohl(buf->magic);
    buf->hdr_size 		= ntohl(buf->hdr_size);
    buf->data_size 		= ntohl(buf->data_size);
    buf->data_encoding 	= ntohl(buf->data_encoding);
    buf->sample_rate 	= ntohl(buf->sample_rate);
    buf->nchannels 		= ntohl(buf->nchannels);

    lseek(fd, buf->hdr_size, SEEK_SET);

    return buf->magic == SUN_AUDIO_MAGIC;
}

#if NeedFunctionPrototypes
static int set_volume(int volume)
#else
static int set_volume(volume)
	int volume;
#endif
{
    static struct sb_mixer_levels save;	/* Saved mixer level settings */
    struct sb_mixer_levels l;		/* New mixer level settings */
    static int mixer = 0;

    if (!mixer) 
	{
		mixer = open ("/dev/sb_mixer", O_RDWR, 0);

		if (mixer < 0)
	    	perror ("open mixer");
		else if (ioctl (mixer, MIXER_IOCTL_READ_LEVELS, &save) < 0)
	    	perror ("mixer ioctl");
    }
    else if (mixer > 0) 
	{
		/* Just change master volume */
		l.master.l = l.master.r = volume;

		/* If bad volume, put it back */
		if (ioctl (mixer, MIXER_IOCTL_SET_LEVELS, &l) == -1)
	    	ioctl (mixer, MIXER_IOCTL_SET_LEVELS, &save);
    }

    return 0;
}

/* Send an ioctl to the DSP with error checking. */
#define IOCTL(cmd, arg) \
    if (ioctl(Audio_fd, DSP_IOCTL_##cmd, arg) == -1) \
        perror ("ioctl") \

#if NeedFunctionPrototypes
int SetUpAudioSystem(Display *display)	
#else
int SetUpAudioSystem(display)	
	Display *display;
#endif
{
    Audio_fd = open("/dev/sb_dsp", O_RDWR);
    if (Audio_fd < 1) 
	{
		/* Unable to open audio device so barf */
        ErrorMessage("/dev/sb_dsp audio device is busy.");
		return False;
    }

	/* Audio device is open */
    return True;
}

#if NeedFunctionPrototypes
void SetMaximumVolume(int Volume)
#else
void SetMaximumVolume(Volume)
	int Volume;
#endif
{
    /* Set the maximum volume for the audio system */
    set_volume(volume);
}

#if NeedFunctionPrototypes
int GetMaximumVolume(void)
#else
int GetMaximumVolume()
#endif
{
    /* Return the maximum volume as a % of 100 */
    return 100;
}


#if NeedFunctionPrototypes
void FreeAudioSystem(void)
#else
void FreeAudioSystem()
#endif
{
    /* Close the audio device thanks */
    (void) close(Audio_fd);
}

#if NeedFunctionPrototypes
void playSoundFile(char *filename, int volume)
#else
void playSoundFile(filename, volume)
	char *filename; 
	int volume;
#endif
{
    Sun_audio_hdr 	ahdr;
    int 			fd;
    unsigned char 	buf[BSIZE];
    int 			i, len;
    int 			zero = 0;
    int 			one = 1;
    char 			soundfile[FILENAME_MAX];
    char 			*str;

    /* Set to the required volume */
    set_volume(volume);

	/* Construct the sounds file path and use env var if exists */
    if ((str = getenv("XBOING_SOUND_DIR")) != NULL)
    	sprintf(soundfile, "%s/%s.au", str, filename);
    else            
        sprintf(soundfile, "%s/%s.au", SOUNDS_DIR, filename);

    /* Open the sound file for reading */
    if ((fd = open(soundfile, O_RDONLY, 0)) < 0)
    {
        /* Issue an error about not opening sound file */
        sprintf(errorString, "Unable to open sound file %s.", soundfile);
        WarningMessage(errorString);
        return;
    }

    if (read_audio_header(fd, &ahdr) == 0)
	{
        /* Cannot understand the sound file so close file and return */
        sprintf(errorString, "Unable to play sound file %s.", soundfile);
        WarningMessage(errorString);
   		(void) close(fd); /* Close the sound file */
		return;
	}

    /* Set up the DSP, now that we know a little about sample */
    if (ioctl(Audio_fd, DSP_IOCTL_STEREO, 
		(ahdr.nchannels == 1) ? &zero : &one) == -1)
	{
        /* Cannot set up the DSP */
        WarningMessage("Unable to setup DSP stero setting.");
   		(void) close(fd); /* Close the sound file */
        return;
	}

    if (ioctl (Audio_fd, DSP_IOCTL_SPEED, &ahdr.sample_rate) == -1)
	{
        /* Cannot set up the DSP */
        WarningMessage("Unable to setup DSP speed setting.");
   		(void) close(fd); /* Close the sound file */
        return;
	}

    while ((len = read(fd, buf, BSIZE)) > 0) 
	{
		/* Is it mulaw?  Else assume linear (usually mulaw so far) */
		if (ahdr.data_encoding == 1) 
		{
			/* Convert data to linear format - ie: uncompress */
	    	for(i = 0; i < len; i++)
				buf[i] = (char) 128 + (ulaw_to_linear(buf[i]) / 256);
		}

		/* Write the data to the DSP for the sound hopefully */
		if (len != write(Audio_fd, buf, len)) 
		{
        	WarningMessage("Unable to write to DSP.");
    		(void) close(fd); /* Close the sound file */
	    	return;
		}
    }

    /* Close the sound file */
    (void) close(fd);
}

#if NeedFunctionPrototypes
void audioDeviceEvents(void)
#else
void audioDeviceEvents()
#endif
{
	/* None to do */
}

