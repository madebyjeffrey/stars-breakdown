# FAttack original group-0x20 torpedo initiative and range — E37

E37 binds the remaining two original `_torp` fields used by the group-0x20
FAttack packed-part path: `init` and `dRangeMax`. After the packed part resolves,
FAttack now replaces resolver-provided initiative/range with the selected
original record values. Initiative contributes to the attacker's initiative
comparison; range uses the original inclusive coordinate-distance gate, with
the raw `grobj == 1` allowance.

## Original executable evidence

CodeView places the twelve-entry `rgtorp` table at NE segment 2:`0x2180`; the
`_torp` record is 60 bytes. The fields are at byte offsets 52 (`dRangeMax`),
54 (`dp`), and 56 (`init`). Values below are read little-endian from
`analysis/ne/memory/segment_02.bin` at `0x2180 + item*60 + field_offset`:

- `dRangeMax`: `4, 4, 4, 5, 5, 5, 5, 6, 5, 5, 6, 6`
- `dp`: `5, 12, 26, 48, 90, 169, 316, 60, 85, 150, 280, 525`
- `init`: `0, 1, 1, 2, 2, 3, 4, 0, 0, 1, 2, 3`

`stars_lookup_torpedo_init` and `stars_lookup_torpedo_range` expose the
original fields and reject item indices >=12. In the successful group-0x20
packed FAttack branch, the table values replace resolver fixtures. The
strict-C89 dispatch probe checks the whole table and invalid boundary, then
uses item 9 (`init=1`, `dRangeMax=5`) with `initBase=3`: initiative 4 and
`grobj=1` admit coordinate distance 6, while distance 7 is rejected before
damage services.

## Validation

- Focused host tests pass for torpedo dispatch, beam scaling, and packed-part
  integration. The torpedo dispatch report is 17/17; beam scale is 86/86; the
  packed-part matrix is 13/13.
- Fresh `C:\FAPNE37` VC++ 1.52c build and link pass. Native reports:
  - beam scale: 86 checks, 0 failures;
  - packed part: 13 checks, 0 failures;
  - raw damage: 9 checks, 0 failures;
  - torpedo target: 5 checks, 0 failures;
  - raw torpedo dispatch: 17 checks, 0 failures.
- All exercised guest processes exit 0. The post-run window inventory contains
  no Program Error dialog. Compiler and linker logs are preserved with the run
  return.
- Full verbose host suite: 522 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations; all pass. `tools/baseline.py --check` and
  `git diff --check` pass. Earlier quiet-mode signal-10 anomalies remain
  historical and are not hidden by this passing verbose run.

Evidence is preserved in `runs/vmctl-vc152fapne37/`; the reproducible native
packet is `compat/vc152/f-attack-complex-native-e37/`.

## Phase 2 status

This is supporting evidence and closes **0 named Step-5 units**. FAttack
remains **0/3 at Step 5, 5/6 overall**. Complete packed battle-token/object-
kind mapping and full beam/torpedo damage-service acceptance (including hull,
resource, shield, jam, and capacity behavior) remain in E1; E2 original damage
differentials and E3 full native battle replay/differential remain; Step-6
promotion is also open.
