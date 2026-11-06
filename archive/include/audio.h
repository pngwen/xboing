/**
 * @file audio.h
 * @brief Compile-only audio interface for the archived X11 sources.
 *
 * The live @c include/audio.h targets raylib. The archived sources still call
 * the original audio API, so keep a small standalone declaration set here.
 * This file is only used when compiling the files under @c archive/.
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "faketypes.h"

int SetUpAudioSystem(Display *display);
void FreeAudioSystem(void);
void playSoundFile(char *filename, int volume);
void audioDeviceEvents(void);
void SetMaximumVolume(int Volume);
int GetMaximumVolume(void);

#endif
