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
 * $Id: main.c,v 1.1.1.1 1994/12/16 01:36:47 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/main.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:47 $
 *
 * $Log: main.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:47  jck
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
#include <stdlib.h>
#include <stddef.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>

#include "score.h"
#include "presents.h"
#include "editor.h"
#include "special.h"
#include "audio.h"
#include "mess.h"
#include "ball.h"
#include "file.h"
#include "gun.h"
#include "demo.h"
#include "sfx.h"
#include "init.h"
#include "blocks.h"
#include "misc.h"
#include "level.h"
#include "bonus.h"
#include "stage.h"
#include "paddle.h"
#include "intro.h"
#include "inst.h"
#include "highscore.h"
#include "keys.h"
#include "keysedit.h"
#include "preview.h"
#include "dialogue.h"
#include "error.h"
#include "eyedude.h"

#include "main.h"

/*
 *  Internal macro definitions:
 */

/*
 *  Internal type declarations:
 */

#if NeedFunctionPrototypes
static KeySym 	GetKeySym(XEvent event);
static void 	handleGameMode(Display *display);
static void 	handleEventLoop(Display *display);
static void 	ToggleGamePaused(Display *display);
static void 	SetGamePaused(Display *display);
static void 	handleGameStates(Display *display);
static void 	handleMiscKeys(Display *display, KeySym keysym);
static void 	handleSpeedKeys(Display *display, KeySym keysym);
static void 	handleExitKeys(Display *display);
static void 	handleQuitKeys(Display *display);
#else
static void 	handleQuitKeys();
static void 	handleExitKeys();
static void 	handleSpeedKeys();
static void 	handleMiscKeys();
static void 	handleGameStates();
static KeySym 	GetKeySym();
static void 	handleGameMode();
static void 	handleEventLoop();
static void 	ToggleGamePaused();
static void 	SetGamePaused();
#endif

/*
 *  Internal variable declarations:
 */

int 			paddleMotion = 0;
int 			paddleDx = 0;
int 			speedLevel = 5;
int 			frame, gameActive;
int 			mode, oldMode;
static int 		iconified = False;
long 			speed;
static int 		userDelay = 1;
static int 		paddleControl;
static time_t 	pauseStartTime;
time_t 			pausedTime;
int 			UserTilts;


#if NeedFunctionPrototypes
int GetWarpSpeed(void)
#else
int GetWarpSpeed()
#endif
{
	/* Return warp speed in user terms */
	return (speedLevel);
}

#if NeedFunctionPrototypes
void SetUserSpeed(int delay)
#else
void SetUserSpeed(delay)
	int delay;
#endif
{
	long temp;

	/* Set an entire game speedup or slowdown speed */
	temp = (speed / (long) userDelay);
	userDelay = delay;
	speed = (long) (temp * userDelay);
	speedLevel = 10 - delay;
}

#if NeedFunctionPrototypes
int GetPaddleControlMode(void)
#else
int GetPaddleControlMode()
#endif
{
	/* Get the paddle control mode */
	return paddleControl;
}

#if NeedFunctionPrototypes
void SetPaddleControlMode(int type)
#else
void SetPaddleControlMode(type)
	int type;
#endif
{
	/* Set the paddle control mode to the new mode */
	paddleControl = type;
}

#if NeedFunctionPrototypes
void SetGameSpeed(int delay)
#else
void SetGameSpeed(delay)
	int delay;
#endif
{
	/* This is the speed used in the sleeping routine */
	if (delay >= 0)
		speed = (long) (delay * userDelay);
}

#if NeedFunctionPrototypes
static KeySym GetKeySym(XEvent event)
#else
static KeySym GetKeySym(event)
	XEvent event;
#endif
{
	int count;
	char key;
	KeySym keysym;
	XComposeStatus compose;

	/* Lookup a keysym using the event key */
	count = XLookupString(&event.xkey, &key, 1, &keysym, &compose);

	return keysym;
}

#if NeedFunctionPrototypes
int paddleIsMoving(void)
#else
int paddleIsMoving()
#endif
{
	/* Returns direction of paddle 1 right -1 left 0 stopped */
	return paddleMotion;
}

#if NeedFunctionPrototypes
void handlePaddleMoving(Display *display)
#else
void handlePaddleMoving(display)
	Display *display;
