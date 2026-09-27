# DumpPlanets complete original-output E2

The production `stars_dump_planets` formatter boundary now reproduces the
complete preserved normal `GAME.pla` stream: header, both planet rows, tabs,
and CRLF terminators. The strict-C89 host probe reports:

`DUMP_PLANETS_SERIALIZATION_CHECKS=14 FAILURES=0 RESULT=PASS`

The matrix retains the raw/formatted callback checks and adds an exact
full-stream comparison against the preserved two-row original artifact at
`runs/vmctl-vc152current/GAME.pla`.

A fresh VC++ 1.52c / Win98 packet in `C:\DPSOR2` reports the same 14/14 result.
Compile and link passed, the process exited 0, and the final guest window
inventory contained no Program Error dialog.

This closes the named complete serialized-output E2 unit. The localized normal
serialization E1 and packed-service E3 units are already closed. Detailed or
service-column variants, original executable behavior beyond the preserved
export, and Step-6 promotion remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/dump_planets.c`
- `tests/dump_planets_original_output_probe.c`
- `compat/vc152/dump-planets-original-output-e2/`
- `runs/vmctl-vc152dpsor2/DPSSTA.TXT`
- `runs/vmctl-vc152dpsor2/DPSCL.LOG`
- `runs/vmctl-vc152dpsor2/DPSLINK.LOG`
- `runs/vmctl-vc152dpsor2/DPSR.TXT`
- `runs/vmctl-vc152current/GAME.pla`
