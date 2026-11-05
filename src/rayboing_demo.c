#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <stdbool.h>
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

#include "demo_gamemodes.h"
#include "demo_blockloader.h"
#include "demo_ball.h"
#include "paddle.h"
#include "audio.h"
#include "intro.h"
#include "scoreboard.h"

const int SCREEN_WIDTH = 575;
const int SCREEN_HEIGHT = 720;

bool mouseControls = true;

bool ValidateParamFilename(int argumentCount, char* arguments[]);
bool IsDevFlag(const char* argument);
bool IsTerminalLaunch(void);
void ReleaseResources(void);
void UpdatePaddleInput(void);

int main(int argumentCount, char* arguments[]) {
    SetTraceLogLevel(LOG_NONE);

    int rtnCode = 1;
    bool windowInitialized = false;

    /* Parse developer flags (e.g. -dev/-debug) and an optional level file. */
    bool devRequested = false;
    char providedLevel[512] = {0};
    for (int i = 1; i < argumentCount; i++) {
        if (IsDevFlag(arguments[i])) {
            devRequested = true;
        } else if (providedLevel[0] == '\0') {
            strncpy(providedLevel, arguments[i], sizeof(providedLevel) - 1);
            providedLevel[sizeof(providedLevel) - 1] = '\0';
        }
    }

    bool developerMode = devRequested || IsTerminalLaunch();
#if defined(_DEBUG)
    developerMode = true;
#endif
    SetDevMode(developerMode);
    if (developerMode) {
        fprintf(stdout, "Developer mode enabled (level skip available).\n");
    }

    // must InitWindow before loading textures
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rayboing Demo");
    SetTargetFPS(60);
    windowInitialized = true;

    // If no filename was provided on the command line, show the intro/start menu
    // and wait for the player to press Enter/Space. After the intro returns,
    // continue with normal initialization (textures/audio/etc.).
    if (argumentCount == 1)
    {
        ShowIntroScreen();

        // If the window was closed while on the intro screen, exit now.
        if (WindowShouldClose())
        {
            if (windowInitialized)
                CloseWindow();
            ReleaseResources();
            return rtnCode;
        }
        SetGameMode(MODE_INITGAME);
    }

    if (!ValidateParamFilename(argumentCount, arguments))
    {
        // Validation only fails for incorrect command-line usage or bad file
        // when an argument was supplied. If it fails here, halt.
        fprintf(stderr, "Program halt on map validation");
    }
    else if (!loadBlockTextures()) {
        fprintf(stderr, "Program halt on iniitalize block texture");
    }
    else if (!InitialisePaddle()) {
        fprintf(stderr, "Program halt on initialize paddle");
    }
    else if (!InitializeBall()) {
        fprintf(stderr, "Program halt on initialize ball");
    }
    else if (!initAudioFiles()) {
        fprintf(stderr, "Program halt on initialize sounds");
    }
    else {

        initializePlayArea();
        SetGameMode(MODE_INITGAME);
        rtnCode = 0;
    }

    // If no filename was provided, supply the default level path
    const char* defaultLevel = "resource/levels/level01.data";

    // main game loop
    // main game loop
    GAME_MODES currentMode = GetGameMode();
    while (currentMode != MODE_EXIT)
    {
        UpdatePaddleInput();

        // Handle game modes
        switch (currentMode)
        {
        case MODE_INITGAME: {
            const char* levelPath = NULL;
            const char* currentLevel = GetCurrentLevelFile();
            if (currentLevel != NULL && currentLevel[0] != '\0') {
                levelPath = currentLevel;           // stay on current level
            }
            else if (providedLevel[0] != '\0') {
                levelPath = providedLevel;           // use provided filename
            }
            else if (argumentCount == 2 && !IsDevFlag(arguments[1])) {
                levelPath = arguments[1];           // use provided filename
            }
            else {
                levelPath = defaultLevel;           // fall back to default level
            }

            RunInitGameMode(levelPath);             // always run init
            break;
        }


        case MODE_PLAY:
            RunPlayMode();
            break;

        case MODE_WIN:
        case MODE_LOSE: {
            
            if (GetLivesRemaining() > 0) {
                SetGameMode(MODE_INITGAME);
            }
            else {
                //out of lives, show scoreboard and end
                RunEndMode();

				LoadHighScores("scores.txt"); // Load existing high scores

                int lastScore = GetCurrentScore();
                bool isHighScore = CheckIfHighScore(lastScore); // Function to check if lastScore is a high score
                if (isHighScore) { 
                    ScoreInputScreen(lastScore, isHighScore); // Show scoreboard screen
				}
				ShowScoreBoard(); // Always Show scoreboard after end

                // Safety check for window close
                if (WindowShouldClose()) { //if window closed during scoreboard
                    SetGameMode(MODE_EXIT); // set mode to exit
                }
                else { //otherwise restart game
                    SetGameMode(MODE_INITGAME);
                }
                break;

            }
        }
        case MODE_CANCEL: {
            RunEndMode();

            //show scoreboard after end + safety save scores
			
			ShowScoreBoard();

            // Safety check for window close
            if (WindowShouldClose()) { //if window closed during scoreboard
                SetGameMode(MODE_EXIT); // set mode to exit
            }
            else { //otherwise restart game
                SetGameMode(MODE_INITGAME);
            }
            break;

        }
        default:
            SetGameMode(MODE_CANCEL);
            break;
        }

        if (WindowShouldClose()) SetGameMode(MODE_EXIT);
        currentMode = GetGameMode();
    }



    if (windowInitialized) CloseWindow();
    ReleaseResources();

	//always save scores on exit
	saveHighScores("scores.txt"); // Load existing high scores
    // exit program
    return rtnCode;
}

