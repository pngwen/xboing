#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include "intro.h"

static const int SCREEN_WIDTH = 575;
static const int SCREEN_HEIGHT = 720;

void ShowIntroScreen(void) {
    // Simple loop that draws text and waits for key press
    printf("Intro started\n");
    fflush(stdout);
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            const char *title = "Rayboing";
            const char *prompt = "Press ENTER or SPACE to Start";
            int titleSize = 48;
            int promptSize = 20;
            int titleWidth = MeasureText(title, titleSize);
            int promptWidth = MeasureText(prompt, promptSize);
            DrawText(title, (SCREEN_WIDTH - titleWidth)/2, SCREEN_HEIGHT/3, titleSize, RAYWHITE);
            DrawText(prompt, (SCREEN_WIDTH - promptWidth)/2, SCREEN_HEIGHT/3 + 80, promptSize, LIGHTGRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE)) {
            // Start requested
            printf("Intro ended - start requested\n"); fflush(stdout);
            return;
        }
    }

    // If window closed, just return (main will handle exit)
}