#endif
{
	static oldx = 0;
	int x, y;

	if (paddleControl == CONTROL_KEYS)
	{
		switch (paddleMotion)
		{
			case 1:		/* Move the paddle to the right 1 increment */
				MovePaddle(display, playWindow, 
					PADDLE_RIGHT, currentPaddleSize, 0);
				break;

			case -1:		/* Move the paddle to the left 1 increment */
				MovePaddle(display, playWindow, 
					PADDLE_LEFT, currentPaddleSize, 0);
				break;

			default:
				break;
		}
	} else if (paddleControl == CONTROL_MOUSE)
	{
		if (ObtainMousePosition(display, playWindow, &x, &y))
		{
			/* Has the pointer moved since our last poll */
			if (x != oldx)
			{
				paddleDx = x - oldx;
	
				/* Move the paddle to the position of the mouse pointer */
				MovePaddle(display, playWindow, 
					PADDLE_NONE, currentPaddleSize, x);
				oldx = x;
	
				/* Adjust the paddle motion variable so the ball moves when in
				 * the BALL_READY state and BALL_CREATE state.
				 */
				if (x > oldx)
					paddleMotion = 1;
				else
					paddleMotion = -1;
			}
			else
			{
				/* Reset to no motion */
				paddleMotion = 0;
				paddleDx = 0;
			}
		}
	}
}


#if NeedFunctionPrototypes
static void ToggleGamePaused(Display *display)
#else
static void ToggleGamePaused(display)
	Display *display;
#endif
{
	if (mode == MODE_PAUSE)
	{
		/* Finished pause resume game */
		mode = MODE_GAME;
		SetCurrentMessage(display, messWindow, "- Play ball -", False);
		
		/* How many seconds were we paused for? */
		pausedTime += (time(NULL) - pauseStartTime);

 		XSelectInput(display, mainWindow, 
			KeyPressMask | KeyReleaseMask | ButtonPressMask |
   			ButtonReleaseMask | ButtonMotionMask | ExposureMask | 
			StructureNotifyMask | PointerMotionHintMask);

 		GrabPointer(display, mainWindow);
	}
	else 
		SetGamePaused(display);
}

#if NeedFunctionPrototypes
static void SetGamePaused(Display *display)
#else
static void SetGamePaused(display)
	Display *display;
#endif
{
	if (mode == MODE_GAME)
	{
		/* Set game to paused mode */
		mode = MODE_PAUSE;
		SetCurrentMessage(display, messWindow, 
			"- Game paused -", False);
		
		/* we need to keep track of how long we were paused so that later
		 * in the highscore thing I can take that off the time.
		 */
		pauseStartTime = time(NULL);

		XSelectInput(display, mainWindow, 
			KeyPressMask | ExposureMask | StructureNotifyMask);

		UnGrabPointer(display);
	}
}

#if NeedFunctionPrototypes
void handleIconify(Display *display)
#else
void handleIconify(display)
	Display *display;
#endif
{
	ToggleGamePaused(display);
}

#if NeedFunctionPrototypes
void SelectiveRedraw(Display *display)
#else
void SelectiveRedraw(display)
	Display *display;
#endif
{
	switch (mode)
	{
		case MODE_GAME:
		case MODE_PAUSE:
			RedrawPlayWindow(display, playWindow);
			break;

		case MODE_EDIT:
			RedrawEditor(display, playWindow);
			break;

		case MODE_INTRO:
			RedrawIntroduction(display, playWindow);
			break;

		case MODE_DEMO:
			RedrawDemonstration(display, playWindow);
			break;

		case MODE_PREVIEW:
			RedrawPreviewLevel(display, playWindow);
			break;

		case MODE_INSTRUCT:
			RedrawInstructions(display, playWindow);
			break;

		case MODE_KEYS:
			RedrawKeys(display, playWindow);
			break;

		case MODE_KEYSEDIT:
			RedrawKeysEdit(display, playWindow);
			break;

		case MODE_BONUS:
			RedrawBonus(display, mainWindow);
			break;

		case MODE_HIGHSCORE:
			RedrawHighScore(display, playWindow);
			break;

		default:
			break;
	}

	/* Redisplay the message and the level/score info */
	RedrawLevelInfo(display, levelWindow);
	DisplayCurrentMessage(display, messWindow);

	/* To be sure - to be sure */
	XFlush(display);
}

#if NeedFunctionPrototypes
void handleExposure(Display *display, XEvent event)
#else
void handleExposure(display, event)
	Display *display;
	XEvent event;
#endif
{
	/* Only redraw window once so wait until all expose events have sent
	 * and then redraw all that we need to redraw based on current game
	 * mode.
	 */
	if (event.xexpose.count == 0)
		SelectiveRedraw(display);
}

#if NeedFunctionPrototypes
void handleMouseButtons(Display *display, XEvent event, int Down)
#else
void handleMouseButtons(display, event, Down)
	Display *display;
	XEvent event;
	int Down;
#endif
{
	if (mode == MODE_EDIT)
	{
		/* Allow the editor window to have control over buttons */
		HandleEditorMouseButtons(display, event, Down);
		return;
	}
	
	if (Down == True)
	{
		/* Button pressed down */
		switch(event.xbutton.button)
		{
			/* Shoot a bullet on all buttons */
			case Button1:
			case Button2: 
			case Button3:
				/* If we are playing the game and a ball needs to be started
				 * then start it otherwise shoot a bullet.
				 */
				if (mode == MODE_GAME)
					if (ActivateWaitingBall(display, playWindow) == False)
						shootBullet(display, playWindow);
				break;
		}
	}
}

