# Shared AI PLORD repair boundary

The recovered packed layout is a four-byte `PLORD` header followed by
18-byte order records. A shared host helper now models the raw missing-first-
order repair used by the AI personalities: it sets `iordMac` to one, changes
the first order's low kind nibble to six, writes the two recovered repair words
to the first record, and preserves the header maximum, the first record's
coordinates/target bytes, and all later order bytes.

The Robotoid repair callback exercises this helper through the actual typed
turn boundary. The strict host C89 result is:

`AI_PLORD_REPAIR_SOURCE_CHECKS=9 FAILURES=0 RESULT=PASS`

The helper is shared by the Robotoid, Turin, and Cyber repair boundaries, but
this remains a host-only mutation checkpoint. It does not close the broader
Step-5 raw-PLORD unit: multi-order routing, colonizer/transport/attack action
selection, native replay, and original-turn comparison remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/ai_plord.c`
- `reconstructed/src/do_robotoid_ai_turn.c`
- `reconstructed/src/do_turin_drone_ai_turn.c`
- `reconstructed/src/do_cyber_ai_turn.c`
- `tests/ai_plord_repair_source_probe.c`
- `tests/do_robotoid_ai_turn_repair_boundary_probe.c`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c`
