# DrawNewGame3 localized rendering — E3

This checkpoint closes the localized-rendering evidence unit for
`DrawNewGame3`. The source now accepts an optional typed numeric formatter and
obtains the negative-width suffix through resource ID `0x0af4`, retaining the
strict numeric fallback for callers that do not provide the service.

The strict host C89 probe reports
`DRAW_NEW_GAME3_LOCALIZATION_E3_CHECKS=34 FAILURES=0 RESULT=PASS`. It checks
18 formatter calls, 18 value lookups, 18 right-aligned outputs, the localized
`pct` suffix, the first formatted value text, all 27 sequential label IDs
starting at `0x3aa`, and the complete 36-reference output boundary.

The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152dng3loc/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same 34-check result. The first native
attempt exposed and corrected a stale package header; the rerun compiled and
linked cleanly, exited with status 0, and showed no Program Error dialog.
Complete page GDI/original comparison and Step-6 promotion remain open.
