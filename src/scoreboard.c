#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include "scoreboard.h"
#include <string.h>

#define MAX_NAME_LENGTH 3
#define SCREEN_WIDTH 575
#define SCREEN_HEIGHT 720
#define MAX_HIGH_SCORES 10

static int lives = 3;
static HighScoreEntry highScores[MAX_HIGH_SCORES];
static int highScoreCount = 0;

Rectangle InputBox = { (SCREEN_WIDTH - 200) / 2, SCREEN_HEIGHT / 3 + 30, 200, 40 };
Rectangle QuoteBox = { (SCREEN_WIDTH - 300) / 2, SCREEN_HEIGHT / 2, 300, 40 }; 

bool LoadHighScores(const char* filename) {
	highScoreCount = 0; // Reset count before loading
	FILE* file = fopen(filename, "r"); // Open file for reading, r = reading file mode
	if (!file) { //safety check
		return false;
	}
	while (highScoreCount < MAX_HIGH_SCORES && fscanf(file, "%3[^,],%63[^,],%d\n",
		highScores[highScoreCount].name,
		highScores[highScoreCount].quote,
		&highScores[highScoreCount].score) == 3) {
			highScoreCount++;
	}
	fclose(file); // Close the file
	return true;
}

bool saveHighScores(const char* filename) {
	FILE* file = fopen(filename, "w"); // overwrite file with sorted list
	if (!file) return false;

	for (int i = 0; i < highScoreCount; i++) {
		fprintf(file, "%s,%s,%d\n",
			highScores[i].name,
			highScores[i].quote,
			highScores[i].score);
	}

	fclose(file);
	return true;
}



void AddHighScore(const char* name, const char* quote, int score) {
	// Sort descending first, so you dont erase a higher score
	for (int i = 0; i < highScoreCount - 1; i++) {
		for (int j = i + 1; j < highScoreCount; j++) {
			if (highScores[j].score > highScores[i].score) {
				HighScoreEntry tmp = highScores[i];
				highScores[i] = highScores[j];
				highScores[j] = tmp;
			}
		}
	}
	// If not full, just add
	if (highScoreCount < MAX_HIGH_SCORES) {
		strncpy(highScores[highScoreCount].name, name, MAX_NAME_LENGTH);
		highScores[highScoreCount].name[MAX_NAME_LENGTH] = '\0';

		strncpy(highScores[highScoreCount].quote, quote, MAX_QUOTE_LENGTH);
		highScores[highScoreCount].quote[MAX_QUOTE_LENGTH] = '\0';

		highScores[highScoreCount].score = score;
		highScoreCount++;
	}
	else {
		// If full, replace lowest score if new one is higher
		if (score > highScores[highScoreCount - 1].score) {
			strncpy(highScores[highScoreCount - 1].name, name, MAX_NAME_LENGTH);
			highScores[highScoreCount - 1].name[MAX_NAME_LENGTH] = '\0';

			strncpy(highScores[highScoreCount - 1].quote, quote, MAX_QUOTE_LENGTH);
			highScores[highScoreCount - 1].quote[MAX_QUOTE_LENGTH] = '\0';

			highScores[highScoreCount - 1].score = score;
		}
	}
	// Re-sort after adding
	for (int i = 0; i < highScoreCount - 1; i++) {
		for (int j = i + 1; j < highScoreCount; j++) {
			if (highScores[j].score > highScores[i].score) {
				HighScoreEntry tmp = highScores[i];
				highScores[i] = highScores[j];
				highScores[j] = tmp;
			}
		}
	}
}


HighScoreEntry* GetHighScores(void) {
	return highScores;
}

int GetCurrentScore(void) {
	return GetBlocksDestroyed();
}


bool CheckIfHighScore(int score) {
	if (highScoreCount < MAX_HIGH_SCORES) return true;

	int minScore = highScores[0].score;
	for (int i = 1; i < highScoreCount; i++) {
		if (highScores[i].score < minScore) {
			minScore = highScores[i].score;
		}
	}
	return score > minScore;
}