#if NeedFunctionPrototypes
static void handleControlKeys(Display *display)
#else
static void handleControlKeys(display)
	Display *display;
#endif
{
	/* Toggle game mode */
	if (GetPaddleControlMode() == CONTROL_KEYS)
	{
		SetCurrentMessage(display, messWindow, 
			"Control: Mouse", True);
		SetPaddleControlMode(CONTROL_MOUSE);
	}
	else
	{
		SetCurrentMessage(display, messWindow, 
			"Control: Keys", True);
		SetPaddleControlMode(CONTROL_KEYS);
	}

	/* Play a bit of sound */
	if (noSound == False)
		playSoundFile("toggle", 50);
}

#if NeedFunctionPrototypes
static void handleSoundKey(Display *display)
#else
static void handleSoundKey(display)
	Display *display;
#endif
{
	if (noSound == False)
	{
		/* Try and turn audio off */
		FreeAudioSystem();

		noSound = True;
		SetCurrentMessage(display, messWindow, 
			"- Audio OFF -", True);
	}
	else
	{
		/* Try and turn audio on */
		if (SetUpAudioSystem(display) == False)
		{
			/* Unable to turn audio on */
			noSound = True;
			SetCurrentMessage(display, messWindow, 
				"- Audio unavailable -", True);
		}
		else
		{
			/* Audio is now active */
			noSound = False;
			SetCurrentMessage(display, messWindow, 
				"- Audio ON -", True);
		}
	}
}

#if NeedFunctionPrototypes
void SetTiltsZero(void)
#else
void SetTiltsZero()
#endif
{
	/* Initialise the user tilt variable to zero tilts */
	UserTilts = 0;
}

#if NeedFunctionPrototypes
static void handleGameKeys(Display *display, KeySym keysym)
#else
static void handleGameKeys(display, keysym)
	Display *display;
	KeySym keysym;
#endif
{
	int temp;
	char astr[80];

	/* Switch on the keysym */
	switch (keysym)
	{
		case XK_z: case XK_Z:
			if (saving == True)
				SaveCurrentGame(display, playWindow);
			else
				SetCurrentMessage(display, messWindow, "Not available yet!", 
					True);
			break;

		case XK_X: case XK_x:
			LoadSavedGame(display, playWindow);
			break;

		case XK_t:	case XK_T:	
			/* Obtain an active ball and tilt it */
			if ((temp = GetAnActiveBall()) >= 0)
			{
				/* Check that the user is tilt happy */
				if (UserTilts < MAX_TILTS)
				{
					/* Bump the ball please */
					DoBoardTilt(display, temp);
					UserTilts++;

					sprintf(astr, "You have %d %s left!", 
						MAX_TILTS - UserTilts, 
						(MAX_TILTS - UserTilts) == 1 ? "tilt" : "tilts");
					SetCurrentMessage(display, messWindow, 
						astr, True);
				}
				else
				{
					/* All tilts used up for this level. */
					SetCurrentMessage(display, messWindow, 
						"Maximum tilts reached!", True);
				}
			}
			break;

		case XK_d:	case XK_D:	
			/* Obtain an active ball - ie: not on paddle */
			if ((temp = GetAnActiveBall()) >= 0)
			{
				/* Erase and reset ball to new one */
				ClearBallNow(display, playWindow, temp);
			}
			break;

		case XK_Left: case XK_j: case XK_J:
			/* Set paddle to move left */
			paddleMotion = -1;
			break;

		case XK_k: case XK_K:
			/* Shoot a bullet if available */
			if (ActivateWaitingBall(display, playWindow) == False)
				shootBullet(display, playWindow);
			break;

		case XK_Right: case XK_l: case XK_L:
			/* Set paddle to move right */
			paddleMotion = 1;
			break;

		case XK_Escape:
			if (YesNoDialogue(display, "Abort current game? [y/n]"))
				handleQuitKeys(display);
			break;

      	case XK_equal:
			if (debug == True)
			{
				/* Special cheat key for debugging mode */
            	SkipToNextLevel(display, playWindow);
            	SetCurrentMessage(display, messWindow,
            		"Cheating, skip level ...", True);
			}
			else
			{
            	SetCurrentMessage(display, messWindow,
            		"Stop trying to cheat!!", True);
			}
            break;

		case XK_p: case XK_P:
			ToggleGamePaused(display);
			break;

		default: 	/* All other keys */
			handleMiscKeys(display, keysym);
	}
}

#if NeedFunctionPrototypes
static void handleIntroKeys(Display *display, KeySym keysym)
#else
static void handleIntroKeys(display, keysym)
	Display *display;
	KeySym keysym;
