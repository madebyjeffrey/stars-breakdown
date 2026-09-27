# DoCyberAiTurn route-plan boundary

The typed Cyber reconstruction now separates fleet action planning from the
typed fleet visitor. The planner receives the fleet-role policy and armada
potency and can expose the shared clear-task, movement, colonization,
wormhole, starbase, attack, armada, transport, and freighter action families.
The applier receives the exact plan only for a non-empty action; a no-op plan
proceeds directly to the visitor and the legacy fleet callback remains after
the typed path. Each action now dispatches through its named callback when
supplied, with the generic applier retained as a compatibility fallback; the
probe exercises the specialized colonize callback.

The strict host C89 probe reports:

`CYBER_AI_ROUTE_PLAN_BOUNDARY_CHECKS=14 FAILURES=0 RESULT=PASS`

It verifies raw first-order repair before planning, policy and potency
forwarding, colonizer plan fields, no-op suppression, apply payload
preservation, visitor ordering, non-owned filtering, and planner/applier
failure propagation.

This host-only source checkpoint closes no authoritative Step-5 unit. Exact
Cyber route predicates, packet/freighter behavior, multi-order packed `PLORD`
mutation, native replay, and original-turn differential remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_cyber_ai_turn.c`
- `reconstructed/src/ai_plord.c`
- `tests/do_cyber_ai_turn_route_plan_boundary_probe.c`
- `analysis/decompiler/export/raw/10a8_002a_DoCyberAiTurn.c`
