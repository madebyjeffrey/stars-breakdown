# Cyber freighter progress-bit evidence

The typed `DoCyberFreighter` reconstruction now advances the drop-off
progress field only for carrying/drop-off orders, matching the raw branch. A
pickup order no longer consumes drop-off progress. The owned-planet
no-destination path still rolls back supply and advances the separate
planet-progress field.

The strict host C89 probe reports:

`DO_CYBER_FREIGHTER_PROGRESS_CHECKS=9 FAILURES=0 RESULT=PASS`

It covers pickup without drop-off-bit mutation, successful drop-off progress,
and carrying-without-destination rollback with the expected two supply
transfers and planet-progress increment.

This is host source-fidelity evidence only. Full raw mineral-rate/stock
predicates, native replay of the corrected progress behavior, and
original-turn differential remain open; no authoritative Step-5 or promotion
count changes.

Evidence:

- `reconstructed/src/do_cyber_freighter.c`
- `tests/do_cyber_freighter_progress_probe.c`
- `tests/do_cyber_freighter_source_probe.c`
- `analysis/decompiler/export/raw/10a8_37b0_DoCyberFreighter.c`
