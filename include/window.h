#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <raylib.h>

/**
 * @brief Base (virtual) resolution of the game.
 *
 * All game drawing and input happens in this coordinate space, at this fixed
 * aspect ratio. window.c letterbox-scales the render target to the actual OS
 * window, so changing GAME_WIDTH/GAME_HEIGHT here changes the size of the
 * game and everything inside it (play area, paddle, ball, blocks) in one place.
 */
#define GAME_WIDTH  575
#define GAME_HEIGHT 720

/**
 * @brief Opens a resizable OS window and creates the game render target.
 *
 * Must be called before any textures are loaded. Initializes the window with
 * the base resolution, enables resizing, and prepares internal scaling state.
 */
void InitGameWindow(void);

/**
 * @brief Begin drawing the frame in game space.
 *
 * Must wrap all game drawing for the frame. After the matching EndWindowSize()
 * the frame is automatically letterbox-scaled to the OS window.
 */
void BeginWindowSize(void);

/**
 * @brief Finish drawing the frame in game space.
 *
 * Copies the game frame to the OS window, centered and uniformly scaled to
 * fill it as much as possible while preserving the base aspect ratio.
 */
void EndWindowSize(void);

/**
 * @brief Returns the mouse position translated from screen to game space.
 *
 * Use instead of GetMousePosition() for all game input, since the game is
 * scaled and letterboxed inside the OS window.
 *
 * @return Vector2 position within the base resolution
 */
Vector2 GetGameMousePosition(void);

/**
 * @brief Returns the current uniform scale from game space to screen.
 *
 * @return float scale factor applied this frame (client height / GAME_HEIGHT)
 */
float GetWindowScale(void);

/**
 * @brief Moves the OS cursor to the position matching a position in game space.
 *
 * Inverse of GetGameMousePosition(); used to lock the visible cursor to the
 * paddle.
 *
 * @param position position within the base resolution
 */
void SetGameMousePosition(Vector2 position);

/**
 * @brief Unloads the render target and closes the OS window.
 *
 * Counterpart to InitGameWindow(); call exactly once at program exit.
 */
void FreeGameWindow(void);

#endif // _WINDOW_H_
