//once the player completes the level, if it is a new high score, prompt for their name and a quote and save the score
//similar to the intro screen, but shows the player's score and prompts for name and quote if high score
// Also shows the top 10 high scores
//Press Enter or Space to try again

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <stdbool.h>

#define MAX_NAME_LENGTH 3
#define MAX_QUOTE_LENGTH 64
#define MAX_HIGH_SCORES 10

typedef struct {
    char name[MAX_NAME_LENGTH + 1];
    char quote[MAX_QUOTE_LENGTH + 1];
    int score;
} HighScoreEntry;

// Show the scoreboard screen after a level ends
void ScoreInputScreen(int score, bool isHighScore);
void ShowScoreBoard(void);
// High score management
bool LoadHighScores(const char* filename);
void AddHighScore(const char* name, const char* quote, int score);
HighScoreEntry* GetHighScores(void);

// Lives and score helpers
int GetLivesRemaining(void);
int GetCurrentScore(void);
bool CheckIfHighScore(int score);



#endif 
