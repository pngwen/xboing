# Xboing

XBoing is a blockout type game where you have a paddle which you use to
bounce a ball around the game area blowing up blocks with the ball. You win by
obtaining points for each block destroyed and each level completed. The person
with the highest score wins.

## Building Xboing
Build files and project configurations can be generated with [Premake 5](https://premake.github.io).
To get source code and configure a build enviroment, see the [Getting Started] page.

[Getting Started]: docs/setup/getting-started.md

## Debug power-up commands

In a Premake **Debug** build, launch the game from a terminal and type `help`
in that terminal while the game runs. During play, use
`spawn <sticky|reverse|speed|shrink|expand> <row> <column>` to place an
implemented power-up block in an empty cell. Rows are 0–14 and columns 0–8;
the ball must hit the block to activate it. Existing blocks cannot be replaced.
Commands are unavailable in Release builds. On Windows, input requires an
interactive console (not redirected stdin).

The command parser can be tested without running the game, for example with
`cc -DDEBUG_COMMANDS -Iinclude -Iraylib/src tests/debug_commands_test.c src/debug_commands.c -o debug_commands_test`.
Run the resulting `debug_commands_test` executable to check the assertions.

Manual check: start a level in Debug, spawn each type in an empty cell, and
hit it with the ball to verify its normal effect. Also check occupied and
out-of-range cells, malformed commands, and commands outside play; none should
change the board. Confirm gameplay continues while the terminal is idle and
that Release builds ignore terminal commands.
