# PtDisplayPlanetPopInfo packed-record integration checkpoint

The packed-record adapter exercises the existing `lookup_planet` boundary for
`PtDisplayPlanetPopInfo`. It decodes the recovered 56-byte `_planet` record's
identity, owner, mineral-rate, and population-change storage, then supplies
the derived habitability, maximum-population, desirability, and foreign
environment values through the existing service boundary.

The checked raw fields are the planet id at offset 0, owner at 2, flags at 4,
the packed population-change byte at offset 20, and the first mineral-rate
long at offset 28. The adapter runs the owned, unowned, foreign, and negative-
desirability branches through the current typed source and checks the emitted
labels, wrapping, lookup identity, and returned geometry.

Host strict C89 and native VC++ 1.52c reports are:

`PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=19 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152ppint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog. An earlier diagnostic report is retained as
`PPINTDEBUG.TXT`; it caught incorrect expected label indices in the negative-
desirability branch, not a source or compiler failure.

This advances `PtDisplayPlanetPopInfo` within Phase 2 step 5/6. It does not
promote the procedure: complete packed planet/player lookup, localization,
fixed-point formatting, detailed stream/wrap GDI behavior, original
differential coverage, and final review remain open.

Evidence:

- `tests/pt_display_planet_pop_info_packed_probe.c`
- `compat/vc152/pt-display-planet-pop-info-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/10c0_228e_PtDisplayPlanetPopInfo.c`
- `analysis/reconstruction/pt-display-planet-pop-info.md`
- `runs/vmctl-vc152ppint/PPINTSTA.TXT`
- `runs/vmctl-vc152ppint/PPINTCL.LOG`
- `runs/vmctl-vc152ppint/PPINTLINK.LOG`
- `runs/vmctl-vc152ppint/PPINT.TXT`
- `runs/vmctl-vc152ppint/PPINTDEBUG.TXT`
