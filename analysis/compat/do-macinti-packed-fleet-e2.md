# DoMacintiAiTurn MA-E2: packed fleet/action bridge

Status: **supporting evidence only; MA-E2 remains open**.

CodeView records `_fleet` as 124 bytes with ID at 0, owner at 2,
current planet at 6, point at 8/10, sixteen signed 16-bit `rgcsh`
values at 12–43, and `cord` at 98. The first packed PLORD order begins
after a four-byte header, with its kind word at offset 10. The earlier
Macinti adapter held ship counts in eight-bit typed fields and its fleet
action loop ran only when the observation callback was installed.

The bound source now preflights each fleet's packed length, ID, nonnegative
order count and ship counts, and a first PLORD record for an owned fleet
with an order. It forms a transient view from packed owner, planet, point,
`cord`, all sixteen 16-bit counts, and first-order kind. The owned action
loop runs without `visit_fleet_typed`; optional planning/action, transport,
and finalization callbacks all receive the bound view. First-order repair
still mutates the caller-owned PLORD before action planning. The later
transport/finalization passes rebind the view, so they see the repaired
kind. The unbound typed path retains its earlier callback-driven behavior.

The original `10a0:1a3b` second fleet sweep is now bound separately from
the first action loop. It examines only owned fleets with `cord < 2`, asks
the original `FIsAiTransport` service, then (if true) changes packed
`iplan` from its byte-96 value to 4 through a selected-fleet/refresh
callback before `IdTargetMacFreighter`. The source no longer treats
ship slot 1 alone as the transport predicate in this bound pass. The
callback owns the live selected-fleet mutation; the transient view passed
to the route callback reflects plan 4 after a successful update. The
unbound compatibility path retains its older slot-1 rule.

The strict-C89 probe `tests/do_macinti_packed_fleet_e2_probe.c` passes
**12/12** checks. It uses stale typed owner/count/point fields and a packed
300-ship transport count, verifies repair → plan/action → transport →
finalize ordering without an observer, confirms the typed source was not
overwritten, rejects short fleet/PLORD, negative ship count, and ID
mismatch before actions, and excludes a foreign packed owner. It also
checks the `cord >= 2` and false-transport exclusions, the already-plan-4
skip, and missing-service preflight. The earlier
integration probe no longer truncates its typed conversion to 8 bits and
still passes 31/31. The full host suite passes **574 tests**, 153 expected
skips, zero failures; baseline and diff checks are clean.

A fresh combined E1/E2/E3 DOS-8.3/CRLF VC1.52c packet is staged at
`compat/vc152/macinti-planet-table-e1-r16/` for `C:\MAE1N16`. It includes
the current 31/13/12/10/16/8/14 probes, separated compile logs and a combined report; it has
**not** run in Win98 because STARSAGT is disconnected. MA-E2 still needs
the first-sweep transport/retarget/colonizer predicates, candidate choice,
packed route/order sequencing, fresh native return, and an action-bearing
original comparison. The 12/12 check closes none of those as a whole.
