# DrawMineSurvey E3 caller/localization integration checkpoint

This packet closes the named Step-5 E3 boundary for `DrawMineSurvey`. The
real typed `MineWndProc` paint path now invokes the real
`stars_draw_mine_survey` implementation rather than a stand-in survey
callback. The probe supplies the localized button resource through the
`0x04bd` lookup, verifies the title/control creation contract, and follows
the paint sequence through the adjusted survey rectangle, planet lookup,
typed planet details, state restoration, and paint completion.

The strict host C89 probe reports:

```text
DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS
```

A fresh VC++ 1.52c / Win98 package in `C:\DMSIE3` compiles and links the
current `MineWndProc`, `DrawMineSurvey`, and integration probe sources. The
native report is:

```text
DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS
```

The build status is `COMPILE=PASS` / `LINK=PASS`, the process exits 0, the
redirected console output is empty, and the post-run window inventory contains
no Program Error dialog. This closes E3 and advances `DrawMineSurvey` to
`2/3` named Step-5 evidence units. E1 detailed GDI layout remains open; final
Step-6 promotion therefore remains open.

Evidence: `tests/draw_mine_survey_integration_probe.c`,
`reconstructed/src/mine_wnd_proc.c`, `reconstructed/src/draw_mine_survey.c`,
`compat/vc152/draw-mine-survey-e3/`, and
`runs/vmctl-vc152dmsint3/`.
