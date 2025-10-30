/**
 * @file editor.c
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Controls functions for editor and deals with clicks and button presses
 * @version 1.1.1.1
 * @date 16 Dec 1994
 * 
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved 
 * @copyright (see COPYRIGHT file for full text)
 * 
 * Editor.c/h handles the editor and manages functions for saving and loading levels, building the editor, and managing the 
 * clicks and button presses
 *  
 */

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
 * $Id: editor.c,v 1.1.1.1 1994/12/16 01:36:44 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/editor.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:44 $
 *
 * $Log: editor.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:44  jck
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
// TODO: REMOVE X11 #include <X11/Xlib.h>
// TODO: REMOVE X11 #include <X11/Xutil.h>
// TODO: REMOVE X11 #include <X11/Xos.h>
// TODO: REMOVE X11 #include <X11/keysym.h>
//#include <xpm.h>

#include "include/error.h"
#include "include/misc.h"
#include "include/gun.h"
#include "include/main.h"
#include "include/eyedude.h"
#include "include/bonus.h"
#include "include/init.h"
#include "include/dialogue.h"
#include "include/file.h"
#include "include/inst.h"
#include "include/stage.h"
#include "include/blocks.h"
#include "include/sfx.h"
#include "include/special.h"
#include "include/ball.h"
#include "include/score.h"
#include "include/paddle.h"
#include "include/level.h"
#include "include/mess.h"
#include "include/audio.h"
#include "include/intro.h"
#include "include/keys.h"
#include "include/version.h"

#include "include/editor.h"

/*
 *  Internal macro definitions:
 */

#define GAP 10

#define X2COL(col, x) (col = x / colWidth)
#define Y2ROW(row, y) (row = y / rowHeight)

#define ED_NOP			0
#define ED_DRAW			1
#define ED_ERASE		2

#define MAX_ROW_EDIT	(MAX_ROW - 3)
#define MAX_COL_EDIT	(MAX_COL)

/*
 *  Internal type declarations:
 */

void SetEditWait(enum EditStates newMode, int waitFrame);
void DoEditWait(void);
static void HandleRandomBlocks(Display *display);
void SetCurrentSymbol(Display *display, int i);

/*
 *  Internal variable declarations:
 */

enum EditStates EditState;
enum EditStates oldEditState;
static int waitingFrame;
enum EditStates waitMode;
static int oldWidth, oldHeight;
static int curBlockType;
static int drawAction = ED_NOP;
static int oldCol = -1;
static int oldRow = -1;
static char tempName[1024];
static int modified = False;
extern int paddleMotion;

struct editData
{
	Region      clickArea;
	int			symbolType;
	int			slideIndex;
} EditorInfo[50];

/**
 * @brief Calls ResetEditor in order to set up the editor for XBoing
 *
 * @param Display display X11 display
 * @param Window window window X11 ID for window
 * @param Colormap colormap Colormap for X11 system
 *
 * @todo remove reliance on X11
 *
 */
void SetUpEditor(display, window, colormap)
	Display *display;
	Window window;
	Colormap colormap;
{
	ResetEditor();
}

static void DrawEditorGrid(Display *display, Window window)
/**
 * @brief Draws the grid for the editor
 *
 * @param Display display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
static void DrawEditorGrid(display, window)
	Display *display;
	Window window;
{
	/* Draw a grid on editor window */
	int x, y, xinc, yinc;

	xinc = PLAY_WIDTH / MAX_COL;
	yinc = PLAY_HEIGHT / MAX_ROW;

	for (x = xinc; x <= PLAY_WIDTH; x += xinc)
		DrawLine(display, window, x, 0, x, 
			(PLAY_HEIGHT - 4 - ((MAX_ROW - MAX_ROW_EDIT) * yinc)), reds[4], 1);

	for (y = yinc; y <= PLAY_HEIGHT - ((MAX_ROW - MAX_ROW_EDIT) * yinc); 
		y += yinc)
		DrawLine(display, window, 0, y, PLAY_WIDTH, y, reds[4], 1);
}

