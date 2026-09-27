# DoCyberFreighter reconstruction

`reconstructed/src/do_cyber_freighter.c` is a typed semantic reconstruction of Cyber freighter behavior, preserving target selection, movement/order state, cargo/action gates, and resulting fleet updates.

Validation: `tests/do_cyber_freighter_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DCFSTA3.TXT` and `runs/vmctl-vc152ew/DCFRES.TXT`.

The corrected mineral-rate/stock gate, repeated-turn progress bits, enemy and
special-planet branches, open-space short circuit, and movement-to-packed-order
boundary now have a fresh combined native replay. The strict-C89 and VC++
1.52c/Win98 probes report
`DO_CYBER_FREIGHTER_E5_CHECKS=26 FAILURES=0 RESULT=PASS`; compile/link and
process exit passed, and no Program Error dialog appeared. This strengthens
the source boundary but does not close complete raw fleet-cargo behavior,
packet/freighter interaction, the full Cyber AI turn, or original-turn
differential. Evidence is in
`analysis/compat/ai-cyber-freighter-native-e5.{md,json}` and
`runs/vmctl-vc152cybfe5/`.
