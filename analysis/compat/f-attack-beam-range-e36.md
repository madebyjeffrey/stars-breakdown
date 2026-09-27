# FAttack original group-0x10 beam range gate — E36

E36 binds `_beam.dRangeMax` into FAttack's actual packed group-0x10 target
filter. The selected part's range is not just metadata: raw FAttack computes
the allowed coordinate distance by adding one when the attacker's `grobj` is
1, then accepts targets whose `DzFromBrcBrc` distance is less than or equal to
that value.

## Original executable evidence

CodeView places the 24-entry `_beam` table `rgbeam` at NE segment 2 offset
`0x2450`; the 60-byte record has `dRangeMax` at byte 52, `dp` at byte 54, and
`init` at byte 56. The raw FAttack export computes
`iStack_2e = (attacker.grobj == 1) + part.wtCargoMax` at line 149 and applies
`DzFromBrcBrc(...) <= iStack_2e` at lines 194 and 277. At byte 52, the
overlapping `_hul.wtCargoMax` is the `_beam.dRangeMax` field.

The original 24 range values are:

`1, 1, 2, 1, 0, 2, 3, 3, 2, 1, 0, 2, 3, 3, 2, 1, 0, 2, 3, 3, 3, 2, 1, 2`.

`stars_lookup_beam_range` recovers the table and rejects items >=24. On a
successful packed group-0x10 resolution, FAttack enables the raw coordinate
gate and sets its threshold to `dRangeMax + (attacker_object_kind == 1)`.
`StarsAttackInput.attacker_object_kind` carries the raw `_tok.grobj` value;
mapping that byte through a full packed battle-token/orchestrator replay is
still part of the open E1/E3 evidence.

## Validation

- The strict-C89 beam test checks all 24 range, `dp`, and `init` values, all
  invalid item lookups, the prior scaling cases, and three range boundaries:
  kind 1 accepts distance 2 for item 0, rejects distance 3, and kind 2 rejects
  distance 2. `F_ATTACK_BEAM_SCALE_CHECKS=86 FAILURES=0 RESULT=PASS`.
- Packed-part integration remains 13/13; raw damage is 9/9.
- Fresh `C:\FAPNE36` VC++ 1.52c build and native replay pass. Reports:
  beam/range 86/86, packed part 13/13, raw damage 9/9, torpedo target 5/5,
  and torpedo dispatch 11/11. Compile/link pass, process exits 0, and the
  post-run window list has no Program Error.
- Full verbose host suite passes 521 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and diff checks pass. Two prior
  full quiet-mode invocations exited on signal 10 before summary; the anomaly
  remains visible despite successful full verbose runs.

Run evidence: `runs/vmctl-vc152fapne36/`.

## Phase 2 status

This is supporting evidence only and closes **0 named Step-5 units**. FAttack
remains **0/3 at Step 5, 5/6 overall**. Complete packed battle-token/object-kind
mapping and beam/torpedo service acceptance remain open, including the full
hull/resource/shield/jam/capacity path. E2 original damage differentials, E3
full battle replay, and Step-6 promotion also remain open.
