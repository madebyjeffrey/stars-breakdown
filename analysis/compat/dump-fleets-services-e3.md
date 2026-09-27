# DumpFleets packed service-resolution E3

The production `stars_dump_fleets_packed` bridge now exposes the recovered
service boundary for values that are not stored in the 124-byte `_fleet`
record. After decoding the record, it calls `resolve_services` with the raw
record, its size, and the typed `StarsDumpFleet` view; the normal
`stars_dump_fleets` coordinator then applies its detailed-mode and destination
visibility gates before writing the row.

The strict-C89 host probe reports:

```text
DUMP_FLEETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS
```

The matrix checks callback ordering, raw-record identity and size, decoded
identity forwarding, service overrides for ETA, cloak, scanner range/detail,
task, destination, battle plan, minefield values, and their preservation into
the final report row together with the decoded ship and cargo totals.

Fresh VC++ 1.52c/Win98 validation in `C:\DFSE3` also reports
`DUMP_FLEETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS`; compile and link
passed, the process exited 0, and the final guest window inventory contained
no Program Error dialog.

This closes the named packed-service integration unit. Localized names and
field formatting, complete serialized stream comparison, original row-output
differential, and final promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/1108_9530_DumpFleets.c`
- `reconstructed/include/core.h`
- `reconstructed/src/dump_fleets.c`
- `tests/dump_fleets_services_probe.c`
- `compat/vc152/dump-fleets-services-e3/`
- `runs/vmctl-vc152dfse3/DFSSTA.TXT`
- `runs/vmctl-vc152dfse3/DFSCL.LOG`
- `runs/vmctl-vc152dfse3/DFSLINK.LOG`
- `runs/vmctl-vc152dfse3/DFSR.TXT`
