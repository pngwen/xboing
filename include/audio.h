/**
 * @file audio.h
 * @brief Audio system for XBoing
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <raylib.h>
//#include <faketypes.h>

/*
// Multi-variation sound effect
typedef struct {
    int count;       // Number of variations
    Sound *sounds;   // Array of Raylib Sound objects
} SoundEffect;
*/
// Audio system
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
    */
    int masterVolume;     // 0–100
} AudioSystem;

extern AudioSystem audio;

// Initialize audio system
bool initAudioFiles(void);

// Free all resources
void FreeAudioSystem(void);
void FreeAudioSystemHelper(AudioSystem *audio);

// Play single-instance sound
void startSound(Sound S);

// Play multi-variation sound
//void PlaySoundEffect(SoundEffect *effect);

// Volume control
//void SetMaximumVolume(AudioSystem *audio, int volume);
//int GetMaximumVolume(AudioSystem *audio);

#endif
