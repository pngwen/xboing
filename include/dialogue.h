#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include <raylib.h>
#include <stdbool.h>

#define DIALOGUE_WIDTH 440
#define DIALOGUE_HEIGHT 120

#define DISK_ICON 1
#define TEXT_ICON 2

#define TEXT_ENTRY_ONLY 1
#define NUMERIC_ENTRY_ONLY 2
#define ALL_ENTRY 3
#define YES_NO_ENTRY 4

typedef enum DialogueStates {
    DIALOGUE_MAP,
    DIALOGUE_UNMAP,
    DIALOGUE_TEXT,
    DIALOGUE_NONE,
    DIALOGUE_FINISHED
} DialogueStates;

/*
 * Raylib-compatible replacements for the legacy X11 dialogue API.
 */
void ProcessDialogue(void);
void handleDialogueKeys(void);
char *UserInputDialogueMessage(const char *message, int type, int entryValidation);
void FreeDialoguePixmaps(void);
void InitialiseDialoguePixmaps(void);

#endif /* _DIALOGUE_H_ */
