#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "debug_commands.h"

static int calls;
static int lastRow, lastCol;
static char lastType;

bool spawnDebugBlock(int row, int col, char type) {
    calls++;
    lastRow = row;
    lastCol = col;
    lastType = type;
    return row < 15 && col < 9;
}

int main(void) {
    const char *commands[] = {"sticky", "reverse", "speed", "shrink", "expand"};
    const char types[] = {'s', 'R', 'B', '<', '>'};
    char line[80];
    for (int i = 0; i < 5; i++) {
        snprintf(line, sizeof(line), "spawn %s 2 3", commands[i]);
        RunDebugCommand(line, true);
        assert(calls == i + 1 && lastRow == 2 && lastCol == 3 && lastType == types[i]);
    }
    RunDebugCommand("spawn sticky 2 3", false);
    RunDebugCommand("spawn bogus 2 3", true);
    RunDebugCommand("spawn sticky -1 3", true);
    RunDebugCommand("spawn sticky 9999999999999999999999 3", true);
    RunDebugCommand("spawn sticky 2 3 extra", true);
    RunDebugCommand("spawn sticky 2x 3", true);
    RunDebugCommand("help", true);
    assert(calls == 5);
    RunDebugCommand("spawn sticky 99 3", true);
    assert(calls == 6);
    return 0;
}
