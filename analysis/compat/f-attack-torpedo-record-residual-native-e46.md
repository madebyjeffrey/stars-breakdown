# FAttack raw torpedo record and residual sequence E46

## Result

The existing multi-pass/residual FAttack regression now resolves group-0x20
weapon records through `stars_lookup_part_record` using the original
unrelocated NE segment-2 image. Item 8 resolves to offset `0x2360` with
`dRangeMax=5`, `dp=85`, and `init=0`. The probe verifies initiative/range
selection, then the raw torpedo split dispatches damage 85 and 42 over its
residual sequence, including the selected target, callback flags, hit count,
and packet-state transition. Additional cases still exercise item 7, item 9's
inclusive range boundary, and one-past-range rejection.

## Evidence

- Host strict-C89 result:
  `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS`.
- Full host suite: 524 tests pass, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and `git diff --check` pass.
- Fresh E46 Win98/VC++ 1.52c package compiles and links two current-source
  executables. The packed-record probe reports
  `F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0 RESULT=PASS`; the raw torpedo
  record/residual probe reports
  `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS`. Both
  processes exit 0; window inventory and screenshot show no Program Error.
- Package and returns, including both EXEs, maps, objects, logs, reports, and
  screenshot: `compat/vc152/f-attack-packed-record-native-e46/` and
  `runs/vmctl-vc152fapne46/`.

## Limits and ledger impact

This is supporting native execution of reconstructed code over copied static
original bytes, with a scripted random callback; it is not a full original
executable differential, a live CS snapshot, or whole-battle replay. It
closes **0 named Step-5 units**. `FAttack` remains 0/3 at Step 5 and 5/6
overall. Broader damage differentials, full native battle replay, and
promotion remain. TODO6 counters remain 120/152 evidence units closed, 32
open, 10 promotion units, and 42 practical open work items.
