#include <stdio.h>
#include <stdbool.h>

#include "demo_blockloader.h"
#include "demo_gamemodes.h"
#include "demo_ball.h"

const int PLAY_X_OFFSET = 35;
const int PLAY_Y_OFFSET = 60;

const int PLAY_X_PADDING = 40;
const int PLAY_Y_PADDING = 70;

const int BLOCK_WIDTH = 40;
const int BLOCK_HEIGHT = 20;

const int PADDLE_ROWS = 3;
const int PLAY_BORDER_WIDTH = 2;

typedef struct {
    int playWidth;
    int playHeight;
    int colWidth;
    int rowHeight;
} PLAY_AREA;

PLAY_AREA playArea = {0};

char levelName[256];
int timeBonus = 0;
int blocksRemaining = 0;

Texture2D HYPERSPACE_BLK, 
          BULLET_BLK, 
          MAXAMMO_BLK, 
          RED_BLK, 
          GREEN_BLK,
		  BLUE_BLK, 
          TAN_BLK, 
          PURPLE_BLK, 
          YELLOW_BLK,
		  BLACK_BLK, 
          ROAMER_BLK, 
          BOMB_BLK, 
          DEATH_BLK, 
          EXTRABALL_BLK,
		  MGUN_BLK, 
          WALLOFF_BLK, 
          RANDOM_BLK, 
          DROP_BLK, 
          TIMER_BLK,
		  MULTIBALL_BLK, 
          STICKY_BLK, 
          REVERSE_BLK, 
          PAD_SHRINK_BLK,
		  PAD_EXPAND_BLK;

Texture2D COUNTER_BLK[6];

const int COL_MAX = 9;
const int ROW_MAX = 15;

Block game_blocks[15][9];

Vector2 getPlayCorner(CORNERS corner);
bool isBlockTypeInteractive(char ch);
void deactivateBlock(int row, int col);


void initializePlayArea(void) {

    playArea.playWidth = GetScreenWidth() - (PLAY_X_PADDING * 2);
    playArea.playHeight = GetScreenHeight() - (PLAY_Y_PADDING * 2);

    playArea.colWidth = playArea.playWidth / COL_MAX;
    playArea.rowHeight = playArea.playHeight / (ROW_MAX + PADDLE_ROWS);

}


bool loadBlocks(const char* filename) {

    blocksRemaining = 0;

	FILE* fp = fopen(filename, "r");
	if(fp == NULL){
		printf("File '%s' could not be opened.", filename);
		return false;
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
			addBlock(row, column, ch);
			column++;
			row += column / COL_MAX;
			column %= COL_MAX;
		}
		ch = getc(fp);
	}
	fclose(fp);

    return true;
}


void drawBlocks(void){

	/* Loop through all blocks */
    for (int row = 0; row < 15; row++){

        for (int col = 0; col < 9; col++){

            /* If there is a block, draw it */
    		if(!game_blocks[row][col].active) continue;

            DrawTexture(game_blocks[row][col].texture,
                game_blocks[row][col].position.x,
                game_blocks[row][col].position.y,
                WHITE);
        }
    }
}


void drawBorder(void) {
    /* The the red gamne outline */
    Vector2 upperLeft = getPlayCorner(UPPER_LEFT);
    Vector2 lowerRight = getPlayCorner(LOWER_RIGHT);
    DrawRectangleLinesEx((Rectangle){upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y},PLAY_BORDER_WIDTH, RED);
}


