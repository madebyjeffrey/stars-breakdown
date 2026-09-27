# AI fleet action to packed PLORD integration

The reviewed `FColonizeAiFleet` and `FGotoWormholeAiFleet` constructors now
have a focused host integration through the shared packed-order adapter. The
probe supplies real planet/wormhole inputs, routes the constructors' real
`StarsMoveOrder` values through `stars_ai_move_order_to_plord`, and verifies
the resulting 18-byte record plus the fleet movement flag.

The strict host C89 probe reports:

`AI_FLEET_ACTION_PLORD_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`

It covers colonizer target/flag construction, wormhole target/flag
construction, ideal-warp nibble placement, first-order replacement, order
count preservation, selection forwarding, and the movement-success fleet flag
for both actions.

This is a host-only action-to-packed-record checkpoint and closes no
authoritative Step-5 unit. Personality-specific target searches, exact route
precedence, native replay, and original-turn differential remain open.

Evidence:

- `reconstructed/src/ai.c`
- `reconstructed/src/ai_plord.c`
- `reconstructed/include/core.h`
- `tests/ai_fleet_action_plord_integration_probe.c`
- `analysis/decompiler/export/raw/1090_0c78_FColonizeAiFleet.c`
- `analysis/decompiler/export/raw/1090_0d5c_FGotoWormholeAiFleet.c`