#endif
{
	/* Switch on the keysym */
	switch (keysym)
	{
		case XK_space:
			if (mode == MODE_INTRO || mode == MODE_HIGHSCORE 
			  || mode == MODE_INSTRUCT || mode == MODE_KEYS || 
			  mode == MODE_KEYSEDIT || mode == MODE_DEMO || 
			  mode == MODE_PREVIEW)
			{
				ResetBorderGlow(display, playWindow);
				SetGameSpeed(FAST_SPEED);
				gameActive = False;
				mode = MODE_GAME;
			}

			if (mode == MODE_BONUS)
				SetBonusWait(BONUS_FINISH, frame);
			break;

		case XK_c: case XK_C:
			/* Cycle through the introduction screens if note in a game */
			if (mode == MODE_INTRO)
			{
				/* Ok - Goto the instructions mode */
				SetGameSpeed(FAST_SPEED);
				ResetInstructions();
				mode = MODE_INSTRUCT;
			} else if (mode == MODE_INSTRUCT)
			{
				/* Ok - Goto the demo mode */
				SetGameSpeed(FAST_SPEED);
				ResetDemonstration();
				mode = MODE_DEMO;
			} else if (mode == MODE_DEMO)
			{
				/* Ok - Goto the keys mode */
				SetGameSpeed(FAST_SPEED);
				ResetKeys();
				mode = MODE_KEYS;
			} else if (mode == MODE_KEYS)
			{
				/* Ok - Goto the keysedit mode */
				SetGameSpeed(FAST_SPEED);
				ResetKeysEdit();
				mode = MODE_KEYSEDIT;
			} else if (mode == MODE_KEYSEDIT)
			{
				/* Ok - Goto the highscore mode */
				SetGameSpeed(FAST_SPEED);
				ResetHighScore(GLOBAL);
				mode = MODE_HIGHSCORE;
			} else if (mode == MODE_HIGHSCORE)
			{
				/* Ok - Goto to the preview mode */
				SetGameSpeed(FAST_SPEED);
				ResetPreviewLevel();
				mode = MODE_PREVIEW;
			} else if (mode == MODE_PREVIEW)
			{
				/* Ok - Goto back to the intro mode */
				SetGameSpeed(FAST_SPEED);
				ResetIntroduction();
				mode = MODE_INTRO;
			} 
			break;

		case XK_H:	/* Personal highscores */
			if (mode == MODE_INTRO || mode == MODE_INSTRUCT 
				|| mode == MODE_KEYS || mode == MODE_HIGHSCORE 
				|| mode == MODE_DEMO || mode == MODE_PREVIEW ||
				mode == MODE_KEYSEDIT)
			{
				/* Display the high scores thanks */
				SetGameSpeed(FAST_SPEED);
				ResetHighScore(PERSONAL);
				mode = MODE_HIGHSCORE;

				/* Play a bit of sound */
				if (noSound == False)
					playSoundFile("toggle", 50);
				
			}
			break;

		case XK_h:	/* Global highscores */
			if (mode == MODE_INTRO || mode == MODE_INSTRUCT 
				|| mode == MODE_KEYS || mode == MODE_HIGHSCORE
				|| mode == MODE_DEMO || mode == MODE_PREVIEW ||
				mode == MODE_KEYSEDIT)
			{
				SetGameSpeed(FAST_SPEED);
				ResetHighScore(GLOBAL);
				mode = MODE_HIGHSCORE;

				/* Play a bit of sound */
				if (noSound == False)
					playSoundFile("toggle", 50);
			}
			break;

		case XK_s: case XK_S:
			if (mode == MODE_INTRO || mode == MODE_INSTRUCT 
				|| mode == MODE_KEYS || mode == MODE_HIGHSCORE
				|| mode == MODE_DEMO || mode == MODE_PREVIEW ||
				mode == MODE_KEYSEDIT)
			{
				/* toggle the special effects system */
				if (getSpecialEffects(display) == True)
				{
					/* Turn off special effects */
					useSpecialEffects(False);

					SetCurrentMessage(display, messWindow, 
						"- SFX OFF -", True);
				}
				else
				{
					/* Cannot use sfx on this display */
					if (getSpecialEffects(display) == -1)
					{
						SetCurrentMessage(display, messWindow, 
							"- SFX Unavailable -", True);
					}
					else
					{
						/* Try and turn on special effects */
						useSpecialEffects(True);

						SetCurrentMessage(display, messWindow, 
							"- SFX ON -", True);
					}
				}
			}
			break;

		case XK_w: case XK_W:
			ChangeStartingLevel(display);
			break;

		case XK_e: case XK_E:
			/* Change to editor mode */
			ResetEditor();
			mode = MODE_EDIT;
			break;

		default: 	/* All other keys */
			handleMiscKeys(display, keysym);
			handleSpeedKeys(display, keysym);
			break;
	}
}

