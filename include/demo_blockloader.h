#ifndef _DEMO_BLOCKLOADER_H_
#define _DEMO_BLOCKLOADER_H_

#include <raylib.h>
#include <stdbool.h>

typedef struct Block {
	int blockOffsetX;
	int blockOffsetY;
	Vector2 position;
    Texture2D texture;
	char type;
	bool active;
} Block;

typedef enum {
	UPPER_LEFT,
	UPPER_RIGHT,
	LOWER_LEFT,
	LOWER_RIGHT
} CORNERS;

typedef enum {
	WALL_LEFT,
	WALL_RIGHT,
	WALL_TOP,
	WALL_BOTTOM
} WALLS;

bool loadBlocks(const char* filename);
void drawBlocks(void);
void drawBorder(void);
bool loadBlockTextures(void);
void freeBlockTextures(void);
void addBlock(int row, int col, char ch);
void initializePlayArea(void);
Rectangle getBlockCollisionRec(int row, int col);
int getBlockRowMax(void);
int getBlockColMax(void);
bool isBlockActive(int row, int col);
void activateBlock(int row, int col);
Rectangle getPlayWall(WALLS wall);
void drawWalls(void);
int getBlockCount(void);

#endif // _DEMO_BLOCKLOADER_H