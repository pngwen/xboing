/**
 * @file audio.c
 * @brief Stub implementations for audio functions of XBoing
 */
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "audio.h"


AudioSystem audio;

bool initAudioFiles(){
    audio.ballShot = LoadSound("resource/sounds/ballshot.mp3");
    if (audio.ballShot.frameCount == 0) {
        // Failed to load
        fprintf(stderr, "didnt load it"); // print
        return false;
        
    }
    fprintf(stderr, "loaded it"); // print
    audio.masterVolume = 100;

    return true;
}

void FreeAudioSystem(void){
    FreeAudioSystemHelper(&audio);
}
/* Free audio system resources */
void FreeAudioSystemHelper(AudioSystem *a) {
    if (!a) return;
    UnloadSound(a->ballShot);
    CloseAudioDevice();
    fprintf(stderr, "deloaded it"); // print
}

/* Play a sound file */
void startSound(Sound S) {
    PlaySound(S);
}


/*
 Process audio device events 
void audioDeviceEvents(void) {
    TODO: implement audio event processing 
}

Set maximum volume 
void SetMaximumVolume(int Volume) {
    TODO: implement volume adjustment 
}

Get current maximum volume 
int GetMaximumVolume(void) {
     TODO: implement volume retrieval 
    return 0;
}
*/