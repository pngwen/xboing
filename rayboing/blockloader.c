#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

// Play "window" attributes
#define PLAY_WIDTH 495
#define PLAY_HEIGHT 580
#define PLAY_X_OFFSET 35
#define PLAY_Y_OFFSET 60
#define PLAY_BORDER_WIDTH 2

#define BLOCK_WIDTH 40
#define BLOCK_HEIGHT 20

#define COL_MAX 9
#define ROW_MAX 15

typedef struct Block{

	int blockOffsetX;
	int blockOffsetY;
} Block;

Texture2D HYPERSPACE_BLK, BULLET_BLK, MAXAMMO_BLK, RED_BLK, GREEN_BLK,
			BLUE_BLK, TAN_BLK, PURPLE_BLK, YELLOW_BLK,
			BLACK_BLK, ROAMER_BLK, BOMB_BLK, DEATH_BLK, EXTRABALL_BLK,
			MGUN_BLK, WALLOFF_BLK, RANDOM_BLK, DROP_BLK, TIMER_BLK,
			MULTIBALL_BLK, STICKY_BLK, REVERSE_BLK, PAD_SHRINK_BLK,
			PAD_EXPAND_BLK;
Texture2D COUNTER_BLK[6];

// include xboing pieces here
const int screenWidth = 575;
const int screenHeight = 720;

// Level Data
char gameBlocks[15][9];
char levelName[256];
int timeBonus;

// Read in all level data from a file.
// Fills gameBlocks, levelName, and timeBonus
void loadBlocks(char* filename);

// Draw all blocks represented in gameBlocks
void drawBlocks();

// Sets all characters in the gameBlocks array to '.'
void clearBlocks();

void loadTextures();

// Program main entry point
int main(){

    // Initialization
    InitWindow(screenWidth, screenHeight, "xboing asset test");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

	char filename[] = "levels/level25.data";

	loadBlocks(filename);// Load data from file

    loadTextures();// Load required block textures

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
			drawBlocks();

        EndDrawing();
    }

    // De-Initialization

    CloseWindow(); // Close window and OpenGL context

    return 0;
}

void loadBlocks(char* filename){

	clearBlocks();

	FILE* fp = fopen(filename, "r");
	if(fp == NULL){
		printf("File '%s' could not be opened.", filename);
		exit(1);
	}

	// Get file data
	fgets(levelName, 256, fp);
	fscanf(fp, "%d", &timeBonus);
	getc(fp);

	int row = 0;
	int column = 0;
	char ch;

	ch = getc(fp);
	while(ch != EOF){

		if(ch != '\n'){
			gameBlocks[row][column] = ch;
			column++;
			row += column / COL_MAX;
			column %= COL_MAX;
		}
		ch = getc(fp);
	}
	fclose(fp);
}

