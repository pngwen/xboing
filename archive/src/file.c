/**
 * @file file.c
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Controls file input/output for game states and initializes for game play
 * @version 1.1.1.1
 * @date 16 Dec 1994
 * 
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved 
 * @copyright (see COPYRIGHT file for full text)
 * 
 * file.h/.c ontrols file input and output of saved games state data as well as level template data. 
 * When loading from a save, file.h/.c also initializes the game stage based on the data loaded.
 *  
 */


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <xpm.h>

#include "audio.h"
#include "misc.h"
#include "mess.h"
#include "gun.h"
#include "error.h"
#include "bonus.h"
#include "special.h"
#include "main.h"
#include "init.h"
#include "stage.h"
#include "ball.h"
#include "score.h"
#include "paddle.h"
#include "level.h"
#include "sfx.h"
#include "blocks.h"
#include "eyedude.h"

#include "file.h"

/**
 * @def BUF_SIZE
 * 
 * @todo update #define to const uint32_t
 * 
 */

#define BUF_SIZE            1024


saveGameStruct saveGame;

void SetupStage(Display *display, Window window)
{
    char levelPath[1024];
    char *str;
    char str2[1024];
    static int bgrnd = 1;
    u_long newLevel;

    bgrnd++;
    if (bgrnd == 6) bgrnd = 2;
    DrawStageBackground(display, window, bgrnd, true);

    ClearAllBalls();
    currentPaddleSize   = PADDLE_HUGE;
    ResetPaddleStart(display, window);
    ResetBallStart(display, playWindow);

    SetTiltsZero();
    SetUnlimitedBullets(false);
    ClearBullets();
    SetNumberBullets(NUMBER_OF_BULLETS_NEW_LEVEL);
    ResetNumberBonus();

	ChangeEyeDudeMode(EYEDUDE_NONE);
    TurnSpecialsOff(display);
    SetReverseOff();
    DisplayScore(display, scoreWindow, score);
    DisplayLevelInfo(display, levelWindow, level);
    DrawSpecials(display);

    /*
	 * map the level number for the filename so that after the maximum
     * level is reached it will wrap around to 1 again but will not
     * affect the level number
     */
	newLevel = level % (MAX_NUM_LEVELS);
	if (newLevel == 0) newLevel = MAX_NUM_LEVELS;

    /* Construct the level filename */
    if ((str = getenv("XBOING_LEVELS_DIR")) != NULL)
        snprintf(levelPath, "%s/level%02ld.data", str, newLevel);
    else
        snprintf(levelPath, "%s/level%02ld.data", LEVEL_INSTALL_DIR, newLevel);

    if (ReadNextLevel(display, window, levelPath, true) == false)
	{
    	snprintf(str2, "Level%2d = %s", (int) newLevel, GetLevelName());
        ShutDown(display, 1, str2);
	}

    /* Display level name for all to see */
    snprintf(str2, "- %s -", GetLevelName());
    SetCurrentMessage(display, messWindow, str2, true);

    XFlush(display);
}

int LoadSavedGame(Display *display, Window window)
{
	/*
	 * This routine will load the last saved game. 
	 * It will load the level data and other data such as scores and 
     * livesleft etc from another file. The game is then setup and 
   	 * started like usual.
	 */

    FILE *saveFile;
    char levelPath[1024];
    char str[80];
    static int bgrnd = 1;

	/* Save the file in home directory - construct path */
	snprintf(levelPath, "%s/.xboing-saveinfo", GetHomeDir());

    /* Open the save file info for reading */
    if ((saveFile = fopen(levelPath, "r+")) == NULL)
    {
        /* Cannot open the save file */
   		SetCurrentMessage(display, messWindow, "Unable to load game", true);
        return false;
    }

    /* Read the save game info header */
    if (fread((char*)&saveGame, sizeof(saveGameStruct), 1, saveFile) != 1)
    {
        /* Cannot load game file */
   		SetCurrentMessage(display, messWindow, "Unable to load game", true);
   		WarningMessage("Cannot read save game info file.");

    	if (fclose(saveFile) < 0) {
    		WarningMessage("Cannot close save game info file.");
        return false;

        }

        return true;
    }

    if (fclose(saveFile) < 0) {
    	WarningMessage("Cannot close save game info file.");

    }

	/* Check the version of the save file format */
	if (saveGame.version != (u_long) SAVE_VERSION)
	{
   		SetCurrentMessage(display, messWindow, "Unable to load game", true);
   		WarningMessage("Incorrect save game info version.");
		return false;
	}

	/* Setup the game using header information */
	SetTheScore(saveGame.score);
	SetLevelNumber(saveGame.level);
	SetLevelTimeBonus(display, timeWindow, saveGame.levelTime);
	SetLivesLeft(saveGame.livesLeft);
	SetStartingLevel(saveGame.startLevel);
    currentPaddleSize = saveGame.paddleSize;
	/*saveGame.gameTime;*/

    bgrnd++;
    if (bgrnd == 7) bgrnd = 2;
    DrawStageBackground(display, window, bgrnd, true);

    ClearAllBalls();
    ResetPaddleStart(display, window);
    ResetBallStart(display, playWindow);

    SetUnlimitedBullets(false);
    ClearBullets();
    SetNumberBullets(saveGame.numBullets);
    ResetNumberBonus();

	ChangeEyeDudeMode(EYEDUDE_NONE);
    TurnSpecialsOff(display);
    SetReverseOff();
    DisplayScore(display, scoreWindow, score);
    DisplayLevelInfo(display, levelWindow, level);

	/* Load the saved file in home directory - construct path */
	snprintf(levelPath, "%s/.xboing-savelevel", GetHomeDir());

	/* Read in the saved level data */
    if (ReadNextLevel(display, window, levelPath, true) == false)
        ShutDown(display, 1, "trying to read saved level data");

    /* Display level name for all to see */
    snprintf(str, "Resuming: %s", GetLevelName());
    SetCurrentMessage(display, messWindow, str, true);

    XFlush(display);

	return true;
}

