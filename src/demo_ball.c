#include <stdbool.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "demo_ball.h"
#include "demo_gamemodes.h"
#include "demo_blockloader.h"
#include "audio.h"

#define BALL_TEXTURES "resource/textures/balls/"

const int INITIAL_BALL_SPEED = 400;  // pixels per second
const int MAX_BALL_IMG_COUNT = 4;
const int GUIDE_LENGTH = 100;

const float bounceVariance = 10.0f;

typedef struct {
    Texture2D img[4];
    int imgIndex;
    Vector2 position;
    Vector2 oldPosition;
    Vector2 velocity; // directional velocity
    int speed;        // pixels per second
    bool sticky;      // it will stick to paddle next collision
    bool attached;    // it is attached to the paddle
    bool spawned;
    bool boostedThisFrame;

    Vector2 anchor;
} Ball;

Ball ball = {0};

float releaseAngle = 0;

Vector2 GetSpawnPoint(void);
void AnimateBall(void);
Rectangle GetBallCollisionRec(void);
void DrawGuide(void);
bool FindFirstBlockHit(int *row, int *col, bool *flipX, bool *flipY);


bool InitializeBall(void) {

    for (int i = 0; i < MAX_BALL_IMG_COUNT; i++) {

        char fileName[64];
        snprintf(fileName, sizeof(fileName), BALL_TEXTURES "ball%d.png", i + 1);

        ball.img[i] = LoadTexture(fileName);
        if (ball.img[i].id == 0)  return false;

    }

    return true;

}

bool BallExists(void) {
    return ball.spawned || !ball.attached;
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
        startSound(SND_BALLSHOT);   
    }

    if (ball.attached){
        ball.attached = false;
        startSound(SND_BALLSHOT);   
    }

    // Start the countdown timer on the first ball release
    setTimerActive(true);
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

    const int ROTATING_LEFT = 1;
    const int ROTATING_RIGHT = -1;

    const float centerAngle = PI / 2.0f;  // 90 degrees straight up
    const float angleSway = PI / 4.0f;    // +/- 45 degrees

    static float rotateSpeed = PI / 2.0f; // degrees per second
    static int direction = 1;

    // rotate the guide between 45 and 135 degrees
    releaseAngle += rotateSpeed * GetFrameTime();
    if (releaseAngle > centerAngle + angleSway && direction == ROTATING_LEFT) {
        direction = ROTATING_RIGHT;
        rotateSpeed *= -1;
    } else if (releaseAngle < centerAngle - angleSway && direction == ROTATING_RIGHT) {
        direction = ROTATING_LEFT;
        rotateSpeed *= -1;
    }

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


