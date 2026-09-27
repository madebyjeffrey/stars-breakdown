# DrawPlanetShipList combo-width/flag boundary

The typed `DrawPlanetShipList` view now preserves the raw combo control's
cached-width behavior. The first `SetWindowPos` for a new width uses flags
`0x14`; a repeated width uses `0x15`, matching the original `dxShipDD`
comparison. The legacy geometry callback remains available, while
`set_combo_position_ex` exposes the exact flags to native callers.

The strict host C89 probe reports:

`DRAW_PLANET_SHIP_LIST_COMBO_CHECKS=4 FAILURES=0 RESULT=PASS`

It covers the computed 112-pixel width, first-call/new-width behavior,
repeated-width behavior, and transfer lookup on both passes. This is host
source-fidelity evidence only: native control flags, complete transfer and
drawing behavior, localized text/gauges, original differential, and promotion
remain open. No authoritative Step-5 or promotion count changes.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/draw_planet_ship_list.c`
- `tests/draw_planet_ship_list_combo_boundary_probe.c`
- `analysis/decompiler/export/raw/1048_377e_DrawPlanetShipList.c`
