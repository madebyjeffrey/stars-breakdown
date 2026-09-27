# TransferStuff E3 packed lookup and Win16 dialog checkpoint

This checkpoint closes the final Step-5 evidence unit for `TransferStuff`.
The source now exposes the recovered `FLookupObject` boundary for initial
packed records and the specialized negative-id write-back boundary after the
dialog. It also preserves the original `grobj=4` split/jettison sentinel: in
mode 1 the first fleet is split into a new fleet record, while mode 0 routes
the sentinel to the source-fleet jettison service.

The strict host C89 probe reports:

`TRANSFER_STUFF_E3_CHECKS=8 FAILURES=0 RESULT=PASS`

The fresh VC++ 1.52c / Win98 package in `C:\TRCNE3` reports the same result.
`TRCNE3STA.TXT` records `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the post-run window inventory contained no Program Error dialog.

The packet covers:

- the packed fleet split path, including source identity and newly-created
  fleet identity;
- initial packed lookup and post-dialog update lookup for fleet, planet, and
  thing records;
- the real Win16 task queue through `PostAppMessage`/`PeekMessage`, with the
  transfer dialog procedure receiving initialization and OK command messages;
- cargo balance and selection/scanner refresh ordering; and
- lookup rejection before dialog creation.

Together with the earlier source, packed-record, original-dialog, and E2
cargo-conservation packets, this closes the remaining Step-5 acceptance
boundary. The review ledger, coverage, TODO5, TODO6, and reconstruction note
are synchronized, so `TransferStuff` is promoted to Phase 2 **6/6**.

Evidence:

- `tests/transfer_stuff_e3_probe.c`
- `reconstructed/src/transfer_stuff.c`
- `reconstructed/src/transfer_dlg.c`
- `compat/vc152/transfer-stuff-e3/`
- `runs/vmctl-vc152trcne3/TRCNE3STA.TXT`
- `runs/vmctl-vc152trcne3/TRCNE3CL.LOG`
- `runs/vmctl-vc152trcne3/TRCNE3LINK.LOG`
- `runs/vmctl-vc152trcne3/TRCNE3.TXT`
- `runs/vmctl-vc152trcne3/TRCNE3.MAP`
- `runs/vmctl-vc152trcne3/TRCNE3.EXE`