// Sweep the ball's collision rectangle from its previous position to its
// current one and report the first block it reaches. Each block is tested
// against the ball rectangle expanded by the ball's size (a Minkowski sum), so
// the chosen block and the bounce axis do not depend on row/column scan order.
bool FindFirstBlockHit(int *row, int *col, bool *flipX, bool *flipY) {

    Rectangle ballRec = GetBallCollisionRec();
    Vector2 prevPos = {ball.oldPosition.x, ball.oldPosition.y};
    Vector2 delta = {ballRec.x - prevPos.x, ballRec.y - prevPos.y};

    int bestRow = -1;
    int bestCol = -1;
    float bestTime = 2.0f; // later than any hit within this frame
    bool bestFlipX = false;
    bool bestFlipY = false;

    for (int blockRow = 0; blockRow < getBlockRowMax(); blockRow++) {
        for (int blockCol = 0; blockCol < getBlockColMax(); blockCol++) {
            if (!isBlockActive(blockRow, blockCol)) continue;

            Rectangle block = getBlockCollisionRec(blockRow, blockCol);
            if (!CheckCollisionRecs(ballRec, block)) continue;

            float minX = block.x - ballRec.width;
            float maxX = block.x + block.width;
            float minY = block.y - ballRec.height;
            float maxY = block.y + block.height;

            float enterX = -1e30f, exitX = 1e30f;
            float enterY = -1e30f, exitY = 1e30f;

            if (delta.x != 0.0f) {
                float t1 = (minX - prevPos.x) / delta.x;
                float t2 = (maxX - prevPos.x) / delta.x;
                enterX = fminf(t1, t2);
                exitX = fmaxf(t1, t2);
            }

            if (delta.y != 0.0f) {
                float t1 = (minY - prevPos.y) / delta.y;
                float t2 = (maxY - prevPos.y) / delta.y;
                enterY = fminf(t1, t2);
                exitY = fmaxf(t1, t2);
            }

            float enter = fmaxf(enterX, enterY);
            float exit = fminf(exitX, exitY);
            if (enter > exit) continue;

            bool hitFlipX;
            bool hitFlipY;

            if (enter < 0.0f) {
                // The ball was already overlapping before this move; fall back
                // to the shallowest axis.
                enter = 0.0f;
                float dX = (ballRec.x + ballRec.width / 2) - (block.x + block.width / 2);
                float dY = (ballRec.y + ballRec.height / 2) - (block.y + block.height / 2);
                float overlapX = (ballRec.width + block.width) / 2 - fabsf(dX);
                float overlapY = (ballRec.height + block.height) / 2 - fabsf(dY);
                hitFlipX = (overlapX < overlapY);
                hitFlipY = !hitFlipX;
            } else {
                // The slab crossed last is the face the ball hit.
                hitFlipY = (enterY >= enterX);
                hitFlipX = !hitFlipY;
            }

            if (enter < bestTime) {
                bestTime = enter;
                bestRow = blockRow;
                bestCol = blockCol;
                bestFlipX = hitFlipX;
                bestFlipY = hitFlipY;
            }
        }
    }

    if (bestRow < 0) return false;

    *row = bestRow;
    *col = bestCol;
    *flipX = bestFlipX;
    *flipY = bestFlipY;
    return true;
}


