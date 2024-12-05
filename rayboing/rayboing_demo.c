#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>

#include "demo_gamemodes.h"
#include "demo_blockloader.h"
#include "demo_ball.h"
#include "paddle.h"


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
    SetTargetFPS(60);
    windowInitialized = true;

    if (!ValidateParamFilename(argumentCount, arguments)) {
        fprintf(stderr, "Program halt on map validation");
    } else if (!loadBlockTextures()) {
        fprintf(stderr, "Program halt on iniitalize block texture");
    } else if (!InitialisePaddle()) {
        fprintf(stderr, "Program halt on initialize paddle");
    } else if (!InitializeBall()) {
        fprintf(stderr, "Program halt on initialize ball");
    } else {

        initializePlayArea();
        SetGameMode(MODE_INITGAME);
        rtnCode = 0;

    }


    // main game loop
    GAME_MODES currentMode = GetGameMode();
    while (currentMode != MODE_EXIT) {

        switch (currentMode) {

            case MODE_INITGAME:
                RunInitGameMode(arguments[1]);
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

        } // switch 

        if (WindowShouldClose()) SetGameMode(MODE_EXIT);
        currentMode = GetGameMode();

    } // while


    if (windowInitialized) CloseWindow();
    ReleaseResources();
    

    // exit program
    return rtnCode;

}

bool ValidateParamFilename(int argumentCount, char *arguments[]) {

    // ensure only one argument
    if (argumentCount != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", arguments[0]);
        return false;
    }

    // check if file exists
    const char *fileName = arguments[1];
    FILE *file = fopen(fileName, "r");
    if (!file) {
        fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", fileName);
        return false;
    } else {
        fprintf(stdout, "Running Rayboing with map '%s'\n", fileName);
        fclose(file);
    }

    return true;
}

void ReleaseResources(void) {
    FreePaddle();
    FreeBall();
    freeBlockTextures();
}