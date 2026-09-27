# FAttack complex target selection integration — E12

This checkpoint connects the E10/E11 raw complex target-score boundary to the
readable `stars_f_attack` beam candidate loop. It is supporting evidence only;
the complete FAttack Step-5 E1 unit remains open.

## Scope

When `StarsAttackSection.raw_scaling` is nonzero, `stars_f_attack` now ranks
eligible beam candidates with `stars_f_attack_complex_target_score` rather
than treating the ordinary beam damage as the ranking value. After selection,
the callback still receives ordinary beam damage. Existing sections with
`raw_scaling == 0` retain the typed best-damage path.

The host probe checks the higher-value target selection, ordinary callback
damage, hit count, and first-target battle-record update.

## Native evidence

The fresh VC++ 1.52c packet is
`compat/vc152/f-attack-complex-native-e12/` and ran from `C:\FAPNE12`.
Final evidence is preserved in `runs/vmctl-vc152fapne12/`:

- `FAPNATSTA-final2.TXT`: `COMPILE=PASS`, `LINK=PASS`;
- `SELCL-final2.LOG`: clean compile output;
- `SELLINK-final2.LOG`: successful link;
- `SELR-final.TXT`: `F_ATTACK_COMPLEX_SELECTION_CHECKS=3 FAILURES=0 RESULT=PASS`.

The process exited normally and the post-run Win98 window inventory contained
no Program Error dialog.

## Remaining scope

The raw complex beam still needs complete part-dependent scaling, capacity and
jam branches, distance and damage dispatch, shield/hull mutation, original
differential cases, full battle replay, and Step-6 promotion. This packet
therefore closes 0 named Step-5 units.
