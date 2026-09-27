# DoRobotoidAiTurn repair eligibility and route ordering — E4 host support

The raw procedure's late-turn repair branch is reached only for an owned
fleet with slot 0 nonzero, turn at least `0x29` (41), `cord == 1`, and an empty
first-order kind. The original listing also attempts a `Random(5)` nearby
route before the repair; a selected route jumps past the repair. The typed
policy now exposes the deterministic eligibility, asks the route planner first,
and repairs only when that planner returns no action. The exact random draw,
nearby-planet selection, and RNG-consumption behavior remain explicit open
work rather than being claimed as reconstructed.

The raw C shows the candidate gates and random-route bypass at
`analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c:476-588`. The
instruction listing at `analysis/decompiler/export/raw/1088_0312.asm:130d-136f`
checks the first-order nibble, writes kind 6 and the two `0x0005` words, then
calls `FLookupFleet`.

Strict-C89 probes pass:

- `ROBOTOID_AI_REPAIR_BOUNDARY_CHECKS=7 FAILURES=0 RESULT=PASS`
- `ROBOTOID_AI_ROUTE_PLAN_BOUNDARY_CHECKS=18 FAILURES=0 RESULT=PASS`
- Source, phase, production, packed integration, fleet-gate, and typed-fleet
  probes pass at 23, 6, 12, 18, 10, and 2 checks.
- Full suite: 519 passed, 153 expected skips; 2,241 generated layout
  assertions and 846 declarations. Baseline and diff checks pass.

This is host support only and closes no Step-5 evidence unit. The random-route
predicate and draw consumption, complete personality/table integration,
native VC++ 1.52c replay of this source revision, and original-turn differential
remain open.

Evidence:

- `reconstructed/src/do_robotoid_ai_turn.c`
- `tests/do_robotoid_ai_turn_repair_boundary_probe.c`
- `tests/do_robotoid_ai_turn_route_plan_boundary_probe.c`
- `analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c`
- `analysis/decompiler/export/raw/1088_0312.asm`
