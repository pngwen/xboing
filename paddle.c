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
 * $Id: paddle.c,v 1.1.1.1 1994/12/16 01:36:45 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/paddle.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:45 $
 *
 * $Log: paddle.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:45  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <raylib.h>
#include "faketypes.h"
#include "paddle.h"
#include "demo_blockloader.h"

#define PADDLE_COUNT  3

const int PADDLE_INITIAL_INDEX = 1;
const int PADDLE_VEL = 600;  // pixels per second

typedef struct {
	Texture2D img;
	char *description;
	int size;
	char *filepath;
} Paddle;

Paddle paddles[PADDLE_COUNT];

int reverseOn;
int paddleIndex;
int	paddlePosition;

int GetPaddlePositionY(void);

void DrawPaddle(void) { 
	DrawTexture(paddles[paddleIndex].img, paddlePosition, GetPaddlePositionY(), WHITE);
 }

 int GetPaddlePositionY(void) {
	//return GetScreenHeight() - DIST_BASE;
	return getPlayWall(WALL_BOTTOM).y - DIST_BASE;
 }

bool InitialisePaddle(void) { 

	// do not load images if program is closing
	if (WindowShouldClose()) return false;

	Texture2D emptyTexture = {0};

	// textures must be loaded from smallest to largest
	paddles[0] = (Paddle){emptyTexture, "Small",  40, "./bitmaps/paddle/padsml.png"};
	paddles[1] = (Paddle){emptyTexture, "Medium", 50, "./bitmaps/paddle/padmed.png"};
	paddles[2] = (Paddle){emptyTexture, "Huge",   70, "./bitmaps/paddle/padhuge.png"};

	// initialize variables before loop
	int errorFlag = False;

	// create textures for each paddle size
	for (int i = 0; i < PADDLE_COUNT; i++) {

		// load paddle texture 
		Image img = LoadImage(paddles[i].filepath);
		paddles[i].img = LoadTextureFromImage(img);

		// check if texture loaded successfully
		if (paddles[i].img.id == 0) {
			fprintf(stderr, "Error: failed to load texture InitialisePaddle() file: %s.\n", paddles[i].filepath);
			errorFlag = True;
		}

		UnloadImage(img);
	}

	// stop program if textures failed to load
	return !errorFlag;

}

void SetReverseOff(void) {
	reverseOn = False;
}

void ToggleReverse(void) {

    reverseOn = (reverseOn == True) ? False : True;

	// TODO: add display text when implemented
	// DrawSpecials(display);
}

void FreePaddle(void) {
	for (int i = 0; i < PADDLE_COUNT; i++) {
		UnloadTexture(paddles[i].img);
	}
}

void MovePaddle(int direction) {

	// calculate the movement distance, adjusted for reverse flag
	int distance = PADDLE_VEL * (reverseOn == True ? -1 : 1) * GetFrameTime();

	// apply the move based on direction
	switch(direction) {
		case PADDLE_LEFT:
			paddlePosition -= distance;
			break;
		case PADDLE_RIGHT:
			paddlePosition += distance;
			break;
	}

	// keep position within window boundries
	int x = getPlayWall(WALL_LEFT).width;
	if (paddlePosition < x) paddlePosition = x;

	int maxHPosition = getPlayWall(WALL_RIGHT).x - paddles[paddleIndex].size;
	if (paddlePosition > maxHPosition) paddlePosition = maxHPosition;
	
}

int GetPaddleSize(void) {
	return paddles[paddleIndex].size;
}

int GetPaddlePositionX(void) {
	return paddlePosition;
}

Rectangle GetPaddleCollisionRec(void) {
	return (Rectangle){
		paddlePosition,
		GetPaddlePositionY(),
		paddles[paddleIndex].img.width,
		paddles[paddleIndex].img.height
	};
}

int GetPaddleReverse(void) {
	return reverseOn;
}

char *GetPaddleDescription(void) {
	return paddles[paddleIndex].description;
}

void ResetPaddleStart(void) {

	// set size and center paddle
	paddleIndex = PADDLE_INITIAL_INDEX;
	paddlePosition = (GetScreenWidth() - paddles[paddleIndex].size) / 2;
	reverseOn = False;

 }

void ChangePaddleSize(int changeDirection) {

	// capture the old pixel size
	int oldSize = paddles[paddleIndex].size;
	
	// adjust paddle index based on change in size
	switch (changeDirection) {
		case SIZE_UP:
			if (paddleIndex < PADDLE_COUNT -1) paddleIndex++;
			break;

		case SIZE_DOWN:
			if (paddleIndex > 0) paddleIndex--;
			break;
	}

	// adjust position to center the change in size
	paddlePosition -= (paddles[paddleIndex].size - oldSize) / 2;

	// move to ensure resize remains inside window
	MovePaddle(PADDLE_NONE);

}


Vector2 GetBallSpawnPointOnPaddle() {
	return (Vector2){
		paddlePosition + paddles[paddleIndex].size / 2,
		GetPaddlePositionY()
	};
}