Add spike on top when bullet hits (fixes #64)

Summary
- Implements the classic XBoing behaviour where a spike appears at the top of the play area when a bullet reaches the top edge. If the ball later collides with that spike at approximately the same x position while the spike is active, the ball is destroyed.

Motivation
- Restores behaviour described in pngwen/xboing#64 that existed in the original XBoing and is expected by players.

What changed (minimal, focused edits)
- `archive/include/gun.h`: Added prototype `int IsSpikeAt(int x);` so other modules can query active spikes.
- `archive/src/gun.c`: Implemented `IsSpikeAt(int x)` which checks existing `tinks[]` for active spikes. No change to the tink/spike rendering or lifetime logic.
- `archive/src/ball.c`: Included `include/gun.h` and updated the top-wall handling so the ball is set to `BALL_DIE` (destroyed) when it hits the top and `IsSpikeAt(ballx)` returns true; otherwise the ball bounces as before.

Behavior details
- Spike rendering & lifetime: unchanged (uses existing `tinks`/`TINK_DELAY`).
- Spike hit tolerance: a conservative width was chosen (current tolerance: `TINK_WC + BULLET_WC + BALL_WC`). This can be adjusted later if maintainers want a narrower or wider hit region.
- Sound: reuses existing sounds (the ball death uses the `ballshot` sound where applicable).

How to test locally
1. Ensure dependencies are installed (notably `raylib`) and build the project with your normal workflow.
2. Run the game and reproduce these steps:
   - Shoot a bullet so it reaches the top of the play area without hitting blocks or the ball; a spike (tink) should appear where the bullet hit.
   - Let the ball move and contact the top at that x position while the spike is active: the ball should be destroyed instead of bouncing.
3. Optionally, tune `TINK_DELAY` or the hit tolerance in `IsSpikeAt` to modify spike lifetime or sensitivity.

Patch file
- I created a patch for this change at `/workspaces/xboing/patches/0001-Add-spike-on-top-collision-IsSpikeAt-and-ball-death-.patch` in the workspace for review.

Notes
- Changes are intentionally minimal and should be straightforward to review. If maintainers request adjustments (hit range, separate sprite, different sound), I can update the branch `fix/spike-top-64` with follow-up commits.

Fixes: pngwen/xboing#64

Branch: `fix/spike-top-64` (pushed to `liznava/xboing`)

