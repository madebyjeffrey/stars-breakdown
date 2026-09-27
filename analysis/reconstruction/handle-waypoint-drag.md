# FHandleWayPointDrag reconstruction

`FHandleWayPointDrag` is the scanner waypoint drag loop at NE
`000c:8176-8af6` (2,432 bytes). The reviewed source is
[`reconstructed/src/handle_waypoint_drag.c`](../../reconstructed/src/handle_waypoint_drag.c).

The source preserves the meaningful procedure stages: logical/scan coordinate
conversion, client-area clamping while the mouse is captured, the first-move
waypoint guard, nearest-object preview with the control-key radius equivalent,
XOR preview replacement, scanner-bar update, duplicate-adjacent-waypoint
confirmation/deletion, order-record kind/id/coordinate update, warp selection,
fleet refresh, scan-selection update, capture/cursor/DC cleanup, and the
starbase redraw path.

The Win16 object tables, GDI preview primitives, mouse polling, fleet lookup,
alert, and title/invalidation services are represented by typed callbacks. The
order transition itself is ordinary C state mutation; no assembly or emitted
byte replay is used.

Validation:

- host C89 probe: `WAYPOINT_DRAG_CHECKS=5 FAILURES=0 RESULT=PASS`;
- native VC1.52c compile/link: `runs/vmctl-vc152ex/waypoint-drag/WDSTA.TXT`;
- native Win98 execution: `runs/vmctl-vc152ex/waypoint-drag/WDR.TXT`, with
  `WAYPOINT_DRAG_CHECKS=5 FAILURES=0 RESULT=PASS`.
