# Cyber armada target to packed PLORD integration

The reviewed `TargetCyberArmada` boundary now has a focused host integration
through the shared packed-order adapter. The probe covers both principal
target forms: an ordinary planet destination and an enemy-fleet fallback.

The strict host C89 probe reports:

`AI_CYBER_ARMADA_PLORD_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`

It verifies destination precedence, target kind and identity, point
forwarding, planet marking, exact packed order flags (`0x1140` for a planet
and `0x1240` for a fleet), and first-order replacement while preserving the
18-byte order record.

This is a host-only action-to-record checkpoint. The existing native evidence
for `TargetCyberArmada` remains separate from this new packed-record replay;
complete original-table state, all stale-order and potency branches, native
replay of this integration, and original-turn differential evidence remain
open. Therefore it does not promote or close an authoritative Step-5 unit.

Evidence:

- `reconstructed/src/target_cyber_armada.c`
- `reconstructed/src/ai_plord.c`
- `tests/ai_cyber_armada_plord_integration_probe.c`
- `tests/target_cyber_armada_source_probe.c`
- `analysis/decompiler/export/raw/10a8_51a4_TargetCyberArmada.c`
