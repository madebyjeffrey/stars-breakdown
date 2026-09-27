# DumpFleets complete original-output E4

The production `stars_dump_fleets` coordinator now exposes the recovered
tabular serialization boundary through raw and formatted text callbacks. The
source owns the field order and normal/detail selection; the caller owns
localized text and numeric formatting.

The strict-C89 host probe reports:

```text
DUMP_FLEETS_SERIALIZATION_CHECKS=26 FAILURES=0 RESULT=PASS
```

The matrix checks the normal 12-field sequence (name, coordinates, planet,
destination, battle plan, ship total, and five cargo values) and the detailed
29-field sequence, including player, ETA, warp, weight, cloak, scanner/task,
minefield/terraform values, the other-hull bucket, and all four hull-class
buckets. It also checks the row values forwarded at representative field
positions and the exactly-once row terminator. The formatted callback path
then checks localized fleet/planet/destination/battle-plan text, numeric
values, the preserved original `GAME.fle` first-row text oracle, and detailed
localized task/scanner fields. The final scenario materializes all six fleet
rows from the preserved original run and compares the complete header/data/
CRLF stream byte-for-byte with `runs/vmctl-vc152current/GAME.fle`.

Fresh VC++ 1.52c/Win98 validation in `C:\DFSER4` reports the same
26/26 result; compile and link passed, the process exited 0, and the final
guest window inventory contained no Program Error dialog.

This closes the named complete original-output E4 unit. The localized
scanner/task E1, serialized field-order E2, and packed service-resolution E3
units are also closed. All four named Step-5 evidence units for `DumpFleets`
are now closed; only Step-6 ledger promotion remains.

Evidence:

- `analysis/decompiler/export/raw/1108_9530_DumpFleets.c`
- `reconstructed/include/core.h`
- `reconstructed/src/dump_fleets.c`
- `tests/dump_fleets_serialization_probe.c`
- `compat/vc152/dump-fleets-serialization-e4/`
- `runs/vmctl-vc152dfser4/DFSERSTA.TXT`
- `runs/vmctl-vc152dfser4/DFSERCL.LOG`
- `runs/vmctl-vc152dfser4/DFSERLINK.LOG`
- `runs/vmctl-vc152dfser4/DFSERR.TXT`
