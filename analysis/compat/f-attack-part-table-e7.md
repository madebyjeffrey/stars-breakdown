# FAttack / FLookupPart table-address checkpoint

Date: 2026-09-21

The readable lookup boundary now exposes the fixed far-table metadata recovered
from `FLookupPart`: segment `0x10e0`, table base/stride, and the resulting item
offset. The FAttack packed-slot probe checks the group `0x0010`, item `7`
shield table as:

```text
segment = 0x10e0
base    = 0x2450
stride  = 0x003c
offset  = 0x25f4
```

The raw `_hs` slot still controls the lookup, and the resolved component
identity remains `0x00100007`. Unsupported-item and zero-count cases remain
rejected before lookup/dispatch.

```text
F_ATTACK_PACKED_PART_CHECKS=13 FAILURES=0 RESULT=PASS
LOOKUP_PART_CHECKS=8 FAILURES=0 RESULT=PASS
```

This is host-source evidence only and closes **0** authoritative Step-5 units.
The native table segment/address binding, all resource-relative table families,
complete battle-part scaling, broader original damage differentials, and native
replay remain open.

Evidence:

- `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- `analysis/DATA_LAYOUTS.md`
- `reconstructed/include/common.h`
- `reconstructed/src/lookup_part.c`
- `reconstructed/src/f_attack.c`
- `tests/f_attack_packed_part_probe.c`
- `tests/lookup_part_source_probe.c`
