# DrawPlanShipBitmap packed-selection E2 native validation

The current production `DrawPlanShipBitmap` source now consumes the complete
226-byte `_sel` view directly when a packed selection record is supplied. The
bridge decodes the original `grobj` and `id` words at offsets 4 and 8, rejects
an undersized record before dispatch, and then runs the unchanged fleet and
planet rendering paths.

The strict-C89 host probe reports
`DRAW_PLAN_SHIP_BITMAP_PACKED_E2_CHECKS=11 FAILURES=0 RESULT=PASS`.
The same current source and probe were compiled and linked with VC++ 1.52c in
fresh Win98 guest directory `C:\DPSBE2`. The native report is the same,
`COMPILE=PASS` and `LINK=PASS` are recorded, the process exited 0, and the
final guest window inventory contained no Program Error dialog.

The 11 checks cover fleet selection kind/id and callback dispatch, planet
selection kind/id and callback/DIB dispatch, exact tile source coordinates,
button suppression, and the short-record rejection path. The packet preserves
the raw selection bytes and the production `core.h`/`draw_plan_ship_bitmap.c`
source used for the replay.

This closes DrawPlanShipBitmap E2, leaving E3 localized-name behavior and E4
complete planet-window/original differential integration open. The procedure
therefore remains at Phase 2 step 5/6.

Evidence:

- `tests/draw_plan_ship_bitmap_packed_e2_probe.c`
- `compat/vc152/draw-plan-ship-bitmap-packed-e2-native/`
- `analysis/decompiler/export/raw/1048_3336_DrawPlanShipBitmap.c`
- `analysis/debug/types.json`
- `runs/vmctl-vc152dpsbe2/DPSBINTSTA-pass.TXT`
- `runs/vmctl-vc152dpsbe2/DPSBINTCL-pass.LOG`
- `runs/vmctl-vc152dpsbe2/DPSBINTLINK-pass.LOG`
- `runs/vmctl-vc152dpsbe2/DPSBE2-pass.TXT`
