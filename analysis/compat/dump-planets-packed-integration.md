# DumpPlanets packed-record integration checkpoint

The production bridge reads one recovered 56-byte `_planet` record and maps
its CodeView offsets into the typed `StarsDumpPlanet` view before running the
existing detailed report coordinator. It checks planet identity, owner and
visibility flags, turn age, environment and mineral arrays, population,
packed mine/factory weights, scanner range, starbase, wormhole, and report-row
dispatch.

Both strict host C89 and VC++ 1.52c/Win98 report:

`DUMP_PLANETS_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`

The fresh historical native run was `C:\DMPINT2`; compilation and linking
passed, the process exited 0, and no Program Error dialog appeared. That
packet used the earlier equivalent test adapter. The host probe now calls the
production `stars_dump_planets_packed` bridge directly and checks all three
environment/original-environment slots, all three mineral concentration/rate
slots, scanner detail, and the complete starbase/wormhole and row-derived
service fields. A fresh native packet containing this production bridge is
pending the guest-agent reconnect. This advances the packed-data checkpoint
but not final promotion. Localized formatting, complete service callbacks,
full original report differential, and promotion remain open.

Evidence:

- `tests/dump_planets_packed_probe.c`
- `reconstructed/src/dump_planets.c`
- `reconstructed/include/core.h`
- `compat/vc152/dump-planets-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1108_86e4_DumpPlanets.c`
- `runs/vmctl-vc152dmpint2/DPINTSTA.TXT`
- `runs/vmctl-vc152dmpint2/DPINTCL.LOG`
- `runs/vmctl-vc152dmpint2/DPINTLINK.LOG`
- `runs/vmctl-vc152dmpint2/DPINT.TXT`
