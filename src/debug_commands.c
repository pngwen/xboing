#ifdef DEBUG_COMMANDS
#include "debug_commands.h"
#include "demo_blockloader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <sys/select.h>
#include <unistd.h>
#endif

void RunDebugCommand(const char *line, bool playing) {
    char command[32], name[32], rowText[32], colText[32], extra;
    char *end;
    int row, col;
    long value;
    char type = 0;

    if (sscanf(line, " %31s", command) != 1) return;
    if (strcmp(command, "help") == 0 && sscanf(line, " %31s %c", command, &extra) == 1) {
        puts("Debug: spawn <sticky|reverse|speed|shrink|expand> <row> <col> (zero-based); help");
        return;
    }
    if (strcmp(command, "spawn") != 0 ||
        sscanf(line, " %31s %31s %31s %31s %c", command, name, rowText, colText, &extra) != 4) {
        puts("Invalid debug command. Type help for usage.");
        return;
    }
    errno = 0;
    value = strtol(rowText, &end, 10);
    if (errno || *end || end == rowText || value < 0 || value > INT_MAX) {
        puts("Invalid row.");
        return;
    }
    row = (int)value;
    errno = 0;
    value = strtol(colText, &end, 10);
    if (errno || *end || end == colText || value < 0 || value > INT_MAX) {
        puts("Invalid column.");
        return;
    }
    col = (int)value;
    if (strcmp(name, "sticky") == 0) type = 's';
    else if (strcmp(name, "reverse") == 0) type = 'R';
    else if (strcmp(name, "speed") == 0) type = 'B';
    else if (strcmp(name, "shrink") == 0) type = '<';
    else if (strcmp(name, "expand") == 0) type = '>';
    else {
        puts("Unknown power-up. Type help for usage.");
        return;
    }
    if (!playing) puts("Power-ups can only be spawned during play.");
    else if (!spawnDebugBlock(row, col, type)) puts("Invalid or occupied cell; no block spawned.");
    else printf("Spawned %s at row %d, column %d.\n", name, row, col);
}

// Read only characters already available, so terminal input never stalls a frame.
void PollDebugCommands(bool playing) {
    static char line[256];
    static unsigned int length = 0;
    static bool overflow = false;
    int ch;
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) return;
    while (_kbhit()) {
        ch = _getche();
        if (ch == '\r') { putchar('\n'); ch = '\n'; }
#else
    fd_set ready;
    struct timeval timeout;
    for (;;) {
        unsigned char byte;
        FD_ZERO(&ready);
        FD_SET(STDIN_FILENO, &ready);
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        if (select(STDIN_FILENO + 1, &ready, NULL, NULL, &timeout) <= 0 ||
            read(STDIN_FILENO, &byte, 1) != 1) break;
        ch = byte;
#endif
        if (ch == '\n') {
            if (overflow) puts("Debug command too long.");
            else { line[length] = '\0'; RunDebugCommand(line, playing); }
            length = 0;
            overflow = false;
        } else if (ch == '\b' || ch == 127) {
            if (length > 0 && !overflow) length--;
        } else if (ch != '\r') {
            if (length < sizeof(line) - 1 && !overflow) line[length++] = (char)ch;
            else overflow = true;
        }
    }
    fflush(stdout);
}
#endif
