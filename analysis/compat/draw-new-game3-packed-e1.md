# DrawNewGame3 packed signed-width table — E1

This checkpoint closes the packed-width evidence unit for `DrawNewGame3`.
The source now accepts the original 18-byte signed table (`9 rows x 2
columns`) while retaining the named matrix as a host-adapter fallback. The
probe deliberately supplies conflicting named widths so the recovered packed
table is the controlling input.

The strict host C89 probe reports
`DRAW_NEW_GAME3_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS`. It covers signed
negative widths and their percent suffix, the zero-width second-column skip,
the resulting 17 value lookups, the final value index, all 34 paired reference
rectangles, and consumption of the full 18-byte table.

The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152dng3pack/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same 7-check result. The process exited
with status 0; the post-run window inventory showed no Program Error dialog.
The raw decompilation's `row * 2 + column` byte addressing and signed `abs`
operation are now represented directly. Packed rectangles, localized
formatting, complete GDI integration, original page comparison, and Step-6
promotion remain open.
