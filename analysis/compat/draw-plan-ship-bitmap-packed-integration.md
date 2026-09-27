# DrawPlanShipBitmap packed-selection integration checkpoint

`DrawPlanShipBitmap` receives the current selection object and the tile/window
state from the planet frame. The packed adapter decodes the recovered 226-byte
`_sel` record into the typed object-kind and object-id boundary, while the
fleet and planet subrecords remain available for identity checks.

The checked direct fields are `_sel.grobj` at offset 4, `_sel.id` at offset 8,
the 124-byte `_fleet` view at offset 28, and the 56-byte `_planet` view at
offset 152. The adapter verifies fleet-name and fleet-bitmap dispatch, planet
name and framed-DIB dispatch, the planet tile source coordinates, hidden
button cleanup, grid positioning, and the no-button path.

Host strict C89 and native VC++ 1.52c reports are:

`DRAW_PLAN_SHIP_BITMAP_PACKED_CHECKS=17 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152dpsbint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This advances `DrawPlanShipBitmap` within Phase 2 step 5/6. It does not
promote the procedure: exact Win16 GDI resource handles, complete packed
fleet/planet selection integration, localized name behavior, original
differential coverage, and final review remain open.

Evidence:

- `tests/draw_plan_ship_bitmap_packed_probe.c`
- `compat/vc152/draw-plan-ship-bitmap-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1048_3336_DrawPlanShipBitmap.c`
- `analysis/reconstruction/draw-plan-ship-bitmap.md`
- `runs/vmctl-vc152dpsbint/DPSBINTSTA.TXT`
- `runs/vmctl-vc152dpsbint/DPSBINTCL.LOG`
- `runs/vmctl-vc152dpsbint/DPSBINTLINK.LOG`
- `runs/vmctl-vc152dpsbint/DPSBINT.TXT`
