# FAttack / FLookupPart current-CS table pointers E46

Date: 2026-09-22

Raw assembly for all sixteen valid `FLookupPart` groups computes an offset
from immediate base and stride values, then copies `CS` into the far pointer's
segment word. `FLookupPart` is at NE `02:524E`; the selected records are in
the procedure's code segment (NE segment 2 in the static analysis image). The
literal `0x10e0` seen in decompiler pointer text is Ghidra's synthetic selector
for NE29, not DGROUP and not the selector stored by these branches. Same-offset
CodeView labels from DGROUP must not be mistaken for the pointer's segment.

| Group | Base | Byte stride | `MOV DX,CS` instruction |
|---:|---:|---:|---:|
| `0x0001` hull | `0x0000` | `0x4E` | `1008:52AF` |
| `0x0002` engine | `0x0768` | `0x38` | `1008:5C36` |
| `0x0004` scanner | `0x0AE8` | `0x36` | `1008:53F6` |
| `0x0008` armor | `0x04E0` | `0x36` | `1008:5720` |
| `0x0010` shield/beam | `0x2450` | `0x3C` | `1008:5CFF` |
| `0x0020` torpedo | `0x2180` | `0x3C` | `1008:5D95` |
| `0x0040` bomb | `0x1E1A` | `0x3A` | `1008:5DEA` |
| `0x0080` mine | `0x1508` | `0x36` | `1008:5B0B` |
| `0x0100` special | `0x12EC` | `0x36` | `1008:5A07` |
| `0x0200` planetary | `0x4D8A` | `0x38` | `1008:592B` |
| `0x0400` message | `0x4872` | `0x8F` | `1008:5684` |
| `0x0800` mech | `0x0D04` | `0x36` | `1008:57AB` |
| `0x1000` fuel | `0x109A` | `0x36` | `1008:5899` |
| `0x2000` battle | `0x19E2` | `0x36` | `1008:5FB9` |
| `0x4000` component | `0x29F0` | `0x8F` | `1008:5481` |
| `0x8000` ZIP | `0x16B8` | `0x36` | `1008:5E8D` |

The readable `StarsLookupPartInput` now marks current-CS addressing for every
valid family, keeps `table_segment` zero rather than fabricating a selector,
and reports the original CS-relative family base, item offset, and stride.
The strict-C89 probe checks all sixteen valid families and an unsupported
group:

```text
LOOKUP_PART_TABLE_CHECKS=66 FAILURES=0 RESULT=PASS
LOOKUP_PART_CHECKS=8 FAILURES=0 RESULT=PASS
```

The packed-FAttack host regression was updated to expect the corrected address
model (`table_segment == 0`, current-CS flag set); its targeted test passes.
The first post-change full-suite attempt exposed that stale expectation. A
subsequent full-suite run completed successfully:

Fresh VC++ 1.52c / Win98 packet `compat/vc152/f-lookup-part-anchor-native-e48/`
compiles and links. `LKTRES.TXT` passes 66/66; the process exits 0 and the
post-run window list contains no Program Error. The full host suite passes 523
tests with 153 expected skips, 2,241 layout assertions, and 846 declarations;
baseline and `git diff --check` pass. Native report and logs are retained in
`runs/vmctl-faptae48/`.

E44's ZIP stride and E45's selector/overlay claims are superseded. Their
reports and returns are preserved but not credited as original-fidelity
evidence. This E46 checkpoint closes **0 named Step-5 units**: actual CS table
memory binding, full race/hidden/`TechStatus` integration, FAttack damage
acceptance, original battle differential/replay, and promotion remain open.

## Evidence

- Raw instructions: `analysis/decompiler/export/raw/1008_524e.asm`
- Static original segment bytes: `analysis/ne/memory/segment_02.bin`
- Selector model: `analysis/decompiler/README.md`
- Source: `reconstructed/src/lookup_part.c`,
  `reconstructed/include/common.h`
- Probe: `tests/lookup_part_table_probe.c`
- Native packet/return: `compat/vc152/f-lookup-part-anchor-native-e48/`,
  `runs/vmctl-faptae48/`
- Superseded E44/E45 records: `analysis/compat/f-attack-live-table-anchors-e44.{md,json}`,
  `analysis/compat/f-attack-live-table-anchors-e45.{md,json}`
