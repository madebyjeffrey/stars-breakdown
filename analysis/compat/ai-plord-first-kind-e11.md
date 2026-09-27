# AI PLORD first-order kind writes — E11 host support

The raw Robotoid listing contains repeated branches that change only the low
kind nibble of the first packed order word, using `(word & 0xfff0) | 5` or
`(word & 0xfff0) | 6`. A shared host helper now models that operation without
resetting the packed queue count or overwriting coordinates, target, repair,
or later-order bytes. The existing first-order repair continues to use the
same helper and still writes its separately recovered count and repair words.

The strict-C89 host probe reports:

`AI_PLORD_FIRST_KIND_SOURCE_CHECKS=12 FAILURES=0 RESULT=PASS`

The existing repair regression also remains green:

`AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=0 RESULT=PASS`

The repository regression after the header/source change reports `Ran 519
tests`, `OK (skipped=153)`, with 2,241 generated layout assertions and 846
function declarations. `python3 tools/baseline.py --check` and `git diff
--check` also pass.

This is supporting source evidence only. It does not claim native validation,
complete Robotoid route selection, or original-turn equivalence, and closes
no named Step-5 unit. The Win98 guest agent is still disconnected.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/ai_plord.c`
- `tests/ai_plord_first_kind_source_probe.c`
- `tests/ai_plord_repair_source_probe.c`
- `analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c`
