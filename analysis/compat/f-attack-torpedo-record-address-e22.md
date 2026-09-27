# FAttack group-0x20 torpedo record address — E22 (superseded by E29)

## Finding

The raw FAttack torpedo path reads the selected part's word at byte offset
54 through Ghidra's inferred `_hul.wtFuelMax` view. The recovered 60-byte
`_torp` layout identifies the same offset as `dp`
(`STARS_OFFSET_12aa_dp=54`). The readable semantic name is therefore the
torpedo record's `dp` word; `raw_torpedo_power` remains the FAttack-specific
name for its role in allocation math.

Raw `FLookupPart` selects group `0x20` with 12 entries and 60-byte records.
The E22 address inference from the decompiler's `0x10e0` selector and the
DGROUP M6108 string was incorrect. The acceptance disassembly at
`analysis/decompiler/acceptance/1008_524e.asm:1117-1123` shows the group branch
computing `0x2180 + item*0x3c`, then storing `CS` as the far-pointer selector.
CodeView independently places public `rgtorp` at NE segment 2, offset
`0x2180`; `_torp` is 60 bytes and its `dp` word is at byte 54. FLookupPart is
also in NE segment 2. The earlier “overlaid math-error text” conclusion
mistook a same-offset string in NE autodata segment 37 for this code-segment
table.

The twelve `dp` values recovered from `analysis/ne/memory/segment_02.bin` at
`0x2180 + i*0x3c + 54` are `5, 12, 26, 48, 90, 169, 316, 60, 85, 150, 280,
525`. E29/E30 add the production lookup and current-source/native evidence;
525`. E29/E30 add the production lookup and current-source/native evidence;
E31 also corrects the selector metadata to identify runtime `CS` explicitly.
See `analysis/compat/f-attack-torpedo-table-native-e29.md`.

## Validation

- Historical E22 checks and its “not yet runtime-backed” limitation are
  retained as the original checkpoint, but that limitation is superseded by
  the evidence and tests in E29/E30.

## Remaining

The complete complex-beam integration, broader original damage differential,
and full native battle replay remain open. E29/E30 are supporting evidence
only and close no named TODO6 Step-5 unit.
