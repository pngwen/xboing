Fix compile error in demo_ball.c: bouncVariance typo (fixes #159)

Summary
- Corrects the misspelled identifier `bouncVariance` in `src/demo_ball.c:321`, which currently stops the project from compiling at all.

Motivation
- `main` does not build. The compiler reports:
  `src/demo_ball.c(321): error C2065: 'bouncVariance': undeclared identifier`
- The constant is declared as `bounceVariance` at `src/demo_ball.c:18`, but line 321 used the misspelled name twice. The typo was introduced by commit `a4f721e` ("Prevent ball limbo: add jitter + vertical nudge"), which landed via PR #143.
- Since the failure is in a translation unit that every build includes, no other change can be compiled, run, or reviewed until this is corrected. It is therefore worth landing as a standalone fix rather than being folded into an unrelated branch.

What changed (minimal, focused edits)
- `src/demo_ball.c:321`: replaced both uses of `bouncVariance` with the correctly declared `bounceVariance`.

Behavior details
- No behavioural change. `bounceVariance` (10.0f) is the value that was always intended, so the jitter calculation is unchanged once the name is spelled correctly.

How to test locally
1. Check out this branch and build the project with your normal workflow.
2. The build should now complete with no errors.
3. Run the game and bounce the ball off a wall. The bounce angle should still jitter symmetrically around the incoming angle by up to +/-10 degrees, and the ball should not become stuck bouncing purely between the left and right walls.

Notes
- The change is intentionally minimal so that it can be merged and reviewed independently of any other work.
- Verified with MSVC 14.29 (Visual Studio 2019) against raylib 5.5: the project now compiles and links with no errors or warnings, and the resulting executable launches and runs.

Fixes: pngwen/xboing#159

Branch: `fix/issue-159-bounce-variance-typo` (pushed to `cshaff11/xboing`)
