# Shared AI PLORD move-order adapter

The shared AI packed-order layer now includes
`stars_ai_move_order_to_plord`, the direct 18-byte record copy used by the
reviewed `FMoveAiFleet` reconstruction after `FColonizeAiFleet`,
`FGotoWormholeAiFleet`, or a personality-specific target helper constructs a
`StarsMoveOrder`. It preserves the four-byte allocation header, selects the
first or tail record according to the append flag, compares packed endpoints,
copies all nine order words, updates `iordMac`, and rejects a full allocation
without mutation.

The strict host C89 probe reports:

`AI_PLORD_MOVE_ORDER_CHECKS=12 FAILURES=0 RESULT=PASS`

The existing reviewed source probes also remain green:

- `F_MOVE_AI_FLEET_CHECKS=2 FAILURES=0 RESULT=PASS`
- `F_COLONIZE_AI_FLEET_CHECKS=1 FAILURES=0 RESULT=PASS`

This is a host-only packed mutation checkpoint. It does not yet close an
authoritative Step-5 unit: personality-specific target searches, complete
multi-order route decisions, native replay, and original-turn differential
remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/ai_plord.c`
- `reconstructed/src/ai.c`
- `tests/ai_plord_move_order_probe.c`
- `tests/f_move_ai_fleet_source_probe.c`
- `tests/f_colonize_ai_fleet_source_probe.c`
- `analysis/decompiler/export/raw/1090_3d0a_FMoveAiFleet.c`
- `analysis/decompiler/export/raw/1090_0c78_FColonizeAiFleet.c`
