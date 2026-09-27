# FAttack original torpedo table and current-source replay — E29/E30/E31

## Result

The group-0x20 record is now tied to original executable data rather than a
test-only power value. `FLookupPart` forms `CS:(0x2180 + item*0x3c)`; CodeView
places `rgtorp` at NE segment 2, offset `0x2180`; and `_torp` has size 60 with
`dp` at byte 54. Reading those words from `analysis/ne/memory/segment_02.bin`
produces the twelve powers:

`5, 12, 26, 48, 90, 169, 316, 60, 85, 150, 280, 525`.

The table metadata now also represents the selector correctly. In
`StarsLookupPartInput`, group `0x20` sets `table_segment_is_current_cs=1` and
leaves the literal selector zero; other fixed families retain their
documented selector. The recovered family names are corrected as well:
group `0x08` is armor (`rgarmor` at `0x04e0`), while group `0x20` is torpedo
(`rgtorp` at `0x2180`). The host and E31 native metadata matrices verify this
distinction.

This corrects E22's mistaken DGROUP/overlay interpretation: its `0x10e0`
selector was a decompiler inference; the accepted original instruction
sequence explicitly stores `CS`. The math-error string at the same numeric
offset is in NE autodata segment 37, not the segment-2 torpedo table.

`stars_lookup_torpedo_dp` now returns those original `dp` values and rejects
item 12. The reconstructed FAttack path replaces a resolver fixture's
hard-coded value with the table-backed value for a successful group-0x20
lookup. The strict-C89 dispatch probe checks all twelve table values and
demonstrates item 8 uses `dp=85` (rather than the resolver fixture's 16) to
drive its split/residual damage calls; item 7 uses `dp=60`.

## Validation

- Host dispatch probe: `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=11 FAILURES=0`.
- Host lookup metadata probe: `LOOKUP_PART_TABLE_CHECKS=38 FAILURES=0`,
  including the group-0x20 current-CS marker and zero literal selector.
- Full host suite: 519 tests pass, 153 expected skips; 2,241 layout assertions
  and 846 declarations.
- E29 fresh VC++ 1.52c/Win98 build: `COMPILE=PASS`, `LINK=PASS`; raw-damage
  9/9, torpedo target 5/5, current raw torpedo dispatch 11/11, packed-part
  13/13, lookup metadata 34/34, complex score 9/9, and complex selection
  3/3 all pass. The process exits 0 and final window inventory has no Program
  Error dialog. Logs and reports are in `runs/vmctl-vc152fapne29/`.
- E30 separately compiles and runs the current `tests/f_attack_source_probe.c`
  with current `FA.C` and `LOOKUP.C`; it reports
  `F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS`, process exit 0, and a clean
  final window inventory. Logs and report are in
  `runs/vmctl-vc152fapne30/`.
- E31 validates the current-CS metadata correction in fresh Win98/VC++ 1.52c:
  compile/link PASS; lookup table 38/38, raw damage 9/9, torpedo target 5/5,
  current torpedo dispatch 11/11, current FAttack source probe 34/34, and
  packed part 13/13 all pass. The raw replay exits 0 and the final window
  inventory has no Program Error. Logs and reports are in
  `runs/vmctl-vc152fapne31/`.
- E29's earlier copied `FASCALE.C` is stale and still reports one failure; that
  failed artifact is preserved. E30 is the authoritative fresh-source result.
  E28's incomplete native dependency packet is likewise retained and not
  counted as a pass.

## Remaining / ledger impact

This is supporting Step-5 evidence and closes **0** named units. `FAttack`
remains 0/3 at Step 5 and 5/6 overall. Complete raw complex-beam
hull/resource/shield/jam/capacity scaling, broader original damage
differentials, and full native battle replay remain. TODO6's authoritative
counters therefore remain **120/152 evidence units closed, 32 open, 10
promotions, and 42 practical open work items**.

## Source anchors

- Original instruction sequence: `analysis/decompiler/acceptance/1008_524e.asm`
  around `5d87–5da2` (multiply item by 60, base `0x2180`, `DX=CS`, store far
  pointer).
- CodeView symbol: `analysis/debug/globals.json`, `rgtorp`, NE segment 2,
  offset 8576 / `0x2180`.
- CodeView record type: `analysis/debug/types.json`, `_torp`, size 60, `dp`
  offset 54.
- Original table bytes: `analysis/ne/memory/segment_02.bin`, twelve records at
  `0x2180`, 60-byte stride.
- Readable implementation: `reconstructed/src/lookup_part.c` and
  `reconstructed/src/f_attack.c`; probe:
  `tests/f_attack_raw_torpedo_dispatch_probe.c`.
