# FAttack packed-part resolver boundary

The raw `FAttack` loop reads each hull `_hs` entry, requires a nonzero high
byte count and a group containing `0x10` or `0x20`, then calls `FLookupPart`
before deriving initiative or dispatching damage. The typed API previously
accepted only pre-resolved `StarsAttackSection` values.

The new optional resolver receives the recovered group, item, and count
fields, fills a local resolved section, and leaves the caller's packed section
unchanged. The host adapter invokes the existing reconstructed
`stars_lookup_part` gate before filling the battle values, so the accepted
`0x10/item 7` component identity and the out-of-range `item 24` rejection are
both exercised. Empty slots are suppressed before the resolver, and resolver
failure prevents initiative matching and damage dispatch.

The strict host probe passes:

```text
F_ATTACK_PACKED_PART_CHECKS=8 FAILURES=0 RESULT=PASS
```

This is host-only source-fidelity evidence. It closes no named Step-5 unit:
the resolver is not yet a complete native packed `FLookupPart` binding with
the original table storage and all race/hidden/technology services, and native
packed replay, complete battle-part scaling, original damage differentials,
and full battle replay remain open.

Evidence:

- `analysis/decompiler/export/raw/10f0_69ac_FAttack.c`
- `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- `reconstructed/include/core.h`
- `reconstructed/src/f_attack.c`
- `tests/f_attack_packed_part_probe.c`
