# FTrackXfer E3 packed lookup and Win16 queue checkpoint

This checkpoint closes the last named Step-5 evidence unit for `FTrackXfer`.
The host and native probes use the recovered 128-byte transfer records rather
than detached cargo objects. `ChgCargo` lookup/store callbacks load and write
the packed fleet mineral words, while the `mdXferDlg` branch uses its separate
direct active/other-record mutation boundary. The event adapter replays the
recovered `FGetMouseMove` filtering with real Win16 task messages in the native
run: `PostAppMessage`, `PeekMessage`, `SetCapture`, and `ReleaseCapture`.

The strict host and fresh VC++ 1.52c / Win98 run from `C:\FTXE3G` both report:

`F_TRACK_XFER_E3_CHECKS=8 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS`; the process exited
normally and the post-run window inventory contained no Program Error dialog.
The trace confirms five packed lookup calls and two stores for the drag/service
case, direct dialog mutation without an `XferSupply` call, and zero mutation
when the owner gate rejects the drag.

Together with the earlier packed E1 and service E2 packets, this closes every
named Step-5 evidence unit. `FTrackXfer` is promoted to 6/6 after the review,
coverage, TODO5, and reconstruction ledgers are synchronized.

Evidence:

- `tests/f_track_xfer_e3_probe.c`
- `compat/vc152/f-track-xfer-e3g/`
- `analysis/decompiler/export/raw/1050_5a16_FTrackXfer.c`
- `analysis/decompiler/export/raw/1040_4146_FGetMouseMove.c`
- `analysis/decompiler/export/raw/1040_354c_InitBtnTrack.c`
- `analysis/decompiler/export/raw/1040_364a_FTrackBtn.c`
- `analysis/compat/f-track-xfer-packed-integration.{md,json}`
- `analysis/compat/f-track-xfer-service-e12.{md,json}`
- `runs/vmctl-vc152ftxe3/FTXE3.TXT`
- `runs/vmctl-vc152ftxe3/FTXE3GSTA.TXT`
- `runs/vmctl-vc152ftxe3/FTXE3GCL.LOG`
- `runs/vmctl-vc152ftxe3/FTXE3GLINK.LOG`
- `runs/vmctl-vc152ftxe3/FTXE3G.EXE`
- `runs/vmctl-vc152ftxe3/FTXE3G.MAP`
- `runs/vmctl-vc152ftxe3/FTXE3GN.OBJ`
