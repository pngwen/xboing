Add an integer coordinate type and use it for whole-pixel positions (fixes #10)

Summary
- Adds a new `Coord` type to the raylib port: a cartesian coordinate with two integer components, plus helpers to build one and to convert to and from raylib's float `Vector2`.
- Converts the positions in the active `src/` tree that are already whole pixels over to it: block origins, the play area corners, and the ball spawn point.

Motivation
- Many positions in xboing are carried around as two loose ints, which makes signatures long and easy to mix up. The issue proposes tying the components together in a coordinate struct so a location can travel as a single argument.
- raylib's `Vector2` stores floats, so it cannot be reused directly for positions that are whole pixels. `Coord` fills that gap.

What changed (minimal, focused edits)
- `include/coord.h` (new): defines `Coord` (`int x`, `int y`) and declares `MakeCoord()`, `CoordToVector2()` and `Vector2ToCoord()`.
- `src/coord.c` (new): implements the above.
- `include/demo_blockloader.h`: `Block.position` changed from `Vector2` to `Coord`; added the `getPlayCorner()` prototype, which previously only existed as a local declaration inside `demo_blockloader.c`.
- `src/demo_blockloader.c`: `getPlayCorner()` now returns a `Coord`; block positions are built with `MakeCoord()`.
- `include/paddle.h` / `src/paddle.c`: `GetBallSpawnPointOnPaddle()` now returns a `Coord`.
- `src/demo_ball.c`: `GetSpawnPoint()` now returns a `Coord`, and the two places that copy it into the ball's position use `CoordToVector2()`.

Behavior details
- No behavioural change. Every value that was converted was already produced by integer arithmetic (`paddlePosition` and `GetPaddlePositionY()` are ints, and the block offsets are ints), so storing them as ints and converting back to float is exact.
- The ball itself keeps `Vector2`, because it travels at fractional speeds and needs sub-pixel precision. `CoordToVector2()` is the bridge between the two, used where a whole-pixel coordinate has to be handed to the ball's float position.
- `Vector2ToCoord()` truncates towards zero. It is provided for completeness and is not called by the current code.

How to test locally
1. Check out this branch and build the project with your normal workflow.
2. The build should complete with no errors or warnings.
3. Run the game and play a level. Blocks should appear in exactly the same positions as before, the play area border should be drawn in the same place, and the ball should still sit centred on the paddle when it is released.
4. Move the paddle to the far left and far right and release the ball again: the spawn point should still track the paddle centre.

Notes
- The change is intentionally limited to the positions that are genuinely whole pixels, so that it stays easy to review. The type is now available for the remaining integer positions to be converted incrementally as and when each is touched.
- `include/paddle.h` previously used `Vector2` without including `raylib.h`, relying on its includers to have done so. Including `coord.h` there resolves that.
- Built and verified with MSVC 14.29 (Visual Studio 2019) against raylib 5.5 in both Debug and Release: no errors, no warnings, and `src/coord.c` is additionally clean at `/W4 /WX`. The resulting executable launches and runs.
- This branch is stacked on `fix/issue-159-bounce-variance-typo`, which fixes a compile error on `main`. See PR #202.

Fixes: pngwen/xboing#10

Branch: `issue-10-coordinate-struct` (pushed to `cshaff11/xboing`)
