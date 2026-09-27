# DoAutomitronAiTurn action dispatch matrix — E5 host support

The complete Automitron caller now passes each non-empty fleet plan through
its named callback for clear-task, movement, colonization, wormhole, starbase,
attack, armada targeting, transport, or freighter targeting. A strict-C89
host matrix selects all nine action values in one turn and checks the exact
callback, complete plan payload, owned-fleet filter, and subsequent visitor.

Host result:

`AUTOMITRON_AI_ACTION_MATRIX_CHECKS=20 FAILURES=0 RESULT=PASS`

This is host-only supporting evidence. Native replay, exact raw target
predicates, packed order writes, and the original-turn differential remain
open; it closes no named Step-5 unit.

Evidence:

- `reconstructed/src/do_automitron_ai_turn.c`
- `reconstructed/include/core.h`
- `tests/do_automitron_ai_turn_action_matrix_probe.c`
- `analysis/decompiler/export/raw/1098_01e0_DoAutomitronAiTurn.c`
