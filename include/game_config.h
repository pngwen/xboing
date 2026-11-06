#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

/*
 * Shared configuration values for the raylib port.
 *
 * Screen metrics were previously repeated in rayboing_demo.c, intro.c and
 * scoreboard.c; keep the single source of truth here.
 */

#define SCREEN_WIDTH 575
#define SCREEN_HEIGHT 720

/* Frame rate cap for the main loop. */
#define TARGET_FPS 60

/* Persistent high-score table, relative to the working directory. */
#define HIGH_SCORE_FILE "scores.txt"

/* Level loaded when no level file is given on the command line. */
#define DEFAULT_LEVEL_FILE "resource/levels/level01.data"

#endif /* GAME_CONFIG_H */