void addBlock(int row, int col, char ch){

    game_blocks[row][col].blockOffsetX	= (playArea.colWidth - BLOCK_WIDTH) / 2;
	game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - BLOCK_HEIGHT) / 2;

    game_blocks[row][col].type = ch;

    switch(ch){

        case 'H' :  /* hyperspace block - walls are now gone */
            game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 31) / 2;
			game_blocks[row][col].blockOffsetY = (playArea.rowHeight - 31) / 2;
			game_blocks[row][col].texture = HYPERSPACE_BLK;
		break;

        case 'B' :  /* bullet block - ammo */
			game_blocks[row][col].texture = BULLET_BLK;
		break;

        case 'c' :  /* maximum ammo bullet block  */
            game_blocks[row][col].texture = MAXAMMO_BLK;
        break;

        case 'r' :  /* A red block */
            game_blocks[row][col].texture = RED_BLK;
        break;

        case 'g' :  /* A green block */
            game_blocks[row][col].texture = GREEN_BLK;
        break;

        case 'b' :  /* A blue block */
            game_blocks[row][col].texture = BLUE_BLK;
        break;

        case 't' :  /* A tan block */
            game_blocks[row][col].texture = TAN_BLK;
        break;

        case 'p' :  /* A purple block */
            game_blocks[row][col].texture = PURPLE_BLK;
        break;

        case 'y' :  /* A yellow block */
            game_blocks[row][col].texture = YELLOW_BLK;
        break;

        case 'w' :  /* A solid wall block */
            game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 50) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 30) / 2;
			game_blocks[row][col].texture = BLACK_BLK;
        break;

        case '0' :  /* A counter block - no number */
            game_blocks[row][col].texture = COUNTER_BLK[0];
        break;

        case '1' :  /* A counter block level 1 */
            game_blocks[row][col].texture = COUNTER_BLK[1];
        break;

        case '2' : /* A counter block level 2 */
            game_blocks[row][col].texture = COUNTER_BLK[2];
        break;

        case '3' : /* A counter block level 3 */
            game_blocks[row][col].texture = COUNTER_BLK[3];
        break;

        case '4' : /* A counter block level 4 */
            game_blocks[row][col].texture = COUNTER_BLK[4];
        break;

        case '5' : /* A counter block level 5  - highest */
            game_blocks[row][col].texture = COUNTER_BLK[5];
        break;

        case '+' : /* A roamer block */
            game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 25) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 27) / 2;
			game_blocks[row][col].texture = ROAMER_BLK;
        break;

        case 'X' : /* A bomb */
            game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 30) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 30) / 2;
			game_blocks[row][col].texture = BOMB_BLK;
        break;

        case 'D' : /* A death block */
            game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 30) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 30) / 2;
			game_blocks[row][col].texture = DEATH_BLK;
        break;

        case 'L' : /* An extra ball block */
			game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 30) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 19) / 2;
            game_blocks[row][col].texture = EXTRABALL_BLK;
        break;

        case 'M' : /* A machine gun block */
			game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 35) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 15) / 2;
            game_blocks[row][col].texture = MGUN_BLK;
        break;

        case 'W' : /* A wall off block */
			game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 27) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 23) / 2;
            game_blocks[row][col].texture = WALLOFF_BLK;
        break;

        case '?' : /* A random changing block */
            game_blocks[row][col].texture = RANDOM_BLK;
		break;

        case 'd' : /* A dropping block */
            game_blocks[row][col].texture = DROP_BLK;
        break;

        case 'T' : /* A extra time block */
			game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 21) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 21) / 2;
            game_blocks[row][col].texture = TIMER_BLK;
        break;

        case 'm' : /* A multiple ball block */
            game_blocks[row][col].texture = MULTIBALL_BLK;
        break;

        case 's' : /* A sticky block */
			game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 32) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 27) / 2;
            game_blocks[row][col].texture = STICKY_BLK;
        break;

        case 'R' :  /* reverse block - switch paddle control */
			game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 33) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 16) / 2;
            game_blocks[row][col].texture = REVERSE_BLK;
        break;

        case '<' :  /* shrink paddle block - make paddle smaller */
			game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 40) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 15) / 2;
            game_blocks[row][col].texture = PAD_SHRINK_BLK;
        break;

        case '>' :  /* expand paddle block - make paddle bigger */
            game_blocks[row][col].blockOffsetX	= (playArea.colWidth - 40) / 2;
			game_blocks[row][col].blockOffsetY 	= (playArea.rowHeight - 15) / 2;
            game_blocks[row][col].texture = PAD_EXPAND_BLK;
        break;

        default:
            game_blocks[row][col].blockOffsetX = -1;
        break;
    }

    game_blocks[row][col].position = (Vector2){
        (col * playArea.colWidth) + game_blocks[row][col].blockOffsetX + PLAY_X_OFFSET,
        (row * playArea.rowHeight) + game_blocks[row][col].blockOffsetY + PLAY_Y_OFFSET
    };

    if (game_blocks[row][col].blockOffsetX != -1) {
        game_blocks[row][col].active = true;
        if (ch != 'w') blocksRemaining++;  //solid wall blocks cannot be destroyed and should not count 
    } else {
        game_blocks[row][col].active = false;
    }

}


