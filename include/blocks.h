#ifndef _BLOCKS_H_
#define _BLOCKS_H_

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
 * $Id: blocks.h,v 1.1.1.1 1994/12/16 01:36:50 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/blocks.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:50 $
 *
 * $Log: blocks.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:50  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Dependencies on other include files:
 */

// TODO: Remove X11 #include <X11/Xlib.h>
#include <faketypes.h>

/*
 *  Constants and macros:
 */

/* 
 * ALWAYS change the SetupBlockInfo() function as well if you change defines. 
 */

#define NONE_BLK		-2
#define KILL_BLK		-1

#define RED_BLK			0
#define BLUE_BLK		1
#define GREEN_BLK		2
#define TAN_BLK			3
#define YELLOW_BLK		4
#define PURPLE_BLK		5
#define BULLET_BLK		6
#define BLACK_BLK		7
#define COUNTER_BLK		8
#define BOMB_BLK		9
#define DEATH_BLK		10
#define REVERSE_BLK		11
#define HYPERSPACE_BLK	12
#define EXTRABALL_BLK	13
#define MGUN_BLK		14
#define WALLOFF_BLK		15
#define MULTIBALL_BLK	16
#define STICKY_BLK		17
#define PAD_SHRINK_BLK	18
#define PAD_EXPAND_BLK	19
#define DROP_BLK		20
#define MAXAMMO_BLK		21
#define ROAMER_BLK		22
#define TIMER_BLK		23
#define RANDOM_BLK		24

#define DYNAMITE_BLK	25
#define BONUSX2_BLK		26
#define BONUSX4_BLK		27
#define BONUS_BLK		28
#define BLACKHIT_BLK	29

#define MAX_STATIC_BLOCKS 	25
#define MAX_BLOCKS 			30

#define MAX_ROW			18	
#define MAX_COL			9	

#define BLOCK_WIDTH		40
#define BLOCK_HEIGHT	20

#define SPACE			7

#define REGION_NONE		0
#define REGION_TOP		1
#define REGION_BOTTOM	2
#define REGION_LEFT		4
#define REGION_RIGHT	8

#define EXPLODE_DELAY               10
#define BONUS_DELAY                 150
#define BONUS_LENGTH                1500
#define NUMBER_OF_BULLETS_NEW_LEVEL 4
#define DEATH_DELAY1                100
#define DEATH_DELAY2                700
#define EXTRABALL_DELAY             300
#define RANDOM_DELAY                500
#define DROP_DELAY                  1000
#define INFINITE_DELAY              9999999
#define ROAM_EYES_DELAY             300
#define ROAM_DELAY                  1000
#define EXTRA_TIME                  20


/*
 *  Type declarations:
 */

struct blockInfo
{
	int 	blockType;
	int 	width;
	int 	height;
	int		slide;
};

struct aBlock
{
	/* General properties of the block */
	int     	occupied;
	int         blockType;
	int 		hitPoints;

	/* Used when block explodes */
	int     	exploding;
	int 		explodeStartFrame;
	int 		explodeNextFrame;
	int 		explodeSlide;

	/* Used for animation of object */
	int     	currentFrame;
	int     	nextFrame;
	int     	lastFrame;

	/* Used for positioning of block in arena */
	int			blockOffsetX;
	int			blockOffsetY;
	int			x;
	int			y;
	int         width;
	int         height;

	/* Used for ball collision with block */
	Region		regionTop;
	Region		regionBottom;
	Region		regionLeft;
	Region		regionRight;

	/* Indexes into animation frames for object */
	int 		counterSlide;		/* For counter blocks only */
	int 		bonusSlide;			/* For bonus blocks only */

	/* Special types of block flags */
	int			random;
	int			drop;
	int     	specialPopup;
	int 		explodeAll;

	/* Used for splitting of the ball in multiball mode */
	int 		ballHitIndex;
	int			balldx;
	int			balldy;
};

typedef struct aBlock **BLOCKPTR;

/*
 *  Function prototypes:
 */

void FreeBlockPixmaps(Display *display);
void InitialiseBlocks(Display *display, Window window, Colormap colormap);
void DrawBlock(Display *display, Window window, int row, int col, 
	int blockType);
void ExplodeBlocksPending(Display *display, Window window);
void RedrawAllBlocks(Display *display, Window window);
void DrawTheBlock(Display *display, Window window, int x, int y, 
	int blockType, int slide, int r, int c);
void ExplodeBlockType(Display *display, Window window, int x, int y,
	int row, int col, int type, int slide);
void AddNewBlock(Display *display, Window window, int row, int col,
	int blockType, int counterSlide, int drawIt);
void HandlePendingAnimations(Display *display, Window window);
void AddBonusBlock(Display *display, Window window, int *row, int *col,
	int type);
void ClearBlockArray(void);
int StillActiveBlocks(void);
void SkipToNextLevel(Display *display, Window window);
void PlaySoundForBlock(int type);
void AddSpecialBlock(Display *display, Window window, int *row, int *col,
	int type, int kill_shots);
void HandlePendingSpecials(Display *display, Window window, int type,
	int r, int c);
int GetRandomType(int blankBlock);
void SetExplodeAllType(Display *display, Window window, int type);
void EraseVisibleBlock(Display *display, Window window, int row, int col);
void ClearBlock(int row, int col);
void SetupBlockInfo(void);

extern struct aBlock blocks[MAX_ROW][MAX_COL];
extern int rowHeight;
extern int colWidth;
extern int blocksExploding;
extern Pixmap exyellowblock[3], exyellowblockM[3];
extern struct blockInfo    BlockInfo[MAX_BLOCKS];


#endif
