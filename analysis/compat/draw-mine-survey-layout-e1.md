# DrawMineSurvey E1 detailed GDI layout checkpoint

This packet closes the named Step-5 E1 detailed GDI-layout boundary for
`DrawMineSurvey`. The readable source now carries the recovered outer-panel
PATBLT geometry as typed layout records: the 13-command fleet frame, the
13-command ordinary thing frame, and the 7-command compact thing frame. Each
command preserves the exact rectangle offset, width, height, raster
operation, and shadow/highlight brush role from the original Win16 trace.

The strict host C89 layout probe reports:

```text
DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=0 RESULT=PASS
```

A fresh VC++ 1.52c / Win98 package in `C:\DMSIE1` compiles and links the
current source. The native report is:

```text
DRAW_MINE_SURVEY_LAYOUT_CHECKS=13 FAILURES=0 RESULT=PASS
```

The build status is `COMPILE=PASS` / `LINK=PASS`, the process exits 0, the
redirected console output is empty, and the post-run window inventory contains
no Program Error dialog. Together with E2 packed-record integration and E3
localized caller integration, this closes all three named Step-5 units. The
review record is promoted to Step 6.

Evidence: `tests/draw_mine_survey_layout_probe.c`,
`reconstructed/src/draw_mine_survey.c`,
`compat/vc152/draw-mine-survey-e1/`, and
`runs/vmctl-vc152dmslay/`.
