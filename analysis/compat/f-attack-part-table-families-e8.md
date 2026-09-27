# FAttack / FLookupPart fixed-table family checkpoint

Date: 2026-09-21

The lookup seam now exercises every fixed DGROUP/far-table family recovered
from `FLookupPart`, rather than only the shield example used by the previous
checkpoint. For each family the probe checks the recovered segment `0x10e0`,
the family base, the item stride, the computed item offset, and the resolved
component identity. It also checks that a resource-relative family leaves the
runtime table metadata unset instead of inventing a fixed address.

The fixed families covered are hull, engine, scanner, weapon, shield, bomb,
mine, mech, fuel, battle, and component. The resource-relative control case is
the special-family group `0x0100`. The strict-C89 host report is:

```text
LOOKUP_PART_TABLE_CHECKS=34 FAILURES=0 RESULT=PASS
```

This is host-source evidence only and closes **0** authoritative Step-5 units.
The remaining native gap is now specifically the VC++/Win98 service binding
for these table addresses plus the unresolved resource-relative families,
complete battle-part scaling, broader original damage differentials, and full
battle replay.

Evidence:

- `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- `analysis/DATA_LAYOUTS.md`
- `reconstructed/include/common.h`
- `reconstructed/src/lookup_part.c`
- `reconstructed/src/f_attack.c`
- `tests/lookup_part_table_probe.c`
- `tests/f_attack_packed_part_probe.c`
