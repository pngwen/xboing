#include <raylib.h>

// Play "window" attributes
#define PLAY_WIDTH 495
#define PLAY_HEIGH 580
#define PLAY_X_OFFSET 35
#define PLAY_Y_OFFSET 60
#define PLAY_BORDER_WIDTH 2

// include xboing pieces here
const int screenWidth = 575;
const int screenHeight = 720;

// Level Data
char gameBlocks[15][9];
char levelName[256];
int timeBonus;

// Read in all level data from a file.
// Fills gameBlocks, levelName, and timeBonus
int readInBlocks(char* filename);

// Loads in all block textures from files
int loadTextures();

// Draw all blocks represented in gameBlocks
void drawBlocks();

// Program main entry point
int main(){

    // Initialization
    InitWindow(screenWidth, screenHeight, "xboing asset test");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();

            ClearBackground(BLACK);
            drawBlocks();

        EndDrawing();
    }

    // De-Initialization

    CloseWindow(); // Close window and OpenGL context

    return 0;
}

int readInBlocks(char* filename)
{
    FILE *levelFile;
    int row, col, type;
    int timeLimit = 180;
	char str[BUF_SIZE];
    char *temp;

    /* Clear all existing mess in structures */
    ClearBlockArray();

    /* Setup the new level data */
    blocksExploding = 0;
    colWidth    = PLAY_WIDTH / MAX_COL;
    rowHeight   = PLAY_HEIGHT / MAX_ROW;
    bonusBlock  = False;
    ResetNumberBonus();

    /* Open the new level data file for reading */
    if ((levelFile = fopen(levelName, "r")) == NULL)
    {
		/* Cannot open the level data file */
		perror(levelName);
        ErrorMessage("Cannot load level data - check level directory path.");
        return False;
    }

    /* Obtain the title string */
    fgets(levelTitle, BUF_SIZE, levelFile);

    /* Remove the carriage return in the title */
    temp = strchr(levelTitle, '\n');
    *temp = '\0';

    if (debug == True) sprintf(str, "level #%ld : <%s>", level, levelTitle);
    DEBUG(str)

    /* Now get the time bonus from the level file */
    fgets(str, BUF_SIZE, levelFile);
    if (sscanf(str, "%d", &timeLimit) != 1)
    {
		/* Error in the time limit of the level data file */
        ErrorMessage("Cannot parse level data - time bonus error.");
        return False;
    }

    /* Set and draw the time limit for the level */
    SetLevelTimeBonus(display, timeWindow, timeLimit);

	/* Loop through all rows and cols */
    for (row = 0; row < (MAX_ROW - 3); row++)
    {
        for (col = 0; col < MAX_COL; col++)
        {
            /* Get the next character from the level data file */
            type = fgetc(levelFile);

            switch (type)
            {
                case 'H' :  /* hyperspace block - walls are now gone */
                    AddNewBlock(display, window, row, col, HYPERSPACE_BLK, 0, 
						draw);
                    break;

                case 'B' :  /* bullet block - ammo */
                    AddNewBlock(display, window, row, col, BULLET_BLK, 0, draw);
                    break;

                case 'c' :  /* maximum ammo bullet block  */
                    AddNewBlock(display, window, row, col, MAXAMMO_BLK, 0, 
						draw);
                    break;

                case 'r' :  /* A red block */
                    AddNewBlock(display, window, row, col, RED_BLK, 0, draw);
                    break;

                case 'g' :  /* A green block */
                    AddNewBlock(display, window, row, col, GREEN_BLK, 0, draw);
                    break;

                case 'b' :  /* A blue block */
                    AddNewBlock(display, window, row, col, BLUE_BLK, 0, draw);
                    break;

                case 't' :  /* A tan block */
                    AddNewBlock(display, window, row, col, TAN_BLK, 0, draw);
                    break;

                case 'p' :  /* A purple block */
                    AddNewBlock(display, window, row, col, PURPLE_BLK, 0, draw);
                    break;

                case 'y' :  /* A yellow block */
                    AddNewBlock(display, window, row, col, YELLOW_BLK, 0, draw);
                    break;

                case 'w' :  /* A solid wall block */
                    AddNewBlock(display, window, row, col, BLACK_BLK, 0, draw);
                    break;

                case '0' :  /* A counter block - no number */
                    AddNewBlock(display, window, row, col, COUNTER_BLK, 0, 
						draw);
                    break;

                case '1' :  /* A counter block level 1 */
                    AddNewBlock(display, window, row, col, COUNTER_BLK, 1, 
						draw);
                    break;

                case '2' : /* A counter block level 2 */
                    AddNewBlock(display, window, row, col, COUNTER_BLK, 2, 
						draw);
                    break;

                case '3' : /* A counter block level 3 */
                    AddNewBlock(display, window, row, col, COUNTER_BLK, 3, 
						draw);
                    break;

                case '4' : /* A counter block level 4 */
                    AddNewBlock(display, window, row, col, COUNTER_BLK, 4, 
						draw);
                    break;

                case '5' : /* A counter block level 5  - highest */
                    AddNewBlock(display, window, row, col, COUNTER_BLK, 5, 
						draw);
                    break;

                case '+' : /* A roamer block */
                    AddNewBlock(display, window, row, col, ROAMER_BLK, 0, draw);
                    break;

                case 'X' : /* A bombaklat - kurwa! */
                    AddNewBlock(display, window, row, col, BOMB_BLK, 0, draw);
                    break;

                case 'D' : /* A death block */
                    AddNewBlock(display, window, row, col, DEATH_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                case 'L' : /* An extra ball block */
                    AddNewBlock(display, window, row, col, EXTRABALL_BLK, 0, 
						draw);
                    break;

                case 'M' : /* A machine gun block */
                    AddNewBlock(display, window, row, col, MGUN_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                case 'W' : /* A wall off block */
                    AddNewBlock(display, window, row, col, WALLOFF_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                case '?' : /* A random changing block */
                    AddNewBlock(display, window, row, col, RANDOM_BLK, 0, draw);
                    break;

                case 'd' : /* A dropping block */
                    AddNewBlock(display, window, row, col, DROP_BLK, 0, draw);
                    break;

                case 'T' : /* A extra time block */
                    AddNewBlock(display, window, row, col, TIMER_BLK, 0, draw);
                    break;

                case 'm' : /* A multiple ball block */
                    AddNewBlock(display, window, row, col, MULTIBALL_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                case 's' : /* A sticky block */
                    AddNewBlock(display, window, row, col, STICKY_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                case 'R' :  /* reverse block - switch paddle control */
                    AddNewBlock(display, window, row, col, REVERSE_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                case '<' :  /* shrink paddle block - make paddle smaller */
                    AddNewBlock(display, window, row, col, PAD_SHRINK_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                case '>' :  /* expand paddle block - make paddle bigger */
                    AddNewBlock(display, window, row, col, PAD_EXPAND_BLK,
                        SHOTS_TO_KILL_SPECIAL, draw);
                    break;

                default:
                    break;
            }

        }

        /* Get the newline */
        type = fgetc(levelFile);
    }

    /* Close our level data file */
    if (fclose(levelFile) < 0)
        WarningMessage("Cannot close level data file.");

	/* Success */
    return True;
}

void drawBlocks(){

    struct aBlock *blockP;
	char tmp[10];
	int w, h, x1, y1, len;

	/* Get the pointer to the block we need */
	blockP = &blocks[r][c];

	switch(blockType)
	{
		case DYNAMITE_BLK:	/* dynamite block block */
			/* 
			 * This is not a block type but an image that overlays a block
			 * when it becomes an explode all block.
			 */
			RenderShape(display, window, dynamite,
				dynamiteM, x, y, 29, 15, False);
			break;

		case TIMER_BLK:		/* extra time clock block */
			RenderShape(display, window, timeblock,
				timeblockM, x, y, 21, 21, False);
			break;

		case PAD_SHRINK_BLK:	/* Paddle shrink block */
			RenderShape(display, window, paddleshrink,
				paddleshrinkM, x, y, 40, 15, False);
			break;

		case PAD_EXPAND_BLK:	/* Paddle expand block */
			RenderShape(display, window, paddleexpand,
				paddleexpandM, x, y, 40, 15, False);
			break;

		case BULLET_BLK:	/* Draw a bullet shape */
			RenderShape(display, window, yellowblock, yellowblockM, 
				x, y, 40, 20, False);
			DrawTheBullet(display, window, x+6, y+10);
			DrawTheBullet(display, window, x+15, y+10);
			DrawTheBullet(display, window, x+24, y+10);
			DrawTheBullet(display, window, x+33, y+10);
			break;

		case MAXAMMO_BLK:
			RenderShape(display, window, unlimitammo, unlimitammoM, 
				x, y, 40, 20, False);
			break;

		case MULTIBALL_BLK:	/* Draw multiple ball block */
			RenderShape(display, window, multiball, multiballM, 
				x, y, 40, 20, False);
			break;

		case STICKY_BLK:	/* Draw sticky block */
			RenderShape(display, window, sticky, stickyM, 
				x, y, 32, 27, False);
			break;

		case RANDOM_BLK:	/* Draw a red block shape for random */
			RenderShape(display, window, redblock, redblockM, 
				x, y, 40, 20, False);
			strcpy(tmp, "- R -");
			len = strlen(tmp);
			w 	= XTextWidth(dataFont, tmp, len);
			h 	= dataFont->ascent + dataFont->descent;
			x1 	= x + (20 - w/2);
			y1  = y + (10 - h/2);
			DrawText(display, window, x1, y1, dataFont, black, tmp, len);
			break;

		case RED_BLK:	/* Draw a red block shape */
			RenderShape(display, window, redblock, redblockM, 
				x, y, 40, 20, False);
			break;

		case BLACK_BLK:		/* Draw a solid wall block */	
			RenderShape(display, window, blackblock, blackblockM, 
				x, y, 50, 30, False);
			break;

		case BLACKHIT_BLK:		/* Draw a solid wall block hit */	
			RenderShape(display, window, blackhit, blackhitM, 
				x, y, 50, 30, False);
			break;

		case GREEN_BLK:		/* Draw a green block */
			RenderShape(display, window, greenblock, greenblockM, 
				x, y, 40, 20, False);
			break;

		case DROP_BLK:		/* Draw a drop block same as green block */
			RenderShape(display, window, greenblock, greenblockM, 
				x, y, 40, 20, False);
			sprintf(tmp, "%d", blockP->hitPoints);	
			len = strlen(tmp);
			w 	= XTextWidth(dataFont, tmp, len);
			h 	= dataFont->ascent + dataFont->descent;
			x1 	= x + (20 - w/2);
			y1  = y + (10 - h/2);
			DrawText(display, window, x1, y1, dataFont, black, tmp, len);
			break;

		case BLUE_BLK:		/* Draw a blue block */
			RenderShape(display, window, blueblock, blueblockM, 
				x, y, 40, 20, False);
			break;

		case YELLOW_BLK:	/* Draw a yellow block */
			RenderShape(display, window, yellowblock, yellowblockM, 
				x, y, 40, 20, False);
			break;

		case TAN_BLK:		/* Draw a tan block */
			RenderShape(display, window, tanblock, tanblockM, 
				x, y, 40, 20, False);
			break;

		case PURPLE_BLK:	/* Draw a purple block */
			RenderShape(display, window, purpleblock, purpleblockM, 
				x, y, 40, 20, False);
			break;

		case ROAMER_BLK:	/* Draw a frame of roaming dude block */
			RenderShape(display, window, roamer[slide], 
				roamerM[slide], x, y, 25, 27, False);
			break;

		case COUNTER_BLK:	/* Draw a frame of counter block */
			RenderShape(display, window, counterblock[slide], 
				counterblockM[slide], x, y, 40, 20, False);
			break;

		case BONUSX2_BLK:	/* Draw a bonus x2 coin block */
			RenderShape(display, window, x2bonus[slide], 
				x2bonusM[slide], x, y, 27, 27, False);
			break;

		case BONUSX4_BLK:	/* Draw a bonus x4 coin block */
			RenderShape(display, window, x4bonus[slide], 
				x4bonusM[slide], x, y, 27, 27, False);
			break;

		case BONUS_BLK:	/* Draw a bonus coin block */
			RenderShape(display, window, Bonus[slide], 
				BonusM[slide], x, y, 27, 27, False);
			break;

		case BOMB_BLK:		/* Draw a bomb block */
			RenderShape(display, window, bombblock, bombblockM, 
				x, y, 30, 30, False);
			break;

		case DEATH_BLK:		/* Draw the pirate death block */
			RenderShape(display, window, death[slide], deathM[slide], 
				x, y, 30, 30, False);
			break;

		case REVERSE_BLK:	/* Draw the reverse block */
			RenderShape(display, window, revblock, revblockM, 
				x, y, 33, 16, False);
			break;

		case EXTRABALL_BLK:	/* Draw the extra ball block */
			RenderShape(display, window, extraball[slide], extraballM[slide], 
				x, y, 30, 19, False);
			break;

		case HYPERSPACE_BLK:	/* Draw the hyperspace block */
			RenderShape(display, window, hyperblock, hyperblockM, 
				x, y, 31, 31, False);
			break;

		case MGUN_BLK:	/* Draw the machine gun block */
			RenderShape(display, window, mgunblock, mgunblockM, 
				x, y, 35, 15, False);
			break;

		case WALLOFF_BLK:	/* Draw the wall off block */
			RenderShape(display, window, walloffblock, walloffblockM, 
				x, y, 27, 23, False);
			break;
	}
}

static void CalculateBlockGeometry(int row, int col)
{
	struct aBlock *blockP;
	XPoint points[4];
	int halfWidth, halfHeight;

	/* Pointer to the correct block we need - speed things up */
	blockP = &blocks[row][col];

	switch (blockP->blockType)
	{
		case COUNTER_BLK:
			blockP->width 			= BLOCK_WIDTH;
			blockP->height			= BLOCK_HEIGHT;
			blockP->blockOffsetX	= (colWidth - BLOCK_WIDTH) / 2;
			blockP->blockOffsetY 	= (rowHeight - BLOCK_HEIGHT) / 2;
			break;

		case TIMER_BLK:
			blockP->width 			= 21;
			blockP->height			= 21;
			blockP->blockOffsetX	= (colWidth - 21) / 2;
			blockP->blockOffsetY 	= (rowHeight - 21) / 2;
			break;

		case ROAMER_BLK:
			blockP->width 			= 25;
			blockP->height			= 27;
			blockP->blockOffsetX	= (colWidth - 25) / 2;
			blockP->blockOffsetY 	= (rowHeight - 27) / 2;
			break;

		case MGUN_BLK:
			blockP->width 			= 35;
			blockP->height			= 15;
			blockP->blockOffsetX	= (colWidth - 35) / 2;
			blockP->blockOffsetY 	= (rowHeight - 15) / 2;
			break;

		case WALLOFF_BLK:
			blockP->width 			= 27;
			blockP->height			= 23;
			blockP->blockOffsetX	= (colWidth - 27) / 2;
			blockP->blockOffsetY 	= (rowHeight - 23) / 2;
			break;

		case REVERSE_BLK:
			blockP->width 			= 33;
			blockP->height			= 16;
			blockP->blockOffsetX	= (colWidth - 33) / 2;
			blockP->blockOffsetY 	= (rowHeight - 16) / 2;
			break;

		case EXTRABALL_BLK:
			blockP->width 			= 30;
			blockP->height			= 19;
			blockP->blockOffsetX	= (colWidth - 30) / 2;
			blockP->blockOffsetY 	= (rowHeight - 19) / 2;
			break;

		case HYPERSPACE_BLK:
			blockP->width 			= 31;
			blockP->height			= 31;
			blockP->blockOffsetX	= (colWidth - 31) / 2;
			blockP->blockOffsetY 	= (rowHeight - 31) / 2;
			break;

		case BOMB_BLK:
		case DEATH_BLK:
			blockP->width 			= 30;
			blockP->height			= 30;
			blockP->blockOffsetX	= (colWidth - 30) / 2;
			blockP->blockOffsetY 	= (rowHeight - 30) / 2;
			break;

		case STICKY_BLK:
			blockP->width 			= 32;
			blockP->height			= 27;
			blockP->blockOffsetX	= (colWidth - 32) / 2;
			blockP->blockOffsetY 	= (rowHeight - 27) / 2;
			break;

		case BLACK_BLK:
			blockP->width 			= 50;
			blockP->height			= 30;
			blockP->blockOffsetX	= (colWidth - 50) / 2;
			blockP->blockOffsetY 	= (rowHeight - 30) / 2;
			break;

        case PAD_SHRINK_BLK:
		case PAD_EXPAND_BLK:
			blockP->width 			= 40;
			blockP->height			= 15;
			blockP->blockOffsetX	= (colWidth - 40) / 2;
			blockP->blockOffsetY 	= (rowHeight - 15) / 2;
			break;

		case BONUS_BLK:
		case BONUSX4_BLK:
		case BONUSX2_BLK:
			blockP->width 			= 27;
			blockP->height			= 27;
			blockP->blockOffsetX	= (colWidth - 27) / 2;
			blockP->blockOffsetY 	= (rowHeight - 27) / 2;
			break;

		default:		/* All other blocks */
			blockP->width 			= BLOCK_WIDTH;
			blockP->height			= BLOCK_HEIGHT;
			blockP->blockOffsetX	= (colWidth - BLOCK_WIDTH) / 2;
			blockP->blockOffsetY 	= (rowHeight - BLOCK_HEIGHT) / 2;
			break;

	}	

	/* Calculate the offset within the block grid */
	blockP->x = (col * colWidth) + blockP->blockOffsetX;
	blockP->y = (row * rowHeight) + blockP->blockOffsetY;

	/* Used below */
	halfWidth = blockP->x + (blockP->width / 2);
	halfHeight = blockP->y + (blockP->height / 2);

	/* Create the XPoint array for the top region */
	points[0].x = blockP->x;
	points[0].y = blockP->y;
	points[1].x = halfWidth;
	points[1].y = halfHeight;
	points[2].x = blockP->x + blockP->width;
	points[2].y = blockP->y;
	points[3].x = points[0].x;
	points[3].y = points[0].y;

	/* Create the top region for the block */
    blockP->regionTop = XPolygonRegion(points, 4, EvenOddRule);

	/* Create the XPoint array for the bottom region */
	points[0].x = blockP->x;
	points[0].y = blockP->y + blockP->height;
	points[1].x = halfWidth;
	points[1].y = halfHeight;
	points[2].x = blockP->x + blockP->width;
	points[2].y = points[0].y;
	points[3].x = points[0].x;
	points[3].y = points[0].y;

	/* Create the bottom region for the block */
    blockP->regionBottom = XPolygonRegion(points, 4, EvenOddRule);

	/* Create the XPoint array for the left region */
	points[0].x = blockP->x;
	points[0].y = blockP->y;
	points[1].x = halfWidth;
	points[1].y = halfHeight;
	points[2].x = blockP->x;
	points[2].y = blockP->y + blockP->height;
	points[3].x = points[0].x;
	points[3].y = points[0].y;

	/* Create the left region for the block */
    blockP->regionLeft = XPolygonRegion(points, 4, EvenOddRule);

	/* Create the XPoint array for the right region */
	points[0].x = blockP->x + blockP->width;
	points[0].y = blockP->y;
	points[1].x = halfWidth;
	points[1].y = halfHeight;
	points[2].x = points[0].x;
	points[2].y = blockP->y + blockP->height;
	points[3].x = points[0].x;
	points[3].y = points[0].y;

	/* Create the right region for the block */
    blockP->regionRight = XPolygonRegion(points, 4, EvenOddRule);
}

void EraseVisibleBlock(Display *display, Window window, int row, int col)
{
	/* Actually erase the block from the arena */
	struct aBlock *blockP;

	if (row > MAX_ROW || row < 0) return;
	if (col > MAX_COL || col < 0) return;

	/* Erase the old block */
   	blockP = &blocks[row][col];

	if (blockP->occupied)
		XClearArea(display, window, blockP->x, blockP->y, blockP->width,
			blockP->height, False);
}

void AddNewBlock(Display *display, Window window, int row, int col,
	int blockType, int counterSlide, int drawIt)
{
	struct aBlock *blockP;

	if (row > MAX_ROW || row < 0) return;
	if (col > MAX_COL || col < 0) return;

	ClearBlock(row, col);

	/* Pointer to the block we want */
	blockP = &blocks[row][col];

	/* Now set the block structure with new values */
	blockP->blockType 		= blockType;
	blockP->occupied 		= 1;
	blockP->counterSlide 	= counterSlide;
	blockP->lastFrame 		= frame + INFINITE_DELAY;

	/* Handle the special case for a random block */
	if (blockType == RANDOM_BLK)
	{
		/* Setup the random block so it has a next frame and new type */
		blockP->random 	  = True;
		blockP->blockType = RED_BLK;
		blockP->nextFrame = frame + 1;
	} else if (blockType == DROP_BLK)
	{
		/* Setup for a dropping block */
		blockP->drop 	  = True;
		blockP->nextFrame = frame + (rand() % DROP_DELAY) + 200;
	} else if (blockType == ROAMER_BLK)
	{
		/* Setup for a roaming block */
		blockP->nextFrame = frame + (rand() % ROAM_EYES_DELAY) + 50;
		blockP->lastFrame = frame + (rand() % ROAM_DELAY) + 300;
	}

	/* Work out all the block geometry stuff */
	CalculateBlockGeometry(row, col);
	
	/* Add the number of points that will be awarded for each block */
	switch(blockType)
	{
		case BULLET_BLK:
		case MAXAMMO_BLK:
			blockP->hitPoints = 50;
			break;

		case RED_BLK:
			blockP->hitPoints = 100;
			break;

		case GREEN_BLK:
			blockP->hitPoints = 120;
			break;

		case BLUE_BLK:
			blockP->hitPoints = 110;
			break;

		case TAN_BLK:
			blockP->hitPoints = 130;
			break;

		case YELLOW_BLK:
			blockP->hitPoints = 140;
			break;

		case PURPLE_BLK:
			blockP->hitPoints = 150;
			break;

		case BOMB_BLK:
			blockP->hitPoints = 50;
			break;

		case ROAMER_BLK:
			blockP->hitPoints = 400;
			break;

		case DROP_BLK:
			blockP->hitPoints = ((MAX_ROW - row) * 100);
			break;

		case COUNTER_BLK:
			blockP->hitPoints = 200;
			break;

		case EXTRABALL_BLK:
			blockP->nextFrame = frame + EXTRABALL_DELAY;
			blockP->hitPoints = 100;
			break;

		case TIMER_BLK:
		case HYPERSPACE_BLK:
		case MGUN_BLK:
		case WALLOFF_BLK:
		case REVERSE_BLK:
		case MULTIBALL_BLK:
		case STICKY_BLK:
        case PAD_SHRINK_BLK:
		case PAD_EXPAND_BLK:
			blockP->hitPoints = 100;
			break;

		case DEATH_BLK:
			blockP->hitPoints = 0;
			blockP->nextFrame = frame + DEATH_DELAY2;
			break;

		default:
			break;
	}

	/* Draw the blocks please */
	if (drawIt) DrawBlock(display, window, row, col, blockType);
}