bool loadBlockTextures(void){

    HYPERSPACE_BLK = LoadTexture("./rayboing/blocks/hypspc.png");
    if (HYPERSPACE_BLK.id == 0) return false;

    BULLET_BLK = LoadTexture("./rayboing/blocks/speed.png");// Green block drawn without bullet texture
    if (BULLET_BLK.id == 0) return false;

    MAXAMMO_BLK = LoadTexture("./rayboing/blocks/lotsammo.png");
    if (MAXAMMO_BLK.id == 0) return false;

    RED_BLK = LoadTexture("./rayboing/blocks/redblk.png");
    if (RED_BLK.id == 0) return false;
    
    GREEN_BLK = LoadTexture("./rayboing/blocks/grnblk.png");
    if (GREEN_BLK.id == 0) return false;
    
	BLUE_BLK = LoadTexture("./rayboing/blocks/blueblk.png");
    if (BLUE_BLK.id == 0) return false;
    
    TAN_BLK = LoadTexture("./rayboing/blocks/tanblk.png");
    if (TAN_BLK.id == 0) return false;
    
    PURPLE_BLK = LoadTexture("./rayboing/blocks/purpblk.png");
    if (PURPLE_BLK.id == 0) return false;
    
    YELLOW_BLK = LoadTexture("./rayboing/blocks/yellblk.png");
    if (YELLOW_BLK.id == 0) return false;
    
	BLACK_BLK = LoadTexture("./rayboing/blocks/blakblk.png");
    if (BLACK_BLK.id == 0) return false;
    
    ROAMER_BLK = LoadTexture("./rayboing/blocks/roamer.png");
    if (ROAMER_BLK.id == 0) return false;
    
    BOMB_BLK = LoadTexture("./rayboing/blocks/bombblk.png");
    if (BOMB_BLK.id == 0) return false;
    
    DEATH_BLK = LoadTexture("./rayboing/blocks/death1.png");
    if (DEATH_BLK.id == 0) return false;
    
    EXTRABALL_BLK = LoadTexture("./rayboing/blocks/xtrabal.png");
    if (EXTRABALL_BLK.id == 0) return false;
    
	MGUN_BLK = LoadTexture("./rayboing/blocks/machgun.png");
    if (MGUN_BLK.id == 0) return false;
    
    WALLOFF_BLK = LoadTexture("./rayboing/blocks/walloff.png");
    if (WALLOFF_BLK.id == 0) return false;
    
    RANDOM_BLK = LoadTexture("./rayboing/blocks/redblk.png");// Red block loaded instead of random block selection
    if (RANDOM_BLK.id == 0) return false;
    
    DROP_BLK = LoadTexture("./rayboing/blocks/grnblk.png");// Green block drawn without hit points (text)
    if (DROP_BLK.id == 0) return false;
    
    TIMER_BLK = LoadTexture("./rayboing/blocks/clock.png");
    if (TIMER_BLK.id == 0) return false;
    
	MULTIBALL_BLK = LoadTexture("./rayboing/blocks/multibal.png");
    if (MULTIBALL_BLK.id == 0) return false;
    
    STICKY_BLK = LoadTexture("./rayboing/blocks/stkyblk.png");
    if (STICKY_BLK.id == 0) return false;
    
    REVERSE_BLK = LoadTexture("./rayboing/blocks/reverse.png");
    if (REVERSE_BLK.id == 0) return false;
    
    PAD_SHRINK_BLK = LoadTexture("./rayboing/blocks/padshrk.png");
    if (PAD_SHRINK_BLK.id == 0) return false;
    
	PAD_EXPAND_BLK = LoadTexture("./rayboing/blocks/padexpn.png");
    if (PAD_EXPAND_BLK.id == 0) return false;
    

    COUNTER_BLK[0] = LoadTexture("./rayboing/blocks/cntblk.png");
    COUNTER_BLK[1] = LoadTexture("./rayboing/blocks/cntblk1.png");
    COUNTER_BLK[2] = LoadTexture("./rayboing/blocks/cntblk2.png");
    COUNTER_BLK[3] = LoadTexture("./rayboing/blocks/cntblk3.png");
    COUNTER_BLK[4] = LoadTexture("./rayboing/blocks/cntblk4.png");
    COUNTER_BLK[5] = LoadTexture("./rayboing/blocks/cntblk5.png");

    for (int i = 0; i < 6; i++) {
        if (COUNTER_BLK[i].id == 0) return false;
    }
    
    return true;
}


