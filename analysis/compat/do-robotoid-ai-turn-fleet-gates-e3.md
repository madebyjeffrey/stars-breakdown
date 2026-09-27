# DoRobotoidAiTurn fleet-gate matrix

The repair-eligibility expectations in this E3 note are superseded by E4 at
`analysis/compat/do-robotoid-ai-turn-repair-gate-e4.md`.

The strict host C89 probe exercises the typed fleet inputs that feed the
remaining Robotoid order-repair and routing seam.  It covers an empty friendly
fleet, colonizer and transport fleets with no order, an already ordered armed
fleet, separate support/attack design bins, the `warship_count` fallback, and
the non-owned-fleet filter.

The probe reports:

`ROBOTOID_AI_FLEET_GATE_CHECKS=10 FAILURES=0 RESULT=PASS`

This is a host-only source boundary.  It confirms that the reconstruction
exposes the raw decision inputs without conflating them with the still-missing
actions.  It does not close a Step-5 evidence unit: direct `PLORD` mutation,
the exact colonizer/transport/attack route choices, complete AI-table state,
and native/original-turn differential evidence remain open.

Evidence:

- `tests/do_robotoid_ai_turn_fleet_gate_probe.c`
- `reconstructed/src/do_robotoid_ai_turn.c`
- `reconstructed/include/core.h`
- `analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c`
