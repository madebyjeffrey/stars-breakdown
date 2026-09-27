# DoMacintiAiTurn MA-E3: early-turn packed order kind

Status: **supporting evidence only; MA-E3 remains open**.

The original first fleet sweep at 10a0:2154–21b0 checks game turn
at most 10, then an owned fleet's signed colonizer count at record offset
12 greater than zero or ship slot 2 at offset 16 nonzero. It selects
that fleet, preserves every bit of the first PLORD kind word except its
low nibble, sets that nibble to 5 at offset 10, calls FLookupFleet to
publish the selected change, and immediately advances to the next fleet.

The bound Macinti path now preflights the first 18-byte PLORD record and
selection/refresh callbacks for this predicate. It mutates the selected
packed allocation rather than only its transient policy view, and skips
the generic action callback after the successful write. It does not claim
the earlier first-sweep attack/transport/miner/colonizer branches are
reconciled; in the original those can run before this late sweep label.

The strict-C89 host probe in tests/do_macinti_early_order_e3_probe.c
has **7/7** kind-5 checks within its expanded 14/14 result: turn-10
colonizer and ship-slot-2 writes, turn-11 and
foreign-owner exclusions, missing callback, short source PLORD, and
short selected PLORD rejection. It checks selected callback order,
preserved high bits, refresh publication, and next-fleet behavior. The
prior E1/E2/E3 probes remain 31/31, 12/12, and 8/8. The other seven
checks cover the [earlier kind-3 branch](do-macinti-attack-order-e3.md).
The full host suite passes **574 tests**, 153 expected skips, zero
failures; baseline and diff checks are clean.

The fresh combined DOS-8.3/CRLF VC1.52c packet at
compat/vc152/macinti-planet-table-e1-r16/ targets C:\MAE1N16 and
includes all seven probes (31/13/12/10/16/8/14). It is staged, not compiled or run
in Win98. A local listener bound port 2122 but received no STARSAGT
connection during this checkpoint; it was stopped without changing the
guest. Complete first-sweep ordering, multi-order PLORD route
sequencing, native return, and original-executable comparison remain
open. This work closes no named Step-5 unit or promotion.