void drawBlocks(){

    /* Setup the new level data */
    int colWidth    = PLAY_WIDTH / COL_MAX;
    int rowHeight   = PLAY_HEIGHT / ROW_MAX;
	Texture2D blockTexture;
	Block blockP;

	/* Loop through all blocks */
    for (int row = 0; row < 15; row++){

        for (int col = 0; col < 9; col++){

			blockP.blockOffsetX	= (colWidth - BLOCK_WIDTH) / 2;
			blockP.blockOffsetY 	= (rowHeight - BLOCK_HEIGHT) / 2;

            int empty = 0;

            switch (gameBlocks[row][col]){

                case 'H' :  /* hyperspace block - walls are now gone */
                    blockP.blockOffsetX	= (colWidth - 31) / 2;
					blockP.blockOffsetY 	= (rowHeight - 31) / 2;
					blockTexture = HYPERSPACE_BLK;
					break;

                case 'B' :  /* bullet block - ammo */
					blockTexture = BULLET_BLK;
					break;

                case 'c' :  /* maximum ammo bullet block  */
                    blockTexture = MAXAMMO_BLK;
                    break;

                case 'r' :  /* A red block */
                    blockTexture = RED_BLK;
                    break;

                case 'g' :  /* A green block */
                    blockTexture = GREEN_BLK;
                    break;

                case 'b' :  /* A blue block */
                    blockTexture = BLUE_BLK;
                    break;

                case 't' :  /* A tan block */
                    blockTexture = TAN_BLK;
                    break;

                case 'p' :  /* A purple block */
                    blockTexture = PURPLE_BLK;
                    break;

                case 'y' :  /* A yellow block */
                    blockTexture = YELLOW_BLK;
                    break;

                case 'w' :  /* A solid wall block */
                    blockP.blockOffsetX	= (colWidth - 50) / 2;
					blockP.blockOffsetY 	= (rowHeight - 30) / 2;
					blockTexture = BLACK_BLK;
                    break;

                case '0' :  /* A counter block - no number */
                    blockTexture = COUNTER_BLK[0];
                    break;

                case '1' :  /* A counter block level 1 */
                    blockTexture = COUNTER_BLK[1];
                    break;

                case '2' : /* A counter block level 2 */
                    blockTexture = COUNTER_BLK[2];
                    break;

                case '3' : /* A counter block level 3 */
                    blockTexture = COUNTER_BLK[3];
                    break;

                case '4' : /* A counter block level 4 */
                    blockTexture = COUNTER_BLK[4];
                    break;

                case '5' : /* A counter block level 5  - highest */
                    blockTexture = COUNTER_BLK[5];
                    break;

                case '+' : /* A roamer block */
                    blockP.blockOffsetX	= (colWidth - 25) / 2;
					blockP.blockOffsetY 	= (rowHeight - 27) / 2;
					blockTexture = ROAMER_BLK;
                    break;

                case 'X' : /* A bomb */
                    blockP.blockOffsetX	= (colWidth - 30) / 2;
					blockP.blockOffsetY 	= (rowHeight - 30) / 2;
					blockTexture = BOMB_BLK;
                    break;

                case 'D' : /* A death block */
                    blockP.blockOffsetX	= (colWidth - 30) / 2;
					blockP.blockOffsetY 	= (rowHeight - 30) / 2;
					blockTexture = DEATH_BLK;
                    break;

                case 'L' : /* An extra ball block */
					blockP.blockOffsetX	= (colWidth - 30) / 2;
					blockP.blockOffsetY 	= (rowHeight - 19) / 2;
                    blockTexture = EXTRABALL_BLK;
                    break;

                case 'M' : /* A machine gun block */
					blockP.blockOffsetX	= (colWidth - 35) / 2;
					blockP.blockOffsetY 	= (rowHeight - 15) / 2;
                    blockTexture = MGUN_BLK;
                    break;

                case 'W' : /* A wall off block */
					blockP.blockOffsetX	= (colWidth - 27) / 2;
					blockP.blockOffsetY 	= (rowHeight - 23) / 2;
                    blockTexture = WALLOFF_BLK;
                    break;

                case '?' : /* A random changing block */
                    blockTexture = RANDOM_BLK;
					break;

                case 'd' : /* A dropping block */
                    blockTexture = DROP_BLK;
                    break;

                case 'T' : /* A extra time block */
					blockP.blockOffsetX	= (colWidth - 21) / 2;
					blockP.blockOffsetY 	= (rowHeight - 21) / 2;
                    blockTexture = TIMER_BLK;
                    break;

                case 'm' : /* A multiple ball block */
                    blockTexture = MULTIBALL_BLK;
                    break;

                case 's' : /* A sticky block */
					blockP.blockOffsetX	= (colWidth - 32) / 2;
					blockP.blockOffsetY 	= (rowHeight - 27) / 2;
                    blockTexture = STICKY_BLK;
                    break;

                case 'R' :  /* reverse block - switch paddle control */
					blockP.blockOffsetX	= (colWidth - 33) / 2;
					blockP.blockOffsetY 	= (rowHeight - 16) / 2;
                    blockTexture = REVERSE_BLK;
                    break;

                case '<' :  /* shrink paddle block - make paddle smaller */
					blockP.blockOffsetX	= (colWidth - 40) / 2;
					blockP.blockOffsetY 	= (rowHeight - 15) / 2;
                    blockTexture = PAD_SHRINK_BLK;
                    break;

                case '>' :  /* expand paddle block - make paddle bigger */
                    blockP.blockOffsetX	= (colWidth - 40) / 2;
					blockP.blockOffsetY 	= (rowHeight - 15) / 2;
                    blockTexture = PAD_EXPAND_BLK;
                    break;

                default:
                    empty = 1;
                    break;
            }

    		if(!empty){
                DrawTexture(blockTexture,
                    (col * colWidth) + blockP.blockOffsetX + PLAY_X_OFFSET,
					(row * rowHeight) + blockP.blockOffsetY + PLAY_Y_OFFSET,
                    WHITE);
            }
        }
    }

    DrawRectangleLinesEx((Rectangle){PLAY_X_OFFSET, PLAY_Y_OFFSET, PLAY_WIDTH, PLAY_HEIGHT},
                        PLAY_BORDER_WIDTH, RED);
}

