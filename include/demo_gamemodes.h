#ifndef _DEMO_GAMEMODES_H_
#define _DEMO_GAMEMODES_H_

typedef enum {
    MODE_INITGAME,
    MODE_PLAY,
    MODE_WIN,
    MODE_LOSE,
    MODE_CANCEL,
    MODE_EXIT
} GAME_MODES;

GAME_MODES GetGameMode(void);
void SetGameMode(GAME_MODES mode);

void RunInitGameMode(const char *fileName);
void RunPlayMode(void);
void RunEndMode(void);

// Developer mode helpers (enabled only for developer/terminal-launched
// instances). When enabled, additional debug controls such as skipping the
// current level are available.
extern bool devMode;
void SetDevMode(bool enabled);
void SkipToNextLevel(void);

// Returns the file name of the currently loaded level (empty if none).
const char* GetCurrentLevelFile(void);

void LoseLife(void);
int GetLivesRemaining(void);


#endif // _DEMO_GAMEMODES_H_