/**
 * @brief Loads a level by getting the window size, getting the input, and constructing the file
 *
 * @param Display display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
static void DoLoadLevel(display, window)
	Display *display;
	Window window;
{
    char levelPath[1024];
    char *str;

	/* Resize the main window to hold the editor window of blocks */
	ObtainWindowWidthHeight(display, mainWindow, &oldWidth, &oldHeight);
	if (!ResizeMainWindow(display, mainWindow, oldWidth + EDITOR_TOOL_WIDTH, 
	  	oldHeight))
		ErrorMessage("Cannot resize main window");
	SetWindowSizeHints(display, oldWidth + EDITOR_TOOL_WIDTH, oldHeight);

	XMapWindow(display, blockWindow);
	XMapWindow(display, typeWindow);

	XFlush(display);

    XSelectInput(display, playWindow,
    	KeyPressMask | KeyReleaseMask | ButtonPressMask |
        ButtonReleaseMask | ButtonMotionMask | ExposureMask |
        StructureNotifyMask | PointerMotionHintMask);

    DrawStageBackground(display, window, 3, True);
    DrawStageBackground(display, blockWindow, 3, True);
    DrawStageBackground(display, typeWindow, 3, True);
	ResetBorderGlow(display, playWindow);

	DrawEditorGrid(display, window);

	ChangePointer(display, playWindow, CURSOR_PLUS);
	ChangePointer(display, mainWindow, CURSOR_POINT);

    /* Construct the Edit level filename */
    if ((str = getenv("XBOING_LEVELS_DIR")) != NULL)
        sprintf(levelPath, "%s/editor.data", str);
    else
        sprintf(levelPath, "%s/editor.data", LEVEL_INSTALL_DIR);

	/* Read in a Edit level */
    if (ReadNextLevel(display, window, levelPath, True) == False)
        ShutDown(display, 1, "Sorry, invalid level specified.");

	/* Be very friendly */
	SetCurrentMessage(display, messWindow, "<< Level Editor >>", False);

	DisplayLevelInfo(display, levelWindow, (u_long) 1);

	TurnSpecialsOff(display);
	DrawSpecials(display);

	modified = False;
}

/**
 * @brief Redraws the entire editor window
 *
 * @param Display display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
static void RedrawEditorArea(display, window)
	Display *display;
	Window window;
{
    DrawStageBackground(display, window, 3, True);

	if (EditState != EDIT_TEST)
		DrawEditorGrid(display, window);

	RedrawAllBlocks(display, window);

	SetCurrentSymbol(display, curBlockType);
}


static void ClearEditorInfo(void)
/**
 * @brief Destroys editor page
 *
 */
static void ClearEditorInfo()
{
	int i;

	/* Clear all the editor info blocks */
	for (i = 0; i < 50; i++)
	{
    	/* Destroy the region of the block */
    	if (EditorInfo[i].clickArea != (Region) NULL)
    	{
        	XDestroyRegion(EditorInfo[i].clickArea);
        	EditorInfo[i].clickArea = (Region) NULL;
    	}

    	EditorInfo[i].symbolType = -1;
    	EditorInfo[i].slideIndex = 0;
	}
}

/**
 * @brief Checks which box is hit
 *
 * @param int x X position of block
 * @param int y y position of block
 *
 */
static int CheckBlockClicked(int x, int y)
{
	int i;

	/* Scan through the list and find if coords fall in any block region */
	for (i = 0; i < 50; i++)
	{
    	/* Destroy the region of the block */
    	if (EditorInfo[i].clickArea != (Region) NULL)
    	{
		    /* See if the point intersects with the block's region */
            if (XPointInRegion(EditorInfo[i].clickArea, x, y))
				return i;
    	}
	}

	return -1;
}


/**
 * @brief Sets the current symbol in the game
 *
 * @param Display display X11 display
 * @param int i symbol id
 *
 * @todo Remove reliance on X11
 *
 */
void SetCurrentSymbol(Display *display, int i)
{
	int y;

	XClearWindow(display, typeWindow);

	y = ((TYPE_HEIGHT / 2) - ((dataFont->ascent + dataFont->descent) / 2));
	DrawShadowText(display, typeWindow, dataFont, 
		"Active:", 10, y, green);

	y = ((TYPE_HEIGHT / 2) - (BlockInfo[i].height / 2));
	DrawTheBlock(display, typeWindow, 65, y, 
		EditorInfo[i].symbolType, EditorInfo[i].slideIndex, 0, 0);
}

/**
 * @brief Checks mouse position and if a button is pressed
 *
 * @param Display display X11 display
 * @param XEvent event Event to carry out
 * @param int Down What is pressed
 *
 * @todo remove reliance on X11
 *
 */
void HandleEditorToolBar(Display *display, XEvent event, int Down)
{
    int x, y, i;

	/* Obtain the mouse position in block window */
	if (ObtainMousePosition(display, blockWindow, &x, &y))
	{
		if (x < 0 || x > EDITOR_TOOL_WIDTH) return;
		if (y < 0 || y > PLAY_HEIGHT) return;
	}
	else return;

    if (Down == True)
    {
        /* Button pressed DOWN */
        switch(event.xbutton.button)
        {
            case Button1:	
            case Button2:
            case Button3:
				if ((i = CheckBlockClicked(x, y)) >= 0) 
				{
					curBlockType = i;
					PlaySoundForBlock(EditorInfo[i].symbolType);
					SetCurrentSymbol(display, i);
				}
                break;
        }
	}
}

