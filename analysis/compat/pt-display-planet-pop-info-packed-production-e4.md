# PtDisplayPlanetPopInfo production packed-planet bridge

The readable `PtDisplayPlanetPopInfo` path now owns the recovered 56-byte
`_planet` decode when `StarsPlanetPopInfoInput.packed_planet_record` is
provided. It reads the planet identity and owner, the signed population-change
byte at offset 20, and the mineral-rate word at offset 28. Selected player,
habitability, maximum population, desirability, and foreign-environment values
remain explicit sidecars because they are service-derived rather than part of
this view.

The focused strict-C89 probe now calls the production path directly. It passes
`PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS`, covering
raw identity precedence, owned/unowned/foreign/negative-desirability output,
the signed population-change field, and safe fallback to `lookup_planet` when
the supplied record is shorter than 56 bytes.

The strict host C89 probe passes with exit 0. A fresh VC++ 1.52c/Win98
production-bridge replay in `C:\PPPE4` records:

```text
PPINTSTA.TXT: COMPILE=PASS
PPINTSTA.TXT: LINK=PASS
PPPE4.TXT: PT_DISPLAY_PLANET_POP_INFO_PACKED_CHECKS=21 FAILURES=0 RESULT=PASS
```

The post-run window list contained no Program Error dialog. The native packet
exposed and corrected two probe defects: a valid packed record must bypass
`lookup_planet`, and the final self-check must not count itself as one of the
21 semantic checks. With those corrections, host and native reports agree.
This closes E1, the complete packed planet/player lookup acceptance unit. E2
formatting is closed separately; detailed stream/wrap GDI and original
comparison (E3), and final promotion remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/pt_display_planet_pop_info.c`
- `tests/pt_display_planet_pop_info_packed_probe.c`
- `analysis/compat/pt-display-planet-pop-info-packed-integration.{md,json}`
- `compat/vc152/pt-display-planet-pop-info-packed-production-e4/`
- `runs/vmctl-vc152pppe4/PPINTSTA-final.TXT`
- `runs/vmctl-vc152pppe4/PPINTCL-final.LOG`
- `runs/vmctl-vc152pppe4/PPINTLINK-final.LOG`
- `runs/vmctl-vc152pppe4/PPPE4-final2.TXT`
