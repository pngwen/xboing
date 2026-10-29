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

void LoseLife(void);
int GetLivesRemaining(void);


#endif // _DEMO_GAMEMODES_H_