/**
 * @brief Creates array for blocks as well as the click region
 *
 * @param int x x point
 * @param int y y point
 * @param int w w point
 * @param int h h point
 * @param int type type of symbol
 * @param int i id for block
 * @param int slide id for slide index
 *
 */
static void SetupBlockEditorInfo(int x, int y, int w, int h, int type, int i, 
	int slide)
{
	XPoint points[5];

    /* Create the XPoint array for the top region */
    points[0].x = x;
    points[0].y = y;
    points[1].x = x + w;
    points[1].y = y;
    points[2].x = x + w;
    points[2].y = y + h;
    points[3].x = x;
    points[3].y = y + h;
    points[4].x = points[0].x;
    points[4].y = points[0].y;

    /* Create the click region for the edit block */
    EditorInfo[i].clickArea = XPolygonRegion(points, 5, EvenOddRule);
    EditorInfo[i].symbolType = type;
    EditorInfo[i].slideIndex = slide;
}

/**
 * @brief Draws the blocks in the window
 *
 * @param Display display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
static void SetupBlockWindow(Display *display, Window window)
{
	int x, y, x1, y1, i, j, k;
	int rowHeight = (PLAY_HEIGHT / MAX_ROW);

	/* Draw the first column of blocks */
	for (i = 0, y1 = 0; i < (MAX_ROW) && i < MAX_STATIC_BLOCKS; i++)
	{
		/* Calculate the centered position for the block in this column */
		x = (EDITOR_TOOL_WIDTH / 4) - (BlockInfo[i].width / 2);
		y = y1 + ((rowHeight / 2) - (BlockInfo[i].height / 2));
		DrawTheBlock(display, window, x, y, BlockInfo[i].blockType, 0, 0, 0);
		SetupBlockEditorInfo(x, y, BlockInfo[i].width, BlockInfo[i].height, 
			i, i, 0);
		y1 += rowHeight;
	}

	x1 = EDITOR_TOOL_WIDTH / 2;

	/* Draw the 2nd column of blocks */
	for (j = i, k = 0, y1 = 0; k < (MAX_ROW) && j < MAX_STATIC_BLOCKS; 
		k++, j++)
	{
		/* Calculate the centered position for the block in this column */
		x = x1 + (EDITOR_TOOL_WIDTH / 4) - (BlockInfo[j].width / 2);
		y = y1 + ((rowHeight / 2) - (BlockInfo[j].height / 2));
		DrawTheBlock(display, window, x, y, BlockInfo[j].blockType, 0, 0, 0);
		SetupBlockEditorInfo(x, y, BlockInfo[j].width, BlockInfo[j].height, 
			j, j, 0);
		y1 += rowHeight;
	}

	/* Add the counter blocks 1 - 5 */
	for (k = j, j = 1; j <= 5; k++, j++)
	{
		/* Calculate the centered position for the block in this column */
		x = x1 + (EDITOR_TOOL_WIDTH / 4) - (BlockInfo[COUNTER_BLK].width / 2);
		y = y1 + ((rowHeight / 2) - (BlockInfo[COUNTER_BLK].height / 2));
		DrawTheBlock(display, window, x, y, COUNTER_BLK, j, 0, 0);
		SetupBlockEditorInfo(x, y, BlockInfo[COUNTER_BLK].width, 
			BlockInfo[COUNTER_BLK].height, COUNTER_BLK, k, j);
		y1 += rowHeight;
	}
}

/**
 * @brief Carries out actions to close editor window and return to normal XBoing mode
 *
 * @param Display display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
static void DoFinish(Display *display, Window window)
{
	/* Close down the editor and return to normal state */

	ChangePointer(display, playWindow, CURSOR_NONE);
	ChangePointer(display, mainWindow, CURSOR_NONE);
	XUnmapWindow(display, blockWindow);
	XUnmapWindow(display, typeWindow);
	XFlush(display);

	if (!ResizeMainWindow(display, mainWindow, oldWidth, oldHeight))
		ErrorMessage("Cannot resize main window");
	SetWindowSizeHints(display, oldWidth, oldHeight);
	XFlush(display);

    mode = MODE_INTRO;
    ResetIntroduction();

    XSelectInput(display, playWindow,
    	KeyPressMask | KeyReleaseMask | ButtonPressMask |
        ButtonReleaseMask | ExposureMask | StructureNotifyMask);

    if (noSound == False) playSoundFile("evillaugh", 50);

    SetGameSpeed(FAST_SPEED);
}

/**
 * @brief Deals with all mouse and button events
 *
 * @param Display display X11 display ID
 * @param XEvent events The button or motion event
 *
 * @todo Remove Reliance on X11
 * @todo Possibly split this function into smaller functions
 *
 */
