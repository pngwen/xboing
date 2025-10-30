// #ifndef _DEMO_CONTROLS_H_
// #define _DEMO_CONTROLS_H_


// #include <stdbool.h>
// #include <GLFW/glfw3.h>

// bool IsInputPaddleLeft(void);
// bool IsInputPaddleRight(void);
// bool IsInputQuitGame(void);
// bool IsInputReleaseBall(void);
// bool IsInputRestartAfterEnd(void);
// void InitMouseControls(void);
// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// void SetPaddlePosition(float x);
// float GetPaddleWidth(void);
// #endif // _DEMO_CONTROLS_H_

#ifndef _DEMO_CONTROLS_H_
#define _DEMO_CONTROLS_H_

#include <stdbool.h>
#include <GLFW/glfw3.h>

bool IsInputPaddleLeft(void);
bool IsInputPaddleRight(void);
bool IsInputQuitGame(void);
bool IsInputReleaseBall(void);
bool IsInputRestartAfterEnd(void);

#endif // _DEMO_CONTROLS_H_
