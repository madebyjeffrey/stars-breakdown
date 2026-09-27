# Cyber armada current-order retention evidence

The typed `TargetCyberArmada` reconstruction now preserves the raw
current-order branch distinction recovered from the original procedure. The
previous early return treated every nearby or non-planet order alike; the
corrected source retains valid planet orders for validation, abandons stale
planet orders, retains nearby fleet orders, and allows distant fleet orders to
continue into target selection.

The strict host C89 regression probe reports:

`TARGET_CYBER_ARMADA_ORDER_RETENTION_CHECKS=12 FAILURES=0 RESULT=PASS`

It covers valid near-planet continuation, stale-owner rejection, nearby-fleet
retention without lookup, and distant-fleet continuation into ordinary planet
selection. The existing two-check source probe and 12-check packed-order
integration probe remain green.

This is a host source-fidelity checkpoint. Because the corrected branch still
needs fresh VC++ 1.52c/native replay and original-turn comparison, it closes
no authoritative Step-5 unit and does not change promotion counts.

Evidence:

- `reconstructed/src/target_cyber_armada.c`
- `tests/target_cyber_armada_order_retention_probe.c`
- `tests/target_cyber_armada_source_probe.c`
- `tests/ai_cyber_armada_plord_integration_probe.c`
- `analysis/decompiler/export/raw/10a8_51a4_TargetCyberArmada.c`
