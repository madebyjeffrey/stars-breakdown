# FAttack raw torpedo candidate ranking — E16

This host checkpoint corrects target selection and RNG ordering in the
readable `FAttack` torpedo path. The raw listing
`analysis/decompiler/export/raw/10f0_69ac_FAttack.c:290-343` computes the
candidate's resource value and effective hull/shield capacities, including
the packed `_dv` reduction. Its torpedo branch (`:383-515`) ranks by resource
value per expected shield/hull torpedoes, selects the highest-valued eligible
token, and only then resolves random hit count through `CTorpHit`
(`:623-757`). The previous readable loop called `CTorpHit` for every eligible
candidate and selected by the resulting random hit counts, consuming the
random stream in a different order.

The typed source now preserves the raw truncation sequence for the adjusted
hit percentage, effective hull capacity, shield/hull shot estimates, and
resource-per-shot ranking. `CTorpHit` is invoked only after the best target is
selected. The existing torpedo shield/hull dispatch is still only a partial
model of the raw split and residual loop.

## Host evidence

`tests/f_attack_torpedo_target_probe.c` passes strict C89 with
`F_ATTACK_TORPEDO_TARGET_CHECKS=5 FAILURES=0 RESULT=PASS`. It verifies that the
higher raw value target wins, exactly three RNG draws are consumed for that
target's three torpedoes (not once per candidate), and the resulting damage
and first-hit record are applied to the selected token. When jam fully reduces
the raw score's adjusted hit percentage to zero, no target is selected and no
random draw or damage dispatch occurs.

Focused regressions pass: FAttack source 34, complex score 9, complex
selection 3, packed-part 13, and raw stateful damage 7 checks. The repository
suite passes 519 tests with 153 expected skips; generated headers retain 2,241
layout assertions and 846 declarations. Baseline and diff checks pass.

## Native status and remaining scope

This is supporting host evidence only and closes **0** named Step-5 units.
The E15 native packet contains the prior `f_attack.c` revision, so it must not
be used to claim E16 native validation. A fresh 42-file packet is prepared in
`compat/vc152/f-attack-complex-native-e16/` for `C:\FAPNE16`; all 41 text files
are ASCII with CRLF line endings, and its packaged `FA.C` and `TORPSEL.C` pass
a strict-C89 syntax check against the packet's reduced `CORE.H`. Run it after
the guest agent reconnects. The raw shield/hull torpedo damage split,
residual-torpedo loop, broader original differentials, full native battle
replay, and Step-6 promotion remain open.
