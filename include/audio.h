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

//#include <faketypes.h>

/*
// Multi-variation sound effect
typedef struct {
    int count;       // Number of variations
    Sound *sounds;   // Array of Raylib Sound objects
} SoundEffect;
*/
// Audio system
/*
typedef struct {
    // Single-instance sounds
    Sound ammo;//reload sound of hitting ammo block
    Sound applause;//play when level complete
    Sound ball2ball; 
    Sound ballLost;
    Sound ballShot;
    Sound boing;
    Sound bonus;
    Sound buzzer;
    Sound click;
    Sound ddloo;
    Sound doh1;
    Sound doh2;
    Sound doh3;
    Sound doh4;
    Sound evilLaugh;
    Sound gameOver;
    Sound gate;
    Sound hitHere;
    Sound hypspc;
    Sound intro;
    Sound key;
    Sound looksBad;
    Sound metal;
    Sound mgun;
    Sound ouch;
    Sound paddle;
    Sound ping;
    Sound shark;
    Sound shoot;
    Sound shotgun;
    Sound spring;
    Sound stamp;
    Sound sticky;
    Sound supbons;
    Sound toggle;
    Sound tone;
    Sound touch;
    Sound wallsOff;
    Sound warp;
    Sound weeek;
    Sound whizzo;
    Sound whoosh;
    Sound wzzz;
    Sound wzzz2;
    Sound youagod;

    /*
    SoundEffect bounce;   // Ball bounce variations
    SoundEffect explosion; // Explosion variations
    
    int masterVolume;     // 0–100
} AudioSystem;*/

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

// Play multi-variation sound
//void PlaySoundEffect(SoundEffect *effect);

// Volume control
//void SetMaximumVolume(AudioSystem *audio, int volume);
//int GetMaximumVolume(AudioSystem *audio);

#endif // _AUDIO_H_
