# FTrackXfer packed-record integration checkpoint

`FTrackXfer` receives the selected transfer control and applies drag or
button changes through the transfer service boundary. The packed adapter
decodes the recovered 128-byte `_xfer` view and routes cargo changes back into
the packed fleet cargo words.

The checkpoint verifies `_xfer.id` at offset 0, `_xfer.grobj` at offset 2,
the overlapping fleet owner at offset 6, the fleet cargo view at offset 80,
and the overlapping `_thing.turn` field at offset 20. It then checks owner
gating, the selected button, Win16-style capture/release, non-zero-origin
drag proportionality, signed cargo supply, bounded dialog-mode transfer,
and mutation of both packed cargo records.

Host strict C89 and native VC++ 1.52c reports are:

`F_TRACK_XFER_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152ftxint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This adds packed-record and cargo-mutation evidence within Phase 2 step 5/6.
It does not promote the procedure: exact original `XferSupply`/`ChgCargo`
semantics, full fleet/planet/thing lookup, complete Win16 capture behavior,
original differential coverage, and final review remain open.

Evidence:

- `tests/f_track_xfer_packed_probe.c`
- `compat/vc152/f-track-xfer-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1050_5a16_FTrackXfer.c`
- `analysis/reconstruction/f-track-xfer.md`
- `runs/vmctl-vc152ftxint/FTXINTSTA.TXT`
- `runs/vmctl-vc152ftxint/FTXINTCL.LOG`
- `runs/vmctl-vc152ftxint/FTXINTLINK.LOG`
- `runs/vmctl-vc152ftxint/FTXINT.TXT`
- `runs/vmctl-vc152ftxint/guest-screen.bmp`
