# PtDisplayPlanetStateInfo formatting boundary

`PtDisplayPlanetStateInfo` now exposes an optional typed `prepare_segment`
callback. It receives every localized/value segment after the source selects
the branch, string id, kind, value, and bold state, but before font and
stream/wrap dispatch. Existing callers remain compatible.

The strict host C89 source probe now reports:

`PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS`

The added checks cover every segment in the known-limit path and verify that
the environment service's fixed-point input reaches a kind-2 segment. The
existing unknown-limit and signed-difference branches remain covered, and the
23-check packed-record probe still passes.

A fresh VC++ 1.52c packet under
`compat/vc152/pt-display-planet-state-info-format-e2/` was replayed in
`C:\STFE2`. The native result is:

```text
STFSTA.TXT: COMPILE=PASS
STFSTA.TXT: LINK=PASS
STFE2.TXT: PT_DISPLAY_PLANET_STATE_INFO_FORMAT_CHECKS=35 FAILURES=0 RESULT=PASS
```

The post-run window list contained no Program Error dialog. This closes E2,
the localized/fixed-point formatting acceptance unit. Complete packed
player/planet lookup (E1), detailed stream/wrap GDI and original comparison
(E3), and final promotion remain open.

Evidence is preserved in `runs/vmctl-vc152stfe2/`.