#if NeedFunctionPrototypes
static void handleQuitKeys(Display *display)
#else
static void handleQuitKeys(display)
	Display *display;
#endif
{
	/* Save out the scores if you were playing */
	if (oldMode == MODE_GAME || oldMode == MODE_BONUS)
	{
		/* Save out scores when quitting */
		UpdateHighScores(display);
	}

	/* Abort game and return to intros */
	SetGameSpeed(FAST_SPEED);
	ResetIntroduction();
	mode = MODE_INTRO;
}

#if NeedFunctionPrototypes
static void handleExitKeys(Display *display)
#else
static void handleExitKeys(display)
	Display *display;
#endif
{
	/* Save out the scores if you were playing */
	if (oldMode == MODE_GAME || oldMode == MODE_BONUS)
	{
		/* Save out scores when quitting */
		UpdateHighScores(display);
	}

	if (noSound == False) playSoundFile("game_over", 100);

	/* Shut down and exit game */
	ShutDown(display, 0, "Thank you for playing XBoing.");
}

#if NeedFunctionPrototypes
static void handlePresentsKeys(Display *display, KeySym keysym)
#else
static void handlePresentsKeys(display, keysym)
	Display *display;
	KeySym keysym;
#endif
{
	/* Switch on the keysym */
	switch (keysym)
	{
		case XK_space:
			QuickFinish(display, mainWindow);
			break;

		case XK_Q: case XK_q:
			/* Shut down and exit game */
			ShutDown(display, 0, "Thank you for playing XBoing.");
			break;

		default:
			break;
	}
}

#if NeedFunctionPrototypes
static void handleSpeedKeys(Display *display, KeySym keysym)
#else
static void handleSpeedKeys(display, keysym)
	Display *display;
	KeySym keysym;
#endif
{
	/* Switch on the keysym */
	switch (keysym)
	{
		case XK_1:	/* Set speed to speed 1 */
			SetUserSpeed(9);
			SetCurrentMessage(display, messWindow, "Warp 1 - Slow", True);
			if (noSound == False) playSoundFile("tone", 10);
			break;

		case XK_2:	/* Set speed to speed 2 */
			SetUserSpeed(8);
			SetCurrentMessage(display, messWindow, "Warp 2", True);
			if (noSound == False) playSoundFile("tone", 20);
			break;

		case XK_3:	/* Set speed to speed 3 */
			SetUserSpeed(7);
			SetCurrentMessage(display, messWindow, "Warp 3", True);
			if (noSound == False) playSoundFile("tone", 30);
			break;

		case XK_4:	/* Set speed to speed 4 */
			SetUserSpeed(6);
			SetCurrentMessage(display, messWindow, "Warp 4", True);
			if (noSound == False) playSoundFile("tone", 40);
			break;

		case XK_5:	/* Set speed to speed 5 */
			SetUserSpeed(5);
			SetCurrentMessage(display, messWindow, "Warp 5 - Medium", True);
			if (noSound == False) playSoundFile("tone", 50);
			break;

		case XK_6:	/* Set speed to speed 6 */
			SetUserSpeed(4);
			SetCurrentMessage(display, messWindow, "Warp 6", True);
			if (noSound == False) playSoundFile("tone", 60);
			break;

		case XK_7:	/* Set speed to speed 7 */
			SetUserSpeed(3);
			SetCurrentMessage(display, messWindow, "Warp 7", True);
			if (noSound == False) playSoundFile("tone", 70);
			break;

		case XK_8:	/* Set speed to speed 8 */
			SetUserSpeed(2);
			SetCurrentMessage(display, messWindow, "Warp 8", True);
			if (noSound == False) playSoundFile("tone", 80);
			break;

		case XK_9:	/* Set speed to speed 9 */
			SetUserSpeed(1);
			SetCurrentMessage(display, messWindow, "Warp 9 - Fast", True);
			if (noSound == False) playSoundFile("tone", 90);
			break;

		default: 	/* All other keys */
			break;
	}
}

#if NeedFunctionPrototypes
static void handleMiscKeys(Display *display, KeySym keysym)
#else
static void handleMiscKeys(display, keysym)
	Display *display;
	KeySym keysym;