void HandleButtonMotion(Display *display, XEvent event)
{
	/* Handle all button down and motion events */
    int x, y, col, row;

	/* Obtain the mouse position in play window */
	if (ObtainMousePosition(display, playWindow, &x, &y))
	{
		if (x < 0 || x > PLAY_WIDTH) return;
		if (y < 0 || y > PLAY_HEIGHT) return;

		/* Find out what row and column it is in ie: which cell */
		X2COL(col, x);
		Y2ROW(row, y);
	}
	else 
	{
		ChangePointer(display, playWindow, CURSOR_PLUS);
		drawAction = ED_NOP;
		return;
	}

	if (row < 0 || row >= MAX_ROW_EDIT) return;
	if (col < 0 || col >= MAX_COL_EDIT) return;

    switch (drawAction)
    {
    	case ED_DRAW:	
			if (oldCol != col || oldRow != row)
			{
				EraseVisibleBlock(display, playWindow, row, col);
				AddNewBlock(display, playWindow, row, col, 
					EditorInfo[curBlockType].symbolType, 
					EditorInfo[curBlockType].slideIndex, True);
				oldCol = col; oldRow = row;
				modified = True;

    			if (noSound == False) playSoundFile("bonus", 20);
			}
            break;

        case ED_ERASE:
			if (oldCol != col || oldRow != row)
			{
				EraseVisibleBlock(display, playWindow, row, col);
				ClearBlock(row, col);
				oldCol = col; oldRow = row;
				modified = True;

    			if (noSound == False) playSoundFile("bonus", 20);
			}
            break;

        case ED_NOP:
            break;
	}
}

/**
 * @brief Carries out actions based on mouse and button presses
 *
 * @param Display Display X11 Display
 * @param XEvent event the button event
 * @param int Down tells if the button is pressed or not
 *
 * @todo remove reliance on X11
 *
 */
void HandleEditorMouseButtons(Display *display, XEvent event, int Down)
{
    int x, y, col, row;
	struct aBlock *blockP;

	/* Handle the play test mode */
	if (EditState == EDIT_TEST)
	{
    	if (Down == True)
    	{
        	/* Button pressed DOWN */
        	switch(event.xbutton.button)
        	{
            	case Button1:	
            	case Button2:	
            	case Button3:	
                    if (ActivateWaitingBall(display, playWindow) == False)
                        shootBullet(display, playWindow);
			}
		}

		return;
	}

	if (event.xbutton.subwindow == blockWindow)
		HandleEditorToolBar(display, event, Down);

	/* Obtain the mouse position in play window */
	if (ObtainMousePosition(display, playWindow, &x, &y))
	{
		if (x < 0 || x > PLAY_WIDTH) return;
		if (y < 0 || y > PLAY_HEIGHT) return;

		/* Find out what row and column it is in ie: which cell */
		X2COL(col, x);
		Y2ROW(row, y);
	}
	else return;

	if (row < 0 || row >= MAX_ROW_EDIT) return;
	if (col < 0 || col >= MAX_COL_EDIT) return;

    if (Down == True)
    {
        /* Button pressed DOWN */
        switch(event.xbutton.button)
        {
            case Button1:	
				EraseVisibleBlock(display, playWindow, row, col);
				AddNewBlock(display, playWindow, row, col, 
					EditorInfo[curBlockType].symbolType, 
					EditorInfo[curBlockType].slideIndex, True);
				drawAction = ED_DRAW;
				oldCol = col; oldRow = row;
				modified = True;

    			if (noSound == False) playSoundFile("bonus", 20);
                break;

            case Button2:
				ChangePointer(display, playWindow, CURSOR_SKULL);
				EraseVisibleBlock(display, playWindow, row, col);
				ClearBlock(row, col);
				drawAction = ED_ERASE;
				oldCol = col; oldRow = row;
				modified = True;

    			if (noSound == False) playSoundFile("bonus", 20);
                break;

            case Button3:
				/* Pointer to the correct block we need - speed things up */
				blockP = &blocks[row][col];

				if (blockP->occupied)
					DisplayScore(display, scoreWindow, blockP->hitPoints);
				else
					DisplayScore(display, scoreWindow, 0L);

				drawAction = ED_NOP;
                break;
        }
	}
	else
	{
		/* Button pressed UP */
		switch(event.xbutton.button)
		{
			case Button1:	
				drawAction = ED_NOP;
				break;

			case Button2:
				ChangePointer(display, playWindow, CURSOR_PLUS);
				drawAction = ED_NOP;
				break;

			case Button3:
				drawAction = ED_NOP;
				break;
		}
	}
}


/**
 * @brief Sets Up playtest for xboing
 *
 * @param Display display X11 display
 *
 * @todo remove reliance on X11
 *
 */
