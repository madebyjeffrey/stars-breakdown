# DoCyberAiTurn order-repair boundary

The typed Cyber turn now dispatches an explicit `repair_order` service for a
friendly fleet with ships, `cord < 2`, and no first order, before the normal
typed fleet visitor. The probe also checks the five raw merge masks remain in
their existing order. Ordered fleets and non-owned fleets bypass the callback;
a callback failure aborts the turn.

The strict host C89 probe reports:

`CYBER_AI_REPAIR_BOUNDARY_CHECKS=6 FAILURES=0 RESULT=PASS`

This exposes the raw phase boundary without claiming packed `PLORD` mutation,
packet/freighter routing, native replay, or original-turn differential
evidence. No Step-5 unit is closed.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_cyber_ai_turn.c`
- `tests/do_cyber_ai_turn_repair_boundary_probe.c`
- `analysis/decompiler/export/raw/10a8_002a_DoCyberAiTurn.c`
