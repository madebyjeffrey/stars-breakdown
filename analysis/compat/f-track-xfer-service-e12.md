# FTrackXfer service-boundary E1/E2 checkpoint

This checkpoint closes the two named service-side evidence units for
`FTrackXfer`. The host probe connects the typed tracking routine to the
reconstructed `stars_xfer_supply` and `stars_change_cargo` services instead of
using a test-only cargo callback. It exercises the drag path in both signed
directions, source-availability clamping, conservation across the two mineral
records, button-mode stepping, dialog-mode bounds, and the final refresh and
redraw callbacks.

The strict host probe and the fresh VC++ 1.52c / Win98 run both report:

`F_TRACK_XFER_SERVICE_E12_CHECKS=12 FAILURES=0 RESULT=PASS`

The native package ran from `C:\FTXSE2`. `FTXSE2STA.TXT` reports
`COMPILE=PASS` and `LINK=PASS`; the process exited 0, wrote the report, and
the post-run window inventory contained no Program Error dialog.

This closes E1 packed transfer/service identity coverage together with the
earlier `F_TRACK_XFER_PACKED_CHECKS=22` packet, and closes E2 exact
`XferSupply`/`ChgCargo` mutation semantics at the direct endpoint boundary.
`FTrackXfer` remains at 5/6 because E3 is still open: full fleet/planet/thing
lookup and store behavior, complete USER capture/message-loop integration,
and original differential cases for partial/rejected transfers and refresh
side effects still need evidence. Step-6 review-ledger promotion also remains.

Evidence:

- `tests/f_track_xfer_service_e12_probe.c`
- `compat/vc152/f-track-xfer-service-e12/`
- `analysis/decompiler/export/raw/1050_5a16_FTrackXfer.c`
- `analysis/reconstruction/f-track-xfer.md`
- `analysis/compat/f-track-xfer-packed-integration.{md,json}`
- `runs/vmctl-vc152ftxse2/FTXSE2STA.TXT`
- `runs/vmctl-vc152ftxse2/FTXSE2CL.LOG`
- `runs/vmctl-vc152ftxse2/FTXSE2LINK.LOG`
- `runs/vmctl-vc152ftxse2/FTXSE2.TXT`
- `runs/vmctl-vc152ftxse2/FTXSE2.EXE`
- `runs/vmctl-vc152ftxse2/FTXSE2.MAP`
- `runs/vmctl-vc152ftxse2/FTXSE2N.OBJ`