#endif
{
	int vol = 0;
	char str[30];

	/* Switch on the keysym */
	switch (keysym)
	{
		case XK_plus: case XK_KP_Add:
			if (noSound == False)
			{
				vol = GetMaximumVolume();
				if (vol < 100)
					vol++;
				SetMaximumVolume(vol);
				sprintf(str, "Maximum volume: %d%%", vol);
				SetCurrentMessage(display, messWindow, str, True);
			}
			break;

		case XK_minus: case XK_KP_Subtract:
			if (noSound == False)
			{
				vol = GetMaximumVolume();
				if (vol > 0)
					vol--;
				SetMaximumVolume(vol);
				sprintf(str, "Maximum volume: %d%%", vol);
				SetCurrentMessage(display, messWindow, str, True);
			}
			break;

		case XK_a: case XK_A:
			handleSoundKey(display);
			break;

		case XK_i: case XK_I:
			/* Iconify the window quickly - main loop handles events */
			XIconifyWindow(display, mainWindow, 0);
			break;

		case XK_g: case XK_G:
			handleControlKeys(display);
			break;

		case XK_Q: case XK_q:
			if (YesNoDialogue(display, "Exit XBoing you wimp? [y/n]"))
				handleExitKeys(display);
			break;

		default: 	/* All other keys */
			break;
	}
}

#if NeedFunctionPrototypes
void handleKeyPress(Display *display, KeySym keysym, XEvent event, int Pressed)
#else
void handleKeyPress(display, keysym, event, Pressed)
	Display *display;
	KeySym keysym;
	XEvent event;
	int Pressed;
#endif
{

	if (Pressed == False)
	{
		/* key was released */
		paddleMotion = 0;
	}
	else
	{
		/* Switch on the game mode */
		switch (mode)
		{
			case MODE_DIALOGUE:
				break;

			case MODE_WAIT: case MODE_BALL_WAIT:
			case MODE_PAUSE: case MODE_GAME:
				handleGameKeys(display, keysym);
				break;

			case MODE_HIGHSCORE: case MODE_BONUS: case MODE_INTRO:
			case MODE_INSTRUCT: case MODE_DEMO: case MODE_PREVIEW:
			case MODE_KEYS: case MODE_KEYSEDIT:
				handleIntroKeys(display, keysym);
				break;

			case MODE_PRESENTS:
				handlePresentsKeys(display, keysym);
				break;

			case MODE_EDIT:
				handleEditorKeys(display, keysym);
				break;

			case MODE_NONE:
				break;
		}
	}
}


#if NeedFunctionPrototypes
static void handleGameMode(Display *display)
#else
static void handleGameMode(display)
	Display *display;
