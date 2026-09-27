# FAttack raw beam dispatch scaling — E13

This checkpoint extends the complex-beam integration with the first raw
post-selection beam-damage scaling slice. It is supporting evidence only and
closes no named Phase-2 Step-5 unit.

## Scope

For a raw complex section (`raw_scaling != 0`), the readable beam-damage path
now preserves the raw order:

1. apply attacker `pctCap` when nonzero;
2. apply target `pctBeamDef`;
3. compute `distance * 10 / part_dp` with truncation;
4. multiply by `100 - distance_loss` and divide by 100.

The callback receives the resulting damage, while target ranking continues to
use the separate complex target-score value. Existing typed sections retain
their previous fuel-factor path.

## Native evidence

The fresh packet is `compat/vc152/f-attack-complex-native-e13/`, run from
`C:\FAPNE13`. Final evidence is preserved in `runs/vmctl-vc152fapne13/`:

- `FAPNATSTA-final.TXT`: `COMPILE=PASS`, `LINK=PASS`;
- `SELCL-final.LOG`: clean compile output;
- `SELLINK-final.LOG`: successful link;
- `SELR-final.TXT`: `F_ATTACK_COMPLEX_SELECTION_CHECKS=3 FAILURES=0 RESULT=PASS`.

The process exited normally and the Win98 window inventory contained no
Program Error dialog.

## Remaining scope

Part-dependent weapon scaling, capacity/jam branches beyond the beam slice,
shield/hull mutation and follow-up dispatch, torpedo complex selection,
original differential cases, full battle replay, and Step-6 promotion remain
open.
