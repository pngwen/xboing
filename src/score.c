#include "score.h"

static int currentScore = 0;

void AddToScore(int points) {
    if (points > 0) {
        currentScore += points;
    }
}

void ResetScore(void) {
    currentScore = 0;
}

int GetScore(void) {
    return currentScore;
}
