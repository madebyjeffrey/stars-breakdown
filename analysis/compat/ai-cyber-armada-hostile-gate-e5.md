# Cyber armada hostile-planet gate evidence

The raw `TargetCyberArmada` control flow requires a strong hostile current
planet to stop before ordinary destination selection. The typed reconstruction
now preserves that gate instead of falling through to a normal planet target.

The strict host C89 probe reports:

`TARGET_CYBER_ARMADA_HOSTILE_GATE_CHECKS=4 FAILURES=0 RESULT=PASS`

It verifies the current-planet lookup, absence of destination enumeration,
absence of movement, and absence of task clearing for a strong hostile
planet. The neutral-planet route and weak-hostile branches remain covered by
the existing probes.

This is host source-fidelity evidence only. Fresh native replay of the
corrected source, the full weak-hostile/random branch matrix, and original-turn
differential remain open; no authoritative Step-5 or promotion count changes.

Evidence:

- `reconstructed/src/target_cyber_armada.c`
- `tests/target_cyber_armada_hostile_gate_probe.c`
- `analysis/decompiler/export/raw/10a8_51a4_TargetCyberArmada.c`
