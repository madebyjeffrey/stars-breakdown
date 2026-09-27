# DrawNewGame2 packed player/type tables — E1

This checkpoint closes the packed-table boundary for `DrawNewGame2`. The
renderer can now consume the original one-byte `vrgplrTypeNew` table directly
and resolves saved race filenames from the original 13-byte parallel table,
with an explicit stride and a fallback to the typed shadow record used by the
host adapter.

The strict host C89 probe reports
`DRAW_NEW_GAME2_PACKED_E1_CHECKS=13 FAILURES=0 RESULT=PASS`. It exercises the
empty row, built-in race, custom race with a packed filename, empty packed
filename fallback, and the game-file-mode filename branch. The typed player
records intentionally contain conflicting values, so the passing result
demonstrates that the packed type and filename tables—not those shadow
values—drive the acceptance boundary.

The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152dng2pack/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same 13-check result. The process exited
with status 0 and the post-run window inventory showed no Program Error
dialog. The original decompilation identifies the same `& 3`, `>> 2`, and
`race * 0xd` table operations; the prior original Win98 Step 2 capture
provides the sixteen-row/race-diamond integration boundary. E2 localized
formatting and E3 full GDI/resource integration remain open.
