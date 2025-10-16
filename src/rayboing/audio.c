/**
 * @file audio.c
 * @brief Stub implementations for audio functions of XBoing
 */
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "audio.h"

AudioSystem audio;

static const char *soundFiles[SOUND_COUNT] = {
    "resource/sounds/ammo.mp3", // when ammo block is destroyed
    "resource/sounds/applause.mp3",
    "resource/sounds/ball2ball.mp3", // when ball hits the paddle
    "resource/sounds/ballLost.mp3", // when ball goes off screen
    "resource/sounds/ballShot.mp3", // when ball is shot
    "resource/sounds/boing.mp3", // when block is destroyed
    "resource/sounds/bomb.mp3",
    "resource/sounds/bonus.mp3",
    "resource/sounds/buzzer.mp3",
    "resource/sounds/click.mp3",
    "resource/sounds/ddloo.mp3",
    "resource/sounds/doh1.mp3",
    "resource/sounds/doh2.mp3",
    "resource/sounds/doh3.mp3",
    "resource/sounds/doh4.mp3",
    "resource/sounds/evilLaugh.mp3",
    "resource/sounds/game_over.mp3", // after you run out of lives
    "resource/sounds/gate.mp3",
    "resource/sounds/hitHere.mp3",
    "resource/sounds/hypspc.mp3",
    "resource/sounds/intro.mp3",
    "resource/sounds/key.mp3",
    "resource/sounds/looksBad.mp3",
    "resource/sounds/metal.mp3",
    "resource/sounds/mgun.mp3",
    "resource/sounds/ouch.mp3",
    "resource/sounds/paddle.mp3",
    "resource/sounds/ping.mp3",
    "resource/sounds/shark.mp3",
    "resource/sounds/shoot.mp3", // bullet shot
    "resource/sounds/shotgun.mp3",
    "resource/sounds/spring.mp3",
    "resource/sounds/stamp.mp3",
    "resource/sounds/sticky.mp3", // when sticky powerup is activated
    "resource/sounds/supbons.mp3",
    "resource/sounds/toggle.mp3",
    "resource/sounds/tone.mp3",
    "resource/sounds/touch.mp3",
    "resource/sounds/wallsOff.mp3",
    "resource/sounds/warp.mp3",
    "resource/sounds/weeek.mp3",
    "resource/sounds/whizzo.mp3",
    "resource/sounds/whoosh.mp3",
    "resource/sounds/wzzz.mp3",
    "resource/sounds/wzzz2.mp3",
    "resource/sounds/youagod.mp3"
};


bool initAudioFiles(void) {
    bool success = true;
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (!FileExists(soundFiles[i])) {
            fprintf(stderr, "Missing sound: %s\n", soundFiles[i]);
            success = false;
            continue;
        }

        audio.sounds[i] = LoadSound(soundFiles[i]);
        if (audio.sounds[i].frameCount == 0) {
            fprintf(stderr, "Failed to load sound: %s\n", soundFiles[i]);
            success = false;
        } else {
            //fprintf(stderr, "Loaded: %s\n", soundFiles[i]);
        }
    }

    audio.masterVolume = 100.0f;
    return success;
}

void FreeAudioSystem(void) {
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (audio.sounds[i].frameCount > 0) {
            UnloadSound(audio.sounds[i]);
            audio.sounds[i].frameCount = 0;
        }
    }
    fprintf(stderr, "All sounds unloaded.\n");
}

/* Play a sound file */
void startSound(SoundID id) {
    if (id >= 0 && id < SOUND_COUNT && audio.sounds[id].frameCount > 0) {
        PlaySound(audio.sounds[id]);
    }
}
