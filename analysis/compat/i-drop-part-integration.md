# IDropPart packed/integration checkpoint

The reconstructed `IDropPart` boundary was exercised through the recovered
packed hull records in a fresh VC++ 1.52c / Win98 harness. CodeView identifies
`_hul` as a 123-byte record with `rghs` at offset 58, four-byte `_hs` entries,
and `chs` at offset 122. `_hs` stores `grhst` at offset 0 and the packed
quantity word at offset 2; the quantity word carries the item byte and the
quantity byte used by the procedure.

The adapter converts current and reference hull records into the typed slot
view, runs the reconstructed drop logic, and writes the changed slot words
back to the packed current record. Win16 hit testing and redraw/update effects
remain explicit callbacks because the slot-dialog rectangles and USER/GDI
handles are separate service boundaries.

The integration probe checks raw hull size/count and slot round trips,
empty-side source removal, capacity-limited compatible merging, actual source
decrement, target selection, incompatible-drop rejection, and no-modify
preservation. Host strict C89 and native reports are:

`I_DROP_PART_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152idpint/`. The Win98 process exited 0, the window inventory
returned no Program Error dialog, and the final screen is preserved as a
runtime safety record. The raw listing, CodeView layout, and earlier live
original-executable designer diagnostic establish the surrounding behavior
boundary; the controller still cannot produce a real press/move/release drag
trace.

This advances `IDropPart` from Phase 2 step 4/6 to step 5/6, not promotion.
Exact Win16 slot hit testing and drag initiation, build-dialog redraw/control
integration, and complete original-executable drop behavior remain open.

Evidence:

- `tests/i_drop_part_integration_probe.c`
- `compat/vc152/i-drop-part-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/10c8_5476_IDropPart.c`
- `analysis/reconstruction/i-drop-part.md`
- `analysis/compat/i-drop-part-live-diagnostic.md`
- `runs/vmctl-vc152idpint/IDPISTA.TXT`
- `runs/vmctl-vc152idpint/IDPICL.LOG`
- `runs/vmctl-vc152idpint/IDPILINK.LOG`
- `runs/vmctl-vc152idpint/IDPIR.TXT`
- `runs/vmctl-vc152idpint/IDPINT.MAP`
- `runs/vmctl-vc152idpint/native-screen.bmp`
