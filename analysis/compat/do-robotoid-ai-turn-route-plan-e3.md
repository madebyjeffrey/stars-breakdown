# DoRobotoidAiTurn route-plan boundary

The E3 event-order description is historical and is superseded by E4 at
`analysis/compat/do-robotoid-ai-turn-repair-gate-e4.md`: route planning now
precedes repair, and a selected route bypasses repair.

The typed Robotoid reconstruction now exposes the fleet route-action family
as a separate plan/apply boundary. The planner receives the recovered fleet
role policy and turn, and can select clear-task, movement, colonization,
wormhole, starbase movement, attack, armada targeting, transport, or
freighter targeting. The applier receives the exact typed plan only after a
non-empty action has been selected; a no-op plan does not invoke it. The
existing typed fleet visitor remains after plan/apply, so later order mutation
and native replay can be added without merging decision and mutation logic.
Each action now dispatches through its named callback when supplied, with the
generic applier retained as a compatibility fallback; the probe exercises the
specialized colonize callback.

The strict host C89 probe reports:

`ROBOTOID_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`

It verifies raw first-order repair before planning, policy and turn forwarding,
colonizer plan fields, no-op suppression, apply payload preservation, visitor
ordering, non-owned filtering, and planner/applier failure propagation.

This is a host-only source checkpoint and closes no authoritative Step-5 unit.
The exact original route predicates, multi-order packed `PLORD` mutation,
complete AI-table state, native replay, and original-turn differential remain
open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_robotoid_ai_turn.c`
- `reconstructed/src/ai_plord.c`
- `tests/do_robotoid_ai_turn_route_plan_boundary_probe.c`
- `analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c`
