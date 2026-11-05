#ifndef DEBUG_COMMANDS_H
#define DEBUG_COMMANDS_H

#include <stdbool.h>

void RunDebugCommand(const char *line, bool playing);
void PollDebugCommands(bool playing);

#endif
