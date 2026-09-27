# FAttack packed `_dv` target-score modifier — E11

This checkpoint extends the E10 complex-beam target-score source boundary with
the packed `_tok.dv` hull-capacity modifier. It is supporting evidence only and
closes no named Phase-2 Step-5 unit.

## Source scope

`StarsAttackToken.defense_value` now represents the recovered two-byte `_dv`
word: low seven bits are `pctSh`, and the upper nine bits are `pctDp`. The
complex target-score helper applies the raw truncating sequence before the
minimum-hull clamp:

1. `hull_dp * pctDp / 10`;
2. multiply by `pctSh / 10`;
3. multiply by ship count and divide by `500`;
4. subtract from the ship-count-scaled hull value.

The host probe adds the packed modifier case and passes 9 checks.

## Native evidence

The fresh packet is `compat/vc152/f-attack-complex-native-e11/`, run from
`C:\FAPNE11`. Final evidence is preserved in `runs/vmctl-vc152fapne11/`:

- `FAPNATSTA-final.TXT`: `COMPILE=PASS`, `LINK=PASS`;
- `CMPCL-final.LOG`: clean compile output;
- `CMPLINK-final.LOG`: successful link;
- `CMPR-final.TXT`: `F_ATTACK_COMPLEX_SCORE_CHECKS=9 FAILURES=0 RESULT=PASS`;
- minimum-hull diagnostic: `MIN_HULL_ACTUAL=18181 EXPECTED=18181`.

The process exited normally and the Win98 window inventory contained no
Program Error dialog.

## Remaining scope

The raw complex-beam caller still needs complete target-selection integration,
weapon-part scaling and capacity/jam branches, post-selection dispatch,
original differential cases, full battle replay, and Step-6 promotion.
