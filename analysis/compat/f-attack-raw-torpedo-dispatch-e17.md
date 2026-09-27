# FAttack raw torpedo damage-call bridge — E17

This host checkpoint extends E16's raw torpedo target ranking through the
`FDamageTok` call boundary. The original signature in
`analysis/decompiler/export/raw/10f0_81d4_FDamageTok.c:11-13` is
`FDamageTok(_tok *, short attacker, long *pdpBeam, long dpTorp,
ushort grfWeapon, short fShieldsOnly, long *pcTorp)`. The readable callback
now forwards those mutable and scalar parameters, in that order, in addition
to its adapter context and target/section indices.

For the raw torpedo caller in
`analysis/decompiler/export/raw/10f0_69ac_FAttack.c:741-757`, the shield call
passes beam damage by reference, zero torpedo damage, the damage class plus
`0x80`, shields-only true, and no packet pointer. The hull call passes beam
damage by reference, the same value as torpedo damage, the damage class,
shields-only false, and a mutable remaining-packet count. `stars_f_attack`
now preserves that call shape when a raw callback is installed and retains
the existing typed callback as fallback. The hull call's return remains
ignored, matching the raw caller's unconditional hit-record increment.

## Host evidence

Strict-C89 checks pass:

- `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=6 FAILURES=0 RESULT=PASS`
- `F_ATTACK_RAW_DAMAGE_CHECKS=7 FAILURES=0 RESULT=PASS`
- `F_ATTACK_TORPEDO_TARGET_CHECKS=5 FAILURES=0 RESULT=PASS`

The new dispatch probe checks shield/hull argument values, raw-callback
preference, mutation of the packet count from three to two, and hit-record
behavior when the hull callback reports failure. The E15 raw damage and E16
target-selection probes remain green.

## Native status and remaining scope

The fresh packet is `compat/vc152/f-attack-complex-native-e17/` for guest
directory `C:\FAPNE17`. It contains 43 files: 42 ASCII CRLF text files and
the DOS `WINSTUB.EXE`. The packaged FAttack and three raw/torpedo probes pass
strict-C89 syntax checking against the packet's reduced `CORE.H`. This is
host-side packet validation only; no E17 guest compile or execution report is
currently recorded, so the packet must not be described as native-validated.

The raw `grfWeapon` class is represented as `raw_damage_flags` and forwarded,
but the readable packed-part resolver does not yet derive that value from the
selected weapon and target state. Exact class derivation therefore remains an
explicit gap. The current attack loop also does not yet reproduce the raw
shield/hull split and residual-torpedo iteration. Broader original damage
differentials, full battle replay, native E17 replay, and Step-6 promotion
remain open. This checkpoint closes zero named Step-5 units; the current
TODO6 totals remain 120/152 closed, 32 open, 10 promotion units, and 42
practical open work items.
