# DumpPlanets packed service-resolution E3

The production `stars_dump_planets_packed` bridge now exposes the recovered
service boundary for values that are not stored in the 56-byte `_planet`
record. After decoding the record, it calls `resolve_services` with the raw
record, its size, and the typed `StarsDumpPlanet` view; the normal
`stars_dump_planets` coordinator then applies its existing detailed and
visibility gates before writing the row.

The strict-C89 host probe reports:

```text
DUMP_PLANETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS
```

The matrix checks callback ordering, raw-record identity and size, decoded
identity forwarding, service overrides for environment and mineral rate,
resources, survival, scanner detail, starbase, wormhole, and mine count, and
their preservation into the final report row.

Fresh VC++ 1.52c/Win98 validation in `C:\DPSE3` also reports
`DUMP_PLANETS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS`; compile and link
passed, the process exited 0, and the final guest window inventory contained
no Program Error dialog.

This closes the named packed-service integration unit. Localized names and
field formatting, complete serialized stream comparison, original row-output
differential, and final promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/1108_86e4_DumpPlanets.c`
- `reconstructed/include/core.h`
- `reconstructed/src/dump_planets.c`
- `tests/dump_planets_services_probe.c`
- `compat/vc152/dump-planets-services-e3/`
- `runs/vmctl-vc152dpse3/DPSSTA.TXT`
- `runs/vmctl-vc152dpse3/DPSCL.LOG`
- `runs/vmctl-vc152dpse3/DPSLINK.LOG`
- `runs/vmctl-vc152dpse3/DPSR.TXT`
