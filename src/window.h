#ifndef WINDOW_H
#define WINDOW_H

#include <raylib.h>
#include <stdbool.h>

/* Initialize the window with a logical resolution.
   The OS window becomes resizable and the game is rendered to an off‑screen
   RenderTexture at the logical size.
*/
void WindowInit(int logicalWidth, int logicalHeight, const char *title);

/* Begin a new frame: starts drawing to the off‑screen RenderTexture. */
void WindowBegin(void);

/* End the frame: draws the RenderTexture to the actual window with
   letter‑boxing to preserve aspect ratio.
*/
void WindowEnd(void);

/* Toggle fullscreen (F11 or Alt+Enter). */
bool WindowToggleFullscreen(void);

/* Convert a screen‑space mouse position to logical coordinates. */
Vector2 WindowScreenToLogical(Vector2 screenPos);

/* Accessors for the logical resolution. */
int WindowLogicalWidth(void);
int WindowLogicalHeight(void);

#endif // WINDOW_H
