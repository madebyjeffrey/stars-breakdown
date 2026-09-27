# FAttack caller context from the packed battle token — E38

E38 closes a missing source seam between the FDoCoolBattle per-token attack
callback and FAttack. The coordinator already decoded the packed token into a
typed `StarsCoolBattleToken`, but the prior callback repeated assignments and
omitted `object_kind` and `battle_coordinate`. The new
`stars_attack_bind_cool_battle_token` adapter seeds FAttack's caller context
consistently while leaving weapon sections, targets, packed hull slots, and
damage services callback-owned.

## Field mapping and behavior

The 29-byte packed `_tok` record has player/owner at byte 2, raw `grobj` at
byte 3, and battle coordinate at byte 5. FDoCoolBattle decodes those fields.
The adapter copies token slot index, player, coordinate, and object kind into
`StarsAttackInput`, and passes through the current initiative and surviving-
player mask. This carries the E36/E37 raw `grobj == 1` range rule through the
real per-token callback boundary rather than only through direct fixtures.

The integration probe supplies two raw 29-byte records through
`StarsCoolBattleInput.packed_tokens`, runs the production load/callback/store
path, invokes the adapter in the attack callback, and checks the populated
FAttack input, target coordinate, casualty mutation, and packed write-back.
Host report: `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=29 FAILURES=0
RESULT=PASS`.

## Validation

- Focused host test compiles the current battle coordinator, FAttack, lookup,
  torpedo-hit, movement, board initialization, battle, and target sources with
  strict C89 warnings-as-errors; 29/29 passes.
- Fresh `C:\FCBATE4` VC++ 1.52c build/link passes. The native Win98 report is
  also 29/29; process exit is 0 and the final window inventory has no Program
  Error dialog.
- Full verbose host suite passes 523 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and diff checks pass.
- The first native compile failure (`COMMON.H` omitted from the cloned
  packet) is preserved under `runs/vmctl-vc152fcbate4/`; the corrected
  successful return, including map/object/executable, is under
  `runs/vmctl-vc152fcbate4-success/`.

Packet: `compat/vc152/f-do-cool-battle-attack-adapter-e4/`.

## Phase 2 status

Supporting evidence only; this closes **0 named Step-5 units** for either
FAttack or FDoCoolBattle. FAttack remains **0/3 at Step 5 and 5/6 overall**:
complete packed weapon/hull/target mapping and beam/torpedo service acceptance,
including hull/resource/shield/jam/capacity behavior, remain for E1; E2
original damage differentials, E3 full battle replay, and promotion remain.
FDoCoolBattle still needs exact native `_tok`/`vrgtok` global binding and
production callers, movement/recovery details, full damage/report records,
and original full-battle differential. This validates the token-to-FAttack
caller fields, not the whole battle path.
