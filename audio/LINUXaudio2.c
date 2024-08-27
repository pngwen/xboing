/* Soundblaster Audio - PC LINUX - original code by 
 * Peter C. Ludwig, email: peter@lx1.hrz.uni-dortmund.de 
 */

/* 
 * =========================================================================
 *
 * $Id: LINUXaudio2.c,v 1.1.1.1 1994/12/16 01:36:57 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/audio/LINUXaudio2.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:57 $
 *
 * $Log: LINUXaudio2.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:57  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/* Oh, I was lazy the code that I used to convert from ulaw to dsp is
 * from the voxware sounddriver by Hannu Savolainen hannu@voxware.pp.fi.
 * The function Snd_loadUlawSample is the modified function Snd_loadRawSample
 * from the soundIt library written by Brad Pitzel pitzel@cs.sfu.ca.
 * Yeah, the soundIt library is great.
 */
 

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <fcntl.h>
#include "audio/soundIt.h"
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <linux/soundcard.h>

#include "include/error.h"
#include "include/audio.h"

/*
 *  Internal macro definitions:
 */

/*
 *  Internal type declarations:
 */

struct Transtab
 {
 int nr;
 char *name;
 struct Transtab *next;
 };

/*
 *  Internal function definitions:
 */

int Snd_loadUlawSample( const char *file, Sample *sample );

/*
 *  Internal variable declarations:
 */

static struct Transtab *fsampleptr;
static int channels;
static int samplecount;
static Sample *snd;

static unsigned char ulaw_dsp[] = {
     0,    0,    0,    0,    0,    0,    0,    0, 
     0,    0,    0,    0,    0,    0,    0,    0, 
     0,    0,    0,    0,    0,    0,    0,    0, 
     0,    0,    0,    0,    0,    0,    0,    2, 
     5,    9,   13,   17,   21,   25,   29,   33, 
    37,   41,   45,   49,   53,   57,   61,   65, 
    68,   70,   72,   74,   76,   78,   80,   82, 
    84,   86,   88,   90,   92,   94,   96,   98, 
   100,  101,  102,  103,  104,  105,  106,  107, 
   108,  109,  110,  111,  112,  113,  114,  115, 
   115,  116,  116,  117,  117,  118,  118,  119, 
   119,  120,  120,  121,  121,  122,  122,  123, 
   123,  123,  124,  124,  124,  124,  125,  125, 
   125,  125,  126,  126,  126,  126,  127,  127, 
   127,  127,  127,  127,  128,  128,  128,  128, 
   128,  128,  128,  128,  128,  128,  128,  128, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   252,  248,  244,  240,  236,  232,  228,  224, 
   220,  216,  212,  208,  204,  200,  196,  192, 
   189,  187,  185,  183,  181,  179,  177,  175, 
   173,  171,  169,  167,  165,  163,  161,  159, 
   157,  156,  155,  154,  153,  152,  151,  150, 
   149,  148,  147,  146,  145,  144,  143,  142, 
   142,  141,  141,  140,  140,  139,  139,  138, 
   138,  137,  137,  136,  136,  135,  135,  134, 
   134,  134,  133,  133,  133,  133,  132,  132, 
   132,  132,  131,  131,  131,  131,  130,  130, 
   130,  130,  130,  130,  129,  129,  129,  129, 
   129,  129,  129,  129,  128,  128,  128,  128, 
};

/* Now the functions. */

void translate_bytes (const unsigned char *table, unsigned char *buff, int n)
{
  int i;

  for (i = 0; i < n; i++) buff[i] = table[buff[i]];
}

int Snd_loadUlawSample( const char *file, Sample *sample )
 {
 FILE *fp;

 sample->data = NULL;
 sample->len  = 0;
   
 fp = fopen(file,"r");   
   
 if (fp==NULL) return -1;
   
 /* get length of the file */
 sample->len = lseek( fileno(fp), 0, SEEK_END );
   
 /* go back to beginning of file */
 lseek( fileno(fp), 0, SEEK_SET );

 /* alloc memory for sample */
 sample->data = (unsigned char *)malloc( sample->len );
   
 if (sample->data==NULL)
  {
  fclose(fp);
  return -2;
  }
   
 fread( sample->data, 1, sample->len, fp ); 	
  
 translate_bytes(ulaw_dsp, sample->data, sample->len);

 fclose(fp);
   
 return 0;
 }

#if NeedFunctionPrototypes
int SetUpAudioSystem(Display *display)
#else
int SetUpAudioSystem(display)
	Display *display;
#endif
{
int i, len;
char *sounddir;
char fullname[256];

struct Transtab *lsampleptr, *osampleptr;

struct dirent *dp;
DIR *dfd;

osampleptr = NULL;
channels = 0;

if ((sounddir = getenv("XBOING_SOUND_DIR")) != NULL)
 {
 }
else
 sounddir = SOUNDS_DIR;

if ((dfd = opendir(sounddir)) == NULL)
 {
 fprintf(stderr, "Could not open Sounddir: %s\n", sounddir);
 return 0;
 }

samplecount=0;

while ((dp = readdir(dfd)) != NULL)
 {
 len = strlen(dp->d_name);
 if ((len > 3) && (!strncmp(&(dp->d_name)[len-3], ".au", 3))) 
  {
  lsampleptr = (struct Transtab *)malloc(sizeof(struct Transtab));
  lsampleptr->nr = samplecount;
  lsampleptr->name = (char *)malloc(len+1);
  lsampleptr->next = NULL;
  strcpy(lsampleptr->name, dp->d_name);

  if (osampleptr) osampleptr->next = lsampleptr;
  if (!fsampleptr) fsampleptr = lsampleptr;
  osampleptr = lsampleptr;
  samplecount++;
  }
 }

if (!fsampleptr) 
 {
 fprintf(stderr, "Sorry no sounds!\n");
 return 0;
 }

snd = (Sample *)malloc(samplecount * sizeof(Sample));

lsampleptr = fsampleptr;

for (i = 0; i < samplecount; i++)
 {
 sprintf(fullname, "%s/%s",sounddir ,lsampleptr->name);
 Snd_loadUlawSample(fullname, &(snd[i]));
 lsampleptr = lsampleptr->next;
 }

if (Snd_init( (samplecount+1), snd, 8000, 4, "/dev/dsp" )==EXIT_FAILURE)
 {
 fprintf(stderr, "Can't init soundIt library. yech..\n");
 return 0;
 }

return 1;
}

#if NeedFunctionPrototypes
void FreeAudioSystem(void)
#else
void FreeAudioSystem()
#endif
{
int i;

struct Transtab *osampleptr;

while (fsampleptr != NULL)
 {
 osampleptr = fsampleptr;
 fsampleptr = fsampleptr->next;
 free(osampleptr->name);
 free(osampleptr);
 }

for (i = 0; i < samplecount; i++)
 {
 free(snd[i].data);
 }
free(snd);

Snd_restore();
}

#if NeedFunctionPrototypes
static void flushAudioDevice(void)
#else
static void flushAudioDevice()
#endif
{
/* Nothing */
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
struct Transtab *lsampleptr;

lsampleptr = fsampleptr;

while ((lsampleptr != NULL) && (strncmp(filename, lsampleptr->name, (strlen(lsampleptr->name) - 3)))) lsampleptr = lsampleptr->next;
if (lsampleptr != NULL)
 {
 Snd_effect(lsampleptr->nr, channels);
 }
else
 {
 fprintf(stderr,"Sorry! Could not find sound: %s\n", filename);
 }

channels++;
if (channels > 3) channels = 0;
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

