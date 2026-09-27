# FAttack raw torpedo damage-class selection — E18

E18 closes the torpedo-specific `grfWeapon` derivation gap identified in E17.
The original FAttack listing initializes the torpedo damage class to `4` and
adds bit `8` when the selected packed weapon item is 8 through 11. This class
is then sent to both raw `FDamageTok` calls; the shield-only call additionally
sets bit `0x80`. The readable path now derives the raw callback flags from
`StarsAttackSection.part_item` instead of trusting the generic
`raw_damage_flags` field for torpedoes. Typed fallback flags are unchanged.

## Host evidence

The strict-C89 dispatch probe passes
`F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=7 FAILURES=0 RESULT=PASS`. It verifies
item 8 produces hull class `12` and shield class `0x8c`, while item 7 produces
hull class `4` and shield class `0x84`. E16 target selection still passes its
5-check probe.

## Native status and remaining scope

A fresh 43-file VC++ 1.52c packet is prepared at
`compat/vc152/f-attack-complex-native-e18/` for `C:\FAPNE18`: 42 text files
are ASCII/CRLF and the remaining file is the DOS stub. The packaged FAttack
and raw/torpedo probes pass strict-C89 syntax checks against the reduced
`CORE.H`. Guest compilation/execution has not yet been recorded.

This derives the torpedo class from the item index but does not yet implement
the exact shield/hull damage partition or the loop that consumes residual
torpedoes after target hull depletion. Beam `grfWeapon` still depends on the
part-weight-derived class and is not yet populated by the packed resolver.
Broader original damage differentials, full native battle replay, and Step-6
promotion remain open. This closes zero named Step-5 units; TODO6 remains
120/152 closed, 32 open, 10 promotion units, and 42 practical open work items.
