# PtDisplayPlanetStateInfo packed-record integration checkpoint

The packed-record adapter exercises the existing typed state-row boundary with
the two recovered records used by the raw routine: the 56-byte `_planet`
storage record and the overlapping 22-byte `_popupdata` `GlobalPD` values.
It decodes the planet identity and environment bytes from `_planet`, the
planet/player limit values from `_popupdata`, and then runs the current source
through its known-limit, unknown-limit, environment, desirability-warning,
wrapping, and returned-geometry paths.

The checked fields are `_planet` id at offset 0, current environment at offset
12, original environment bytes at offsets 15 and 16, and `_popupdata`
`iPlanetVar` at 4, `iPlanVal` at 6, `iPlanMin` at 8, `iPlanMax` at 10,
`iPlrVal` at 12, `iPlrMin` at 14, and `iPlrMax` at 16. Derived environment
conversion and desirability remain callback-owned, matching the raw helper
calls rather than being invented in the adapter.

Host strict C89 and native VC++ 1.52c reports are:

`PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=23 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152stint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This advances `PtDisplayPlanetStateInfo` within Phase 2 step 5/6. It does not
promote the procedure: complete packed player/planet lookup, localization,
fixed-point formatting, detailed stream/wrap GDI behavior, original
differential coverage, and final review remain open.

Evidence:

- `tests/pt_display_planet_state_info_packed_probe.c`
- `compat/vc152/pt-display-planet-state-info-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/10c0_1938_PtDisplayPlanetStateInfo.c`
- `analysis/reconstruction/pt-display-planet-state-info.md`
- `runs/vmctl-vc152stint/STINTSTA.TXT`
- `runs/vmctl-vc152stint/STINTCL.LOG`
- `runs/vmctl-vc152stint/STINTLINK.LOG`
- `runs/vmctl-vc152stint/STINT.TXT`
