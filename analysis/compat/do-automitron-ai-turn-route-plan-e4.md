# DoAutomitronAiTurn route plan/apply boundary — E4 host support

The readable Automitron caller now separates fleet route planning from route
application, matching the boundary already established for the other AI
personalities. It carries action, target, order kind, append mode, and reason
through named callbacks for clear-task, movement, colonization, wormholes,
starbases, attacks, armada/freighter targeting, and transport. Specialized
callbacks take precedence over the generic applier.

The strict-C89 host probe reports:

`AUTOMITRON_AI_ROUTE_PLAN_CHECKS=14 FAILURES=0 RESULT=PASS`

It checks owned-fleet filtering, colonize/move payload preservation, callback
ordering, and planner/applier failure propagation through the generic action
applier boundary.

The full repository regression after this public-header/source change reports
`Ran 519 tests`, `OK (skipped=153)`, with 2,241 generated layout assertions
and 846 function declarations. Baseline and diff checks pass.

This remains supporting host evidence only. The raw Automitron target
predicates, complete packed-order mutation, native replay, and original-turn
differential remain open; no Step-5 unit or Step-6 promotion is claimed.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_automitron_ai_turn.c`
- `tests/do_automitron_ai_turn_route_plan_probe.c`
- `analysis/decompiler/export/raw/1098_01e0_DoAutomitronAiTurn.c`
