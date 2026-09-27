# DoCyberAiTurn CY-E3 — colonizer second-order AI marker

The recovered `DoCyberAiTurn` at NE 22:~111C calls
`FColonizeAiFleet`. If it succeeds and the resulting fleet has more than
one order, the caller reads the planet ID from the second 18-byte order
(`PLORD` byte offset 26) and writes `4` to `vlpbAiPlanet[id][15]`.
The reconstructed caller now performs this write after its colonize action
callback succeeds. It binds a bounded raw AI-planet table and raw order
allocation; absent table binding preserves the earlier typed boundary.

The strict-C89 caller probe runs a two-order queue through the production
`stars_ai_move_order_to_plord` helper to append a third order. It checks that
the first two complete records stay intact, the third order is written,
only the second order's AI-planet marker changes, a non-colonize action and
a failed callback do not mark, and a one-order queue that grows to two and
an out-of-range second target are handled. Host result:
`CYBER_SECOND_ORDER_E3_CHECKS=200 FAILURES=0 RESULT=PASS`.
The existing 34-check action matrix and 24-check order-write probes also
pass against the updated caller.

Fresh package `compat/vc152/cyber-second-order-e3-r1/` ran in StarDev at
`C:\CYBE3N1` under VC++ 1.52c. `runs/cyber-second-order-e3-r1/BLDSTAT.TXT`
reports compile/link PASS; `CYBE3RES.TXT` reports 200 checks PASS and the
process exited 0. The return retains compile/link logs, map, four objects,
executable, report, and agent log.

This validates one recovered caller-side table write with synthetic packed
records. It does not prove all Cyber route predicates, original live table
identity, reallocating order storage, or a complete original/native turn.
CY-E3 and CY-E4 remain open.
