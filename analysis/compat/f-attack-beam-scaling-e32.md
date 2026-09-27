# FAttack group-0x10 beam scaling host probe — E32

E32 adds a strict-C89 probe around the production `stars_f_attack` path for
the selected group-0x10 beam's post-selection damage amount and raw damage
class. It is supporting Step-5 evidence only: it closes none of FAttack's
three named Step-5 units and does not promote the procedure.

## Original-instruction boundary

The raw decompilation at
`analysis/decompiler/export/raw/10f0_69ac_FAttack.c:515-561` shows the selected
beam amount being scaled in this order:

1. Multiply by attacker `pctCap / 100` only when `pctCap` is nonzero.
2. Multiply by target `pctBeamDef / 100` when the value is below 100.
3. When target distance and weapon-part `dp` are positive, compute
   `distance * 10 / dp`, then multiply damage by the remaining percentage.
4. Select raw `FDamageTok` damage class 1 for `dp < 200`, otherwise class 2.

The probe passes the section through the same packed-part resolver and attack
candidate loop as production. It does not call a private copy of the scaling
formula.

## Host evidence

`tests/f_attack_beam_scale_probe.c` runs seven cases and expects:

| Case | Capacity | `dp` | Distance | Beam defense | Expected dispatch |
|---|---:|---:|---:|---:|---|
| Ordinary compound scaling | 50 | 50 | 10 | 80 | 39 damage, class 1 |
| Zero `pctCap` bypass | 0 | 50 | 0 | 100 | 100 damage, class 1 |
| Truncating falloff order | 100 | 50 | 5 | 50 | 49 damage, class 1 |
| Ordinary distance loss | 100 | 50 | 20 | 100 | 96 damage, class 1 |
| Class boundary | 100 | 200 | 0 | 100 | 100 damage, class 2 |
| Zero beam defense | 50 | 50 | 1 | 0 | no dispatch |
| Falloff beyond 100% | 100 | 50 | 501 | 100 | no dispatch |

`python3 -m unittest tests.test_f_attack_beam_scale -v` passes. The fresh
VC++ 1.52c packet in `compat/vc152/f-attack-complex-native-e32/` built and
ran in `C:\FAPNE32`. `FAPNATSTA.TXT` records `COMPILE=PASS` and `LINK=PASS`;
`BEAMR.TXT` records `F_ATTACK_BEAM_SCALE_CHECKS=7 FAILURES=0 RESULT=PASS`.
The process exited 0, and the post-run window inventory showed no Program
Error dialog. Build, compile, link, and native report evidence is preserved in
`runs/vmctl-vc152fapne32/`. The compiler reported a harmless duplicate
`BEAM_SCALE_NATIVE` macro definition in the harness; it did not affect the
build or result.

## Remaining FAttack work

E1 still requires complete raw complex-beam and torpedo battle-part scaling
plus full table/service acceptance. E2 broader original damage differentials
and E3 full native battle replay/differential remain open. Current progress is
still **0/3 named Step-5 units, 5/6 overall**.
