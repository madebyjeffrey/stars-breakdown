# DoRobotoidAiTurn order-repair boundary

Superseded for predicate and ordering by
`analysis/compat/do-robotoid-ai-turn-repair-gate-e4.md`. E3 recorded the
initial broad `cord < 2` seam; E4 matches the late-turn slot-0/cord-one gate
and places repair after a route-plan no-op.

The typed Robotoid reconstruction now dispatches an explicit repair callback
for a friendly fleet whose first order is missing. The callback receives the
recovered fleet-role policy and the turn potency values, and it runs before
the existing typed fleet visitor. Already ordered fleets and fleets with no
ships bypass the repair callback. A callback failure aborts the turn at the
same boundary.

The strict host C89 probe reports:

`ROBOTOID_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS`

This is a source-level service boundary, not a claim that the packed `PLORD`
record has been fully reconstructed. Direct order-word mutation, exact route
selection, and native/original-turn evidence remain open, so no Step-5 unit is
closed by this milestone.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_robotoid_ai_turn.c`
- `tests/do_robotoid_ai_turn_repair_boundary_probe.c`
- `analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c`
