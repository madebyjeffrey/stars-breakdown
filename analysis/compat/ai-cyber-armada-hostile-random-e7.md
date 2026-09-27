# Cyber armada hostile random-gate evidence

The typed weak-hostile branch in `TargetCyberArmada` now preserves the raw
short-circuit random-call order. It does not consume a first roll when the
first damage threshold already makes that clause true, and it evaluates the
second and third rolls only when their corresponding thresholds require them.

The strict host C89 probe reports:

`TARGET_CYBER_ARMADA_HOSTILE_RANDOM_CHECKS=20 FAILURES=0 RESULT=PASS`

It covers zero-roll retreat, one-roll rejection, two-roll rejection, and the
three-roll retreat path, including the expected starbase destination callback,
movement, task-clear, and ordinary-destination fall-through behavior.

This is host source-fidelity evidence only. Fresh native replay of the
corrected random-call sequence and original-turn differential remain open; no
authoritative Step-5 or promotion count changes.

Evidence:

- `reconstructed/src/target_cyber_armada.c`
- `tests/target_cyber_armada_hostile_random_probe.c`
- `analysis/decompiler/export/raw/10a8_51a4_TargetCyberArmada.c`
