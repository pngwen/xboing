#include <stdio.h>
#include <raylib.h>
#include "paddle.h"
#include "faketypes.h"

// macros

const int SCREEN_WIDTH = 582;
const int SCREEN_HEIGHT = 720;

void DrawStatusText(void);
void DrawTextLine(int ypos, char *text);

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

        DrawStatusText();
        DrawPaddle();

        EndDrawing();

    }

    // release resources
    FreePaddle();

    // exit program
    CloseWindow();

    return 0;

}

void DrawStatusText() {


    char text[40];
    int size = sizeof(text);
    int ypos = 20;
    const int yoffset = 25;

    DrawTextLine(ypos, "Paddle Information");
    ypos += yoffset * 2;

    snprintf(text, size, "Position:  %d", GetPaddlePositionX());
    DrawTextLine(ypos, text);
    ypos += yoffset;

    snprintf(text, size, "Size:  %s", GetPaddleDescription());
    DrawTextLine(ypos, text);
    ypos += yoffset;

    snprintf(text, size, "Pixels:  %d", GetPaddleSize());
    DrawTextLine(ypos, text);
    ypos += yoffset;

    snprintf(text, size, "Reverse:  %s", GetPaddleReverse() == True ? "True" : "False");
    DrawTextLine(ypos, text);

}

void DrawTextLine(int ypos, char *text) {
    const int FONTSIZE = 20;
    int xpos = (GetScreenWidth() - MeasureText(text, FONTSIZE)) / 2;
    DrawText(text, xpos, ypos, FONTSIZE, WHITE);
}
