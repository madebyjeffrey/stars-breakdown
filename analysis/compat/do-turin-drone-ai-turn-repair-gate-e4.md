# DoTurinDroneAiTurn exact order-repair gate — E4 host support

The prior typed policy repaired an empty first order for any nonempty fleet
with `cord < 2`. The raw listing at
`analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c:675-693`
shows the repair branch is narrower: slots 2, 3, 1, 8, 9, 13, 14, 0, 10, and
11 must be zero; slot 12 must be nonzero; `cord == 1`; and the first-order
kind nibble must be zero. The readable policy now uses this exact predicate.

The strict-C89 repair-boundary probe reports:

`TURIN_DRONE_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS`

Its 15-fleet matrix checks one valid slot-12 repair case, each excluded slot
individually (0, 1, 2, 3, 8, 9, 10, 11, 13, 14), missing slot 12, wrong cord,
nonempty first-order kind, and foreign ownership. The existing
Turin source, packed integration, route-plan, and edge probes pass at 35, 20,
14, and 7 checks. The full repository suite reports `Ran 519 tests`,
`OK (skipped=153)`; generated headers remain at 2,241 layout assertions and
846 declarations. `baseline.py --check` and `git diff --check` pass.

This is host evidence only and closes no named Step-5 unit. Native replay,
complete packed route/order mutation, broader AI-table integration, and the
original-turn differential remain open; the VMCTL1 guest agent has not
connected.

Evidence:

- `reconstructed/src/do_turin_drone_ai_turn.c`
- `tests/do_turin_drone_ai_turn_repair_boundary_probe.c`
- `tests/do_turin_drone_ai_turn_source_probe.c`
- `tests/do_turin_drone_ai_turn_integration_probe.c`
- `tests/do_turin_drone_ai_turn_route_plan_boundary_probe.c`
- `analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c`
