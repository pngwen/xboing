# Open Issue Drafts Not Already Listed in pngwen/xboing

Checked against the open issues in https://github.com/pngwen/xboing/issues. The following are narrower build, setup, and tooling issues rather than duplicates of the existing feature/port issues.

[Repair ad hoc compile test paths]
`tests/compile-test.sh` assumes include and raylib paths that do not match this repository layout. Make the script runnable or document the correct manual command.

[Remove the broken top-level audio.c symlink]
The root `audio.c` symlink points to the missing `audio/SUNaudio.c`. Remove it or replace it with documentation so contributors do not confuse it with `src/audio.c`.

[Handle resource paths independent of launch directory]
Asset paths such as `resource/levels/level01.data` are relative to the current working directory. Document or centralize resource path resolution so the executable behaves consistently when launched from any directory.

[Add a submodule initialization guard]
Provide a setup check or clearer failure when `raylib/` has not been initialized, since the build depends on its headers and source.

[Document generated-file cleanup]
Stale `bin/`, `obj/`, and Makefile outputs can remain after branch or configuration changes. Document the intended clean/rebuild workflow.

[Add basic build verification]
There is no CI or automated compile check for pull requests. Add a minimal build verification step for the raylib port.
