#include <stdbool.h>
#include <raylib.h>

#include "demo_gamemodes.h"
#include "paddle.h"
#include "demo_controls.h"
#include "demo_blockloader.h"
#include "demo_ball.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
const int INITIAL_LIVES = 3;
int livesRemaining = 0;

GAME_MODES gameState = MODE_EXIT;

// track the current level file so we can advance to the next level after a win
static char currentLevelFile[512] = {0};

void RenderGameScreen(void);
void DrawStatusText(const char *displayText);

GAME_MODES GetGameMode(void)
{
    return gameState;
}

void SetGameMode(GAME_MODES mode)
{
    gameState = mode;
}

void RunInitGameMode(const char *fileName)
{

    // Always load blocks when starting a new level file, or when out of lives
    if (livesRemaining <= 0 || (fileName && currentLevelFile[0] != '\0' && strcmp(fileName, currentLevelFile) != 0) || (fileName && currentLevelFile[0] == '\0'))
    {
        // strcmp(fileName, currentLevelFile) != 0 checks whether the requested
        // level filename differs from the one currently loaded. If different,
        // we must reload block data for the new level.
        loadBlocks(fileName);
        // remember which level file was loaded
        if (fileName) {
            // Use strncpy to avoid buffer overflow when copying the filename into
            // our fixed-size `currentLevelFile` buffer. We copy at most
            // sizeof(currentLevelFile)-1 bytes and then explicitly NUL-terminate.
            strncpy(currentLevelFile, fileName, sizeof(currentLevelFile) - 1);
            currentLevelFile[sizeof(currentLevelFile) - 1] = '\0';
        }
        livesRemaining = INITIAL_LIVES;
    }

    livesRemaining--;

    ResetPaddleStart();
    ResetBall();
    // ensure countdown timer does not start until ball released
    setTimerActive(false);

    RenderGameScreen();

    SetGameMode(MODE_PLAY);
}

void RunPlayMode(void)
{
    // --- Keyboard controls ---
    if (IsInputPaddleLeft())
        MovePaddle(PADDLE_LEFT);
    if (IsInputPaddleRight())
        MovePaddle(PADDLE_RIGHT);

    // --- Mouse controls ---
    extern bool mouseControls; // make sure this matches your variable definition
    if (mouseControls)
    {
        Vector2 mousePosition = GetMousePosition();
        SetPaddlePosition(mousePosition.x);
    }

    // --- Ball release ---
    if (IsInputReleaseBall())
        ReleaseBall();

    // --- Update ball ---
    MoveBall();

    // --- Update timer ---
    timeDecrement();

    // --- Render everything ---
    RenderGameScreen();

    // --- Quit handling ---
    if (IsInputQuitGame())
    {
        livesRemaining = 0;
        SetGameMode(MODE_CANCEL);
    }
}

void RunEndMode(void)
{

    RenderGameScreen();

    // If user requested quit, exit
    if (IsInputQuitGame())
    {
        SetGameMode(MODE_EXIT);
        return;
    }

    // If at WIN screen, allow advancing to the next numeric level using Enter/Space
    if (GetGameMode() == MODE_WIN)
    {
        // Check for Enter/Space input
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            // Try to construct next level filename by finding a numeric sequence in the current file
            if (currentLevelFile[0] != '\0')
            {
                // find the last sequence of digits in the filename
                const char *p = currentLevelFile + strlen(currentLevelFile) - 1;
                while (p >= currentLevelFile && !isdigit((unsigned char)*p)) p--;
                if (p >= currentLevelFile)
                {
                    // find start of digit sequence
                    const char *end = p;
                    const char *start = end;
                    while (start > currentLevelFile && isdigit((unsigned char)*(start-1))) start--;

                    int len = (int)(end - start) + 1;
                    char numberBuf[32] = {0};
                    // Copy the digit substring (len characters) into numberBuf and
                    // then NUL-terminate. strncpy could be used here but we copy
                    // the exact known length and add the terminator explicitly.
                    strncpy(numberBuf, start, len);
                    numberBuf[len] = '\0';

                    int num = atoi(numberBuf);
                    int width = len; // preserve zero padding width
                    int next = num + 1;

                    // build next filename
                    char nextFile[512] = {0};
                    // copy prefix up to start
                    int prefixLen = (int)(start - currentLevelFile);
                    // Copy the filename prefix (everything before the numeric
                    // sequence) into nextFile. We copy only prefixLen bytes so the
                    // buffer cannot overflow; the rest will be appended afterwards.
                    if (prefixLen > 0) strncpy(nextFile, currentLevelFile, prefixLen);
                    // append incremented number with preserved width
                    // Build a format string (e.g. "%02d") into `fmt` while
                    // preventing buffer overflow. snprintf bounds the write.
                    char fmt[16];
                    snprintf(fmt, sizeof(fmt), "%%0%dd", width);
                    // Format the incremented numeric part into numStr using the
                    // generated format. We use snprintf to ensure the output is
                    // NUL-terminated and cannot overflow numStr.
                    char numStr[32];
                    snprintf(numStr, sizeof(numStr), fmt, next);
                    // Append the numeric string into nextFile safely. Use
                    // strncat with the remaining buffer space to avoid overflow.
                    strncat(nextFile, numStr, sizeof(nextFile) - strlen(nextFile) - 1);
                    // append the rest of filename after the number
                    const char *rest = end + 1;
                    // Append any suffix (like ".data") using strncat and the
                    // remaining buffer space to avoid overflowing nextFile.
                    if (*rest) strncat(nextFile, rest, sizeof(nextFile) - strlen(nextFile) - 1);

                    // check that file exists
                    FILE *f = fopen(nextFile, "r");
                    if (f)
                    {
                        fclose(f);
                        // force reload of blocks by resetting livesRemaining so RunInitGameMode will load
                        livesRemaining = 0;
                        // load next level
                        RunInitGameMode(nextFile);
                        return;
                    }
                }
            }

            // If we couldn't determine/locate a next level, fall back to restart current or exit
            SetGameMode(MODE_EXIT);
            return;
        }
    }

    // For lose/cancel or other end screens allow restart (replay same level)
    if (IsInputRestartAfterEnd())
    {
        // On restart, reload the same level file if we have one
        if (currentLevelFile[0] != '\0')
        {
            RunInitGameMode(currentLevelFile);
        }
        else
        {
            SetGameMode(MODE_INITGAME);
        }
    }
}

