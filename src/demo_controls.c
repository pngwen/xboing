#include <stdbool.h>
#include <raylib.h>
#include <GLFW/glfw3.h>
#include "demo_controls.h"
#include "paddle.h"
#include <stdio.h>

static bool mouseInitialized = false;

// ----- Keyboard input -----
bool IsInputPaddleLeft(void) {
    return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
}

bool IsInputPaddleRight(void) {
    return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
}

bool IsInputQuitGame(void) {
    return IsKeyPressed(KEY_X);
}

bool IsInputReleaseBall(void) {
    return IsKeyPressed(KEY_SPACE);
}

bool IsInputRestartAfterEnd(void) {
    return IsKeyPressed(KEY_SPACE);
}