static void SetupPlayTest(Display *display)
{
	EditState = EDIT_TEST;

	strcpy(tempName, tempnam("/tmp", "xboing-"));
  	if (SaveLevelDataFile(display, tempName) == False)
   		ShutDown(display, 1, "Sorry, cannot save test play level.");

	/* Read in the save play test level */
    if (ReadNextLevel(display, playWindow, tempName, False) 
		== False)
        ShutDown(display, 1, "Sorry, cannot load play test level.");

	RedrawEditorArea(display, playWindow);

    SetLivesLeft(3);
    ToggleSaving(display, False);
    SetTheScore(0L);

	SetTiltsZero();
    ClearAllBalls();
    currentPaddleSize   = PADDLE_HUGE;
    ResetPaddleStart(display, playWindow);
    ResetBallStart(display, playWindow);

    SetUnlimitedBullets(False);
    ClearBullets();
    SetNumberBullets(NUMBER_OF_BULLETS_NEW_LEVEL);
    ResetNumberBonus();

    TurnSpecialsOff(display);
    SetReverseOff();
    DisplayScore(display, scoreWindow, score);
    DisplayLevelInfo(display, levelWindow, level);
    DrawSpecials(display);
}

/**
 * @brief Ends playtest by saving the playtest and changing the blocks
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11 system
 *
 */
static void FinishPlayTest(Display *display)
{
	EditState = EDIT_NONE;

    TurnSpecialsOff(display);
    SetReverseOff();
    SetTheScore(0L);
    DisplayScore(display, scoreWindow, 0L);
    DisplayLevelInfo(display, levelWindow, level);
    DrawSpecials(display);

	/* Read in the save play test level */
    if (ReadNextLevel(display, playWindow, tempName, False) 
		== False)
        ShutDown(display, 1, "Sorry, cannot load play test level.");

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

	RedrawEditorArea(display, playWindow);

	/* Remove the tempory file */
	unlink(tempName);
}

/**
 * @brief Goes through the board and recreates it horizontally
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11 system
 *
 */
static void FlipBoardHorizontal(Display *display)
{
    register int r, c;
	int temp[MAX_ROW_EDIT][2];

    if (noSound == False) playSoundFile("wzzz", 50);

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

    /* Scan through all block entries and erase their contents */
    for (c = 0; c < (MAX_COL_EDIT / 2); c++)
    {
		for (r = 0; r < MAX_ROW_EDIT; r++)
		{
		  	temp[r][0] = blocks[r][MAX_COL_EDIT - c - 1].blockType;
		  	temp[r][1] = blocks[r][MAX_COL_EDIT - c - 1].counterSlide;
		}

		for (r = 0; r < MAX_ROW_EDIT; r++)
            AddNewBlock(display, playWindow, r, MAX_COL_EDIT - c - 1,
            	blocks[r][c].blockType, blocks[r][c].counterSlide, False);

		for (r = 0; r < MAX_ROW_EDIT; r++)
            AddNewBlock(display, playWindow, r, c,
            	temp[r][0], temp[r][1], False);
    }

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

	modified = True;
}

/**
 * @brief Recreates the entire board slightly differently
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
static void ScrollBoardHorizontal(Display *display)
{
    register int r, c;
	int temp[MAX_ROW_EDIT][2];
	int temp2[MAX_ROW_EDIT][2];

    if (noSound == False) playSoundFile("sticky", 50);

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

	/* Move the outer edge first */
	for (r = 0; r < MAX_ROW_EDIT; r++)
	{
	  	temp[r][0] = blocks[r][0].blockType;
	  	temp[r][1] = blocks[r][0].counterSlide;
	}

	for (r = 0; r < MAX_ROW_EDIT; r++)
    	AddNewBlock(display, playWindow, r, 0,
           	blocks[r][MAX_COL_EDIT - 1].blockType, 
			blocks[r][MAX_COL_EDIT - 1].counterSlide, False);

    for (c = 1; c < MAX_COL_EDIT; c++)
    {
		for (r = 0; r < MAX_ROW_EDIT; r++)
		{
		  	temp2[r][0] = blocks[r][c].blockType;
		  	temp2[r][1] = blocks[r][c].counterSlide;
		}

		for (r = 0; r < MAX_ROW_EDIT; r++)
            AddNewBlock(display, playWindow, r, c,
            	temp[r][0], temp[r][1], False);

		for (r = 0; r < MAX_ROW_EDIT; r++)
		{
		  	temp[r][0] = temp2[r][0];
		  	temp[r][1] = temp2[r][1];
		}
    }

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

	modified = True;
}

/**
 * @brief Recreates the entire board vertically
 *
 * @param Display display X11 display
 * 
 * @todo Remove reliance on X11
 *
 */