void freeBlockTextures(void) {
    UnloadTexture(HYPERSPACE_BLK);
    UnloadTexture(BULLET_BLK);
    UnloadTexture(MAXAMMO_BLK);
    UnloadTexture(RED_BLK);
    UnloadTexture(GREEN_BLK);
    UnloadTexture(BLUE_BLK);
    UnloadTexture(TAN_BLK);
    UnloadTexture(PURPLE_BLK);
    UnloadTexture(YELLOW_BLK);
    UnloadTexture(BLACK_BLK);
    UnloadTexture(ROAMER_BLK);
    UnloadTexture(BOMB_BLK);
    UnloadTexture(DEATH_BLK);
    UnloadTexture(EXTRABALL_BLK);
    UnloadTexture(MGUN_BLK);
    UnloadTexture(WALLOFF_BLK);
    UnloadTexture(RANDOM_BLK);
    UnloadTexture(DROP_BLK);
    UnloadTexture(TIMER_BLK);
    UnloadTexture(MULTIBALL_BLK);
    UnloadTexture(STICKY_BLK);
    UnloadTexture(REVERSE_BLK);
    UnloadTexture(PAD_SHRINK_BLK);
    UnloadTexture(PAD_EXPAND_BLK);

    for (int i = 0; i < 5; i++) {
        UnloadTexture(COUNTER_BLK[i]);
    }
}


Rectangle getBlockCollisionRec(int row, int col) {
    return (Rectangle) {
        game_blocks[row][col].position.x,
        game_blocks[row][col].position.y,
        game_blocks[row][col].texture.width,
        game_blocks[row][col].texture.height
    };
}


int getBlockRowMax(void) {
    return ROW_MAX;
}


int getBlockColMax(void) {
    return COL_MAX;
}


bool isBlockActive(int row, int col) {
    return game_blocks[row][col].active;
}


