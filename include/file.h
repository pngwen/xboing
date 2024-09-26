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


/**
 * @def SAVE_VERSION
 * 
 * @todo update #define to const uint32_t variable
 * 
 */

#define SAVE_VERSION 	2


/**
 * @brief Saved game structure
 * 
 * @todo u_long is a non-standard type. Replace with type uint32_t
 * @todo use type uint32_t instead of type int for portability
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
 * @todo remove preprocessor command for function prototypes.
 *  
 */

#if NeedFunctionPrototypes

/**
 * @brief Loads and displays a level by name
 * 
 * @param display X11 struct _XDisplay
 * @param window  XID for the target window
 * @param levelName text name of the current level
 * @param draw 1 if the screen is to be displayed now, 0 otherwise
 * @return int Returns TRUE if successful, FALSE otherwise
 * 
 * @bug references outdated X11 system
 * 
 * @todo update X11 references to new system
 * @todo replace True/False with true/false for boolean values
 * @todo function returns true/false; update return type to boolean
 * @todo parameter levelName does not change; update to const
 * @todo parameter draw used as true/false; update to boolean
 * @todo timeLimit set to hard-coded value; update 180 to const expression with relavent name
 * @todo new level data is global variables from blocks.c; update to encapsulate data and access via getters/setters
 * @todo add error checking on fgets() to check for NULL —OR— refactor data reads to read and error check all data at one time
 * @todo debug is global variable from init.c; update to encapsulate data and access via getters/setters
 * @todo update DEBUG() from init.h from macro to function
 * @todo move DEBUG() call inside a codeblock for the preceeding if() statement
 * @todo add type to for() loops, either type int or type size_t
 * @todo when looping through rows and cols, add error checking for unexpected EOF
 * @todo refactor switch block; set tmp vars in case, call AddNewBlock() once after block
 * @todo fclose() returns zero on success; updates if() to check for fclose() != 0
 *  
 */
int ReadNextLevel(Display *display, Window window, char *levelName, int draw);


/**
 * @brief initializes and displays the current level
 * 
 * @param display X11 struct _XDisplay 
 * @param window  XID for the target window
 * 
 * @bug references outdated X11 system
 * 
 * @todo update X11 references to new system
 * @todo replace True with true for boolean values
 * @todo update str to const, rename to identify its purpose
 * @todo rename str2 to identify its purpose
 * @todo buffer size set with hard-coded value; update 1024 to const expression with relavent name
 * @todo u_long is a non-standard type. Replace with type uint32_t
 * @todo bgrnd cycles between values; use a const min and max variable
 * @todo replace sprintf() calls with snprintf() 
 * @todo when constructing level name, move the str assignment to its own line for readability, then check for NULL
 * @todo use bang (!) instead of checking for false when checking ReadNextLevel()
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
 * @bug references outdated X11 system
 * 
 * @todo update X11 references to new system
 * @todo function returns true/false' update return type to boolean
 * @todo parameter levelName does not change; update to const
 * @todo replace True/False with true/false for boolean values
 * @todo buffer size set with hard-coded value; update 1024 to const expression with relavent name
 * @todo when constructing level name, move the levelName assignment to its own line for readability, then check for NULL
 * @todo replace sprintf() calls with snprintf()
 * @todo move DEBUG() call inside a codeblock for the preceeding if() statement
 * @todo add type to for() loops, either type int or type size_t
 * @todo refactor switch block to use ternary operators to improve readability
 * @todo fclose() returns zero on success; updates if() to check for fclose() != 0  
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
 * @bug references outdated X11 system
 * 
 * @todo update X11 references to new system
 * @todo function returns true/false' update return type to boolean
 * @todo replace sprintf() calls with snprintf()
 * @todo replace True/False with true/false for boolean values
 * @todo when construction save file, move the saveFile assignment to its own line for readability, then check for NULL
 * @todo remove unnecessary cast in first param of fwrite()
 * @todo update fclose() to check for EOF or != 0 instead of < 0
 * @todo update if(SaveLevelDataFile() == TRUE) to if(SaveLevelDataFile())
 * @todo when setting Current Message to Game Saved, keep expression to one line *  
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
 * @bug references outdated X11 system
 * @bug possible missing 'return false;' after failed ReadNextLevel() call, line 209
 * 
 * @todo update X11 references to new system
 * @todo function returns true/false' update return type to boolean
 * @todo replace True/False with true/false for boolean values
 * @todo replace sprintf() calls with snprintf()
 * @todo buffer size set with hard-coded value; update 1024 to const expression with relavent name
 * @todo char array size set with hard-coded valuel update 80 to const expressiong with relavent name
 * @todo when construction save file, move saveFile assignment to its own line for readability, then check for NULL
 * @todo change fopen param to 'r' instead of 'r+' since the file is only being read
 * @todo remove unnecessary cast in first param of fwrite()
 * @todo update fclose() to check for EOF or != 0 instead of < 0
 * @todo cast to u_long; remove cast if possible, or update to correct type
 * @todo update if(condition == FALSE) to if(!condition)
 * 
 * 
 */
int LoadSavedGame(Display *display, Window window);


#else

int SaveLevelDataFile();
int SaveCurrentGame();
int LoadSavedGame();
void SetupStage();
int ReadNextLevel();

#endif

#endif
