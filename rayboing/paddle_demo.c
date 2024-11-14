#include <stdio.h>
#include <raylib.h>
#include "paddle.h"
#include "faketypes.h"

// macros

const int SCREEN_WIDTH = 582;
const int SCREEN_HEIGHT = 720;


void ShowPaddleSize(void);

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Paddle Test");
    SetTargetFPS(60);

    InitialisePaddle();
    ResetPaddleStart();

    while (!WindowShouldClose()) {
    
        // adjust for paddle movement

        if (IsKeyPressed(KEY_SPACE)) ResetPaddleStart();

        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) ChangePaddleSize(SIZE_UP);
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) ChangePaddleSize(SIZE_DOWN);
        if (IsKeyPressed(KEY_R)) ToggleReverse();
        if (IsKeyPressed(KEY_N)) SetReverseOff();

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) MovePaddle(PADDLE_RIGHT);
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) MovePaddle(PADDLE_LEFT);


        // update display

        BeginDrawing();
        ClearBackground(BLACK);
        ShowPaddleSize();
        DrawPaddle();
        EndDrawing();

    }

    // release resources
    FreePaddle();

    // exit program
    CloseWindow();

    return 0;

}

void ShowPaddleSize(void) {

    const int FONTSIZE = 20;

    char text[40];
    snprintf(text, sizeof(text), "Paddle Size: %d   Position: %d", GetPaddleSize(), GetPaddlePosition());

    int xPos = (GetScreenWidth() - MeasureText(text, FONTSIZE)) / 2;
    int yPos = 20;

    DrawText(text, xPos, yPos, FONTSIZE, WHITE); 

}
