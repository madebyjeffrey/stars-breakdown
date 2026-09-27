# DumpFleets visibility and coordinator boundary

The `DumpFleets` host acceptance matrix now covers the coordinator gates and
row boundaries that were not exercised by the existing ten-check source
probe. The strict C89 probe checks inactive-game and invalid-viewer rejection,
open/header/row failure cleanup, normal versus detailed headers, all four
ship-class buckets plus the other-class total, all five cargo slots, detail
suppression, destination-requires-detail behavior, and the detailed
scanner/task/mine/terraform fields.

The host report is:

`DUMP_FLEETS_BOUNDARY_CHECKS=20 FAILURES=0 RESULT=PASS`

A fresh production-source VC++ 1.52c packet in
`compat/vc152/dump-fleets-boundary-production-e3/` was replayed in `C:\DFBP`.
The build records `COMPILE=PASS` and `LINK=PASS`, and the native report is:

`DUMP_FLEETS_BOUNDARY_CHECKS=20 FAILURES=0 RESULT=PASS`

The process exited 0 and the post-run Win98 window inventory contained no
Program Error dialog. This closes the native boundary replay, but not a named
Step-5 unit: localized scanner/task/minefield/battle-plan serialization,
complete packed service integration, and the original output differential are
still required before the E1 acceptance unit can close. Evidence is preserved
in `runs/vmctl-vc152dfbp/`.