void RenderGameScreen(void)
{

    BeginDrawing();

    ClearBackground(BLACK);

    drawWalls();
    drawBlocks();
    DrawBall();
    DrawPaddle();
    drawBorder();

    switch (GetGameMode())
    {

    case MODE_WIN:
        DrawStatusText("You Won! Congrats!!!");
                // Show flashing prompt to advance when on the win screen
                {
                    const char *prompt = "Press SPACE or ENTER to Move On";
                    const int PROMPT_SIZE = 20;
                    int px = (GetScreenWidth() - MeasureText(prompt, PROMPT_SIZE)) / 2;
                    int py = (GetScreenHeight() / 3) + 80; // below the main status text
                          double t = GetTime();
                          /* Smooth pulsing: use a sine wave to compute alpha in [0,1].
                              Use the numeric constant 2*PI directly to avoid introducing
                              an extra identifier (keeps code C89-friendly). */
                          double alpha = 0.5 * (1.0 + sin(6.283185307179586 * t)); /* 2*PI */
                          if (alpha < 0.0) alpha = 0.0;
                          if (alpha > 1.0) alpha = 1.0;
                          unsigned char a = (unsigned char)(alpha * 255.0);
                          /* Fill Color explicitly to avoid compound-literal syntax that
                              may be unsupported in some C dialects. */
                          Color textColor;
                        // light green color for the prompt (RGB: 144,238,144)
                        textColor.r = 144; textColor.g = 238; textColor.b = 144; textColor.a = a;
                          DrawText(prompt, px, py, PROMPT_SIZE, textColor);
                }
        break;

    case MODE_LOSE:
        if (livesRemaining > 0)
        {
            const char *txt = TextFormat("Remaining attempts: %d", livesRemaining);
            DrawStatusText(txt);
        }
        else
        {
            DrawStatusText("You Lost! Sadface...");
        }
        break;

    case MODE_CANCEL:
        DrawStatusText("Game canceled");

    default:
        break;
    }

    const char *lives = TextFormat("Balls Remaining: %d", livesRemaining);
    DrawText(lives, 10, GetScreenHeight() - 20, 20, WHITE);

    const char *blocks = TextFormat("Blocks Remaining: %d", getBlockCount());
    DrawText(blocks, GetScreenWidth() - MeasureText(blocks, 20) - 10, 10, 20, WHITE);

    // Display remaining time
    const char *time = TextFormat("Time Remaining: %d", getTime());
    DrawText(time, 10, 10, 20, WHITE);

    if (GetPaddleReverse())
    {
        const char *reversed = "REVERSED!";
        DrawText(reversed, (GetScreenWidth() - MeasureText(reversed, 25)) / 2, 35, 25, YELLOW);
    }

    EndDrawing();
}

void DrawStatusText(const char *displayText)
{

    const int FONTSIZE = 40;
    const int PADDING = 20;

    const int width = MeasureText(displayText, FONTSIZE);
    const int xpos = (GetScreenWidth() - width) / 2;
    const int ypos = GetScreenHeight() / 3;

    DrawRectangle(xpos - PADDING, ypos - PADDING, width + 2 * PADDING, FONTSIZE + 2 * PADDING, BLACK);

    DrawText(displayText, xpos - 1, ypos - 1, FONTSIZE, RED);
    DrawText(displayText, xpos, ypos, FONTSIZE, GREEN);
}