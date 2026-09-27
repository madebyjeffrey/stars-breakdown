# DoMacintiAiTurn MA-E3: first-sweep attack-design order kind

Status: **supporting evidence only; MA-E3 remains open**.

The original first fleet sweep at 10a0:1b8d–1ded gates its attack-design
branch when either the colonizer count at packed fleet offset 12 is below
one or the unsigned turn is below 41. If either ship slot 14 or 15 is
positive and the fleet has exactly one order, it reads the first PLORD
kind word at offset 10. When that low nibble is not 3, it selects the
fleet, writes kind 3 while preserving the word's other bits, refreshes
via FLookupFleet, and jumps to the next fleet. An already-kind-3 order
continues into later buddy/miner decisions. This branch precedes the
early-turn kind-5 write at 10a0:2154.

The bound source now preflights the selected-fleet callbacks for that
mutation, uses the same selected PLORD write boundary as the kind-5
branch, and advances after a successful kind-3 refresh. The original
buddy/miner actions following an already-kind-3 order are not yet
implemented; the existing typed action adapter remains transitional.

The expanded strict-C89 order probe in
tests/do_macinti_early_order_e3_probe.c passes **14/14** checks: its seven
earlier kind-5 cases plus attack-design kind-3 write, turn-41
colonizer-absent inclusion, turn-41 colonizer-present exclusion,
already-kind-3 continuation, kind-3 precedence over early kind-5,
missing-callback preflight, and short-selected-PLORD rejection. It
checks high-bit preservation, selected callback sequence, and refresh
publication. Earlier Macinti E1/E2/waypoint probes pass 31/31, 12/12,
and 8/8.
The full host suite passes **574 tests**, 153 expected skips, zero
failures; baseline and diff checks are clean.

The fresh combined DOS-8.3/CRLF VC1.52c packet at
compat/vc152/macinti-planet-table-e1-r16/ targets C:\MAE1N16 and
includes all seven probes (31/13/12/10/16/8/14). It is staged, not compiled or
run in Win98. The remaining first-sweep attack/buddy/miner/transport/
colonizer decisions, complete multi-order PLORD sequencing, native
return, and original-executable comparison keep MA-E2, MA-E3 and MA-E4
open; this check closes no named Step-5 unit or promotion.
