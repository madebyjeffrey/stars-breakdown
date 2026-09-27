# PtDisplayPlanetStateInfo production packed-record bridge

The readable `PtDisplayPlanetStateInfo` path now owns the bounded decode of
the two recovered raw views used by the original state display: the 56-byte
`_planet` record and the 22-byte `_popupdata` value block. The production input
reads the planet identity and environment bytes from `_planet`, the planet and
player limit/value fields from `_popupdata`, and preserves the derived
environment/desirability callbacks. An out-of-range environment index is
clamped to the first recovered byte, and incomplete records leave the typed
state data untouched.

The strict-C89 packed probe now exercises production source directly. It passes
`PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS`,
covering raw field precedence, known and unknown limit branches, environment
and desirability callbacks, wrapping/layout, and fallback when `_popupdata` is
short.

The strict-C89 production-path probe passes with exit 0. A fresh VC++ 1.52c/
Win98 production-bridge replay in `C:\STPE4` records:

```text
STINTSTA.TXT: COMPILE=PASS
STINTSTA.TXT: LINK=PASS
STPE4.TXT: PT_DISPLAY_PLANET_STATE_INFO_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS
```

The post-run window list contained no Program Error dialog. The packet also
adds a short-record preservation assertion for the typed fallback path. This
closes E1, the complete packed player/planet lookup acceptance unit. E2
formatting is closed separately; detailed stream/wrap GDI and original
comparison (E3), and final promotion remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/pt_display_planet_state_info.c`
- `tests/pt_display_planet_state_info_packed_probe.c`
- `analysis/compat/pt-display-planet-state-info-packed-integration.{md,json}`
- `compat/vc152/pt-display-planet-state-info-packed-production-e4/`
- `runs/vmctl-vc152stpe4/STINTSTA-final.TXT`
- `runs/vmctl-vc152stpe4/STINTCL-final.LOG`
- `runs/vmctl-vc152stpe4/STINTLINK-final.LOG`
- `runs/vmctl-vc152stpe4/STPE4-final.TXT`
