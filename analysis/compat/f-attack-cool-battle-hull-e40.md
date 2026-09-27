# FAttack battle callback raw hull-slot binding — E40

E40 composes the E38 per-token battle context adapter with the E39 raw `_hs`
decoder. `stars_attack_bind_cool_battle_hull` binds the attacker token index,
owner, kind, coordinate, initiative, and surviving-player mask, while decoding
the callback-provided raw four-byte hull slots into the same `StarsAttackInput`.
The callback remains responsible for selecting the correct design/hull and
providing its raw slot bytes; this helper does not claim to reproduce
`LpshdefFromTok` or bind native game globals.

The `FDoCoolBattle` integration probe now supplies a raw group-`0x10`, item-7,
count-2 hull slot while its typed section deliberately claims group `0x20`,
item 4, count 1. The combined adapter feeds those bytes through the actual
battle callback into `FAttack`; its part resolver observes the raw values and
the damage callback still deactivates the target token. This joins the packed
token/context seam to packed attacker-part lookup without changing ownership
of target or damage services.

## Validation

- Strict-C89 host integration passes
  `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=33 FAILURES=0 RESULT=PASS`.
- Fresh `C:\FCBATE40` VC++ 1.52c compile/link passes. The Win98 report is
  33/33, the process exits 0, compiler output is warning/error-free, and the
  post-run window inventory contains no Program Error dialog.
- The full host suite passes 523 tests with 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and diff checks pass.
- The packet is `compat/vc152/f-do-cool-battle-attack-hull-e40/`; retained
  native build/report evidence is in `runs/vmctl-fcbate40/`.

## Phase 2 status

This is stronger supporting evidence and closes **0 named Step-5 units** for
either `FAttack` or `FDoCoolBattle`. The callback still owns actual
design-to-hull selection and native `_tok`/`vrgtok` global binding; complete
`FLookupPart` table/service/race/visibility behavior, full beam/torpedo
damage-service acceptance, original damage differential, full battle replay,
and Step-6 promotion remain open.
