#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>

#include "demo_gamemodes.h"
#include "demo_blockloader.h"
#include "demo_ball.h"
#include "paddle.h"
#include "audio.h"
#include "intro.h"

const int SCREEN_WIDTH = 575;
const int SCREEN_HEIGHT = 720;

bool ValidateParamFilename(int argumentCount, char*arguments[]);
void ReleaseResources(void);


int main(int argumentCount, char *arguments[]) {

    SetTraceLogLevel(LOG_NONE);

    int rtnCode = 1; 
    bool windowInitialized = false;

    // must InitWindow before loading textures

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rayboing Demo");
    InitAudioDevice(); // audio
    SetTargetFPS(60);
    windowInitialized = true;

    // If no filename was provided on the command line, show the intro/start menu
    // and wait for the player to press Enter/Space. After the intro returns,
    // continue with normal initialization (textures/audio/etc.).
    if (argumentCount == 1) {
        ShowIntroScreen();
        // If the window was closed while on the intro screen, exit now.
        if (WindowShouldClose()) {
            if (windowInitialized) CloseWindow();
            ReleaseResources();
            return rtnCode;
        }
    }

    if (!ValidateParamFilename(argumentCount, arguments)) {
        // Validation only fails for incorrect command-line usage or bad file
        // when an argument was supplied. If it fails here, halt.
        fprintf(stderr, "Program halt on map validation");
    } else if (!loadBlockTextures()) {
        fprintf(stderr, "Program halt on iniitalize block texture");
    } else if (!InitialisePaddle()) {
        fprintf(stderr, "Program halt on initialize paddle");
    } else if (!InitializeBall()) {
        fprintf(stderr, "Program halt on initialize ball");
    } else if (!initAudioFiles()) {
        fprintf(stderr, "Program halt on initialize sounds");
    } else {

        initializePlayArea();
        SetGameMode(MODE_INITGAME);
        rtnCode = 0;

    }


    // If no filename was provided, supply the default level path
    const char *defaultLevel = "resource/levels/level01.data";

    // main game loop
    // main game loop
GAME_MODES currentMode = GetGameMode();
while (currentMode != MODE_EXIT) {

    // Update input before game logic
    if (GetGameMode() == MODE_PLAY) {
        // Get mouse position
        Vector2 mousePos = GetMousePosition();
        SetPaddlePosition(mousePos.x); // update paddle X

        // Optional: keyboard fallback
        if (IsKeyDown(KEY_LEFT))  MovePaddle(PADDLE_LEFT);
        if (IsKeyDown(KEY_RIGHT)) MovePaddle(PADDLE_RIGHT);
    }

    // Handle game modes
    switch (currentMode) {

        case MODE_INITGAME:
                if (argumentCount == 2) {
                    RunInitGameMode(arguments[1]);
                } else {
                    RunInitGameMode(defaultLevel);
                }
                break;

        case MODE_PLAY:
            RunPlayMode();
            break;

        case MODE_WIN:
        case MODE_LOSE:
        case MODE_CANCEL:
            RunEndMode();
            break;

        default:
            SetGameMode(MODE_CANCEL);
            break;
    }

    if (WindowShouldClose()) SetGameMode(MODE_EXIT);
    currentMode = GetGameMode();
}



    if (windowInitialized) CloseWindow();
    ReleaseResources();
    

    // exit program
    return rtnCode;

}

bool ValidateParamFilename(int argumentCount, char *arguments[]) {

    // If no filename provided, that's OK (we'll show the intro and use default level).
    if (argumentCount == 1) return true;

    // If a filename is provided, ensure it exists and can be opened.
    if (argumentCount == 2) {
        const char *fileName = arguments[1];
        FILE *file = fopen(fileName, "r");
        if (!file) {
            fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", fileName);
            return false;
        } else {
            fprintf(stdout, "Running Rayboing with map '%s'\n", fileName);
            fclose(file);
            return true;
        }
    }

    // Any other argument usage is invalid.
    fprintf(stderr, "Usage: %s <filename>\n", arguments[0]);
    return false;
}

void ReleaseResources(void) {
    FreePaddle();
    FreeBall();
    freeBlockTextures();
    FreeAudioSystem();
}