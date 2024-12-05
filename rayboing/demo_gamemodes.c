#include <stdbool.h>
#include <raylib.h>

#include "demo_gamemodes.h"
#include "paddle.h"
#include "demo_controls.h"
#include "demo_blockloader.h"
#include "demo_ball.h"

const int INITIAL_LIVES = 3;
int livesRemaining = 0;

GAME_MODES gameState = MODE_EXIT;


void RenderGameScreen(void);
void DrawStatusText(const char *displayText);


GAME_MODES GetGameMode(void) {
    return gameState;
}

void SetGameMode(GAME_MODES mode) {
    gameState = mode;
}


void RunInitGameMode(const char *fileName) {


    if (livesRemaining <= 0) {
        loadBlocks(fileName);
        livesRemaining = INITIAL_LIVES;        
    } 

    livesRemaining--;

    ResetPaddleStart();
    ResetBall();

    RenderGameScreen();

    SetGameMode(MODE_PLAY);

}


void RunPlayMode(void) {


    if (IsInputPaddleLeft()) MovePaddle(PADDLE_LEFT);
    if (IsInputPaddleRight()) MovePaddle(PADDLE_RIGHT);

    if (IsInputReleaseBall()) ReleaseBall();

    MoveBall();

    RenderGameScreen();

    if (IsInputQuitGame()) {
        livesRemaining = 0;
        SetGameMode(MODE_CANCEL);
    }

}


void RunEndMode(void) {
    
    RenderGameScreen();

    if (IsInputQuitGame()) {
        SetGameMode(MODE_EXIT);

    } else if (IsInputRestartAfterEnd()) {
        SetGameMode(MODE_INITGAME);
    }
    
}


void RenderGameScreen(void) {

    BeginDrawing(); 

    ClearBackground(BLACK);

    drawWalls();
    drawBlocks();
    DrawBall();
    DrawPaddle();
    drawBorder();

    switch (GetGameMode()) {

        case MODE_WIN: 
            DrawStatusText("You Won! Congrats!!!");
            break;

        case MODE_LOSE:
            if (livesRemaining > 0) {
                const char *txt = TextFormat("Remaining attempts: %d", livesRemaining);
                DrawStatusText(txt);
            } else {
                DrawStatusText("You Lost! Sadface...");
            }
            break;  
        
        case MODE_CANCEL:
            DrawStatusText("Game canceled");

        default:
            break;

    }

    const char *lives = TextFormat("Balls Remaining: %d", livesRemaining);
    DrawText(lives, 10, 10, 20, WHITE);

    const char *blocks = TextFormat("Blocks Remaining: %d", getBlockCount());
    DrawText(blocks, GetScreenWidth() - MeasureText(blocks, 20)- 10, 10, 20, WHITE);

    if (GetPaddleReverse()) {
        const char *reversed = "REVERSED!";
        DrawText(reversed, (GetScreenWidth() - MeasureText(reversed, 25))/ 2, 35, 25, YELLOW);
    }

    EndDrawing();

}

void DrawStatusText(const char *displayText) {

    const int FONTSIZE = 40;
    const int PADDING = 20;

    const int width = MeasureText(displayText, FONTSIZE);
    const int xpos = (GetScreenWidth() - width) / 2;
    const int ypos = GetScreenHeight() / 3;

    DrawRectangle(xpos - PADDING, ypos - PADDING, width + 2 * PADDING, FONTSIZE + 2 * PADDING, BLACK);

    DrawText(displayText, xpos - 1, ypos - 1, FONTSIZE, RED);
    DrawText(displayText, xpos, ypos, FONTSIZE, GREEN);

}