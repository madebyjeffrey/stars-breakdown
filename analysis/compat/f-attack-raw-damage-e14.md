# FAttack raw stateful damage dispatch — E14

This checkpoint adds the next narrow raw-complex boundary: a raw beam section
may route its selected damage event through an optional stateful callback. The
callback receives the damage by reference, so it can invoke the existing
`FDamageTok` reconstruction and preserve shield absorption, residual beam,
casualty mutation, and damage-record order. Typed callers continue to use the
existing callback unchanged.

This is supporting evidence only and closes no named Phase-2 Step-5 unit.

## Host evidence

`tests/f_attack_raw_damage_probe.c` passes strict C89 with
`F_ATTACK_RAW_DAMAGE_CHECKS=4 FAILURES=0 RESULT=PASS`. The probe checks that
the raw callback is selected, the 39-point scaled beam is split into a 6-point
shield record and a 33-point hull record, two ships are removed, and the
FAttack hit/battle-record fields advance once.

## Native evidence

The fresh packet is `compat/vc152/f-attack-complex-native-e14/`, run from
`C:\FAPNE14`. Final evidence is preserved in `runs/vmctl-vc152fapne14/`:

- `FAPNATSTA-final3.TXT`: `COMPILE=PASS`, `LINK=PASS`;
- `RAWCL-final3.LOG`: clean VC++ 1.52c compile;
- `RAWLINK-final3.LOG`: successful link;
- `RAWR-final3.TXT`: `F_ATTACK_RAW_DAMAGE_CHECKS=4 FAILURES=0 RESULT=PASS`.

The process exited normally and the final Win98 window inventory contained no
Program Error dialog.

## Remaining scope

Complete raw part-dependent scaling and capacity/jam formulas, complex
torpedo selection, original differential cases, full battle replay, and
Step-6 promotion remain open. The new callback is a dispatch boundary; it is
not yet evidence that the full original raw battle path has been replayed.
