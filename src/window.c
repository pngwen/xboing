#include <stdio.h>
#include <math.h>
#include "window.h"

static int logicalWidth = 0;
static int logicalHeight = 0;
static RenderTexture2D target = {0};
static bool isFullscreen = false;

void WindowInit(int w, int h, const char *title)
{
    logicalWidth = w;
    logicalHeight = h;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w, h, title);
    SetTargetFPS(60);
    // Create off‑screen render texture at logical size
    target = LoadRenderTexture(logicalWidth, logicalHeight);
    // Use bilinear filter for smoother scaling when window is larger
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
}

void WindowBegin(void)
{
    BeginDrawing();
    BeginTextureMode(target);
    ClearBackground(BLACK);
}

static Vector2 GetWindowScaleAndOffset(int *offsetX, int *offsetY)
{
    int winW = GetScreenWidth();
    int winH = GetScreenHeight();
    float scale = fmin((float)winW / logicalWidth, (float)winH / logicalHeight);
    int drawW = (int)(logicalWidth * scale);
    int drawH = (int)(logicalHeight * scale);
    *offsetX = (winW - drawW) / 2;
    *offsetY = (winH - drawH) / 2;
    return (Vector2){scale, scale};
}

void WindowEnd(void)
{
    EndTextureMode(); // finish drawing to target
    ClearBackground(BLACK); // clear window before drawing scaled texture
    // compute scaling & centering
    int offsetX, offsetY;
    Vector2 scale = GetWindowScaleAndOffset(&offsetX, &offsetY);
    // source rect covers whole texture (flipped vertically)
    Rectangle src = {0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height};
    // destination rect on screen
    Rectangle dst = {(float)offsetX, (float)offsetY, (float)logicalWidth * scale.x, (float)logicalHeight * scale.y};
    DrawTexturePro(target.texture, src, dst, (Vector2){0,0}, 0.0f, WHITE);

    // Fullscreen toggle detection (F11 or Alt+Enter)
    if (IsKeyPressed(KEY_F11) || ((IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) && IsKeyPressed(KEY_ENTER))) {
        WindowToggleFullscreen();
    }
    EndDrawing();
}

bool WindowToggleFullscreen(void)
{
    static int savedWidth = 0;
    static int savedHeight = 0;

    if (isFullscreen)
    {
        // Leaving fullscreen – restore previous window size
        if (savedWidth > 0 && savedHeight > 0)
            SetWindowSize(savedWidth, savedHeight);
        ToggleBorderlessWindowed();
        isFullscreen = false;
    }
    else
    {
        // Entering fullscreen – save current size and resize to monitor
        savedWidth = GetScreenWidth();
        savedHeight = GetScreenHeight();
        ToggleBorderlessWindowed();
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
        isFullscreen = true;
    }
    return isFullscreen;
}

Vector2 WindowScreenToLogical(Vector2 screenPos)
{
    int offsetX, offsetY;
    Vector2 scale = GetWindowScaleAndOffset(&offsetX, &offsetY);
    // Translate screen coordinate into the area where the logical texture is drawn
    float x = (screenPos.x - offsetX) / scale.x;
    float y = (screenPos.y - offsetY) / scale.y;
    return (Vector2){x, y};
}

int WindowLogicalWidth(void) { return logicalWidth; }
int WindowLogicalHeight(void) { return logicalHeight; }