void ScoreInputScreen(int score, bool isHighScore) {
	char nickname[MAX_NAME_LENGTH + 1] = { 0 }; // +1 for null terminator
	char quotein[MAX_QUOTE_LENGTH + 1] = { 0 }; // +1 for null terminator
	int QuotesEntered = 0;
	int LettersEntered = 0;
	bool EnteringQuote = false;
	
	

	// Simple loop that draws scoreboard and waits for key press
	while (!WindowShouldClose()) { //same as intro screen loop
		BeginDrawing();
		ClearBackground(GRAY);
		const char* title = "Scoreboard";
		const char* highscore = "You Got a New High Score!";
		const char* prompt = "Please Enter Your Nickname";
		const char* quote = "Please Enter Your Quote";
		const char* instruction = "Please Press TAB To switch fields and ENTER to Submit";

		
		int quoteSize = 20;
		int promptSize = 20;
		int highscoresize = 35; // Slightly larger for high score message
		int instructionSize = 10;

		
		int quoteWidth = MeasureText(quote, quoteSize);
		int highscoresWidth = MeasureText(highscore, highscoresize);
		int promptWidth = MeasureText(prompt, promptSize);
		int instructionWidth = MeasureText(prompt, promptSize);

		if (isHighScore) {
			DrawText(highscore, (SCREEN_WIDTH - highscoresWidth) / 2, SCREEN_HEIGHT / 4, highscoresize, YELLOW);
			DrawText(prompt, (SCREEN_WIDTH - promptWidth) / 2, SCREEN_HEIGHT / 3 + 10 , promptSize, LIGHTGRAY);
			DrawText(quote, (SCREEN_WIDTH - quoteWidth) / 2, SCREEN_HEIGHT /3 + 100 , quoteSize, YELLOW);
			DrawText(instruction, (SCREEN_WIDTH - instructionWidth) / 2, SCREEN_HEIGHT / 2 + 100, instructionSize, YELLOW);
			
			//highlight box based on entry state
			Color nameBoxColor = EnteringQuote ? RAYWHITE : YELLOW;
			Color quoteBoxColor = EnteringQuote ? YELLOW : RAYWHITE;

			DrawText(nickname, InputBox.x + 10, InputBox.y + 10, 20, RAYWHITE);
			DrawText(quotein, QuoteBox.x + 10, QuoteBox.y + 10, 20, RAYWHITE);

			DrawRectangleLinesEx(InputBox, 2, nameBoxColor); // Draw input box
			DrawRectangleLinesEx(QuoteBox, 2, quoteBoxColor); // Draw input box
			
		
		}
			EndDrawing();
			
			// Input handling
			int letter = GetCharPressed();
			if (letter >= 32 && letter <= 125) {
				if (!EnteringQuote && LettersEntered < MAX_NAME_LENGTH) {
					nickname[LettersEntered++] = (char)letter;
					nickname[LettersEntered] = '\0';
				}
				else if (EnteringQuote && QuotesEntered < MAX_QUOTE_LENGTH) {
					quotein[QuotesEntered++] = (char)letter;
					quotein[QuotesEntered] = '\0';
				}
			}

			// Backspace
			if (IsKeyPressed(KEY_BACKSPACE)) {
				if (!EnteringQuote && LettersEntered > 0) {
					nickname[--LettersEntered] = '\0';
				}
				else if (EnteringQuote && QuotesEntered > 0) {
					quotein[--QuotesEntered] = '\0';
				}
			}

			// Switch to quote entry
			if (IsKeyPressed(KEY_TAB)) {
				EnteringQuote = !EnteringQuote;
			}

			// Finalize on Enter
			if (IsKeyPressed(KEY_ENTER)) {
				if (isHighScore && LettersEntered > 0) {
					LoadHighScores("scores.txt");
					AddHighScore(nickname, quotein, score);
					saveHighScores("scores.txt");
				}
				return;
			}

	}
	// If window closed, just return (main will handle exit)
}

void ShowScoreBoard(void) {
	LoadHighScores("scores.txt"); // ensure we have latest

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("SCOREBOARD", SCREEN_WIDTH / 2 - 100, 50, 30, YELLOW);

		for (int i = 0; i < highScoreCount; i++) {
			char buffer[128];
			snprintf(buffer, sizeof(buffer), "%s - %s - %d",
				highScores[i].name,
				highScores[i].quote,
				highScores[i].score);
			DrawText(buffer, 100, 120 + i * 30, 20, RAYWHITE);
		}

		EndDrawing();

		if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
			return; // exit scoreboard
		}
	}
}


