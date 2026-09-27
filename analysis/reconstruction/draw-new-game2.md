# DrawNewGame2 reconstruction

`reconstructed/src/draw_new_game2.c` is a source-level reconstruction of
`DrawNewGame2` (`0010:9556-99f3`, 1,181 bytes). It models the player-type and
race fields with typed records, preserves editable versus read-only text
selection, dead-player color handling, row spacing, and diamond placement, and
keeps localization and drawing behind callbacks.

The source probe covers DC lifecycle, editable and read-only rows, the special
race-6 label, text/value dispatch, dead-player handling, and diamond rendering.
Host strict C89 compilation and the focused probe pass. The source also
selects the bold font, brackets drawing in the original opaque background
mode, emits multi-digit row labels, and exposes localized composite formatting
for race and file-backed player names. The new E1 packed-table adapter
consumes the original one-byte player kind/race table and thirteen-byte
race-file table with the recovered masks, shift, and stride; a 13-check
host/native packet passes even when the typed shadow records disagree. E2 now
covers the localized empty, race, custom-file, game-file, and type-3 AI
group/level formatting paths with a 15-check host/native packet. E3 now traces
the complete page GDI sequence, geometry, DC ownership, colors, and
read-only/dead rows with a 16-check host/native packet. The original wizard
Step 2 replay and raw table operations agree, so the procedure is promoted to
Phase 2 `6/6`; see the three focused compatibility notes for the retained
packets.