static void FlipBoardVertical(Display *display)
{
    register int r, c;
	int temp[MAX_COL_EDIT][2];

    if (noSound == False) playSoundFile("wzzz2", 50);

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

    /* Scan through all block entries and erase their contents */
    for (r = 0; r < (MAX_ROW_EDIT / 2); r++)
    {
		/* make a copy of row from bottom up */
		for (c = 0; c < MAX_COL_EDIT; c++)
		{
		  	temp[c][0] = blocks[MAX_ROW_EDIT - r - 1][c].blockType;
		  	temp[c][1] = blocks[MAX_ROW_EDIT - r - 1][c].counterSlide;
		}

		/* copy top down row to bottom up row */
		for (c = 0; c < MAX_COL_EDIT; c++)
            AddNewBlock(display, playWindow, MAX_ROW_EDIT - r - 1, c,
            	blocks[r][c].blockType, blocks[r][c].counterSlide, False);

		/* copy temp to top down row */
		for (c = 0; c < MAX_COL_EDIT; c++)
            AddNewBlock(display, playWindow, r, c,
            	temp[c][0], temp[c][1], False);
    }

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

	modified = True;
}

/**
 * @brief Recreates the whole board scrolled vertically
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
static void ScrollBoardVertical(Display *display)
{
    register int r, c;
	int temp[MAX_COL_EDIT][2];
	int temp2[MAX_COL_EDIT][2];

    if (noSound == False) playSoundFile("sticky", 50);

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

	/* Move the outer edge first */
	for (c = 0; c < MAX_COL_EDIT; c++)
	{
	  	temp[c][0] = blocks[0][c].blockType;
	  	temp[c][1] = blocks[0][c].counterSlide;
	}

	for (c = 0; c < MAX_COL_EDIT; c++)
    	AddNewBlock(display, playWindow, 0, c,
           	blocks[MAX_ROW_EDIT - 1][c].blockType,
			blocks[MAX_ROW_EDIT - 1][c].counterSlide, False);

    for (r = 1; r < MAX_ROW_EDIT; r++)
    {
		for (c = 0; c < MAX_COL_EDIT; c++)
		{
		  	temp2[c][0] = blocks[r][c].blockType;
		  	temp2[c][1] = blocks[r][c].counterSlide;
		}

		for (c = 0; c < MAX_COL_EDIT; c++)
            AddNewBlock(display, playWindow, r, c,
            	temp[c][0], temp[c][1], False);

		for (c = 0; c < MAX_COL_EDIT; c++)
		{
		  	temp[c][0] = temp2[c][0];
		  	temp[c][1] = temp2[c][1];
		}
    }

	/* Change all random blocks to RANDOM_BLKS for editor */
	HandleRandomBlocks(display);

	modified = True;
}

/**
 * @brief Changes blocks to RANDOM BLOCKS
 * 
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11 system
 *
 */
static void HandleRandomBlocks(Display *display)
{
	int r, c;

    for (r = 0; r < MAX_ROW_EDIT; r++)
    {
		for (c = 0; c < MAX_COL_EDIT; c++)
		{
			if (blocks[r][c].random)
				blocks[r][c].blockType = RANDOM_BLK;
		}
	}
}

/**
 * @brief This function takes user input and loads a XBoing level into the level editor
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
static void LoadALevel(Display *display)
{
    /*
     * This function will display the user input dialogue and ask the
     * user for a level. It is validated here and if valid it will load the
     * level and allow you to edit it.
     */

    char str[80];
    char levelPath[1024];
    char *str2;
    int num;

    sprintf(str, "Level range is [1-%d]", MAX_NUM_LEVELS);
    SetCurrentMessage(display, messWindow, str, False);

    str[0] = '\0';

    /* Obtain a string from the user. Should contain NULL or numbers */
    strcpy(str, UserInputDialogueMessage(display,
        "Input load level number please.", TEXT_ICON, NUMERIC_ENTRY_ONLY));

    /* Nothing input so just return */
    if (str[0] == '\0') return;

    /* Obtain the start level setting */
    num = atoi(str);
    if ((num > 0) && (num <= MAX_NUM_LEVELS))
    {
	    /* Construct the Edit level filename */
   	 	if ((str2 = getenv("XBOING_LEVELS_DIR")) != NULL)
        	sprintf(levelPath, "%s/level%02ld.data", str2, (u_long) num);
    	else
        	sprintf(levelPath, "%s/level%02ld.data", 
				LEVEL_INSTALL_DIR, (u_long) num);

    	/* Read in a Edit level */
    	if (ReadNextLevel(display, playWindow, levelPath, False) == False)
        	ShutDown(display, 1, "Sorry, invalid level specified.");

		/* Change all random blocks to RANDOM_BLKS for editor */
		HandleRandomBlocks(display);

		level = (u_long) num;
		DisplayLevelInfo(display, levelWindow, (u_long) num);
		RedrawEditorArea(display, playWindow);

        /* Ok now load level number */
        sprintf(str, "Editing level %d", num);
        SetCurrentMessage(display, messWindow, str, False);

		modified = False;
    }
    else
    {
        /* Value out of range so let them know the range. */
        sprintf(str, "Invalid - level range [1-%d]", MAX_NUM_LEVELS);
        SetCurrentMessage(display, messWindow, str, True);
    }
}

