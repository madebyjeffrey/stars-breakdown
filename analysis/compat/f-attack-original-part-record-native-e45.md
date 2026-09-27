# FAttack original beam and torpedo record integration E45

## Result

The current `stars_f_attack` raw-record path is exercised for both weapon
families. The regression loads item 7's beam and item 4's torpedo from original
NE segment-2 bytes. For the torpedo it verifies original `init=2`, `range=5`,
and `dp=90`: with one guaranteed hit and the reconstructed raw split, the
record produces 45 hull damage. Mutating `dp` to 120 changes damage to 60;
setting range to zero or initiative to one prevents dispatch. The beam cases
continue to verify the corresponding field-driven behavior, and undersized
records are rejected.

## Evidence

- Host strict-C89 report: `F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0
  RESULT=PASS`. The host test now links production `c_torp_hit.c` rather than
  a no-hit stub for the guaranteed-hit torpedo case.
- Full host suite: 524 tests pass, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and `git diff --check` pass.
- Fixture: unrelocated segment 2 extracted from `target/stars.exe` (executable
  SHA-256 `ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`,
  segment length 25,256, segment SHA-256
  `52b2fb4ebc7c8ea4527b2d277fcaf3218c916053d9e295e33900cc637a753698`).
- Fresh `C:\FAPNE45` VC++ 1.52c/Win98 build: compile/link PASS, native report
  `F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0 RESULT=PASS`, process exit 0,
  and no Program Error in the post-run window inventory/screenshot.
- The package contains current `FA.C`, `LOOKUP.C`, and `FAPART.C`, along with
  the exact fixture and narrow unexercised-helper stub. Package and returned
  build/report/map/object/screenshot evidence:
  `compat/vc152/f-attack-packed-record-native-e45/` and
  `runs/vmctl-vc152fapne45/`.
- E43's drive-relative wrapper and E44's stale 20-check report are retained,
  but only E45 is accepted for this expanded native test.

## Limits and ledger impact

This is a copied static executable segment, not live Stars! CS mapping or a
full native battle replay. It strengthens the raw group-0x20 path but closes
**0 named Step-5 units**. `FAttack` remains 0/3 at Step 5 and 5/6 overall;
broader original damage differentials, complete scaling/target/damage
acceptance, full native battle replay, and promotion remain. TODO6 counters
remain 120/152 evidence units closed, 32 open, 10 promotion units, and 42
practical open work items.
