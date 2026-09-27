# SatisfyOrders Phase-2 E2 closure

**Historical reopening superseded:** On 2026-09-23, SO-E2 was reopened while
the original task-5 scrap/transfer branch and its fleet/resource/message
effects were still absent from the partial adapter. The packed salvage,
technology-result, message-removal, and resource follow-ups were subsequently
bound to production. The final exact-input high-offer repricing gate then
passed current-source host and VC1.52c/Win98 native validation 74/74; see
[`satisfy-orders-task5-v4-current-source-native.md`](satisfy-orders-task5-v4-current-source-native.md).
SO-E2 is closed. The remaining full-turn/RNG differential is SO-E3.

This is the authoritative aggregation for the SatisfyOrders E2 boundary. It
closes E2 only; the complete `DoOrders`/`SatisfyOrders` full-turn replay and
original executable differential remain the separate E3 unit.

## Closure decision

The E2 acceptance boundary is now covered by independent strict-C89 host
probes and fresh VC++ 1.52c/Win98 replays over the production source:

| E2 concern | Evidence | Native result |
| --- | --- | --- |
| Technology-table/result binding and production research side effects | `satisfy-orders-technology-binding-e2-native`, `satisfy-orders-production-e2-native` | 12/12 and 18/18 pass |
| Created-player fleet and 0x93-byte design materialization | `satisfy-orders-fleet-copy-e2-native`, `satisfy-orders-design-materialization-e2-native` | 10/10 and 15/15 pass |
| Typed/packed salvage handoff and state mutation | `satisfy-orders-production-e2-native` | 18/18 pass, including typed and packed paths |
| Special-order conditions and completion behavior | `satisfy-orders-special-e4-native`, `satisfy-orders-packed-special-e5-native` | 16/16 and 10/10 pass |
| Ordinary, player-transfer, route, mine, merge, shortage, colonist, technology, fuel, and failure message frames | raw E2 packets `SYOE12` and `SYOE13`–`SYOE22` | every packet reports PASS; all recovered payload words are checked |

The raw message matrix includes the caller-resolved fields that cannot be
recovered from the reduced typed order alone: fleet identity markers, target
words and coordinates, `WFromLpfl`, raw target IDs, technology result/amount,
fuel demand/stat values, thing kind, and the pass-four failure gate. The
original `turn3.c` and Ghidra enum evidence supply the branch and message-code
oracle.

The corrected technology rerun also verifies that a technology order emits
only its dedicated signed-result frame and does not duplicate the ordinary
resource frame. All nine affected raw-order probes pass 141/141 checks.

## Evidence boundary

The closure aggregates these production-source native packets and their host
probes:

- `satisfy-orders-production-e2-native` — concrete `itech_learn_a_tech` and
  `drop_salvage` service paths, typed and packed.
- `satisfy-orders-fleet-copy-e2-native` and
  `satisfy-orders-design-materialization-e2-native` — destination fleet/design
  allocation, matching, fixed-record copy, flags, counts, and status.
- `satisfy-orders-technology-binding-e2-native` — signed technology result and
  message-code binding.
- `satisfy-orders-special-native-e4` and
  `satisfy-orders-packed-special-e5` — special-order gates and packed bridge.
- `satisfy-orders-raw-transfer-e2-native` and the corrected raw packets for
  route, mine, merge, shortage, colonist, technology, resource, foreign
  colonist, fuel, and failure frames.

All native packets compiled and linked with VC++ 1.52c, exited normally, and
were observed without a Program Error dialog. The packet reports and logs are
preserved under their unique `runs/vmctl-vc152syoe*/` directories and are
listed in the review ledger.

## Explicitly not closed here

- The complete four-pass `DoOrders`/`SatisfyOrders` turn differential.
- Original executable message-stream comparison over a full live turn.
- Step-6 synchronization and promotion.

Those remain the SatisfyOrders E3 and promotion work. Closing E2 reduces the
procedure from `2 + P` to `1 + P` open Phase-2 units while it remains at the
active-gate label `5/6`.
