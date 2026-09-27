# Cyber armada friendly random-gate evidence

The typed `TargetCyberArmada` friendly-armada gate now matches the raw
short-circuit structure. When a weak friendly armada's first random roll does
not enter the raw gate, routing continues; when the roll enters the gate below
the damage ceiling, the armada remains idle. The recovered second random roll
for damage above the three-times-potency threshold is also preserved.

The strict host C89 probe reports:

`TARGET_CYBER_ARMADA_FRIENDLY_RANDOM_CHECKS=9 FAILURES=0 RESULT=PASS`

It covers fall-through at roll `0`, early return at roll `5` below the damage
ceiling, and the two-roll high-damage path.

This is host source-fidelity evidence only. The complete native random matrix,
weak-hostile branch matrix, and original-turn differential remain open; no
authoritative Step-5 or promotion count changes.

Evidence:

- `reconstructed/src/target_cyber_armada.c`
- `tests/target_cyber_armada_friendly_random_probe.c`
- `analysis/decompiler/export/raw/10a8_51a4_TargetCyberArmada.c`
