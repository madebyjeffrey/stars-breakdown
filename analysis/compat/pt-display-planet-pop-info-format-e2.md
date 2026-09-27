# PtDisplayPlanetPopInfo formatting boundary

`PtDisplayPlanetPopInfo` now exposes an optional typed
`prepare_segment` callback. It receives every localized/value segment after
the source has selected its kind, string id, fixed-point values, and bold
state, but before font selection and stream/wrap dispatch. Existing callers
that own this work inside `stream_text` and `wrap_text` remain compatible.

The strict host C89 source probe now reports:

`PT_DISPLAY_PLANET_POP_INFO_FORMAT_CHECKS=27 FAILURES=0 RESULT=PASS`

It now verifies that every emitted stream/wrap segment crosses the new
boundary and that the negative-desirability path preserves the original
fixed-point pair `2,5` for `-25` tenths. The existing 19-check packed-record
probe also remains passing.

A fresh VC++ 1.52c packet under
`compat/vc152/pt-display-planet-pop-info-format-e2/` was replayed in
`C:\PPFE2`. The native result is:

```text
PPFSTA.TXT: COMPILE=PASS
PPFSTA.TXT: LINK=PASS
PPFE2.TXT: PT_DISPLAY_PLANET_POP_INFO_FORMAT_CHECKS=27 FAILURES=0 RESULT=PASS
```

The post-run window list contained no Program Error dialog. This closes E2,
the localized/fixed-point formatting acceptance unit. Complete packed
planet/player lookup (E1), detailed stream/wrap GDI and original comparison
(E3), and final promotion remain open.

Evidence is preserved in `runs/vmctl-vc152ppfe2/`.
