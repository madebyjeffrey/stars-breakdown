# DrawMineSurvey native checkpoint

The final fresh Win98 guest run is preserved in `runs/vmctl-vc152dms/`.

- `DMSSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DMSCL.LOG`: VC++ 1.52c compiled `DMS.C` and `PROBE.C` without warnings
- `DMSLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `DMSR.TXT` and `DMSR2.TXT`: `DRAW_MINE_SURVEY_CHECKS=5 FAILURES=0 RESULT=PASS`
- `DMS-CONSOLE.TXT`: zero bytes after redirected execution

`DMS.C` was transferred directly from the reviewed
`reconstructed/src/draw_mine_survey.c` file. The native probe exercises
normalized fleet dispatch and ship totals, planet lookup and unknown fallback,
decoded planet survey summary, packed thing kind/owner/id and destination/range
decoding, no-selection suppression, and drawing-state restoration.

This advances `DrawMineSurvey` from Phase 2 step 3/6 to step 4/6. Detailed GDI
layout, localized formatting, packed record/resource integration, fleet/planet
derived arithmetic, and full mine-survey integration remain open.

An expanded fresh run is preserved in `runs/vmctl-vc152dms2/`. The repaired
package includes the exact current source plus a directory-first VC1.52c
wrapper; compile and link pass, the native report is
`DRAW_MINE_SURVEY_CHECKS=9 FAILURES=0 RESULT=PASS`, redirected console output
is empty, and the post-run window list contains no Program Error dialog. The
additional cases cover shallow/unknown planet summaries, non-wormhole thing
decoding, invalid thing selection suppression, and the null-operations guard.
This strengthens the existing Phase 2 step 5/6 evidence without closing the
remaining detailed GDI, packed-resource, localization, and full integration
scope.