#endif
{
	static int bonusRow = 0;
	static int bonusCol = 0;
	static int nextBonusFrame = 0;

	/* If we are going to play then setup first level */
	if (gameActive == False)
	{
		/* Choose a random velocity for the ball */

		/* Always start at level 1 or level specified */
		SetLevelNumber(GetStartingLevel());

		/* Set some important variables */
		/* PLEASE CHANGE SETUP IN EDITOR.C IF YOU CHANGE STUFF HERE */
		SetLivesLeft(3);
		ToggleSaving(display, False);
		SetTheScore(0L);
		nextBonusFrame 		= 0;
		currentPaddleSize 	= PADDLE_HUGE;
		pausedTime			= 0;
		bonusBlock 			= False;
		UserTilts			= 0;

		/* Setup the stage and load 1st level */
		SetupStage(display, playWindow);

		/* Start game play */
		gameActive = True;

		/* Keep track of the game duration - shown in highscores */
		gameTime = time(NULL);
	}

	/* If we need to move the paddle then do so */
	if ((frame % PADDLE_ANIMATE_DELAY) == 0)
		handlePaddleMoving(display);

	if (mode == MODE_GAME)
	{
		HandleBallMode(display, playWindow);

		/* Add bonus coin block at random intervals */
		if (nextBonusFrame == 0 && bonusBlock == False)
			nextBonusFrame = frame + (rand() % BONUS_SEED);

		/* Do we need to add a bonus coin or special? */
		if (nextBonusFrame <= frame && bonusBlock == False)
		{
			/* Add the bonus block now - different types */
			switch (rand() % 27)
			{
				case 0: case 1: 
				case 2: case 3: 
				case 4: case 5: 
				case 6: case 7:
					/* Add a normal bonus block */
					AddBonusBlock(display, playWindow, &bonusRow, &bonusCol, 
						BONUS_BLK);
					DEBUG("Attempting Adding a bonus block.")
					break;

				case 8: case 9:
				case 10: case 11:
					/* Add the x2 bonus block */
					if (x2Bonus == False)
					{
						AddBonusBlock(display, playWindow, &bonusRow, 
							&bonusCol, BONUSX2_BLK);
						DEBUG("Attempting Adding a bonus x2 block.")
					}
					break;

				case 12: case 13:
					/* Add the x4 bonus block */
					if (x4Bonus == False)
					{
						AddBonusBlock(display, playWindow, &bonusRow, 
							&bonusCol, BONUSX4_BLK);
						DEBUG("Attempting Adding a bonus x4 block.")
					}
					break;

				case 14: case 15: 
					/* Add the shrink paddle special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						PAD_SHRINK_BLK, SHOTS_TO_KILL_SPECIAL);
					DEBUG("Attempting Adding a special paddle shrink block.")
					break;

				case 16: case 17: 
					/* Add the expand paddle  special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						PAD_EXPAND_BLK, SHOTS_TO_KILL_SPECIAL);
					DEBUG("Attempting Adding a special paddle expand block.")
					break;

				case 18: 
					/* Add the multiball special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						MULTIBALL_BLK, SHOTS_TO_KILL_SPECIAL);
					DEBUG("Attempting Adding a special multiball block.")
					break;

				case 19: 
					/* Add the reverse special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						REVERSE_BLK, SHOTS_TO_KILL_SPECIAL);
					DEBUG("Attempting Adding a special reverse control block.")
					break;

				case 20: case 21: 
					/* Add the machine gun special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						MGUN_BLK, SHOTS_TO_KILL_SPECIAL);
					DEBUG("Attempting Adding a special machine gun block.")
					break;

				case 22:
					/* Add the walloff special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						WALLOFF_BLK, SHOTS_TO_KILL_SPECIAL);
					DEBUG("Attempting Adding a special walls off block.")
					break;

				case 23:
					/* Add the extraball special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						EXTRABALL_BLK, 0);
					DEBUG("Attempting Adding a special extra ball block.")
					break;

				case 24:
					/* Add the death special block */
					AddSpecialBlock(display, playWindow, &bonusRow, &bonusCol, 
						DEATH_BLK, SHOTS_TO_KILL_SPECIAL);
					DEBUG("Attempting Adding a special death block.")
					break;

				case 25:	/* choose a type to dynamite */
					switch (rand() % 7)
					{
						case 0:	/* Dynamite the block */
							SetExplodeAllType(display, playWindow, YELLOW_BLK);
							DEBUG("Attempting Adding a special dynamite block.")
							break;

						case 1:	/* Dynamite the block */
							SetExplodeAllType(display, playWindow, BLUE_BLK);
							DEBUG("Attempting Adding a special dynamite block.")
							break;

						case 2:	/* Dynamite the block */
							SetExplodeAllType(display, playWindow, RED_BLK);
							DEBUG("Attempting Adding a special dynamite block.")
							break;

						case 3:	/* Dynamite the block */
							SetExplodeAllType(display, playWindow, PURPLE_BLK);
							DEBUG("Attempting Adding a special dynamite block.")
							break;

						case 4:	/* Dynamite the block */
							SetExplodeAllType(display, playWindow, TAN_BLK);
							DEBUG("Attempting Adding a special dynamite block.")
							break;

						case 5:	/* Dynamite the block */
							SetExplodeAllType(display, playWindow, COUNTER_BLK);
							DEBUG("Attempting Adding a special dynamite block.")
							break;

						case 6:	/* Dynamite the block */
							SetExplodeAllType(display, playWindow, GREEN_BLK);
							DEBUG("Attempting Adding a special dynamite block.")
							break;
					}
					break;

				case 26:	/* Try and start the eyedude on his way */
					if (getEyeDudeMode() == EYEDUDE_NONE)
						ChangeEyeDudeMode(EYEDUDE_RESET);
					DEBUG("Attempting to start the eyedude.")
					break;

				default:
					break;
			}

			nextBonusFrame = 0;
		}
	}

	HandleBulletMode(display, playWindow);

	/* If any blocks need exploding then do so */
	ExplodeBlocksPending(display, playWindow);

	/* So blocks need animation all the time so do it */
	HandlePendingAnimations(display, playWindow);

	HandleEyeDudeMode(display, playWindow);

	/* See if the level is finished and update level info if needed */
	if (mode == MODE_GAME)
		CheckGameRules(display, playWindow);
}

#if NeedFunctionPrototypes
static void handleGameStates(Display *display)
#else
static void handleGameStates(display)
	Display *display;
