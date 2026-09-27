# FAttack original group-0x10 beam table integration — E34

E34 connects FAttack's packed group-0x10 beam item to its original damage-point
field. Before this change, the readable FAttack implementation trusted the
resolver's `raw_part_dp`; it now replaces that value with the `_beam.dp` field
for the packed part selected by FLookupPart, just as the group-0x20 torpedo
path already reads its original `_torp.dp` table.

## Original executable evidence

The raw FLookupPart group-0x10 branch selects `CS:(0x2450 + item*0x3c)` and
accepts the 24 beam items. CodeView identifies `rgbeam` at NE segment 2,
offset `0x2450`; `_beam` has size 60 bytes and `dp` at byte 54. Reading the
little-endian word at `0x2450 + item*60 + 54` from the segment image yields:

`10, 16, 13, 26, 90, 26, 82, 26, 31, 66, 231, 66, 211, 66, 80, 169, 592, 169, 140, 541, 169, 204, 433, 433`.

The implementation adds `stars_lookup_beam_dp` with those 24 values and
rejects item 24 and above. After a successful resolver callback, FAttack
overwrites the semantic view's `raw_part_dp` with the table value before
initiative, damage-class, and scaling logic consume it.

## Validation

- The strict-C89 beam test checks all 24 recovered table values, invalid-item
  rejection, and eight beam scaling/broadcast cases: 33/33 pass.
- The raw damage integration deliberately supplies a bogus resolver `dp` and
  selects items 15 (`dp=169`, class 1) and 21 (`dp=204`, class 2). It passes
  9/9; item 15's larger original `dp` reduces integer distance loss to zero,
  so the correct post-shield residual is 34.
- The fresh `compat/vc152/f-attack-complex-native-e34/` packet builds in
  `C:\FAPNE34` with VC++ 1.52c. `FAPNATSTA.TXT` reports compile/link pass;
  native reports pass at beam/table 33/33, raw damage 9/9, torpedo target
  5/5, and raw torpedo dispatch 11/11. Tested processes exit 0. Final compiler
  logs are warning-free and the post-run window list has no Program Error.
- The first native compile failure (duplicate host stubs) and first raw damage
  mismatch (old residual expectation) are kept in the run folder, followed by
  the successful corrected build and reports.
- Full host regression: 520 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and scoped diff checks pass.
- The verbose full-suite run completed successfully. Two quiet-mode full-suite
  invocations exited with signal 10 before reporting a summary; the
  `test_common_source -q` module passes 221/221 alone. Treat this as a visible,
  unresolved test-runner intermittent rather than a source regression.

Run evidence: `runs/vmctl-vc152fapne34/`.

## Phase 2 status

This closes **0 named Step-5 units**. FAttack remains **0/3 at Step 5, 5/6
overall**. Full E1 raw beam/torpedo battle-part and table/service acceptance,
E2 broader original damage differentials, E3 full-battle replay, and Step-6
promotion remain open.
