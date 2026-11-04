#include "editor.h"
#include "demo_blockloader.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

EditStates EditState = EDIT_NONE;
static int editWaitFrame = 0;

void SetUpEditor(void)
{
    EditState = EDIT_LEVEL;
}

void Editor(void)
{
    /*
     * Migration in progress: the original X11 editor drew its own grid,
     * toolbar and symbol browser here.  Until the individual helpers below
     * are implemented this is intentionally a placeholder so the module
     * links under raylib.
     */
    DrawText("Level editor: raylib migration in progress",
             20, 20, 20, RED);
}

void FreeEditor(void)
{
    EditState = EDIT_NONE;
}

void ResetEditor(void)
{
    EditState = EDIT_LEVEL;
}

void handleEditorKeys(int key)
{
    (void)key;
}

void HandleEditorMouseButtons(Vector2 mouse, int down)
{
    (void)mouse;
    (void)down;
}

void HandleButtonMotion(Vector2 mouse)
{
    (void)mouse;
}

void RedrawEditorArea(void)
{
}

void SetUpBlockWindow(void)
{
}

void HandleEditorToolBar(Vector2 mouse, int down)
{
    (void)mouse;
    (void)down;
}

void SetCurrentSymbol(int symbol)
{
    (void)symbol;
}

void HandleRandomBlocks(void)
{
}

void LoadALevel(void)
{
}

void SaveALevel(void)
{
}

void SetTimeForLevel(void)
{
}

void SetNameForLevel(void)
{
}

void handleAllEditorKeys(int key)
{
    (void)key;
}

void SetEditWait(EditStates newMode, int waitFrame)
{
    EditState = newMode;
    editWaitFrame = waitFrame;
}

void DoEditWait(void)
{
    if (editWaitFrame > 0)
        editWaitFrame--;
}
