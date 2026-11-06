#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include "scoreboard.h"
#include "game_config.h"
#include <string.h>

/* Score entry screen layout. */
#define INPUT_BOX_WIDTH 200
#define INPUT_BOX_HEIGHT 40
#define INPUT_BOX_TOP_OFFSET 30
#define QUOTE_BOX_WIDTH 300
#define QUOTE_BOX_HEIGHT 40
#define TEXT_ENTRY_INSET 10
#define BOX_BORDER_THICKNESS 2

#define HIGH_SCORE_FONT_SIZE 35
#define PROMPT_FONT_SIZE 20
#define QUOTE_FONT_SIZE 20
#define INSTRUCTION_FONT_SIZE 10
#define ENTRY_FONT_SIZE 20

#define HIGH_SCORE_Y (SCREEN_HEIGHT / 4)
#define PROMPT_Y (SCREEN_HEIGHT / 3 + 10)
#define QUOTE_Y (SCREEN_HEIGHT / 3 + 100)
#define INSTRUCTION_Y (SCREEN_HEIGHT / 2 + 100)

/* Scoreboard screen layout. */
#define SCOREBOARD_TITLE_FONT_SIZE 30
#define SCOREBOARD_TITLE_X_OFFSET 100
#define SCOREBOARD_TITLE_Y 50
#define SCOREBOARD_ROW_X 100
#define SCOREBOARD_FIRST_ROW_Y 120
#define SCOREBOARD_ROW_SPACING 30
#define SCOREBOARD_LINE_FONT_SIZE 20
#define SCOREBOARD_LINE_BUFFER_SIZE 128

static int lives = 3;
static HighScoreEntry highScores[MAX_HIGH_SCORES];
static int highScoreCount = 0;

Rectangle InputBox = { (SCREEN_WIDTH - INPUT_BOX_WIDTH) / 2, SCREEN_HEIGHT / 3 + INPUT_BOX_TOP_OFFSET, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT };
Rectangle QuoteBox = { (SCREEN_WIDTH - QUOTE_BOX_WIDTH) / 2, SCREEN_HEIGHT / 2, QUOTE_BOX_WIDTH, QUOTE_BOX_HEIGHT }; 

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
	//replace by searching through array and finding lowest score
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

		
		int quoteWidth = MeasureText(quote, QUOTE_FONT_SIZE);
		int highscoresWidth = MeasureText(highscore, HIGH_SCORE_FONT_SIZE);
		int promptWidth = MeasureText(prompt, PROMPT_FONT_SIZE);
		int instructionWidth = MeasureText(prompt, PROMPT_FONT_SIZE);

		if (isHighScore) {
			DrawText(highscore, (SCREEN_WIDTH - highscoresWidth) / 2, HIGH_SCORE_Y, HIGH_SCORE_FONT_SIZE, YELLOW);
			DrawText(prompt, (SCREEN_WIDTH - promptWidth) / 2, PROMPT_Y, PROMPT_FONT_SIZE, LIGHTGRAY);
			DrawText(quote, (SCREEN_WIDTH - quoteWidth) / 2, QUOTE_Y, QUOTE_FONT_SIZE, YELLOW);
			DrawText(instruction, (SCREEN_WIDTH - instructionWidth) / 2, INSTRUCTION_Y, INSTRUCTION_FONT_SIZE, YELLOW);
			
			//highlight box based on entry state
			Color nameBoxColor = EnteringQuote ? RAYWHITE : YELLOW;
			Color quoteBoxColor = EnteringQuote ? YELLOW : RAYWHITE;

			DrawText(nickname, InputBox.x + TEXT_ENTRY_INSET, InputBox.y + TEXT_ENTRY_INSET, ENTRY_FONT_SIZE, RAYWHITE);
			DrawText(quotein, QuoteBox.x + TEXT_ENTRY_INSET, QuoteBox.y + TEXT_ENTRY_INSET, ENTRY_FONT_SIZE, RAYWHITE);

			DrawRectangleLinesEx(InputBox, BOX_BORDER_THICKNESS, nameBoxColor); // Draw input box
			DrawRectangleLinesEx(QuoteBox, BOX_BORDER_THICKNESS, quoteBoxColor); // Draw input box
			
		
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
					LoadHighScores(HIGH_SCORE_FILE);
					AddHighScore(nickname, quotein, score);
					saveHighScores(HIGH_SCORE_FILE);
				}
				return;
			}

	}
	// If window closed, just return (main will handle exit)
}

void ShowScoreBoard(void) {
	LoadHighScores(HIGH_SCORE_FILE); // ensure we have latest

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("SCOREBOARD", SCREEN_WIDTH / 2 - SCOREBOARD_TITLE_X_OFFSET, SCOREBOARD_TITLE_Y, SCOREBOARD_TITLE_FONT_SIZE, YELLOW);

		for (int i = 0; i < highScoreCount; i++) {
			char buffer[SCOREBOARD_LINE_BUFFER_SIZE];
			snprintf(buffer, sizeof(buffer), "%s - %s - %d",
				highScores[i].name,
				highScores[i].quote,
				highScores[i].score);
			DrawText(buffer, SCOREBOARD_ROW_X, SCOREBOARD_FIRST_ROW_Y + i * SCOREBOARD_ROW_SPACING, SCOREBOARD_LINE_FONT_SIZE, RAYWHITE);
		}

		EndDrawing();

		if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
			return; // exit scoreboard
		}
	}
}