void clearBlocks(){

	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 9; j++){
			gameBlocks[i][j] = '.';
		}
	}
}

void loadTextures(){

    HYPERSPACE_BLK = LoadTexture("blocks/hypspc.png");
    BULLET_BLK = LoadTexture("blocks/grnblk.png");// Green block drawn without bullet texture
    MAXAMMO_BLK = LoadTexture("blocks/lotsammo.png");
    RED_BLK = LoadTexture("blocks/redblk.png");
    GREEN_BLK = LoadTexture("blocks/grnblk.png");
	BLUE_BLK = LoadTexture("blocks/blueblk.png");
    TAN_BLK = LoadTexture("blocks/tanblk.png");
    PURPLE_BLK = LoadTexture("blocks/purpblk.png");
    YELLOW_BLK = LoadTexture("blocks/yellblk.png");
	BLACK_BLK = LoadTexture("blocks/blakblk.png");
    ROAMER_BLK = LoadTexture("blocks/roamer.png");
    BOMB_BLK = LoadTexture("blocks/bombblk.png");
    DEATH_BLK = LoadTexture("blocks/death1.png");
    EXTRABALL_BLK = LoadTexture("blocks/xtrabal.png");
	MGUN_BLK = LoadTexture("blocks/machgun.png");
    WALLOFF_BLK = LoadTexture("blocks/walloff.png");
    RANDOM_BLK = LoadTexture("blocks/redblk.png");// Red block loaded instead of random block selection
    DROP_BLK = LoadTexture("blocks/grnblk.png");// Green block drawn without hit points (text)
    TIMER_BLK = LoadTexture("blocks/clock.png");
	MULTIBALL_BLK = LoadTexture("blocks/multibal.png");
    STICKY_BLK = LoadTexture("blocks/stkyblk.png");
    REVERSE_BLK = LoadTexture("blocks/reverse.png");
    PAD_SHRINK_BLK = LoadTexture("blocks/padshrk.png");
	PAD_EXPAND_BLK = LoadTexture("blocks/padexpn.png");

    COUNTER_BLK[0] = LoadTexture("blocks/cntblk.png");
    COUNTER_BLK[1] = LoadTexture("blocks/cntblk1.png");
    COUNTER_BLK[2] = LoadTexture("blocks/cntblk2.png");
    COUNTER_BLK[3] = LoadTexture("blocks/cntblk3.png");
    COUNTER_BLK[4] = LoadTexture("blocks/cntblk4.png");
    COUNTER_BLK[5] = LoadTexture("blocks/cntblk5.png");
}