void activateBlock(int row, int col) {

    switch(game_blocks[row][col].type) {
        
        case 'w': // wall, do nothing
            break;

        case 's': // sticky
            SetBallSticky();
            deactivateBlock(row, col);
            break;
        
        case 'R': //reverse paddle
            ToggleReverse();
            deactivateBlock(row, col);
            break;

        case 'B': // ball speed increased
            IncreaseBallSpeed();
            deactivateBlock(row, col);
            break;

        case '<': //shrink paddle
            ChangePaddleSize(SIZE_DOWN);
            deactivateBlock(row, col);
            break;
        
        case '>': //grow paddle
            ChangePaddleSize(SIZE_UP);
            deactivateBlock(row, col);
            break;

        case 'X': // bomb
            // destroy the surrounding 8 blocks without triggering them
            for (int i = 0; i < 3; i++ ) {
                int rowOffset = row - 1 + i;
                if (rowOffset < 0 || rowOffset >= ROW_MAX) continue;
                for (int j = 0; j < 3; j++) {
                    int colOffset = col - 1 + j;
                    if (colOffset < 0 || colOffset >= COL_MAX) continue;
                    deactivateBlock(rowOffset, colOffset);
                }
            }
            break;

        case '1': // number block 1
            game_blocks[row][col].texture = COUNTER_BLK[0];
            game_blocks[row][col].type = '0';
            break;

        case '2': // number block 2
            game_blocks[row][col].texture = COUNTER_BLK[1];
            game_blocks[row][col].type = '1';
            break;

        case '3': // number block 3
            game_blocks[row][col].texture = COUNTER_BLK[2];
            game_blocks[row][col].type = '2';
            break;

        case '4': // number block 4
            game_blocks[row][col].texture = COUNTER_BLK[3];
            game_blocks[row][col].type = '3';
            break;

        case '5': // number block 5
            game_blocks[row][col].texture = COUNTER_BLK[4];
            game_blocks[row][col].type = '4';
            break;            

        default:
            deactivateBlock(row, col);
            break;
    }

    if (blocksRemaining == 0) {
        SetGameMode(MODE_WIN);
        return;
    }


}


Vector2 getPlayCorner(CORNERS corner) {
    
    switch (corner) {
        
        case UPPER_LEFT:
            return (Vector2){PLAY_X_OFFSET - 1, PLAY_Y_OFFSET - 1};
            break;

        case UPPER_RIGHT:
            return (Vector2){PLAY_X_OFFSET + playArea.playWidth, PLAY_Y_OFFSET - 1};
            break;

        case LOWER_LEFT:
            return (Vector2){PLAY_X_OFFSET - 1, PLAY_Y_OFFSET + playArea.playHeight};
            break;

        case LOWER_RIGHT:
            return (Vector2){playArea.playWidth + 1, PLAY_Y_OFFSET + playArea.playHeight};
            break;
    }

    // never should return this
    return (Vector2){0};

}


Rectangle getPlayWall(WALLS wall) {
    
    switch(wall) {
        case WALL_LEFT:
            return (Rectangle){0, 0, getPlayCorner(LOWER_LEFT).x, GetScreenHeight()};
            break;

        case WALL_RIGHT:
            return (Rectangle){getPlayCorner(UPPER_RIGHT).x, 0, GetScreenWidth() - getPlayCorner(UPPER_RIGHT).x, GetScreenHeight()};
            break;

        case WALL_TOP:
            return (Rectangle){0, 0, GetScreenWidth(), getPlayCorner(UPPER_RIGHT).y};
            break;

        case WALL_BOTTOM:
            return (Rectangle){0, getPlayCorner(LOWER_LEFT).y + getPlayCorner(UPPER_LEFT).y, GetScreenWidth(), GetScreenHeight() - getPlayCorner(LOWER_RIGHT).y};
            break;
    }

    // never should return this
    return (Rectangle){0};
    
}


void drawWalls(void) {
    DrawRectangleRec(getPlayWall(WALL_LEFT),GRAY);
    DrawRectangleRec(getPlayWall(WALL_RIGHT),GRAY);
    DrawRectangleRec(getPlayWall(WALL_TOP),GRAY);
    DrawRectangleRec(getPlayWall(WALL_BOTTOM),GRAY);
}


bool isBlockTypeInteractive(char ch) {

    return !(ch == 'w');

}

void deactivateBlock(int row, int col) {
    if (!game_blocks[row][col].active || !isBlockTypeInteractive(game_blocks[row][col].type)) return;
    game_blocks[row][col].active = false;
    blocksRemaining--;
}


int getBlockCount(void) {
    return blocksRemaining;
}