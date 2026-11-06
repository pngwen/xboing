#include <stdio.h>
#include <stdbool.h>

#include <raylib.h>

#include "intro.h"

#define INTRO_TEXTURES "resource/textures/presents/"
#define STAR_TEXTURES "resource/textures/stars/"
#define BACKGROUNDS "resource/textures/bgrnds/"

#define LETTER_COUNT 6
#define STAR_FRAME_COUNT 11
#define LETTER_SPACING 10

static const int SCREEN_WIDTH = 575;
static const int SCREEN_HEIGHT = 720;

static const char *letterFiles[LETTER_COUNT] = {
    "titleX.png",
    "titleB.png",
    "titleO.png",
    "titleI.png",
    "titleN.png",
    "titleG.png"
};

static Texture2D introPlanet;
static Texture2D introFlag;
static Texture2D introJustin;
static Texture2D introKibell;
static Texture2D introPresents;
static Texture2D introLetters[LETTER_COUNT];
static Texture2D introStars[STAR_FRAME_COUNT];
static Texture2D backgroundSpace;

// Intro timeline, all values in seconds.
static const float PRESENTS_HOLD_TIME = 0.9f;
static const float PRESENTS_FADE_TIME = 0.3f;
static const float LETTER_REVEAL_DELAY = 0.12f;
static const float PROMPT_FADE_TIME = 0.5f;
static const float SPARKLE_FRAME_TIME = 0.09f;
static const float SPARKLE_PAUSE_TIME = 0.5f;

typedef struct {
    bool active;
    int frame;
    float timer;
    Vector2 position;
} Sparkle;

static bool LoadIntroTextures(void) {
    char path[64];

    backgroundSpace = LoadTexture(BACKGROUNDS "space.png");
    if (backgroundSpace.id == 0) return false;

    introPresents = LoadTexture(INTRO_TEXTURES "presents.png");
    if (introPresents.id == 0) return false;

    introPlanet = LoadTexture(INTRO_TEXTURES "earth.png");
    if (introPlanet.id == 0) return false;

    introFlag = LoadTexture(INTRO_TEXTURES "flag.png");
    if (introFlag.id == 0) return false;

    introJustin = LoadTexture(INTRO_TEXTURES "justin.png");
    if (introJustin.id == 0) return false;

    introKibell = LoadTexture(INTRO_TEXTURES "kibell.png");
    if (introKibell.id == 0) return false;

    for (int i = 0; i < LETTER_COUNT; i++) {
        snprintf(path, sizeof(path), INTRO_TEXTURES "%s", letterFiles[i]);
        introLetters[i] = LoadTexture(path);
        if (introLetters[i].id == 0) return false;
    }

    for (int i = 0; i < STAR_FRAME_COUNT; i++) {
        snprintf(path, sizeof(path), STAR_TEXTURES "star%d.png", i + 1);
        introStars[i] = LoadTexture(path);
        if (introStars[i].id == 0) return false;
    }

    return true;
}

static void UnloadIntroTextures(void) {
    UnloadTexture(backgroundSpace);
    UnloadTexture(introPresents);
    UnloadTexture(introPlanet);
    UnloadTexture(introFlag);
    UnloadTexture(introJustin);
    UnloadTexture(introKibell);

    for (int i = 0; i < LETTER_COUNT; i++) {
        UnloadTexture(introLetters[i]);
    }

    for (int i = 0; i < STAR_FRAME_COUNT; i++) {
        UnloadTexture(introStars[i]);
    }
}

static void DrawSpaceBackground(void) {
    for (int y = 0; y < SCREEN_HEIGHT; y += backgroundSpace.height) {
        for (int x = 0; x < SCREEN_WIDTH; x += backgroundSpace.width) {
            DrawTexture(backgroundSpace, x, y, WHITE);
        }
    }
}

static Rectangle GetTitleRectangle(void) {
    int width = LETTER_SPACING * (LETTER_COUNT - 1);
    int height = 0;

    for (int i = 0; i < LETTER_COUNT; i++) {
        width += introLetters[i].width;
        if (introLetters[i].height > height) height = introLetters[i].height;
    }

    return (Rectangle){
        (SCREEN_WIDTH - width) / 2.0f,
        (SCREEN_HEIGHT / 2.0f) - 100.0f,
        (float)width,
        (float)height
    };
}

static float Clamp01(float value) {
    if (value < 0.0f) return 0.0f;
    if (value > 1.0f) return 1.0f;
    return value;
}

