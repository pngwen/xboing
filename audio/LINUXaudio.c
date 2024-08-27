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
 * $Id: LINUXaudio.c,v 1.1.1.1 1994/12/16 01:36:56 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/audio/LINUXaudio.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:56 $
 *
 * $Log: LINUXaudio.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:56  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/* Soundblaster Audio - PC LINUX - original code by 
 * Peter C. Ludwig, email: urpc01@ux1.uni-dortmund.de 
 * modified by David W. Boyd, email: dave_boyd@Sterling.com
 */

/*
 *  Include file dependencies:
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/soundcard.h>

#include "include/error.h"
#include "include/audio.h"

/*
 *  Internal macro definitions:
 */

#define BUFFER_SIZE				(1024 * SBUF_SIZE)

static int child_pid = 0;
static int snd_pipes[2];

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

static char				*Audio_dev = "/dev/audio";
static int 				Audio_fd;
/* size should depend on sample_rate */
static unsigned char   	buf[BUFFER_SIZE];       
static char 			errorString[255];

#if NeedFunctionPrototypes
int SetUpAudioSystem(Display *display)
#else
int SetUpAudioSystem(display)
	Display *display;
#endif
{
	int err, cnt;
      

	if (child_pid == 0)
	   /* Fork a child to handle the I/O */
           {
	   if (pipe((int *)&snd_pipes))
	      {
		ErrorMessage("Cannot create pipes to communicate with child.");

		/* Bummer - cannot open pipes */
  		return False;
	
  	      }
           if ((child_pid = fork()))
		{
		/* Successfully forked child */
		return True;
                }
           else
                /* Child Thread */
	        {
                static char string[257];     /* String to put read material in */
                static char *str = NULL;
		static char soundfile[512];
		static int ifd;

                str = getenv("XBOING_SOUND_DIR");
                do
		  {
                  read(snd_pipes[0], &string, 256);

                  if (!strcmp(string, "EXIT"))
	             {
                     /* Make sure that the audio device is flushed and reset */
 	             ioctl(Audio_fd, SNDCTL_DSP_RESET, 0);

		     exit(0); 
	             }	

	          /* Try to open the audio device */
 	          if (!(Audio_fd = open(Audio_dev, O_WRONLY)))
  	          {	
		         continue;
  	          }
	
		/* Must be a sound file name */
 		if (str != NULL)
			sprintf(soundfile, "%s/%s.au", str, string);
		else
			sprintf(soundfile, "%s/%s.au", SOUNDS_DIR, string);

		/* Open the sound file for reading */
		if ((ifd = open(soundfile, O_RDONLY, 0)) < 0) 
		{
			/* Issue an error about not opening sound file */
			sprintf(errorString, "Unable to open sound file %s.", soundfile);
                        WarningMessage(errorString);

                }

                /* At this point, we're all ready to copy the data. */
                while ((cnt = read(ifd, (char *) buf, BUFFER_SIZE)) >= 0) 
                {
                        /* If input EOF, write an eof marker */
                        err = write(Audio_fd, (char *)buf, cnt);

                        if (err != cnt) 
                        {
                                sprintf(errorString, "Problem while writing to sound device");
                                WarningMessage(errorString);
                        }    

                        /* End of file? */
                        if (cnt == 0) break;
                }

                if (cnt < 0) 
                {
                        /* Some error - while reading - notify user */
                        sprintf(errorString, "Problem while reading soundfile %s", soundfile);

                        WarningMessage(errorString);
                }
        

 		/* Flush any audio activity */
 		if (ioctl(Audio_fd, SNDCTL_DSP_SYNC, 0) < 0)
  		     {
  			     sprintf(errorString, "Unable to flush audio device.");
  			     WarningMessage(errorString);
  		     }
                /* Close the sound file */
                (void) close(ifd);
		(void) close(Audio_fd);
             } while (True);
          }
       
     }
}
#if NeedFunctionPrototypes
void FreeAudioSystem(void)
#else
void FreeAudioSystem()
#endif
{
	char exit_command[256];
        
    strcpy(exit_command,"EXIT");

	/* Close the audio device */
 	if (write(snd_pipes[1],"EXIT",256) < 0)
		ErrorMessage("Cannot write to audio device.");

	child_pid = 0;
}

#if NeedFunctionPrototypes
static void flushAudioDevice(void)
#else
static void flushAudioDevice()
#endif
{
}

#if NeedFunctionPrototypes
void setNewVolume(unsigned int Volume)
#else
void setNewVolume(Volume)
	unsigned int Volume;
#endif
{
	/* Do nothing here as we don't have audio support */
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
	char snd_file[256];

    strcpy(snd_file,filename);
 	if (write(snd_pipes[1],snd_file,256) < 0)
		ErrorMessage("Cannot write to audio device.");
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
