/*
 * window.c - Central window / resolution module
 *
 * The game draws everything into a virtual render target at the base
 * resolution (GAME_WIDTH x GAME_HEIGHT, defined in window.h). The target is
 * then letterbox-scaled to the real OS window, so the OS window can be any
 * size while the game and everything inside it (play area, paddle, ball,
 * blocks) stays consistent at its designed aspect ratio.
 */

#include <raylib.h>

#include "window.h"

static RenderTexture2D windowTarget = {0};
static float windowScale = 1.0f;
static Vector2 windowOffset = {0};

/*
 * Recompute the uniform scale and letterbox offset for the current window size.
 * The whole game stays the same aspect ratio; unused space becomes black bars.
 */
static void UpdateWindowSize(void)
{
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    // uniform scale: fit the base resolution inside the window
    windowScale = ((float)screenW / GAME_WIDTH < (float)screenH / GAME_HEIGHT)
                      ? (float)screenW / GAME_WIDTH
                      : (float)screenH / GAME_HEIGHT;

    // center the game in the window (letterbox offsets)
    windowOffset.x = ((float)screenW - GAME_WIDTH * windowScale) / 2.0f;
    windowOffset.y = ((float)screenH - GAME_HEIGHT * windowScale) / 2.0f;
}

void InitGameWindow(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(GAME_WIDTH / 2, GAME_HEIGHT / 2);
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Rayboing Demo");
    SetTargetFPS(60);

    // render target all game drawing goes into, at the base resolution
    windowTarget = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
}

void BeginWindowSize(void)
{
    UpdateWindowSize();
    BeginTextureMode(windowTarget);
}

void EndWindowSize(void)
{
    EndTextureMode();

    // present the letterboxed frame on the real screen
    // (the only screen present per frame; game drawing never calls EndDrawing
    //  while the render target is bound)
    BeginDrawing();

    // clear the full window so the letterbox bars are black
    ClearBackground(BLACK);

    // blit the game frame scaled into the letterboxed position
    // (negative source height because OpenGL textures are stored upside down)
    Rectangle source = {0.0f, 0.0f, (float)windowTarget.texture.width,
                        (float)-windowTarget.texture.height};
    Rectangle dest = {windowOffset.x, windowOffset.y, GAME_WIDTH * windowScale,
                      GAME_HEIGHT * windowScale};
    DrawTexturePro(windowTarget.texture, source, dest, (Vector2){0.0f, 0.0f},
                   0.0f, WHITE);

    EndDrawing();
}

Vector2 GetGameMousePosition(void)
{
    // recompute scale/offsets so the transform reflects the current window
    // size even when called before the frame's draw phase (resize frames)
    UpdateWindowSize();

    // map from real screen space back into game space
    Vector2 position = GetMousePosition();

    position.x = (position.x - windowOffset.x) / windowScale;
    position.y = (position.y - windowOffset.y) / windowScale;

    // clamp to the game area so off-window input stays inside the game
    if (position.x < 0.0f)
        position.x = 0.0f;
    if (position.x > GAME_WIDTH)
        position.x = GAME_WIDTH;
    if (position.y < 0.0f)
        position.y = 0.0f;
    if (position.y > GAME_HEIGHT)
        position.y = GAME_HEIGHT;

    return position;
}

float GetWindowScale(void)
{
    return windowScale;
}

void SetGameMousePosition(Vector2 position)
{
    // recompute scale/offsets so the transform is never stale
    UpdateWindowSize();

    // map out of game space into real screen space
    position.x = position.x * windowScale + windowOffset.x;
    position.y = position.y * windowScale + windowOffset.y;

    SetMousePosition((int)position.x, (int)position.y);
}

void FreeGameWindow(void)
{
    if (windowTarget.texture.id != 0)
    {
        UnloadRenderTexture(windowTarget);
        windowTarget.texture.id = 0;
    }
    CloseWindow();
}
