# FAttack original group-0x10 beam initiative binding — E35

E35 follows E34's recovery of `_beam.dp` by binding the selected beam's
original initiative word. The raw FAttack path forms the current initiative
from the attacker's `initBase` plus the selected part's `init`, then compares
that result to the current initiative step. The readable path already
preserves unsigned 16-bit wrap and the cap at 63 when `StarsAttackSection.init`
is negative; it now sources the part contribution from the original table.

## Original executable evidence

CodeView defines `_beam` as a 60-byte record in `rgbeam`, NE segment 2 at
offset `0x2450`. The fields consumed by this combat path are:

- `dRangeMax`, byte 52;
- `dp`, byte 54; and
- `init`, byte 56.

All 24 words were read from the original segment image at
`0x2450 + item*60 + field_offset`. The `dp` list remains the E34 value set;
the original `init` list is:

`9, 9, 12, 9, 10, 7, 14, 5, 12, 9, 10, 7, 14, 5, 13, 9, 11, 8, 6, 14, 6, 13, 9, 8`.

`stars_lookup_beam_init` rejects items >=24. After successful packed
group-0x10 resolution, FAttack replaces `init_delta` with that table value;
`attack_section_init` retains the raw unsigned-add/wrap/cap behavior. The
resolver still owns `init_base` and can provide a direct pre-resolved `init`
for non-raw typed use.

## Validation

- Strict-C89 beam/table/scaling test: `F_ATTACK_BEAM_SCALE_CHECKS=58
  FAILURES=0 RESULT=PASS`, covering every `_beam.dp` and `_beam.init` value,
  both out-of-range lookups, and eight scaling/branch cases.
- Strict-C89 packed path: `F_ATTACK_PACKED_PART_CHECKS=13 FAILURES=0
  RESULT=PASS`. Item 7 has `_beam.init=5`; with `initBase=4`, only the correct
  raw initiative 9 dispatches. The resolver's prior `init_delta=0` is
  intentionally stale.
- Raw beam damage remains 9/9. Fresh native `C:\FAPNE35` compile/link passes;
  beam/table 58/58, packed part 13/13, raw damage 9/9, torpedo target 5/5,
  and raw torpedo dispatch 11/11 all pass. Tested processes exit 0; final
  compiler output has no warnings and the post-run window inventory contains
  no Program Error.
- Full verbose host suite: 521 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and diff checks pass. Two earlier
  full quiet-mode runs exited with signal 10 before summary; this anomaly
  remains visible even though verbose discovery passes.

Run evidence: `runs/vmctl-vc152fapne35/`.

## Phase 2 status

This is supporting evidence only and closes **0 named Step-5 units**. FAttack
remains **0/3 at Step 5, 5/6 overall**. Complete raw beam/torpedo table and
service acceptance remains open, including beam range and hull/resource/
shield/jam/capacity behavior; E2 original damage differentials, E3 full battle
replay, and Step-6 promotion also remain open.