Boolean SaveCurrentGame(Display *display, Window window)
{
	/*
	 * This routine will save the current state of the game for loading
	 * at a later date. It will save the level data in one file and the 
	 * other data such as scores and livesleft etc in another file.
	 */

    FILE *saveFile;
    char levelPath[1024];

	/* Setup the save game header */
	saveGame.version 	= (u_long) SAVE_VERSION;
	saveGame.score 		= score;
	saveGame.level 		= level;
	saveGame.levelTime 	= GetLevelTimeBonus();
	saveGame.gameTime 	= time(NULL);
	saveGame.livesLeft 	= GetNumberLife();
	saveGame.startLevel	= GetStartingLevel();
	saveGame.paddleSize	= currentPaddleSize;
	saveGame.numBullets	= GetNumberBullets();

	/* Save the file in home directory - construct path */
	snprintf(levelPath, "%s/.xboing-saveinfo", GetHomeDir());

    /* Open the save file info for writing */
    if ((saveFile = fopen(levelPath, "w+")) == NULL)
    {
        /* Cannot open the save file */
   		SetCurrentMessage(display, messWindow, "Unable to save game", true);
        return false;
    }

    /* Write the save game info header */
    if (fwrite((char*)&saveGame, sizeof(saveGameStruct), 1, saveFile) != 1)
    {
        /* Cannot save game file */
   		SetCurrentMessage(display, messWindow, "Unable to save game", true);
   		WarningMessage("Cannot write save game info file.");

    	if (fclose(saveFile) < 0) {
    		WarningMessage("Cannot close save game info file.");
        return false;
        }

        return true;
    }

    if (fclose(saveFile) < 0) {
    	WarningMessage("Cannot close save game info file.");
    }

	/* Save the file in home directory - construct path */
	snprintf(levelPath, "%s/.xboing-savelevel", GetHomeDir());

	if (SaveLevelDataFile(display, levelPath) == true)
	{
		/* The level did save successfully */
    	SetCurrentMessage(display, messWindow, 
			"Game saved", true);

		/* Only one chance to save buddy */
        ToggleSaving(display, false);
		DrawSpecials(display);

		return true;
	}

	/* The level didn't save successfully */
   	SetCurrentMessage(display, messWindow, 
		"Unable to save game", True);

	return false;
}


int ReadNextLevel(Display *display, Window window, char *levelName, int draw)
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
    bonusBlock  = false;
    ResetNumberBonus();

    /* Open the new level data file for reading */
    if ((levelFile = fopen(levelName, "r")) == NULL)
    {
		/* Cannot open the level data file */
		perror(levelName);
        ErrorMessage("Cannot load level data - check level directory path.");
        return false;
    }

    /* Obtain the title string */
    fgets(levelTitle, BUF_SIZE, levelFile);

    /* Remove the carriage return in the title */
    temp = strchr(levelTitle, '\n');
    *temp = '\0';

    if (debug == true) snprintf(str, "level #%ld : <%s>", level, levelTitle){

        DEBUG(str);

    }

    /* Now get the time bonus from the level file */
    fgets(str, BUF_SIZE, levelFile);
    if (sscanf(str, "%d", &timeLimit) != 1)
    {
		/* Error in the time limit of the level data file */
        ErrorMessage("Cannot parse level data - time bonus error.");
        return false;
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

                case 'X' : /* A bomb block - arggh! */
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
    if (fclose(levelFile) < 0) {
        WarningMessage("Cannot close level data file.");

	/* Success */
    return true;
    }
}