bool ValidateParamFilename(int argumentCount, char* arguments[]) {

    // Count actual level-file arguments (ignore developer flags).
    int fileCount = 0;
    for (int i = 1; i < argumentCount; i++)
        if (!IsDevFlag(arguments[i])) fileCount++;

    if (fileCount == 0)
        return true;

    const char* fileName = NULL;
    for (int i = 1; i < argumentCount; i++)
        if (!IsDevFlag(arguments[i])) { fileName = arguments[i]; break; }

    if (fileCount > 1) {
        fprintf(stderr, "Usage: %s [-dev|-debug] [filename]\n", arguments[0]);
        return false;
    }

    if (fileName) {
        FILE* file = fopen(fileName, "r");
        if (!file) {
            fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", fileName);
            return false;
        }
        else {
            fprintf(stdout, "Running Rayboing with map '%s'\n", fileName);
            fclose(file);
            return true;
        }
    }

    // Any other argument usage is invalid.
    fprintf(stderr, "Usage: %s <filename>\n", arguments[0]);
    return false;
}

bool IsDevFlag(const char* argument) {
    if (argument == NULL) return false;
    return (strcmp(argument, "-dev") == 0 ||
            strcmp(argument, "--dev") == 0 ||
            strcmp(argument, "-debug") == 0 ||
            strcmp(argument, "--debug") == 0 ||
            strcmp(argument, "-devmode") == 0);
}

bool IsTerminalLaunch(void) {
#if defined(_WIN32)
    return (_isatty(_fileno(stdout)) != 0 ||
            _isatty(_fileno(stdin)) != 0);
#else
    return (isatty(fileno(stdout)) != 0 ||
            isatty(fileno(stdin)) != 0);
#endif
}

void ReleaseResources(void)
{
    FreePaddle();
    FreeBall();
    freeBlockTextures();
    FreeAudioSystem();
}

void UpdatePaddleInput(void)
{
    if (IsKeyPressed(KEY_M))
    {
        mouseControls = !mouseControls;
        printf("M pressed, mouseControls = %d\n", mouseControls);
    }

    if (mouseControls)
    {
        Vector2 mousePos = GetMousePosition();

        // clamp Y to window, if needed
        if (mousePos.y < 0)
            mousePos.y = 0;
        if (mousePos.y > SCREEN_HEIGHT)
            mousePos.y = SCREEN_HEIGHT;

        SetPaddlePosition(mousePos.x);

        // updates mouse position 
        SetMousePosition(GetPaddlePositionX(), mousePos.y);
    }
    else
    {
        // EnableCursor();
        if (IsKeyDown(KEY_LEFT))
            MovePaddle(PADDLE_LEFT);
        if (IsKeyDown(KEY_RIGHT))
            MovePaddle(PADDLE_RIGHT);
    }

    // Optional toggles
    if (IsKeyPressed(KEY_R))
        ToggleReverse();
    if (IsKeyPressed(KEY_Z))
        ChangePaddleSize(SIZE_DOWN);
    if (IsKeyPressed(KEY_X))
        ChangePaddleSize(SIZE_UP);
}