/**
 * @brief Takes user input in order to create a XBoing level save
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
static void SaveALevel(Display *display)
{
    char str[80];
    char levelPath[1024];
    char *str2;
    int num;

    sprintf(str, "Level range is [1-%d]", MAX_NUM_LEVELS);
    SetCurrentMessage(display, messWindow, str, False);

    str[0] = '\0';

    /* Obtain a string from the user. Should contain NULL or numbers */
    strcpy(str, UserInputDialogueMessage(display,
        "Input save level number please.", TEXT_ICON, NUMERIC_ENTRY_ONLY));

    /* Nothing input so just return */
    if (str[0] == '\0') return;

    /* Obtain the start level setting */
    num = atoi(str);
    if ((num > 0) && (num <= MAX_NUM_LEVELS))
    {
	    /* Construct the Edit level filename */
   	 	if ((str2 = getenv("XBOING_LEVELS_DIR")) != NULL)
        	sprintf(levelPath, "%s/level%02ld.data", str2, (u_long) num);
    	else
        	sprintf(levelPath, "%s/level%02ld.data", 
				LEVEL_INSTALL_DIR, (u_long) num);

    	if (SaveLevelDataFile(display, levelPath) == False)
        	ShutDown(display, 1, "Sorry, unable to save level.");

        /* Ok now load level number */
        sprintf(str, "Level %d saved.", num);
        SetCurrentMessage(display, messWindow, str, False);

		modified = False;
    }
    else
    {
        /* Value out of range so let them know the range. */
        sprintf(str, "Invalid - level range [1-%d]", MAX_NUM_LEVELS);
        SetCurrentMessage(display, messWindow, str, True);
    }
}

/**
 * @brief Takes user input to set time limit for XBoing level
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
static void SetTimeForLevel(Display *display)
{
    char str[80];
    int num;

    str[0] = '\0';

    /* Obtain a string from the user. Should contain NULL or numbers */
    strcpy(str, UserInputDialogueMessage(display,
        "Input game time in seconds.", TEXT_ICON, NUMERIC_ENTRY_ONLY));

    /* Nothing input so just return */
    if (str[0] == '\0') return;

    num = atoi(str);
    if ((num > 0) && (num <= 3600))
    {
		/* Set and draw the time limit for the level */
		SetLevelTimeBonus(display, timeWindow, num);
        SetCurrentMessage(display, messWindow, "Time limit adjusted", True);
		modified = True;
	}
	else
	{
        sprintf(str, "Invalid - time range [1-%d]", 3600);
        SetCurrentMessage(display, messWindow, str, True);
	}
}

/**
 * @brief Takes user input in order to set a name for a XBoing level
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
static void SetNameForLevel(Display *display)
{
    char str[80];

	sprintf(str, "Name: %s", levelTitle);
    SetCurrentMessage(display, messWindow, str, False);

    str[0] = '\0';

    /* Obtain a string from the user. Should contain NULL or letters */
    strcpy(str, UserInputDialogueMessage(display,
        "Input new name for level please.", TEXT_ICON, TEXT_ENTRY_ONLY));

    /* Nothing input so just return */
    if (str[0] == '\0') return;

    if (strlen(str) > 25)
	{
    	SetCurrentMessage(display, messWindow, "Level name too long.", True);
		return;
	}

	strcpy(levelTitle, str);
    SetCurrentMessage(display, messWindow, "Level name adjusted", True);
	modified = True;
}

/**
 * @brief Handles all button presses for the editor
 *
 * @param Display display X11 display
 * @param KeySym keysym Button pressed
 *
 * @todo Remove reliance on X11
 *
 */
