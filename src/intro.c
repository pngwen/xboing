#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include "intro.h"
#define INTRO_TEXTURES "resource/textures/presents/"
//TODO: Add star animation frames
#define STAR_TEXTURES "resource/textures/stars/"
//TODO: Add space background
#define BACKGROUNDS "resource/textures/bgrnds/"

Texture2D introPlanet,
          introFlag,
          introJustin,
          introKibell,
          //TODO: animate after "Justin Kibell" introPresents,
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
    /*
    //TODO: Animate after Justin Kibell
    introPresents = LoadTexture(INTRO_TEXTURES "presents.png");
    if (introPresents.id == 0) return false;
    */
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
            DrawTexture(introFlag, (SCREEN_WIDTH - introFlag.width)/2, 20, WHITE);
            DrawTexture(introJustin, (SCREEN_WIDTH - introJustin.width)/2,
            SCREEN_HEIGHT - introKibell.height - introJustin.height - 30, WHITE);
            DrawTexture(introKibell, (SCREEN_WIDTH - introKibell.width)/2, SCREEN_HEIGHT - introKibell.height - 20, WHITE);
            //DrawTexture(introPresents, 100, 50, introTint); //TODO: Animate after Justin Kibell
            //TODO: Animate letters coming in one_by_one

            int xboingY = (SCREEN_HEIGHT / 2) - 100; //Verticle line where "XBOING" sits
            int spacing = 10; //Spacing in between "XBOING" letters
            int totalWidth =
                intro_X.width + intro_B.width + intro_O.width +
                intro_I.width + intro_N.width + intro_G.width +
                spacing * 5;

            int startX = (SCREEN_WIDTH - totalWidth) / 2;
            int x = startX;

            DrawTexture(intro_X, x, xboingY, WHITE); x += intro_X.width + spacing;
            DrawTexture(intro_B, x, xboingY, WHITE); x += intro_B.width + spacing;
            DrawTexture(intro_O, x, xboingY, WHITE); x += intro_O.width + spacing;
            DrawTexture(intro_I, x, xboingY, WHITE); x += intro_I.width + spacing;
            DrawTexture(intro_N, x, xboingY, WHITE); x += intro_N.width + spacing;
            DrawTexture(intro_G, x, xboingY, WHITE);

            const char *prompt = "Press ENTER or SPACE to Start";
            int promptSize = 20;
            int promptWidth = MeasureText(prompt, promptSize);
            DrawText(prompt, (SCREEN_WIDTH - promptWidth)/2, SCREEN_HEIGHT/3 + 80, promptSize, LIGHTGRAY);
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
