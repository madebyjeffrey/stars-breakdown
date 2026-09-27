# DoTurinDroneAiTurn route-plan boundary

The typed Turin reconstruction now separates fleet action planning from the
typed fleet visitor. The planner receives the fleet-role policy and turn and
can expose the shared clear-task, movement, colonization, wormhole, starbase,
attack, armada, transport, and freighter action families. The applier receives
the exact plan only for a non-empty action; a no-op plan proceeds directly to
the visitor. Each action now dispatches through its named callback when
supplied, with the generic applier retained as a compatibility fallback; the
probe exercises the specialized colonize callback.

The strict host C89 probe reports:

`TURIN_DRONE_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`

It verifies raw first-order repair before planning, policy and turn forwarding,
colonizer plan fields, no-op suppression, apply payload preservation, visitor
ordering, non-owned filtering, and planner/applier failure propagation.

This host-only source checkpoint closes no authoritative Step-5 unit. Exact
Turin route predicates, multi-order packed `PLORD` mutation, native replay, and
original-turn differential remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_turin_drone_ai_turn.c`
- `reconstructed/src/ai_plord.c`
- `tests/do_turin_drone_ai_turn_route_plan_boundary_probe.c`
- `analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c`
