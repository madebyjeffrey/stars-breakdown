# FDoCoolBattle packed token round-trip checkpoint

Date: 2026-09-21

## Scope

This host-only checkpoint makes the Step-5 packed-token concern concrete. It
does not claim that the full `_tok`/global mapping is recovered. The strict
C89 probe connects the readable `FDoCoolBattle` coordinator to a pair of
little-endian 29-byte packed token records through the production
`packed_tokens` bridge on `StarsCoolBattleInput`.

The exercised raw offsets are:

| Offset | Width | Meaning used by the bridge |
|---:|---:|---|
| `0x00` | word | token id |
| `0x02` | byte | player/owner |
| `0x03` | byte | object kind |
| `0x04` | byte | ship design |
| `0x05` | byte | battle coordinate |
| `0x06..0x08` | bytes | base/minimum/maximum initiative |
| `0x09` | byte | target token index |
| `0x0a..0x0e` | bytes | cloak/jam/battle/capture/beam-defense percentages |
| `0x0f` | word | token weight |
| `0x11` | word | shield damage points |
| `0x13` | word | token count |
| `0x15` | word | packed defense value (`_dv.dp`) |
| `0x17` | word | target flags |
| `0x19` | word | distance-limit/max-distance/speed/target-count nibbles |
| `0x1b` | word | active bit and movement/log flags |

The production bridge loads both records after initialization and stores the typed state
after movement, attack, and final reporting. The attack callback deactivates
the second token, allowing the coordinator's real survivor termination path to
drive the write-back. It also verifies that the first token's id, owner,
design, count, and every other raw field survive the round-trip. The exact
raw-byte comparison permits only the expected defeated-token count and active
bit changes.

## Result

```text
F_DO_COOL_BATTLE_PACKED_TOKEN_CHECKS=11 FAILURES=0 RESULT=PASS
```

The probe was compiled with `cc -std=c89 -pedantic -Wall -Wextra -Werror`
against the reconstructed battle, movement, board, targeting, and battle
support sources. It is host-source evidence only; no Win98 run was performed
for this checkpoint. Its final case reduces the packed-token capacity below
the initialized token count and verifies that `FDoCoolBattle` rejects the
invalid packed boundary before treating it as a successful battle.

## Step-5 interpretation

This closes a host sub-boundary for the complete typed-to-packed token field
map, ownership, and mutation ordering. It closes **0 authoritative Step-5
units**. The remaining
FDoCoolBattle Step-5 work is:

1. bind the callbacks to the exact native `_tok`/`vrgtok` globals and all
   production callers, including global compaction/append behavior;
2. compare exact movement geometry and speed/initiative behavior;
3. complete damage, shield, casualty, salvage, bombing, and record cases;
4. run the fresh VC++ 1.52c/Win98 packet and capture no-error evidence; and
5. perform the original executable/full battle replay before promotion.

The native guest agent is currently pending a reliable handshake, so the
packed-token packet is intentionally not represented as a native closure.

## Evidence

- `reconstructed/src/f_do_cool_battle.c`
- `reconstructed/include/core.h`
- `tests/f_do_cool_battle_packed_token_probe.c`
- `analysis/decompiler/export/raw/10f0_8bcc_FDoCoolBattle.c`
