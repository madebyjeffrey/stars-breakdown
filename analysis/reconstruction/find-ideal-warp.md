# IFindIdealWarp reconstruction

`reconstructed/src/find_ideal_warp.c` is a typed semantic reconstruction of ideal warp selection, preserving range, fuel, engine, and route constraints and the fallback result.

Validation: `tests/find_ideal_warp_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/FIWSTA.TXT` and `runs/vmctl-vc152ev/FIWRES.TXT`.

## Original instruction correction (2026-09-25)

Earlier ordinary-engine source/native claims do not validate the corrected
scoop behavior. NE11:A902-A95F uses minimum speeds 5 and 6 for reductions
of one and two; A962-A98B uses minimum speed 7 for a reduction of three.
The third check reads fuel_used[warp-3], not an independent ore-cost array.
See `../compat/satisfy-orders-e3-ideal-warp-disassembly.txt`. The source probe
now checks 15 instruction-derived cases and passes. No new native run was
performed. Full SO-E3 integration remains failing at the unbound AI speed
callback, as recorded in the Gate 2 tracker.
