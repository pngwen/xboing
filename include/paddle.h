#ifndef _PADDLE_H_
#define _PADDLE_H_

/*
 * XBoing - An X11 blockout style computer game
 *
 * (c) Copyright 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 *
 * In no event shall the author be liable to any party for direct, indirect,
 * special, incidental, or consequential damages arising out of the use of
 * this software and its documentation, even if the author has been advised
 * of the possibility of such damage.
 *
 * The author specifically disclaims any warranties, including, but not limited
 * to, the implied warranties of merchantability and fitness for a particular
 * purpose.  The software provided hereunder is on an "AS IS" basis, and the
 * author has no obligation to provide maintenance, support, updates,
 * enhancements, or modifications.
 */

/* 
 * =========================================================================
 *
 * $Id: paddle.h,v 1.1.1.1 1994/12/16 01:36:53 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/paddle.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:53 $
 *
 * $Log: paddle.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:53  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

#define PADDLE_NONE		0
#define PADDLE_LEFT		1
#define PADDLE_RIGHT	2

#define DIST_BASE   	30

#define SIZE_UP         1
#define SIZE_DOWN       2


/**
 * @brief Loads paddle images into memory as Raylib Texture2D
 * @note If an image fails to load, Raylib close window flag is set to true
 * 
 */
bool InitialisePaddle(void);


/**
 * @brief Unloads paddle images loaded with InitialisePaddle()
 * 
 */
void FreePaddle(void);


/**
 * @brief Draws the current paddle image at the current paddle position
 * 
 */
void DrawPaddle(void);


/**
 * @brief Moves the paddle horizontally.
 * 
 * The paddle will move horizontally a fixed amount based on the direction passed. 
 * The direction moved takes into account the current value of the Reverse flag.
 * 
 * @param direction PADDLE_LEFT or PADDLE_RIGHT to move
 */
void MovePaddle(int direction);


/**
 * @brief Sets the paddle to the default size, turns off Reverse, and centers the paddle on the screen
 * 
 */
void ResetPaddleStart(void);


/**
 * @brief Returns the paddle size in pixels
 * 
 * @return int 
 */
int GetPaddleSize(void);


/**
 * @brief Returns the paddle size as a description
 * 
 * @return char* c-string description of size
 */
char *GetPaddleDescription(void);


/**
 * @brief Returns the paddle position based on upper left corner
 * 
 * @return int upper left paddle pixel
 */
int GetPaddlePositionX(void);

int GetPaddlePositionY(void);

Rectangle GetPaddleCollisionRec(void);


/**
 * @brief Returns the Reverse flag value
 * 
 * @return int 1 for reverse on, 0 for reverse off
 */
int GetPaddleReverse(void);


/**
 * @brief Flips the value of the reverse flag
 * 
 */
void ToggleReverse(void);


/**
 * @brief Turns reverse flag off
 * 
 */
void SetReverseOff(void);


/**
 * @brief Changes the size of the paddle
 * 
 * Increases or decreases the size of the paddle by one increment.
 * Does nothing if paddle is at the maximum or minimum size
 * 
 * @param changeDirection SIZE_UP or SIZE_DOWN
 */
void ChangePaddleSize(int changeDirection);

Vector2 GetBallSpawnPointOnPaddle();


// not used in program ??
// #define PADDLE_HC  		4
// #define PADDLE_HEIGHT 	9

#endif
