# FAttack simple/broadcast beam falloff boundary — E33

E33 corrects a behavioral mismatch in the readable FAttack source. The raw
group-0x10 broadcast/simple-beam path applies capacity scaling and per-target
beam-defense scaling, but no distance-loss multiplier. Complex target
selection applies the distance falloff after a target is selected. Previously
the readable implementation routed both branches through the same distance
falloff calculation.

## Original instruction evidence

In `analysis/decompiler/export/raw/10f0_69ac_FAttack.c:138-221`, the
`group == 0x10 && (part.rghs->grhst & 2) != 0` branch computes part power,
applies attacker `pctCap` when nonzero, applies each eligible target's
`pctBeamDef`, then dispatches `FDamageTok`. It performs a distance/range gate
before that dispatch, but no `distance * 10 / part.dp` damage reduction. The
complex group-0x10 branch at `:515-561` separately computes distance loss and
scales the selected target's damage.

The readable source now passes an explicit branch-policy argument to the beam
damage helper: the complex target-selection path enables falloff and the
broadcast/simple-beam path disables it. Capacity, beam defense, and damage
class derivation remain shared.

## Validation

`tests/f_attack_beam_scale_probe.c` and
`python3 -m unittest tests.test_f_attack_beam_scale -v` pass eight cases,
including identical inputs proving 39 complex-path damage versus 40
broadcast-path damage. The fresh VC++ 1.52c packet
`compat/vc152/f-attack-complex-native-e33/` ran in `C:\FAPNE33`:

- `FAPNATSTA.TXT`: compile and link pass.
- `BEAMR.TXT`: `F_ATTACK_BEAM_SCALE_CHECKS=8 FAILURES=0 RESULT=PASS`.
- Process exit 0; post-run windows show no Program Error dialog.
- Compile/link/report evidence is preserved in `runs/vmctl-vc152fapne33/`.

## Phase 2 status

This source correction and its host/native tests close **0** named Step-5
units. FAttack remains **0/3 at Step 5, 5/6 overall**. Complete E1 raw beam
and torpedo battle-part/table-service acceptance, E2 broader original damage
differentials, E3 full-battle replay, and Step-6 promotion remain open.
