# DumpPlanets original executable behavior E4

The original Win98 executable was exercised through the complete report path:

`Reports > Dump to Text File > Planet Information`

It wrote `C:\STARS!\GAME.pla` (334 bytes), displayed the success message, and
completed without a Program Error. The artifact hash and screenshots are
preserved in `analysis/compat/report-dump-integration.json` and
`runs/vmctl-vc152current/`.

The production reconstruction now also has independent exact host/native
evidence for the resulting normal stream: E2 in `C:\DPSOR2` reports
`DUMP_PLANETS_SERIALIZATION_CHECKS=14 FAILURES=0 RESULT=PASS`, matching the
preserved header, both rows, tab separators, and CRLF terminators byte-for-byte.

Together with E1 localized normal serialization and E3 packed service
resolution, this closes all four named `DumpPlanets` Step-5 acceptance units.
The review ledger, TODO5/TODO6, coverage, and reconstruction notes are
synchronized; `DumpPlanets` is promoted to Phase 2 **6/6**.

Evidence:

- `analysis/compat/report-dump-integration.md`
- `analysis/compat/report-dump-integration.json`
- `analysis/compat/dump-planets-serialization-e1.{md,json}`
- `analysis/compat/dump-planets-original-output-e2.{md,json}`
- `analysis/compat/dump-planets-original-behavior-e4.{md,json}`
- `runs/vmctl-vc152current/GAME.pla`
- `runs/vmctl-vc152current/planet-dump-save.bmp`
- `runs/vmctl-vc152dpsor2/DPSR.TXT`
