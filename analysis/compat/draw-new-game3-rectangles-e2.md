# DrawNewGame3 output rectangles — E2

This checkpoint closes the packed output-rectangle evidence unit for
`DrawNewGame3`. The probe drives all nine rows with deterministic mapped
control rectangles and checks every one of the 18 value rectangles and their
18 vertically offset partners.

The strict host C89 probe reports
`DRAW_NEW_GAME3_RECTANGLES_E2_CHECKS=95 FAILURES=0 RESULT=PASS`. It covers all
rectangle left/right coordinates, row-dependent top positions, fixed button
height, paired offset geometry, the row-7/8 carry-over positions, all 18 value
lookups, and all 36 draw-button calls. The negative width case also confirms
that percentage spacing changes the following rectangle position.

The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152dng3rect/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same 95-check result. The process exited
with status 0 and the post-run window inventory showed no Program Error dialog.
Localized rendering, complete page GDI/original comparison, and Step-6
promotion remain open.
