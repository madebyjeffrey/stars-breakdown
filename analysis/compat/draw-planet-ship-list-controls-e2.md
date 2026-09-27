# DrawPlanetShipList control-layout E2 validation

The control-layout probe closes a source fidelity gap exposed by the original
instruction stream: the three button windows are positioned in the order
`rghwndBtn[1]`, `rghwndBtn[2]`, `rghwndBtn[0]`, while they are shown in the
logical order 0, 1, 2. The typed interface now exposes that distinction
through `set_button_position`, while retaining the existing fallback callback
for older callers.

The strict host probe checks the populated-planet title path, combo geometry,
the exact button permutation and positions, visibility order, transfer lookup,
reference reset, detail fill, and all three enable decisions.

Host result:

`DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=7 FAILURES=0 RESULT=PASS`

The VC++ 1.52c packet in
`compat/vc152/draw-planet-ship-list-controls-e2/` compiled and linked in
Win98. Its native report is:

`DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=7 FAILURES=0 RESULT=PASS`

The process exited normally and the post-run window list contained no Program
Error dialog. This closes the named E2 control-layout unit. Complete
transfer/drawing behavior and Step-6 promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/1048_377e_DrawPlanetShipList.c`
- `reconstructed/include/core.h`
- `reconstructed/src/draw_planet_ship_list.c`
- `tests/draw_planet_ship_list_controls_probe.c`
- `runs/vmctl-vc152dlist-controls-e2/DLISTSTA.TXT`
- `runs/vmctl-vc152dlist-controls-e2/DLISTCL.LOG`
- `runs/vmctl-vc152dlist-controls-e2/DLISTLINK.LOG`
- `runs/vmctl-vc152dlist-controls-e2/DLISTC.TXT`
