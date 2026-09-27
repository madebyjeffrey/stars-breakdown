# FAttack packed `_hs` slot boundary

Date: 2026-09-21

The raw hull weapon slot is a four-byte `_hs` record: a word `grhst` at
offset `0`, followed by an item byte at offset `2` and a count byte at offset
`3`. `FAttack` now accepts an optional array of these exact slots and derives
the lookup gate from the raw slot, rather than trusting duplicated typed
section fields.

The strict-C89 probe deliberately gives the typed section contradictory values
(`0x20`, item `24`, count `0`) while the packed slot supplies `0x10`, item `7`,
count `2`. The real FAttack path therefore forwards the raw slot to the
resolver, resolves component identity `0x00100007`, and dispatches damage.
Unsupported item and zero-count cases are then tested by changing only the
packed slot bytes.

```text
F_ATTACK_PACKED_PART_CHECKS=10 FAILURES=0 RESULT=PASS
```

This is host source-fidelity evidence only and closes **0** authoritative
Step-5 units. Native packed `FLookupPart` table/service binding, complete
battle-part scaling, broader original damage differentials, native replay, and
full battle replay remain open.

Evidence:

- `analysis/decompiler/export/raw/10f0_69ac_FAttack.c`
- `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- `analysis/DATA_LAYOUTS.md` (`_hs`, 4 bytes)
- `reconstructed/include/core.h`
- `reconstructed/src/f_attack.c`
- `tests/f_attack_packed_part_probe.c`
