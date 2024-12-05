#include <stdbool.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "demo_ball.h"
#include "demo_gamemodes.h"
#include "demo_blockloader.h"

const int INITIAL_BALL_SPEED = 500;  // pixels per second
const int MAX_BALL_IMG_COUNT = 4;
const int GUIDE_LENGTH = 100;

const float bouncVariance = 10.0f;

typedef struct {
    Texture2D img[4];
    int imgIndex;
    Vector2 position;
    Vector2 velocity; // directional velocity    
    int speed;        // pixels per second
    bool sticky;      // it will stick to paddle next collision
    bool attached;    // it is attached to the paddle
    bool spawned;
    Vector2 anchor;
} Ball;

Ball ball = {0};

float releaseAngle = 0;

Vector2 GetSpawnPoint(void);
void AnimateBall(void);
Rectangle GetBallCollisionRec(void);
void DrawGuide(void);


bool InitializeBall(void) {

    for (int i = 0; i < MAX_BALL_IMG_COUNT; i++) {

        char fileName[64];
        snprintf(fileName, sizeof(fileName), "./bitmaps/balls/ball%d.png", i + 1);

        ball.img[i] = LoadTexture(fileName);
        if (ball.img->id == 0)  return false;

    }

    return true;

}


void FreeBall(void) {
    for (int i = 0; i < MAX_BALL_IMG_COUNT; i++) {
        UnloadTexture(ball.img[i]);
    }
}


void ResetBall(void) {

    ball.position = GetSpawnPoint();
    ball.sticky = false;
    ball.attached = false;
    ball.spawned = true;

    ball.speed = 0;
    ball.velocity.x = 0;
    ball.velocity.y = 0;

    releaseAngle = PI / 2.0f;  // points straight up

}


void ReleaseBall(void) {

    if (ball.spawned) {

        ball.spawned = false;
        ball.speed = INITIAL_BALL_SPEED;
        
        ball.velocity.x = cos(releaseAngle) * ball.speed;
        ball.velocity.y = sin(releaseAngle) * ball.speed;
    }

    if (ball.attached) ball.attached = false;

}


Vector2 GetSpawnPoint(void) {
    Vector2 spawn = GetBallSpawnPointOnPaddle();
    return (Vector2){
        spawn.x - ball.img[ball.imgIndex].width / 2,
        spawn.y - ball.img[ball.imgIndex].height
    };
}


void DrawBall(void) {
    AnimateBall();
    if (ball.spawned) DrawGuide();
    DrawTexture(ball.img[ball.imgIndex], ball.position.x, ball.position.y, WHITE);
}


void DrawGuide(void) {
    
    const float centerAngle = PI / 2.0f;
    const float angleSway = PI / 4.0f;

    static float rotateSpeed = PI / 2.0f; // degrees per second

    releaseAngle += rotateSpeed * GetFrameTime();
    if (releaseAngle > centerAngle + angleSway || releaseAngle < centerAngle - angleSway) rotateSpeed *= -1;

    Vector2 startPoint = {
        ball.position.x + ball.img->width / 2,
        ball.position.y + ball.img->height / 2
    };

    Vector2 endPoint = {
        startPoint.x + cos(releaseAngle) * GUIDE_LENGTH,
        startPoint.y - sin(releaseAngle) * GUIDE_LENGTH
    };

    DrawLineV(startPoint, endPoint, YELLOW);

}


void AnimateBall(void) {

    static float elapsedTime = 0.0f;

    elapsedTime += GetFrameTime();

    if (elapsedTime > 0.1f) {
        elapsedTime = 0.0f;
        ball.imgIndex = (ball.imgIndex + 1) % 4;
    }

}


void MoveBall(void) {
    
    // keep spawned ball on paddle center
    if (ball.spawned) {
        ball.position = GetSpawnPoint();
        return;
    } 

    if (ball.attached) {
        ball.position = (Vector2){
            GetPaddlePositionX() - ball.anchor.x,
            GetPaddlePositionY() - ball.anchor.y
        };
        return;
    }

    // move ball
    ball.position = (Vector2){
        ball.position.x + ball.velocity.x * GetFrameTime(),
        ball.position.y - ball.velocity.y * GetFrameTime()
    };

    // check for window boundry collisions

    bool flipx = false;
    bool flipy = false;

    
    if (CheckCollisionRecs(GetBallCollisionRec(), getPlayWall(WALL_BOTTOM))) {
        SetGameMode(MODE_LOSE);
        return;
    }

    if (CheckCollisionRecs(GetBallCollisionRec(), getPlayWall(WALL_LEFT))) {
        flipx = true;
        ball.position.x *= -1;
    } else if (CheckCollisionRecs(GetBallCollisionRec(), getPlayWall(WALL_RIGHT))) {
        flipx = true;
        ball.position.x = (getPlayWall(WALL_RIGHT).x - ball.img[ball.imgIndex].width) * 2 - ball.position.x;
    }
    

    /*
    if (ball.position.y > GetScreenHeight()) {
        SetGameMode(MODE_LOSE);
        return;
    }

    
    if (ball.position.x <= 0) {
        flipx = true;
        ball.position.x *= -1;
    } else if (ball.position.x >= GetScreenWidth() - ball.img[ball.imgIndex].width) {
        flipx = true;
        ball.position.x = (GetScreenWidth() - ball.img[ball.imgIndex].width) * 2 - ball.position.x;
    }
    

    if (ball.position.y <= 0) {
        flipy = true;
        ball.position.y *= -1;
    }
    */

    // check for paddle collisions

    if (CheckCollisionRecs(GetBallCollisionRec(),GetPaddleCollisionRec())) {
        flipy = true;
        ball.position.y = GetPaddlePositionY() - ball.img[ball.imgIndex].height;
        if (ball.sticky) {
            ball.sticky = false;
            ball.attached = true;
            ball.anchor = (Vector2){GetPaddlePositionX() - ball.position.x,  GetPaddlePositionY() - ball.position.y};
        }
    }

    // check for block collisions
    for (int row = 0; row < getBlockRowMax(); row++) {
        for (int col = 0; col < getBlockColMax(); col++) {
            if (!isBlockActive(row,col)) continue;

            Rectangle block = getBlockCollisionRec(row, col);
            if (CheckCollisionRecs(GetBallCollisionRec(), block)) { 
                
                activateBlock(row, col);

                float dX = (ball.position.x + ball.img->width / 2) - (block.x + block.width / 2);
                float dY = (ball.position.y + ball.img->height / 2) - (block.y + block.height / 2);                

                float overlapX = (ball.img->width + block.width) / 2 - fabs(dX);
                float overlapY = (ball.img->height + block.height) / 2 - fabs(dY);

                if (overlapX < overlapY) {
                    flipx = true;
                } else {
                    flipy = true;
                }

            }
        }
    }


    // change directions if needed

    if (flipx) ball.velocity.x *= -1;
    if (flipy) ball.velocity.y *= -1;

    // add variance to the angle on bounce

    if (flipx || flipy) {

        float angle = atan2(ball.velocity.y, ball.velocity.x) + ((rand() % 11) - bouncVariance) * (PI / 180.0f);
        ball.velocity.x = cos(angle) * ball.speed;
        ball.velocity.y = sin(angle) * ball.speed;

    }
        
}


Rectangle GetBallCollisionRec() {
    return (Rectangle) {ball.position.x, ball.position.y, ball.img[ball.imgIndex].width, ball.img[ball.imgIndex].height};
}


void SetBallSticky(void) {
    ball.sticky = true;
}
