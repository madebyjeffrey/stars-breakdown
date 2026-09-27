# FAttack complex target-score slice — E10

This checkpoint validates the first readable slice of the raw complex-beam
target scoring branch in `FAttack` (`001f:69ac-7cdd`). It is supporting
evidence only: it does not close a named Phase-2 Step-5 unit or promote
`FAttack`.

## Scope

The typed helper `stars_f_attack_complex_target_score` now covers the raw
target-value arithmetic for this slice:

- resource plus ironium value multiplied by ship count;
- the raw `100000` saturation boundary and 100x scaling;
- beam-defense percentage reduction;
- shield and hull capacity values, including the minimum hull denominator;
- the weapon-flag shield-only scoring path and zero-shield rejection.

The host probe contains eight normal, boundary, alternate-path, and null-safe
checks. The minimum-hull oracle was corrected after the first native run: with
4000 resource value after 50% defense, 20 shield points, and minimum hull 1,
the denominator is `1 + 20 + 1 = 22`, giving `400000 / 22 = 18181`.

## Native evidence

The fresh VC++ 1.52c packet is `compat/vc152/f-attack-complex-native-e10/`
and ran in `C:\FAPNE10` using the package-specific CRLF wrapper
`RUNFAP.BAT`. The final artifacts are preserved in
`runs/vmctl-vc152fapne10/`:

- `FAPNATSTA-final5.TXT`: `COMPILE=PASS`, `LINK=PASS`;
- `CMPCL-final5.LOG`: clean compile output;
- `CMPLINK-final5.LOG`: successful link;
- `CMPR-final2.TXT`: `F_ATTACK_COMPLEX_SCORE_CHECKS=8 FAILURES=0 RESULT=PASS`;
- the same report records `MIN_HULL_ACTUAL=18181 EXPECTED=18181`.

The process exited normally and the post-run window inventory contained no
Program Error dialog. The connected `STARSAGT 0.4.2` agent remains available.

## Still open

This slice is not the complete raw complex-beam implementation. The selected
target's remaining raw hull/resource/shield/jam/capacity interactions,
post-selection beam/torpedo dispatch, original differential cases, and full
battle replay remain open. The typed helper is currently a source boundary;
its full integration into the production target-selection path still requires
the corresponding raw state and caller evidence.
