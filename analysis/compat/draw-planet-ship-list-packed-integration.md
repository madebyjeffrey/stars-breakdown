# DrawPlanetShipList packed-transfer integration checkpoint

`DrawPlanetShipList` receives the selected planet and the transfer-list result
from the planet frame. The packed adapter decodes the selected planet from
the recovered 226-byte `_sel` record and the transfer result from the
recovered 128-byte `_xfer` record, then supplies the existing typed control
state and lookup boundary.

The checked direct fields are `_sel.pl` at offset 152, `_xfer.id` at offset 0,
`_xfer.grobj` at offset 2, and the overlapping fleet/thing view at offset 4.
The adapter verifies fleet ownership, thing owner/type bits, transfer lookup
gating, fleet and thing-mode button enablement, detail filling, and the
no-object visibility path.

Host strict C89 and native VC++ 1.52c reports are:

`DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152dlistint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This advances `DrawPlanetShipList` within Phase 2 step 5/6. It does not
promote the procedure: complete packed fleet/thing transfer behavior, exact
Win16 control handles and geometry, localized text/gauges, detailed drawing,
original differential coverage, and final review remain open.

Evidence:

- `tests/draw_planet_ship_list_packed_probe.c`
- `compat/vc152/draw-planet-ship-list-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1048_377e_DrawPlanetShipList.c`
- `analysis/reconstruction/draw-planet-ship-list.md`
- `runs/vmctl-vc152dlistint/DLISTSTA.TXT`
- `runs/vmctl-vc152dlistint/DLISTCL.LOG`
- `runs/vmctl-vc152dlistint/DLISTLINK.LOG`
- `runs/vmctl-vc152dlistint/DLIST.TXT`
