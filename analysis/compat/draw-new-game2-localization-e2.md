# DrawNewGame2 localized formatting — E2

This checkpoint closes the localized-formatting boundary for `DrawNewGame2`.
The typed source now represents the original type-3 AI row path: the packed
kind byte selects separate group and level labels, and both are passed to the
localized `0x312` formatter in the original order. The probe also covers the
empty, built-in race, special-race, generic, custom-name, custom-file, and
game-file-mode resource paths (`0x30c`, `0x20b`, `0x20d`, `0x310`, `0x311`,
`0x312`, and `0x0aee`).

The strict host C89 probe reports
`DRAW_NEW_GAME2_LOCALIZATION_E2_CHECKS=15 FAILURES=0 RESULT=PASS`. It checks
the format IDs, string IDs, group/level indices, argument ordering, and the
game-file-mode switch. The fresh VC++ 1.52c/Win98 run in
`runs/vmctl-vc152dng2loc/` reports compile/link success and the same 15-check
result; the process exited with status 0 and the post-run window inventory
showed no Program Error dialog.

The original decompilation supplies the resource IDs and the `type >> 5` /
`(type >> 2) & 7` table indexing; the prior original Win98 Step 2 replay
supplies the sixteen-row page boundary. The remaining DrawNewGame2 E3 unit is
complete page GDI/resource integration and original rendering comparison.