static int GetRevealedLetterCount(float elapsed) {
    float revealTime = elapsed - PRESENTS_HOLD_TIME;
    if (revealTime < 0.0f) return 0;

    int count = (int)(revealTime / LETTER_REVEAL_DELAY) + 1;
    return (count > LETTER_COUNT) ? LETTER_COUNT : count;
}

static float GetPromptAlpha(float elapsed) {
    float start = PRESENTS_HOLD_TIME + LETTER_COUNT * LETTER_REVEAL_DELAY + 0.2f;
    return Clamp01((elapsed - start) / PROMPT_FADE_TIME);
}

static void UpdateSparkle(Sparkle *sparkle, float deltaTime, float elapsed, Rectangle title) {
    // Do not sparkle until the title has finished appearing.
    float start = PRESENTS_HOLD_TIME + LETTER_COUNT * LETTER_REVEAL_DELAY + 0.3f;
    if (elapsed < start) return;

    sparkle->timer += deltaTime;

    if (!sparkle->active) {
        if (sparkle->timer >= SPARKLE_PAUSE_TIME) {
            int maxX = (int)title.width - introStars[0].width;
            int maxY = (int)title.height - introStars[0].height;
            if (maxX < 0) maxX = 0;
            if (maxY < 0) maxY = 0;

            sparkle->timer = 0.0f;
            sparkle->frame = 0;
            sparkle->active = true;
            sparkle->position = (Vector2){
                title.x + GetRandomValue(0, maxX),
                title.y + GetRandomValue(0, maxY)
            };
        }
        return;
    }

    if (sparkle->timer >= SPARKLE_FRAME_TIME) {
        sparkle->timer = 0.0f;
        sparkle->frame++;

        if (sparkle->frame >= STAR_FRAME_COUNT) {
            sparkle->active = false;
            sparkle->timer = 0.0f;
        }
    }
}

void ShowIntroScreen(void) {
    EnableCursor();
    printf("Intro started\n");
    fflush(stdout);

    if (!LoadIntroTextures()) {
        fprintf(stderr, "Failed to load intro textures\n");
        UnloadIntroTextures();
        return;
    }

    float elapsed = 0.0f;
    Sparkle sparkle = {0};
    Rectangle title = GetTitleRectangle();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        elapsed += deltaTime;

        UpdateSparkle(&sparkle, deltaTime, elapsed, title);
        int revealedLetters = GetRevealedLetterCount(elapsed);

        BeginDrawing();
            ClearBackground(BLACK);
            DrawSpaceBackground();

            DrawTexture(introPlanet, 100, 100, WHITE);
            DrawTexture(introFlag, (SCREEN_WIDTH - introFlag.width) / 2, 20, WHITE);
            DrawTexture(introJustin, (SCREEN_WIDTH - introJustin.width) / 2,
                SCREEN_HEIGHT - introKibell.height - introJustin.height - 30, WHITE);
            DrawTexture(introKibell, (SCREEN_WIDTH - introKibell.width) / 2,
                SCREEN_HEIGHT - introKibell.height - 20, WHITE);

            float presentsAlpha = 1.0f - Clamp01((elapsed - PRESENTS_HOLD_TIME) / PRESENTS_FADE_TIME);
            if (presentsAlpha > 0.0f) {
                int presentsX = (SCREEN_WIDTH - introPresents.width) / 2;
                int presentsY = (int)(title.y + (title.height - introPresents.height) / 2.0f);
                DrawTexture(introPresents, presentsX, presentsY, Fade(WHITE, presentsAlpha));
            }

            float x = title.x;
            for (int i = 0; i < revealedLetters; i++) {
                DrawTexture(introLetters[i], (int)x, (int)title.y, WHITE);
                x += introLetters[i].width + LETTER_SPACING;
            }

            if (sparkle.active) {
                DrawTexture(introStars[sparkle.frame],
                    (int)sparkle.position.x, (int)sparkle.position.y, WHITE);
            }

            float promptAlpha = GetPromptAlpha(elapsed);
            if (promptAlpha > 0.0f) {
                const char *prompt = "Press ENTER or SPACE to Start";
                int promptSize = 20;
                int promptWidth = MeasureText(prompt, promptSize);
                int promptY = (int)(title.y + title.height) + 40;
                DrawText(prompt, (SCREEN_WIDTH - promptWidth) / 2, promptY,
                    promptSize, Fade(LIGHTGRAY, promptAlpha));
            }
        EndDrawing();

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE)) {
            printf("Intro ended - start requested\n");
            fflush(stdout);
            break;
        }
    }

    UnloadIntroTextures();
}
