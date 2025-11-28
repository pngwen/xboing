#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include "intro.h"
#define INTRO_TEXTURES "resource/textures/presents/"
//TODO: Add star animation frame
#define STAR_TEXTURES "resource/textures/stars/"
//TODO: Add space background
#define BACKGROUNDS "resource/textures/bgrnds/"

Texture2D introPlanet,
          introFlag,
          introJustin,
          introKibell,
          introPresents,
          intro_X,
          intro_B,
          intro_O,
          intro_I,
          intro_N,
          intro_G;
          //TODO: add "background_Space" and the star animation frames

static const int SCREEN_WIDTH = 575;
static const int SCREEN_HEIGHT = 720;

bool loadIntroTextures(void) {
    introPlanet = LoadTexture(INTRO_TEXTURES "earth.png");
    if (introPlanet.id == 0) return false;
    introFlag = LoadTexture(INTRO_TEXTURES "flag.png");
    if (introFlag.id == 0) return false;
    introJustin = LoadTexture(INTRO_TEXTURES "justin.png");
    if (introJustin.id == 0) return false;
    introKibell = LoadTexture(INTRO_TEXTURES "kibell.png");
    if (introKibell.id == 0) return false;
    introPresents = LoadTexture(INTRO_TEXTURES "presents.png");
    if (introPresents.id == 0) return false;
    intro_X = LoadTexture(INTRO_TEXTURES "titleX.png");
    if (intro_X.id == 0) return false;
    intro_B = LoadTexture(INTRO_TEXTURES "titleB.png");
    if (intro_B.id == 0) return false;
    intro_O = LoadTexture(INTRO_TEXTURES "titleO.png");
    if (intro_O.id == 0) return false;
    intro_I = LoadTexture(INTRO_TEXTURES "titleI.png");
    if (intro_I.id == 0) return false;
    intro_N = LoadTexture(INTRO_TEXTURES "titleN.png");
    if (intro_N.id == 0) return false;
    intro_G = LoadTexture(INTRO_TEXTURES "titleG.png");
    if (intro_G.id == 0) return false;
    /*

    background_Space = LoadTexture(BACKGROUNDS "space.png");
    if (intro_G.id == 0) return false;
    */

    return true;
}

void ShowIntroScreen(void) {
    // Simple loop that draws text and waits for key press
    printf("Intro started\n");
    fflush(stdout);
    // Ensure intro texture is loaded
    if (!loadIntroTextures()) {
        fprintf(stderr, "Failed to load intro textures\n");
        return;
    }
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            Color introTint = WHITE;
            
            DrawTexture(introPlanet, 100, 50, introTint);
            DrawTexture(introFlag,   100, 50, introTint);
            DrawTexture(introJustin, 100, 50, introTint);
            DrawTexture(introKibell, 100, 50, introTint);
            DrawTexture(introPresents, 100, 50, introTint);
            DrawTexture(intro_X, 100, 50, introTint);
            DrawTexture(intro_B, 100, 50, introTint);
            DrawTexture(intro_O, 100, 50, introTint);
            DrawTexture(intro_I, 100, 50, introTint);
            DrawTexture(intro_N, 100, 50, introTint);
            DrawTexture(intro_G, 100, 50, introTint);   

            //const char *prompt = "Press ENTER or SPACE to Start";
            //int promptSize = 20;
            //int promptWidth = MeasureText(prompt, promptSize);
            //DrawText(prompt, (SCREEN_WIDTH - promptWidth)/2, SCREEN_HEIGHT/3 + 80, promptSize, LIGHTGRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE)) {
            // Start requested
            printf("Intro ended - start requested\n"); fflush(stdout);
            UnloadTexture(introPlanet);
            return;
        }
    }

    // If window closed, just return (main will handle exit)
    UnloadTexture(introPlanet);
}
