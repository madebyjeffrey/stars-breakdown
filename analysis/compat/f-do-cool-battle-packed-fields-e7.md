# FDoCoolBattle packed field bridge checkpoint

Date: 2026-09-22

## Scope

This host-only checkpoint strengthens the production `packed_tokens` bridge
without changing the Phase 2 closure count. The probe uses a 32-byte stride
with a 29-byte `_tok` record followed by sentinels, checks every scalar field
through the typed movement callback, checks the packed movement nibbles and
active/log flags, then mutates only the speed nibble and defeated-token state.

The checked packed layout is:

| Offset | Width | Typed value checked |
|---:|---:|---|
| `0x00` | word | id |
| `0x02..0x05` | bytes | player, object kind, design, battle coordinate |
| `0x06..0x08` | bytes | base/minimum/maximum initiative |
| `0x09..0x0e` | bytes | target and five combat percentages |
| `0x0f..0x17` | words | weight, shield, count, defense, target flags |
| `0x19` | word | distance limit/max, speed, and target-count nibbles |
| `0x1b` | word | active and movement/log flags |

The sentinel bytes at offsets `0x1d..0x1f` verify that the bridge honors the
configured stride and does not write beyond the recovered 29-byte record.

## Result

```text
F_DO_COOL_BATTLE_PACKED_FIELD_CHECKS=24 FAILURES=0 RESULT=PASS
```

The strict C89 host probe passed with `-pedantic -Wall -Wextra -Werror`. It
also confirms that changing the typed speed field rewrites only the speed
nibble, while defeating the second token rewrites only its count and active
bit. This is stronger host evidence for the packed bridge, but it is not a
native Win98 replay and does not establish the original global `_tok`/`vrgtok`
caller binding.

## Step-5 interpretation

This closes no authoritative Step-5 unit. The remaining FDoCoolBattle work is
still exact native global binding and compaction, movement geometry and
initiative behavior, complete damage/shield/casualty/salvage/bombing records,
a fresh native full-battle replay, and original-executable differential
evidence before promotion.

## Evidence

- `tests/f_do_cool_battle_packed_field_probe.c`
- `reconstructed/src/f_do_cool_battle.c`
- `reconstructed/include/core.h`
- `analysis/debug/types.json` (`_tok`, 29 bytes)
- `analysis/decompiler/export/raw/10f0_8bcc_FDoCoolBattle.c`
