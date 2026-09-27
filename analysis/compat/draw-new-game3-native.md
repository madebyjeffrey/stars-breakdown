# DrawNewGame3 native checkpoint

The original baseline guest run is preserved in `runs/vmctl-vc152dng3/`; the
packed-width E1 follow-up is preserved in `runs/vmctl-vc152dng3pack/`, and the
rectangle E2 follow-up is preserved in `runs/vmctl-vc152dng3rect/`; the
localized-rendering E3 follow-up is preserved in `runs/vmctl-vc152dng3loc/`,
and the complete integration E4 follow-up is preserved in
`runs/vmctl-vc152dng3int/`.

- `DNG3STA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DNG3CL.LOG`: VC++ 1.52c compilation completed for the current
  `draw_new_game3.c` source and probe
- `DNG3LINK.LOG`: segmented link completed with `DNG3.DEF`
- `DNG3R.TXT`: `DRAW_NEW_GAME3_CHECKS=2 FAILURES=0 RESULT=PASS`
- `DNG3R-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\DNG3`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates the recovered DC
lifecycle, nine-row label/value rendering, signed-width percentage suffix,
paired button rectangles, row-7/8 carry-over geometry, and reference-count
boundary. Exact race-creation tables, localized formatting, packed global
rectangles, and full game GDI integration remain outside this checkpoint.

The follow-up `C:\DNG3P` run adds the packed signed-width boundary:

- `DNG3PSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DNG3PCL.LOG`: VC++ 1.52c compilation completed for the packed source/probe
- `DNG3PLINK.LOG`: segmented link completed with `DNG3P.DEF`
- `DNG3PE1.TXT`: `DRAW_NEW_GAME3_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS`

The process exited with status 0 and the post-run window inventory showed no
Program Error dialog. This closes E1 of the Step-5 packet; packed output
rectangles, localized rendering, complete GDI/original comparison, and ledger
promotion remain open.

The follow-up `C:\DNG3L` run closes the localized-rendering packet:

- `DNG3LSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DNG3LCL.LOG`: VC++ 1.52c compilation completed for the localized source/probe
- `DNG3LLINK.LOG`: segmented link completed with `DNG3L.DEF`
- `DNG3LE3.TXT`: `DRAW_NEW_GAME3_LOCALIZATION_E3_CHECKS=34 FAILURES=0 RESULT=PASS`

The initial guest build caught a stale package header before execution; after
the header was synchronized, the clean rerun exited with status 0 and the
post-run window inventory showed no Program Error dialog. This closes E3 of
the Step-5 packet; complete page GDI/original comparison and ledger promotion
remain open.

The follow-up `C:\DNG3R` run closes the output-rectangle packet:

- `DNG3RSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DNG3RCL.LOG`: VC++ 1.52c compilation completed for the rectangle source/probe
- `DNG3RLINK.LOG`: segmented link completed with `DNG3R.DEF`
- `DNG3RE2.TXT`: `DRAW_NEW_GAME3_RECTANGLES_E2_CHECKS=95 FAILURES=0 RESULT=PASS`

The process exited with status 0 and the post-run window inventory showed no
Program Error dialog. This closes E2 of the Step-5 packet; localized
rendering, complete page GDI/original comparison, and ledger promotion remain
open.

The follow-up `C:\DNG3I` run closes the complete page integration packet:

- `DNG3ISTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DNG3ICL.LOG`: VC++ 1.52c compilation completed for the integration source/probe
- `DNG3ILINK.LOG`: segmented link completed with `DNG3I.DEF`
- `DNG3IE4.TXT`: `DRAW_NEW_GAME3_INTEGRATION_E4_CHECKS=12 FAILURES=0 RESULT=PASS`

The initial guest build caught a stale package header before execution; after
the header was synchronized, the clean rerun exited with status 0 and the
post-run window inventory showed no Program Error dialog. Together with the
original Step-3 wizard observation, this closes the final Step-5 evidence
unit; only Step-6 ledger promotion remains.
