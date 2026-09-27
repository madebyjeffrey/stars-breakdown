# AI starbase action to packed PLORD integration

The reviewed `FMoveToNearestStarbase` constructor now has a focused host
integration through the shared packed-order adapter. The probe supplies the
closest-starbase and planet-point services, routes the real `StarsMoveOrder`
through `stars_ai_move_order_to_plord`, and verifies the replacement order and
the no-starbase rejection path.

The strict host C89 probe reports:

`AI_STARBASE_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`

It covers starbase lookup forwarding, point lookup, replacement mode, target
identity, exact `0x1040` order flags, packed record preservation, and the
no-target short circuit.

This is a host-only action-to-packed-record checkpoint and closes no
authoritative Step-5 unit. Personality-specific target searches, route
precedence, native replay, and original-turn differential remain open.

Evidence:

- `reconstructed/src/ai.c`
- `reconstructed/src/ai_plord.c`
- `tests/ai_starbase_action_plord_integration_probe.c`
- `tests/f_move_to_nearest_starbase_source_probe.c`
- `analysis/decompiler/export/raw/1090_6f7e_FMoveToNearestStarbase.c`