void MoveBall(void) {

    ball.oldPosition = ball.position;
    bool stepBack = false;
    ball.boostedThisFrame = false; //keep track if ball boosted

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

        // check is the ball is hanging off the edge of the paddle
        // when the paddle is moved against the wall
        int boundary = getPlayWall(WALL_LEFT).width;
        if (ball.position.x < boundary) {
            ball.position.x = boundary;
            ball.anchor.x = GetPaddlePositionX() - ball.position.x;
        }

        boundary = getPlayWall(WALL_RIGHT).x - ball.img->width;
        if (ball.position.x > boundary) {
            ball.position.x = boundary;
            ball.anchor.x = GetPaddlePositionX() - ball.position.x;
        }

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
    int sideBounce = 0; // -1 = left, +1 = right, 0 = none

    if (CheckCollisionRecs(GetBallCollisionRec(), getPlayWall(WALL_BOTTOM))) {
        ball.position.y = GetScreenHeight(); // cheesy way to hide ball after loss
        startSound(SND_BALLLOST);  
		
        LoseLife();

		if (GetLivesRemaining() > 0) {
            ResetBall();
			SetGameMode(MODE_INITGAME);
        }
        else {
            SetGameMode(MODE_LOSE);
        }
        
        return;
    } else if (CheckCollisionRecs(GetBallCollisionRec(), getPlayWall(WALL_TOP))) {
        startSound(SND_BOING);       // test to play sound bouncing off wall
        stepBack = true;
        flipy = true;
    }

    if (CheckCollisionRecs(GetBallCollisionRec(), getPlayWall(WALL_LEFT))) {
        startSound(SND_BOING);                                                                                   // test to play sound bouncing off wall
        stepBack = true;
        flipx = true;
        sideBounce = -1;
    } else if (CheckCollisionRecs(GetBallCollisionRec(), getPlayWall(WALL_RIGHT))) {
        startSound(SND_BOING);                                                                                  // test to play sound bouncing off wall
        stepBack = true;
        flipx = true;
        sideBounce = 1;
    }

    // check for paddle collisions

    if (CheckCollisionRecs(GetBallCollisionRec(),GetPaddleCollisionRec())) {
        flipy = true;
        ball.position.y = GetPaddlePositionY() - ball.img[ball.imgIndex].height;
        startSound(SND_PADDLE);
        if (ball.sticky) {
            ball.sticky = false;
            ball.attached = true;
            ball.anchor = (Vector2){GetPaddlePositionX() - ball.position.x,  GetPaddlePositionY() - ball.position.y};
        }
    }

    // check for block collisions
    // Resolve at most one block per frame, and pick the block the ball reaches
    // first along its path. Destroying every overlapping block in a single
    // frame while only flipping the velocity once let the ball "surf" the seam
    // between two rows and clear them almost instantly (issue #138).
    int blockRow;
    int blockCol;
    bool blockFlipX;
    bool blockFlipY;
    if (FindFirstBlockHit(&blockRow, &blockCol, &blockFlipX, &blockFlipY)) {
        stepBack = true;
        activateBlock(blockRow, blockCol);
        if (blockFlipX) flipx = true;
        if (blockFlipY) flipy = true;
    }

    // change directions if needed
    if (stepBack) ball.position = ball.oldPosition;
    if (flipx) ball.velocity.x *= -1;
    if (flipy) ball.velocity.y *= -1;

    // Track repeated side bounces to detect 'limbo' (bouncing purely left-right)
    // and apply stronger nudges if necessary.
    static int consecutiveSideBounces = 0;
    static int lastSide = 0;

    if (flipx && !flipy && sideBounce != 0) {
        if (sideBounce == lastSide) consecutiveSideBounces++; else consecutiveSideBounces = 1;
        lastSide = sideBounce;
    } else {
        // any vertical component or corner bounce resets the counter
        consecutiveSideBounces = 0;
        lastSide = 0;
    }

    // add variance to the angle on bounce and avoid perfectly horizontal/vertical
    if (flipx || flipy) {

        // symmetric jitter in degrees
        float jitter = ((float)rand() / (float)RAND_MAX) * 2.0f * bounceVariance - bounceVariance;
        float angle = atan2f(ball.velocity.y, ball.velocity.x) + jitter * (PI / 180.0f);

        float speed = (float)ball.speed;
        ball.velocity.x = cosf(angle) * speed;
        ball.velocity.y = sinf(angle) * speed;

        // ensure a minimum vertical component so the ball cannot get stuck bouncing
        // purely between left/right walls. Use a small fraction of overall speed.
        const float MIN_VERT_RATIO = 0.15f; // at least 15% of speed vertically
        float minVert = speed * MIN_VERT_RATIO;
        if (fabsf(ball.velocity.y) < minVert && speed > minVert) {
            float signy = (ball.velocity.y >= 0.0f) ? 1.0f : -1.0f;
            ball.velocity.y = signy * minVert;

            // recompute horizontal component preserving total speed
            float vxSign = (ball.velocity.x >= 0.0f) ? 1.0f : -1.0f;
            float vxMag = sqrtf(fmaxf(0.0f, speed*speed - ball.velocity.y * ball.velocity.y));
            ball.velocity.x = vxSign * vxMag;
        }

        // If we've bounced side-to-side many times in a row, apply a stronger nudge
        // to the vertical component to break out of the limbo.
        if (consecutiveSideBounces >= 3) {
            const float NUDGE_RATIO = 0.35f; // stronger vertical push
            float nudge = speed * NUDGE_RATIO;
            float signy = (ball.velocity.y >= 0.0f) ? 1.0f : -1.0f;
            ball.velocity.y = signy * fmaxf(fabsf(ball.velocity.y), nudge);

            // recompute horizontal component preserving total speed
            float vxSign = (ball.velocity.x >= 0.0f) ? 1.0f : -1.0f;
            float vxMag = sqrtf(fmaxf(0.0f, speed*speed - ball.velocity.y * ball.velocity.y));
            ball.velocity.x = vxSign * vxMag;
            consecutiveSideBounces = 0; // reset after nudging
        }

    }

}


Rectangle GetBallCollisionRec() {
	const int padding = 2; // Adjust padding as needed, make collision box bigger
    return (Rectangle) {ball.position.x, ball.position.y, ball.img[ball.imgIndex].width + padding, ball.img[ball.imgIndex].height + padding};
}


void SetBallSticky(void) {
    ball.sticky = true;
}


void IncreaseBallSpeed(void) {
    // stops multiple boosts in a single frame
    if (ball.boostedThisFrame)
    {
        return;
    }

    ball.boostedThisFrame = true;

    if (ball.speed < 1000) {
		ball.speed = (int)(ball.speed * 1.25f); //speed cap, hopefully not as fast anymore?
    }
    
}

