#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <raylib.h>
#include <stdbool.h>

typedef enum {
    EDIT_LEVEL,
    EDIT_NONE,
    EDIT_TEST,
    EDIT_WAIT,
    EDIT_FINISH
} EditStates;

extern EditStates EditState;

/*
 * Raylib-compatible replacements for the legacy X11 editor API.
 * These are migration scaffolds: signatures no longer take Display/Window/
 * XEvent and the implementations are being filled in incrementally.
 */
void SetUpEditor(void);
void Editor(void);
void FreeEditor(void);
void ResetEditor(void);
void handleEditorKeys(int key);
void HandleEditorMouseButtons(Vector2 mouse, int down);
void HandleButtonMotion(Vector2 mouse);
void RedrawEditorArea(void);
void SetUpBlockWindow(void);
void HandleEditorToolBar(Vector2 mouse, int down);
void SetCurrentSymbol(int symbol);
void HandleRandomBlocks(void);
void LoadALevel(void);
void SaveALevel(void);
void SetTimeForLevel(void);
void SetNameForLevel(void);
void handleAllEditorKeys(int key);
void SetEditWait(EditStates newMode, int waitFrame);
void DoEditWait(void);

#endif /* _EDITOR_H_ */
