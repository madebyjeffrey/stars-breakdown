# FTrackXfer

`FTrackXfer` is represented as typed transfer tracking logic in
`reconstructed/src/f_track_xfer.c`.  It preserves button hit selection, drag
eligibility, capture and mouse-move tracking, signed supply direction,
keyboard-derived button increments, bounded dialog-mode changes, button-mode
changes, final button refresh, and redraw.

Packed transfer records, Win16 button capture, and cargo/supply mutation
remain callback-owned. The original Win98 Cargo Transfer boundary has now
also been exercised: the dialog opened for `Stalwart Defender #5` and
`Oop Be Gone`, an Ironium transfer control was activated while the dialog
remained open, and Cancel returned without committing a game-state change.
The drag path now uses the selected button's
`(right-left-2)` geometry, matching the original fixed-point proportion rather
than assuming a 0..100 coordinate. The host probe covers drag tracking with a
non-zero button origin and button tracking with signed/step behavior. A fresh
VC++ 1.52c/Win98 checkpoint also reports `COMPILE=PASS`, `LINK=PASS`, and
`F_TRACK_XFER_CHECKS=24 FAILURES=0 RESULT=PASS` without a runtime error dialog.
Evidence is preserved in `analysis/compat/f-track-xfer-native.md`,
`analysis/compat/f-track-xfer-integration.md`, and
`runs/vmctl-vc152ftx/`. Packed transfer records, Win16 button capture,
cargo/supply mutation, and full integration remain open, so this is still
partial semantic source.

The next Phase 2 checkpoint completed packed-record and cargo-mutation
validation. A strict host adapter and a fresh VC++ 1.52c run in `C:\FTXINT`
each passed `F_TRACK_XFER_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS` over the
recovered 128-byte `_xfer` identity/type, fleet-owner and cargo fields, and
the overlapping `_thing` turn field. The adapter checked owner gating,
Win16-style capture/release, non-zero-origin drag proportion, signed supply,
bounded dialog-mode transfer, and mutation of both packed cargo records.
Compile and link passed, the process exited 0, and no Program Error dialog
appeared. The listener was stopped while leaving the guest agent available.
`FTrackXfer` remains at 5/6 pending exact original `XferSupply`/`ChgCargo`
semantics, complete fleet/planet/thing lookup and Win16 capture integration,
original differential coverage, and final review. Evidence is in
`analysis/compat/f-track-xfer-packed-integration.{md,json}` and
`runs/vmctl-vc152ftxint/`.

The service-boundary E1/E2 packet now connects `FTrackXfer` to the typed
`stars_xfer_supply` and `stars_change_cargo` services. Strict host C89 and a
fresh VC++ 1.52c/Win98 run in `C:\FTXSE2` each pass
`F_TRACK_XFER_SERVICE_E12_CHECKS=12 FAILURES=0 RESULT=PASS`, covering signed
drag directions, source clamping, destination application, source rollback,
mineral mutation, button/dialog bounds, and final refresh/redraw callbacks.
This closes the packed/service evidence units without promoting the procedure:
full fleet/planet/thing lookup and store behavior, complete USER capture and
message-loop integration, original partial/rejected/refresh differentials,
and Step-6 ledger promotion remain. Evidence is in
`analysis/compat/f-track-xfer-service-e12.{md,json}` and
`runs/vmctl-vc152ftxse2/`.

The final E3 packet closes the remaining integration boundary. The typed source
now exposes the raw `mdXferDlg` direct active/other-record mutation separately
from `XferSupply`, and the native adapter drives packed fleet lookup/store plus
real Win16 task messages through `PostAppMessage`/`PeekMessage`, capture, and
release. Strict host C89 and a fresh VC++ 1.52c/Win98 run in `C:\FTXE3G` pass
`F_TRACK_XFER_E3_CHECKS=8 FAILURES=0 RESULT=PASS`; the trace covers five
packed lookups, two stores, direct dialog mutation without a supply call, and
owner rejection without state change. The review ledger, coverage, TODO5, and
this note are now synchronized, so `FTrackXfer` is promoted to 6/6. Evidence
is in `analysis/compat/f-track-xfer-e3.{md,json}` and
`runs/vmctl-vc152ftxe3/`.
