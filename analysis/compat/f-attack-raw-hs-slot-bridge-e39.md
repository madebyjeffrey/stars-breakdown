# FAttack raw `_hs` slot decoding bridge — E39

E39 removes the remaining host-caller shortcut at FAttack's packed hull-slot
boundary. The new `stars_attack_bind_raw_hull_slots` reads each original
four-byte `_hs` record as bytes—little-endian group word, item byte, count
byte—and writes the values into caller-provided typed slots before attaching
them to `StarsAttackInput`. It validates slot stride and destination capacity,
and avoids aliasing/type-punning the packed game storage.

The packed-part probe now starts from raw bytes. Its typed section deliberately
claims a different group/item/count; the decoded raw slot remains authoritative
and drives the normal FAttack → part-resolver → damage path. Unsupported item
and empty-count cases mutate the raw bytes, re-decode, and verify the resolver
or dispatch is correctly suppressed. The new stride/capacity rejection cases
bring the report to `F_ATTACK_PACKED_PART_CHECKS=16 FAILURES=0 RESULT=PASS`.

## Validation

- Focused strict-C89 host tests pass for the raw `_hs` adapter and packed
  attack context.
- Fresh `C:\FAPNE39` VC++ 1.52c build and link pass. Native packed-part
  decoding passes 16/16; beam scale 86/86; raw damage 9/9; torpedo target
  5/5; raw torpedo dispatch 17/17. All launched probe processes exit 0, and
  the post-run window inventory has no Program Error dialog.
- Full verbose host suite passes 523 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and diff checks pass.
- Build status/logs, maps, objects, executables, and reports are retained in
  `runs/vmctl-vc152fapne39/`; the reproducible packet is
  `compat/vc152/f-attack-complex-native-e39/`.

## Phase 2 status

This is supporting evidence only and closes **0 named Step-5 units**. FAttack
remains **0/3 at Step 5 and 5/6 overall**. The adapter now decodes raw hull
slots, but the complete native `FLookupPart` table/service and race/visibility
path, packed attacker/target/design-to-hull resolution, raw beam/torpedo
damage-service acceptance, E2 original differential, E3 full battle replay,
and Step-6 promotion remain open.
