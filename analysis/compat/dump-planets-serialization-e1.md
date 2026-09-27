# DumpPlanets localized normal serialization E1

The production `stars_dump_planets` coordinator now exposes the recovered
twenty-column normal export through an explicit report-host formatter seam.
The coordinator owns callback order and row termination; the report host owns
localized planet/owner/starbase/queue text and number formatting.

The strict C89 host probe reports:

`DUMP_PLANETS_SERIALIZATION_CHECKS=12 FAILURES=0 RESULT=PASS`

It checks the raw callback field sequence over two rows, formatted callback
invocations over all forty fields, localized names/queue/value/defence text,
the preserved CRLF/tab layout of the two-row `GAME.pla` oracle, and rejection
when no row or text callback path is supplied.

A fresh VC++ 1.52c / Win98 packet in `C:\DPSER1` reports the same 12/12 result.
Compile and link passed, the process exited 0, and the final guest window
inventory contained no Program Error dialog. The preserved original artifact
used for the normal-row oracle is `runs/vmctl-vc152current/GAME.pla`.

This closes the named localized normal-serialization E1 unit. Complete
serialization, detailed/service-column coverage, byte-for-byte original
export comparison, and Step-6 promotion remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/dump_planets.c`
- `tests/dump_planets_serialization_probe.c`
- `compat/vc152/dump-planets-serialization-e1/`
- `runs/vmctl-vc152dpser1/DPSSTA.TXT`
- `runs/vmctl-vc152dpser1/DPSCL.LOG`
- `runs/vmctl-vc152dpser1/DPSLINK.LOG`
- `runs/vmctl-vc152dpser1/DPSR.TXT`
- `runs/vmctl-vc152current/GAME.pla`
