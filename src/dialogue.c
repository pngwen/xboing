#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <raylib.h>

#include "dialogue.h"

static DialogueStates dialogueState = DIALOGUE_NONE;
static int dialogueType = TEXT_ICON;
static int validation = ALL_ENTRY;
static char theMessage[80];
static char currentInput[1024];

static bool validateChar(int c)
{
    switch (validation) {
        case TEXT_ENTRY_ONLY:
            return c >= ' ' && c <= 'z';
        case YES_NO_ENTRY:
            return c == 'y' || c == 'Y' || c == 'n' || c == 'N';
        case NUMERIC_ENTRY_ONLY:
            return c >= '0' && c <= '9';
        case ALL_ENTRY:
        default:
            return c >= ' ' && c <= '~';
    }
}

static void redrawDialogue(void)
{
    const int fontSize = 20;
    const int textY = 70;

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText(theMessage, 20, 20, fontSize, GREEN);

    if (strlen(currentInput) > 0) {
        int width = MeasureText(currentInput, fontSize);
        DrawText(currentInput, (DIALOGUE_WIDTH - width) / 2, textY, fontSize, YELLOW);
    } else {
        DrawText("?", DIALOGUE_WIDTH / 2 - 5, textY, fontSize, WHITE);
    }

    DrawLine(10, 45, DIALOGUE_WIDTH - 10, 45, WHITE);
    EndDrawing();
}

static void handleDialogueInput(void)
{
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        dialogueState = DIALOGUE_FINISHED;
        return;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        size_t len = strlen(currentInput);
        if (len > 0) {
            currentInput[len - 1] = '\0';
        }
    }

    /* Accept at most one character per call for first pass; raylib queues input. */
    int c = GetCharPressed();
    while (c > 0) {
        if (validation == YES_NO_ENTRY && strlen(currentInput) > 0) {
            c = GetCharPressed();
            continue;
        }
        if (validateChar(c)) {
            if (MeasureText(currentInput, 20) < (DIALOGUE_WIDTH - 20)) {
                size_t len = strlen(currentInput);
                if (len + 1 < sizeof(currentInput)) {
                    currentInput[len] = (char)c;
                    currentInput[len + 1] = '\0';
                }
            }
        }
        c = GetCharPressed();
    }
}

char *UserInputDialogueMessage(const char *message, int type, int entryValidation)
{
    if (message) {
        strncpy(theMessage, message, sizeof(theMessage) - 1);
        theMessage[sizeof(theMessage) - 1] = '\0';
    } else {
        theMessage[0] = '\0';
    }

    dialogueType = type;
    validation = entryValidation;
    currentInput[0] = '\0';
    dialogueState = DIALOGUE_TEXT;

    while (!WindowShouldClose() && dialogueState != DIALOGUE_FINISHED) {
        handleDialogueInput();
        redrawDialogue();
    }

    dialogueState = DIALOGUE_FINISHED;
    return currentInput;
}

void ProcessDialogue(void)
{
    /* Compatibility no-op: raylib version drives the dialogue directly. */
}

void handleDialogueKeys(void)
{
    /* Compatibility no-op. */
}

void FreeDialoguePixmaps(void)
{
    /* No XPM/pixmap resources are loaded in the raylib version. */
}

void InitialiseDialoguePixmaps(void)
{
    /* No setup required for the raylib version. */
}
