# DrawSlotDlg reconstruction

`DrawSlotDlg` paints the ship-builder slot strip at NE `001a:2650-306a`
(2,586 bytes). The reviewed source is
[`reconstructed/src/draw_slot_dlg.c`](../../reconstructed/src/draw_slot_dlg.c).

The source preserves the build-mode guard, caller-owned versus acquired DC,
fleet bitmap placement, spin-button mode, full-list versus single-slot range,
cargo/dock panel selection, empty versus occupied slot rendering, selected-slot
double-frame behavior, plaque visibility gate, and cleanup ordering. Bitmap,
localized text, and GDI work are named callbacks, while the slot and panel
control flow is ordinary C.

Validation:

- host C89 probe: `DRAW_SLOT_CHECKS=5 FAILURES=0 RESULT=PASS`;
- native VC1.52c compile/link: `runs/vmctl-vc152ex/draw-slot/DSSTA.TXT`;
- native Win98 execution: `runs/vmctl-vc152ex/draw-slot/DSR.TXT`, with
  `DRAW_SLOT_CHECKS=5 FAILURES=0 RESULT=PASS`.
