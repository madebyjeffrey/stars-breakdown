# DumpPlanets visibility and coordinator boundary

The `DumpPlanets` host acceptance matrix now covers the coordinator gates and
the remaining report-row visibility/class boundaries. The strict C89 probe
checks inactive-game and invalid-viewer rejection, open/header/row failure
cleanup, normal versus detailed header counts, turn-age and owner visibility,
scanned versus unscanned service fields, inhabited/unknown planet-class
clearing, and preservation of normal production/resource fields.

The host report is:

`DUMP_PLANETS_BOUNDARY_CHECKS=24 FAILURES=0 RESULT=PASS`

A fresh production-source VC++ 1.52c packet in
`compat/vc152/dump-planets-boundary-production-e3/` was replayed in `C:\DPBP`.
The build records `COMPILE=PASS` and `LINK=PASS`, and the native report is:

`DUMP_PLANETS_BOUNDARY_CHECKS=24 FAILURES=0 RESULT=PASS`

The process exited 0 and the post-run Win98 window inventory contained no
Program Error dialog. This closes the native boundary replay, but not a named
Step-5 unit: localized derived-field serialization, complete packed service
integration, and the original output differential are still required before
the E1 acceptance unit can close. Evidence is preserved in
`runs/vmctl-vc152dpbp/`.
