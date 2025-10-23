
/**
 * @file file.h
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


#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <time.h>
#include "faketypes.h"


/**
 * @def SAVE_VERSION
 * 
 * Identifies the format version of the savefile struct
 * 
 */

#define SAVE_VERSION 	2


/**
 * @brief Saved game structure
 * 
 * Specifies the structure for the savegame data
 * 
 */

typedef struct
{
	u_long 	version;	/* Version of save game format */
    u_long  score;      /* Score so far */
    u_long  level;      /* level number */
	int		levelTime;	/* Time left for level */
    time_t  gameTime;   /* Time taken to complete game so far */
	int		livesLeft;	/* Number of lives left */
	int		startLevel;	/* Level game started on */
	int		paddleSize;	/* The current paddle size */
	int 	numBullets;	/* Number of bullets in store */
} saveGameStruct;


/**
 * @brief Loads and displays a level by name
 * 
 * @param display X11 struct _XDisplay
 * @param window  XID for the target window
 * @param levelName text name of the current level
 * @param draw 1 if the screen is to be displayed now, 0 otherwise
 * @return int Returns TRUE if successful, FALSE otherwise
 *  
 */
int ReadNextLevel(Display *display, Window window, char *levelName, int draw);


/**
 * @brief initializes and displays the current level
 * 
 * @param display X11 struct _XDisplay 
 * @param window  XID for the target window
 * 
 */
void SetupStage(Display *display, Window window);


/**
 * @brief Saves a level as a template with the provided level name
 * 
 * @param display X11 struct _XDisplay
 * @param levelName text name of the level template
 * @return int returns TRUE if successful, FALSE otherwise
 *  
 */
int SaveLevelDataFile(Display *display, char *levelName);


/**
 * @brief Saves the current game state to file
 * 
 * @param display X11 struct _XDisplay
 * @param window XID for the target window
 * @return int returns TRUE on success, FALSE otherwise
 * 
 * 
 
 * 
 */
int SaveCurrentGame(Display *display, Window window);


/**
 * @brief Loads a saved game state to resume play
 * 
 * @param display X11 struct _XDisplay
 * @param window XID for the target window
 * @return int returns TRUE on sucecss, FALSE otherwise
 * 
 */
int LoadSavedGame(Display *display, Window window);

#endif