static void handleAllEditorKeys(Display *display, KeySym keysym)
{
	if (EditState == EDIT_TEST)
	{
    	switch (keysym)
    	{
        	case XK_P: case XK_p:
				/* Turn the test mode off */
				FinishPlayTest(display);
				break;

	        case XK_Left: case XK_j: case XK_J:			/* Key control */
   	         	/* Set paddle to move left */
            	paddleMotion = -1;
            	break;

        	case XK_k: case XK_K:						/* Key control */
            	/* Shoot a bullet if available */
            	if (ActivateWaitingBall(display, playWindow) == False)
                	shootBullet(display, playWindow);
            	break;

        	case XK_Right: case XK_l: case XK_L:		/* Key control */
            	/* Set paddle to move right */
            	paddleMotion = 1;
            	break;

			default:
				break;
		}
		
		return;
	}

    /* Switch on the keysym */
    switch (keysym)
    {
        case XK_Q: case XK_q:
			if (modified == True)
			{
				if (YesNoDialogue(display, 
					"Unsaved work, exit editor? [y/n]"))
					EditState = EDIT_FINISH;
			}
			else
				if (YesNoDialogue(display, "Exit the level editor? [y/n]"))
					EditState = EDIT_FINISH;
            break;

        case XK_R: case XK_r:
			RedrawEditorArea(display, playWindow);
            SetCurrentMessage(display, messWindow, "Redraw", True);
			break;

        case XK_l: case XK_L:
			if (modified == True)
			{
				if (YesNoDialogue(display, 
					"Unsaved work, continue load? [y/n]"))
					LoadALevel(display);
			}
			else
				LoadALevel(display);
			break;

        case XK_s: case XK_S:
			SaveALevel(display);
			break;

        case XK_t: case XK_T:
			SetTimeForLevel(display);
			break;

        case XK_n: case XK_N:
			SetNameForLevel(display);
			break;

        case XK_p: case XK_P:
			SetupPlayTest(display);
            SetCurrentMessage(display, messWindow, "Play test level", True);
			break;

        case XK_c: case XK_C:
			if (YesNoDialogue(display, "Clear this level? [y/n]"))
			{
				ClearBlockArray();
				RedrawEditorArea(display, playWindow);
            	SetCurrentMessage(display, messWindow, "Clear level", True);
			}
			break;

        case XK_h: 
            SetCurrentMessage(display, messWindow, "Flip Horizontal", True);
			FlipBoardHorizontal(display);
			RedrawEditorArea(display, playWindow);
			break;

        case XK_H: 
            SetCurrentMessage(display, messWindow, "Scroll Horizontal", True);
			ScrollBoardHorizontal(display);
			RedrawEditorArea(display, playWindow);
			break;

        case XK_v: 
            SetCurrentMessage(display, messWindow, "Flip Vertical", True);
			FlipBoardVertical(display);
			RedrawEditorArea(display, playWindow);
			break;

        case XK_V: 
            SetCurrentMessage(display, messWindow, "Scroll Vertical", True);
			ScrollBoardVertical(display);
			RedrawEditorArea(display, playWindow);
			break;

        default:    /* All other keys */
            break;
    }
}

/**
 * @brief Calls the handleEditorkeys function to deal with all button presses in the editor
 *
 * @param Display display X11 display
 * @param KeySym keysym The symbol pressed
 *
 * @todo remove reliance on x11
 *
 */
void handleEditorKeys(Display *display, KeySym keysym)
{
	switch (EditState)
	{
		default:
			handleAllEditorKeys(display, keysym);
			break;
	}
}


/**
 * @brief Handles the editor state
 *
 * @param Display display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
void Editor(Display *display, Window window)
{
	switch (EditState)
	{
		case EDIT_LEVEL:
			DoLoadLevel(display, window);
			SetupBlockWindow(display, blockWindow);

			EditState = EDIT_NONE;

			SetCurrentSymbol(display, curBlockType);
			break;

		case EDIT_FINISH:
			DoFinish(display, window);
			break;

		case EDIT_TEST:
			if ((frame % PADDLE_ANIMATE_DELAY) == 0)
				handlePaddleMoving(display);

			HandleBallMode(display, 			playWindow);
			HandleBulletMode(display, 			playWindow);
			ExplodeBlocksPending(display, 		playWindow);
			HandlePendingAnimations(display, 	playWindow);
			HandleEyeDudeMode(display, 			playWindow);
			HandleGameTimer(display, 			playWindow);
			break;

		case EDIT_WAIT:
			DoEditWait();
			break;

		default:
			break;
	}
}

/**
 * @brief Free editor action
 *
 * @param Display display X11 display
 *
 * @todo Remove reliance on X11
 *
 */
void FreeEditor(Display *display)
{
	ClearEditorInfo();
}

/**
 * @brief Sets edit state back to editor mode
 *
 */
void ResetEditor(void)
{
	EditState = EDIT_LEVEL;

	DEBUG("Reset Editor mode.")
}

/**
 * @brief Sets wait time in the editor
 *
 * @param EditStates newMode Is set to wait in the function
 * @param int waitFrame Amount of time to wait
 *
 */
void SetEditWait(enum EditStates newMode, int waitFrame)
{
	waitingFrame = waitFrame;
	waitMode = newMode;
	EditState = EDIT_WAIT;
}

/**
 * @brief Sets state to wait mode if waiting 
 *
 */
void DoEditWait(void)
{
	if (frame == waitingFrame)
		EditState = waitMode;
}
