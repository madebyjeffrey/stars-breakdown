# DrawMineSurvey packed-record E2 checkpoint

This packet closes the named Step-5 packed-record boundary for
`DrawMineSurvey`; it does not claim the remaining detailed GDI or localized
caller work.

The typed source now accepts the recovered Win16 record boundaries directly:
`_fleet` (124 bytes, ship counts at offset 12), `_planet` (56 bytes, owner,
detail, environment, concentration, and mineral fields), and `_thing` (18
bytes, packed identity/point and wormhole destination/range fields). Existing
typed inputs remain supported, so the adapter is a source-level conversion
boundary rather than a byte replay.

The strict host probe exercises all three raw-record paths and checks the
decoded fleet total, planet owner/environment/concentration/mineral values,
thing identity/owner/point, and wormhole destination/range. It reports:

```text
DRAW_MINE_SURVEY_SOURCE=PASS
```

A fresh VC++ 1.52c / Win98 package in `C:\DMSE2` compiles and links the exact
current source. The native report is:

```text
DRAW_MINE_SURVEY_CHECKS=12 FAILURES=0 RESULT=PASS
```

The process exits 0, the build status is `COMPILE=PASS` / `LINK=PASS`, the
redirected console output is empty, and the post-run window inventory contains
no Program Error dialog. This closes DrawMineSurvey E2 and reduces the
procedure from `0/3` to `1/3` named Step-5 evidence units. E1 detailed GDI
layout and E3 localization/complete mine-survey caller integration remain.

Evidence: `tests/draw_mine_survey_source_probe.c`,
`reconstructed/src/draw_mine_survey.c`,
`compat/vc152/draw-mine-survey-e2/`, and
`runs/vmctl-vc152dmse2/`.
