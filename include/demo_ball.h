#ifndef _DEMO_BALL_H_
#define _DEMO_BALL_H_

#include <stdbool.h>

#include "paddle.h"

bool InitializeBall(void);
void ResetBall(void);
void FreeBall(void);
void MoveBall(void);
void DrawBall(void);
void ReleaseBall(void);
void SetBallSticky(void);
void IncreaseBallSpeed(void);


#endif // _DEMO_BALL_H_