char *levelName;
int SaveLevelDataFile(Display *display, levelName)//////
{
    FILE *levelFile;
    int row, col;
	char str[BUF_SIZE];
	struct aBlock *blockP;

    /* Open the new level data file for reading */
    if ((levelFile = fopen(levelName, "w")) == NULL)
    {
		/* Cannot open the level data file */
		perror(levelName);
        ErrorMessage("Cannot save level data - check level directory path.");
        return false;
    }

    /* Write the title string and carriage return */
	snprintf(str, "%s\n", levelTitle);
    fputs(str, levelFile);

    if (debug == True) {

        snprintf(str, "level #%ld : <%s>", level, levelTitle);
        DEBUG(str)

    }

    /* Now put the time bonus into the level file */
	snprintf(str, "%d\n", GetLevelTimeBonus());
    fputs(str, levelFile);

	/* Loop through all rows and cols */
    for (row = 0; row < (MAX_ROW - 3); row++)
    {
        for (col = 0; col < MAX_COL; col++)
        {
			blockP = &blocks[row][col];

			/* Switch on the type of block */
            switch (blockP->blockType)
            {
                case HYPERSPACE_BLK: 	fputc('H', levelFile); break;
				case BULLET_BLK: 		
					if (blockP->random) fputc('?', levelFile); 
					else 				fputc('B', levelFile); break;

				case MAXAMMO_BLK: 		fputc('c', levelFile); break;
				case RED_BLK: 			
					if (blockP->random) fputc('?', levelFile); 
					else 				fputc('r', levelFile); break;

				case GREEN_BLK: 		
					if (blockP->random) fputc('?', levelFile); 
					else 				fputc('g', levelFile); break;

				case BLUE_BLK: 			
					if (blockP->random) fputc('?', levelFile); 
					else 				fputc('b', levelFile); break;

				case TAN_BLK: 		
					if (blockP->random) fputc('?', levelFile); 
					else 				fputc('t', levelFile); break;

				case PURPLE_BLK: 
					if (blockP->random) fputc('?', levelFile); 
					else 				fputc('p', levelFile); break;

				case YELLOW_BLK:
					if (blockP->random) fputc('?', levelFile); 
					else 				fputc('y', levelFile); break;

				case BLACK_BLK: 		fputc('w', levelFile); break;
				case ROAMER_BLK: 		fputc('+', levelFile); break;
				case BOMB_BLK: 			fputc('X', levelFile); break;
				case DEATH_BLK: 		fputc('D', levelFile); break;
				case EXTRABALL_BLK: 	fputc('L', levelFile); break;
				case MGUN_BLK: 			fputc('M', levelFile); break;
				case WALLOFF_BLK: 		fputc('W', levelFile); break;
				case RANDOM_BLK: 		fputc('?', levelFile); break;
				case DROP_BLK: 			fputc('d', levelFile); break;
				case TIMER_BLK: 		fputc('T', levelFile); break;
				case MULTIBALL_BLK: 	fputc('m', levelFile); break;
				case STICKY_BLK: 		fputc('s', levelFile); break;
				case REVERSE_BLK: 		fputc('R', levelFile); break;
				case PAD_SHRINK_BLK: 	fputc('<', levelFile); break;
				case PAD_EXPAND_BLK: 	fputc('>', levelFile); break;
				case COUNTER_BLK:
					switch (blockP->counterSlide)
					{
						/* The current slide of the counter */
						case 0: 		fputc('0', levelFile); break;
						case 1: 		fputc('1', levelFile); break;
						case 2: 		fputc('2', levelFile); break;
						case 3: 		fputc('3', levelFile); break;
						case 4: 		fputc('4', levelFile); break;
						case 5: 		fputc('5', levelFile); break;
					}
                    break;

                default:
					/* Nothing in the block so put dot */
					fputc('.', levelFile); break;
                    break;
            }

        }

		/* New line */
		fputs("\n", levelFile);
    }

    /* Close our level data file */
    if (fclose(levelFile) < 0){
        WarningMessage("Cannot close level data file.");

	/* Success */
    return true;

    }
}

