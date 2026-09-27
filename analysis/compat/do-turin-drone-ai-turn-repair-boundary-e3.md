# DoTurinDroneAiTurn order-repair boundary

Superseded for predicate accuracy by
`analysis/compat/do-turin-drone-ai-turn-repair-gate-e4.md`. E3 recorded the
initial broad host seam; E4 tightens it to the raw fleet-slot gate.

The typed Turin turn now dispatches an explicit `repair_order` service for a
friendly fleet with ships, `cord < 2`, and no first order, before the normal
typed fleet visitor. Ordered fleets and non-owned fleets bypass the callback;
a callback failure aborts the turn.

The strict host C89 probe reports:

`TURIN_DRONE_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS`

This exposes the raw phase boundary without claiming packed `PLORD` mutation,
exact route selection, native replay, or original-turn differential evidence.
No Step-5 unit is closed.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_turin_drone_ai_turn.c`
- `tests/do_turin_drone_ai_turn_repair_boundary_probe.c`
- `analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c`
