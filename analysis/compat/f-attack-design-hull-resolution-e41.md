# FAttack battle token design-to-hull slot resolution — E41

E41 removes the preselected-hull assumption from E40. The new
`stars_attack_bind_cool_battle_design_hull` builds the two original token bytes
used by `LpshdefFromTok` (player at `_tok+2`, design at `_tok+4`) from the
battle token and calls the existing `stars_lpshdef_from_tok` reconstruction.
That shared path selects the player's normal table for design bytes below
`0x10`, or the alternate table with design index reduced by `0x10` otherwise.

The returned `_shdef` begins with the 123-byte `_hul` record. Its `rghs[16]`
inline `_hs` array begins at byte 58 and its active count `chs` is byte 122.
The adapter passes exactly those bytes and the recovered count through E40's
raw-slot/context binder before FAttack runs. A count above the fixed 16-slot
array is rejected rather than read out of bounds.

The integrated probe attacks from player 1/design `0x12`, so hull lookup must
select player 1's alternate table, index 2. It returns a fixture `_shdef`
whose raw `_hs` bytes say group `0x10`, item 7, count 2, while duplicated typed
section fields claim group `0x20`, item 4, count 1. The attack resolver sees
the selected hull's raw fields and target damage/deactivation plus battle
report/cleanup complete.

## Validation

- Focused strict-C89 integration, packed-part, beam-scaling, and raw-torpedo
  tests pass. The integration report is
  `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=38 FAILURES=0 RESULT=PASS`.
- Existing `LpshdefFromTok` source tests cover both normal and alternate table
  branches. The combined integration specifically checks the alternate branch
  and record/slot offsets.
- Fresh `C:\FCBATE41` VC++ 1.52c build/link and Win98 run pass; report 38/38,
  process exit 0, no compiler warning/error, and no Program Error dialog.
- Full suite passes 523 tests, 153 expected skips, 2,241 layout assertions,
  and 846 declarations. Baseline and diff checks pass.
- Packet: `compat/vc152/f-do-cool-battle-design-hull-e41/`; retained evidence:
  `runs/vmctl-fcbate41/`.

## Phase 2 status

This removes the callback-owned design-to-hull selection gap for the
reconstructed adapter, but remains supporting evidence and closes **0 named
Step-5 units**. The callback still provides the per-player hull-table lookup
service; exact native `_tok`/`vrgtok` globals and table far pointers, complete
`FLookupPart` table/service/race/visibility behavior, full beam/torpedo damage
acceptance, original differential, complete battle replay, and Step-6
promotion remain open.