#endif
{
	/* Update the message window if any new messages come along */
	DisplayCurrentMessage(display, messWindow);

	/* In game effects */
	switch (currentSfxMode())
	{
		case SFX_STATIC:
			/* Show some static */
			WindowStaticEffect(display, playWindow, PLAY_WIDTH, PLAY_HEIGHT);
			break;

		case SFX_SHAKE:
			/* Something exploded or bumped the screen */
			WindowShakeEffect(display, playWindow);
			break;

		case SFX_FADE:
			/* fade when play arena is taken off screen */
			WindowFadeEffect(display, playWindow, PLAY_WIDTH, PLAY_HEIGHT);
			break;
			
		case SFX_BLIND:
			/* bring the backing buffer pixmap into view via a blind effect */
			WindowBlindEffect(display, playWindow);
			break;
			
		case SFX_SHATTER:
			/* bring the backing buffer pixmap into view via a blind effect */
			WindowShatterEffect(display, playWindow);
			break;
			
		case SFX_NONE:
		default:
			break;
	}

	/* Switch on the current game mode */
	switch (mode)
	{
		case MODE_GAME:
			handleGameMode(display);
			break;

		case MODE_PRESENTS:
			Presents(display, mainWindow);
			break;

		case MODE_BONUS:
			DoBonus(display, mainWindow);
			break;

		case MODE_DIALOGUE:
			break;

		case MODE_INTRO:
			Introduction(display, playWindow);
			break;

		case MODE_INSTRUCT:
			Instructions(display, playWindow);
			break;

		case MODE_KEYS:
			Keys(display, playWindow);
			break;

		case MODE_KEYSEDIT:
			KeysEdit(display, playWindow);
			break;

		case MODE_DEMO:
			Demonstration(display, playWindow);
			break;

		case MODE_PREVIEW:
			PreviewLevel(display, playWindow);
			break;

		case MODE_HIGHSCORE:
			HighScore(display, playWindow);
			break;

		case MODE_EDIT:
			Editor(display, playWindow);
			break;

		case MODE_PAUSE:  
			break;
	}

	/* Flush the display */
	XFlush(display);
}

#if NeedFunctionPrototypes
static void handleEventLoop(Display *display)
#else
static void handleEventLoop(display)
	Display *display;
#endif
{
	XEvent event;
	int pending;
	KeySym keysym;

	pending = frame = 0;

	/* Initial mode for game is Introduction */
	mode = MODE_PRESENTS;

	/* No special effects yet */
	changeSfxMode(SFX_NONE);

	/* Flush all events until app is fully mapped */
    do
	{
		/* handle audio device events if they exist */
		audioDeviceEvents();

		/* Get the next event */
		XNextEvent(display, &event);
	}
	while (event.type != MapNotify);

	ChangePointer(display, mainWindow, CURSOR_NONE);

 	/* Grab the pointer to the main window */
	GrabPointer(display, mainWindow);

	/* Loop forever and ever */
	while (True)
	{
		/* handle and audio device events if supported */
		audioDeviceEvents();

		/* See if any events are waiting for me to handle */
		if (iconified == False && mode != MODE_PAUSE)
		{
			/* Get an event but don't wait if none arrives */
			pending = XPending(display);

			if (mode != MODE_DIALOGUE)
				frame++;
		}
		else
		{
			/* Wait here for an event and then get the number waiting */
			XPeekEvent(display, &event);
			pending = XPending(display);
		}
		
		/* Handle any events pending */
		while (pending > 0)
		{
			/* Get the next X event thanks */
			XNextEvent(display, &event);

			switch(event.type)
			{
				case UnmapNotify:
					/* Turn off just all events except the mapping ones */
    				XSelectInput(display, mainWindow, StructureNotifyMask);
					handleIconify(display);
					iconified = True;
					break;

				case MapNotify:
					/* Turn back on all the events that are needed */
    				XSelectInput(display, mainWindow, 
						KeyPressMask | KeyReleaseMask | ButtonPressMask |
       					ButtonReleaseMask | ButtonMotionMask | ExposureMask | 
						StructureNotifyMask | PointerMotionHintMask);

					/*SelectiveRedraw(display);*/
 					GrabPointer(display, mainWindow);
					iconified = False;
					break;

				case ButtonRelease:
					handleMouseButtons(display, event, False);
					break;

				case ButtonPress:
					handleMouseButtons(display, event, True);
					break;

				case MotionNotify:
					if (mode == MODE_EDIT)
						HandleButtonMotion(display, event);
					break;

				case KeyRelease:
					keysym = GetKeySym(event);
					handleKeyPress(display, keysym, event, False);
					break;

				case KeyPress:
					keysym = GetKeySym(event);
					handleKeyPress(display, keysym, event, True);
					break;

				case Expose:
					handleExposure(display, event);
					break;

				default:
					break;
			}

			/* Decrement the number of pending events */
			pending--;
		}
		
		/* Sleep a bit if not iconified */
		if (iconified == False)
			sleepSync(display, speed);

		/* handle all game states and animations */
		if (iconified == False) 
			handleGameStates(display);
	}

	/* NOT REACHED */
}

#if NeedFunctionPrototypes
int main(int argc, char **argv)
#else
int main(argc, argv)
	int argc;
	char **argv;
#endif
{
	static Display *display;

	/* Initialise everything and return display */
	display = InitialiseGame(argv, argc);

	SetGameSpeed(FAST_SPEED);
	gameActive = False;
	iconified = False;

	XSelectInput(display, mainWindow, 
		KeyPressMask | KeyReleaseMask | ButtonPressMask |
   		ButtonReleaseMask | ExposureMask | StructureNotifyMask );

	/* main event loop */
	handleEventLoop(display);

	/* NOTREACHED */
}
