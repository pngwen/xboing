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

#define PADDLE_COUNT 3

typedef struct {
	Texture2D img;
	int size;
} Paddle;

const int PIXEL_SMALL  = 40;
const int PIXEL_MEDIUM = 50;
const int PIXEL_HUGE   = 70;

Paddle paddles[PADDLE_COUNT];

int reverseOn;
int stickyOn;
int paddleIndex;
int	paddlePosition;

void DrawPaddle() { 
	DrawTexture(paddles[paddleIndex].img, paddlePosition, GetScreenHeight() - DIST_BASE, WHITE);
 }

void InitialisePaddle(void) { 

	//define parallel arrays for paddle sizes

	const char *images[PADDLE_COUNT] = {
		"./bitmaps/paddle/padsml.png", 
		"./bitmaps/paddle/padmed.png", 
		"./bitmaps/paddle/padhuge.png"
	};

	const int sizes[PADDLE_COUNT] = {PIXEL_SMALL, PIXEL_MEDIUM, PIXEL_HUGE};

	// initialize variables before loop
	int errorFlag = False;

	// create textures for each paddle size
	for (int i = 0; i < PADDLE_COUNT; i++) {

		// load texture and set pixel size
		Image img = LoadImage(images[i]);
		paddles[i].img = LoadTextureFromImage(img);
		paddles[i].size = sizes[i];

		// check if texture loaded successfully
		if (paddles[i].img.id == 0) {
			//fprintf(stderr, "Error: failed to load texture InitialisePaddle() file: %s.\n", images[i]);
			errorFlag = True;
		}

		UnloadImage(img);
	}

	// stop program if textures failed to load
	if (errorFlag == True) CloseWindow();

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

	int distance = PADDLE_VEL * (reverseOn == True ? -1 : 1);

	switch(direction) {
		case PADDLE_LEFT:
			paddlePosition -= distance;
			break;
		case PADDLE_RIGHT:
			paddlePosition += distance;
			break;
	}

	if (paddlePosition < 0) paddlePosition = 0;

	int maxHPosition = GetScreenWidth() - paddles[paddleIndex].size;
	if (paddlePosition > maxHPosition) paddlePosition = maxHPosition;
	
}

int GetPaddleSize(void) {
	return paddles[paddleIndex].size;
}

int GetPaddlePosition(void) {
	return paddlePosition;
}

void ResetPaddleStart() {

	paddleIndex = PADDLE_HUGE;
	paddlePosition = (GetScreenWidth() - paddles[paddleIndex].size) / 2;
	reverseOn = False;

 }

void ChangePaddleSize(int changeDirection) {

	int oldSize = paddles[paddleIndex].size;
	
	switch (changeDirection) {
		case SIZE_UP:
			if (paddleIndex < PADDLE_COUNT -1) paddleIndex++;
			break;

		case SIZE_DOWN:
			if (paddleIndex > 0) paddleIndex--;
			break;
	}

	paddlePosition -= (paddles[paddleIndex].size - oldSize) / 2;

	MovePaddle(PADDLE_NONE);

}
