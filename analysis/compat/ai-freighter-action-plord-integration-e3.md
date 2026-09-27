# AI freighter target to packed PLORD integration

The reviewed `IdTargetFreighter` boundary now has a focused host integration
through the shared packed-order adapter. The probe supplies two eligible
planet candidates, routes the real typed target-selection result through a
`StarsMoveOrder`, and verifies that the higher-scoring candidate becomes the
packed move target.

The strict host C89 probe reports:

`AI_FREIGHTER_ACTION_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`

It covers strict score ordering, target identity and point forwarding, the
exact `0x0100` move flags, first-order replacement, packed record preservation,
and the cargo-free rejection/sentinel path.

This is a host-only typed action-to-packed-record checkpoint. The raw function
has additional original behavior around AI markings, cargo calculations,
enemy/unknown classification, and candidate eligibility; those details still
need exact source treatment, native replay, personality integration, and
original-turn differential evidence. Therefore it closes no authoritative
Step-5 unit.

Evidence:

- `reconstructed/src/ai.c`
- `reconstructed/src/ai_plord.c`
- `tests/ai_freighter_action_plord_integration_probe.c`
- `tests/id_target_freighter_source_probe.c`
- `analysis/decompiler/export/raw/1090_286c_IdTargetFreighter.c`
