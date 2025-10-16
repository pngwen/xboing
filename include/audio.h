/**
 * @file audio.h
 * @brief Audio system for XBoing
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <raylib.h>

#define SOUND_COUNT 46

typedef enum {
    SND_AMMO,
    SND_APPLAUSE,
    SND_BALL2BALL,
    SND_BALLLOST,
    SND_BALLSHOT,
    SND_BOING,
    SND_BOMB,
    SND_BONUS,
    SND_BUZZER,
    SND_CLICK,
    SND_DDLOO,
    SND_DOH1,
    SND_DOH2,
    SND_DOH3,
    SND_DOH4,
    SND_EVILLAUGH,
    SND_GAMEOVER,
    SND_GATE,
    SND_HITHERE,
    SND_HYPSPC,
    SND_INTRO,
    SND_KEY,
    SND_LOOKSBAD,
    SND_METAL,
    SND_MGUN,
    SND_OUCH,
    SND_PADDLE,
    SND_PING,
    SND_SHARK,
    SND_SHOOT,
    SND_SHOTGUN,
    SND_SPRING,
    SND_STAMP,
    SND_STICKY,
    SND_SUPBONS,
    SND_TOGGLE,
    SND_TONE,
    SND_TOUCH,
    SND_WALLSOFF,
    SND_WARP,
    SND_WEEEK,
    SND_WHIZZO,
    SND_WHOOSH,
    SND_WZZZ,
    SND_WZZZ2,
    SND_YOUAGOD
} SoundID;

typedef struct AudioSystem {
    Sound sounds[SOUND_COUNT];
    float masterVolume;
} AudioSystem;

extern AudioSystem audio;

// Initialize audio system
bool initAudioFiles(void);

// Free all resources
void FreeAudioSystem(void);
void FreeAudioSystemHelper(AudioSystem *audio);

// Play single-instance sound
void startSound(SoundID ID);

#endif // _AUDIO_H_
