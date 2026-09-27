# DoAutomitronAiTurn packed repair boundary — E3 host support

The raw decompiler's structured C appears to put a `cord == 1` repair test in
the `else` arm of `cord < 2`. Inspection of the original instruction listing
resolves this as a decompiler control-flow structuring error: at
`analysis/decompiler/export/raw/1098_01e0.asm:1235-129e`, the executed path
checks `rgcsh[6]` at fleet offset `0x18` is nonzero, then checks fleet word
`[BX+0x62] == 1`, then the packed first-order low nibble at
`[PLORD+0x0a] == 0`, and writes kind 6 plus the two `0x0005` words before
calling `FLookupFleet`. The host probe exercises the corresponding cord-one /
empty-kind packed repair seam before the typed fleet visitor.

The strict-C89 host probe reports:

`AUTOMITRON_AI_REPAIR_BOUNDARY_CHECKS=8 FAILURES=0 RESULT=PASS`

It checks owned-fleet filtering, the raw slot-6 gate (a slot-2-only fleet does
not repair), the cord/order predicate, packed mutation, callback ordering,
ordered-fleet bypass, empty-fleet bypass, and callback failure propagation.

After the source correction, Automitron source, production, integration, route,
and action-matrix probes report 31, 12, 20, 14, and 20 checks passed. The full
repository regression reports `Ran 519 tests`, `OK (skipped=153)`, with 2,241
generated layout assertions and 846 function declarations. Baseline and diff
checks pass.

This remains supporting host evidence only and closes no named Step-5 unit:
native replay, complete Automitron route/order behavior, and original-turn
comparison remain open; the Win98 guest agent is still disconnected.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_automitron_ai_turn.c`
- `tests/do_automitron_ai_turn_repair_boundary_probe.c`
- `reconstructed/src/ai_plord.c`
- `analysis/decompiler/export/raw/1098_01e0_DoAutomitronAiTurn.c`
- `analysis/decompiler/export/raw/1098_01e0.asm`
