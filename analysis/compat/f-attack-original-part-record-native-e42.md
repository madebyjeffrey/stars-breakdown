# FAttack original part-record integration E42

## Result

The current readable `stars_f_attack` path can now obtain weapon range,
damage points, and initiative from the selected original part record. Its
optional `lookup_part_record` callback returns a checked record span; the
production routine rejects missing or undersized records and decodes the
little-endian words at offsets 52, 54, and 56. When no record callback is
provided, the existing typed lookup helpers remain the compatibility path.

## Evidence

- The strict-C89 host test loads the unrelocated NE segment-2 bytes extracted
  from `target/stars.exe` (input executable SHA-256
  `ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`; the
  packaged 25,256-byte segment has SHA-256
  `52b2fb4ebc7c8ea4527b2d277fcaf3218c916053d9e295e33900cc637a753698`). It
  checks beam initiative and damage dispatch, then mutates the original
  record's `init`, `dRangeMax`, and `dp` words to verify that each changes the
  attack result. A 57-byte record is rejected. Result:
  `F_ATTACK_PACKED_PART_CHECKS=20 FAILURES=0 RESULT=PASS`.
- Fresh E42 VC++ 1.52c build in the Win98 guest: `COMPILE=PASS`, `LINK=PASS`.
  The executable consumes the transferred segment image and reports the same
  20/20 result. Process exit is 0. Captured window inventory and
  `guest-screen.bmp` show the agent/desktop with no Program Error dialog.
- Native packet and returned evidence: `compat/vc152/f-attack-packed-record-native-e42/`
  and `runs/vmctl-vc152fapne42/`. The generator is
  `tools/package_f_attack_packed_record_native.py`.
- E40's missing `stars_lpshdef_from_tok_globals` link symbol and E41's
  duplicate test stub are retained in their own package/run folders and are
  not counted as passes.
- The full host suite passes 524 tests, with 153 expected skips, 2,241 layout
  assertions, and 846 declarations. `tools/baseline.py --check` and
  `git diff --check` pass.

## Limits and ledger impact

The fixture is a copy of original static executable bytes. This is not proof
of a live Stars! process's CS mapping, full `FLookupPart` service behavior,
whole-battle damage fidelity, or a complete native battle replay. This
supporting evidence closes **0 named Step-5 units**; `FAttack` remains 0/3 at
Step 5 and 5/6 overall. Broader original damage differentials, complete
weapon scaling/target/damage acceptance, full native battle replay, and
promotion remain open. TODO6 counters remain 120/152 evidence units closed,
32 open, 10 promotion units, and 42 practical open work items.
