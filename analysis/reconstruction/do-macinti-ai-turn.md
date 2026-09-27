# DoMacintiAiTurn reconstruction (partial)

`reconstructed/src/do_macinti_ai_turn.c` transfers the design-maintenance and
turn-dispatch prefix of `DoMacintiAiTurn` (NE `0015:0008–2a05`) into typed C.
It preserves the starter-design threshold, merge-mask ordering, armada/support
potency ramps, the 50/70/100 turn-budget bands, six design-status groups, the
raw inactive `0x1d` filter for the first 12–13 status group, the shared
16-byte design-status mask, post-turn design splitting, shdef and
starbase maintenance, colonizer decision, and the explicit planet/fleet scan
boundaries.  The scan boundary now exposes typed planet policy summaries
(ownership, starbase, mineral score, viability, and the strict four-resource
production gate, including equality as affordable and a strictly negative
remainder as a shortage). The planet policy also exposes the raw 24-entry
production-queue capacity gate, while preserving an omitted-limit adapter path.
For an explicitly supplied next hull batch, the policy now repeats the raw
resource subtraction against the queued cost and per-channel true hull cost;
an exact zero remainder remains affordable, while a negative remainder blocks
that production candidate. A zero batch keeps the broader selection callback
available without inventing a design choice that the recovered tables do not
yet expose.
For hostile planets it also decodes the raw packet-strength threat band and
the starbase threat increment into a named threat level, matching the AI
planet-table marker used by the original scan.
Owned, queue-available, affordable planets now enter an explicit typed
production-selection boundary and are counted separately from the broader
planet scan; unowned and full-queue planets are excluded before that boundary.
The boundary now carries explicit queue and finish events: a caller can return
the selected design, quantity, object group, queue mode, finish mode, and raw
reason, while the adapter owns the hull-batch affordability check and records
queued, finished, and rejected actions. The older selection callback remains
as a compatibility path. After the late-turn status pass, the source emits the five raw
`SplitOutShdefs` calls: the shared status mask, masks selecting entries 0 and
1, and masks selecting entries 14/15 and 10/11.
Typed fleet policy summaries retain the friendly-fleet ownership
filtering, colonizer, transport, armed, mission-design, attack-design,
support-design, order-repair counts, and a named
repair/attack/transport/colonize/idle task recommendation), while leaving live
order mutation to callbacks. The fleet boundary now also carries explicit
plan/apply actions for the raw repair, miner, buddy, colonizer, wormhole,
starbase, attack, armada, freighter, and clear-task families, with accepted
and rejected action counters. Each action may now bind directly to its
corresponding reconstructed routing/order procedure, with the generic apply
callback retained as a compatibility fallback. The post-scan phases are
explicit as well: friendly transport fleets pass through a transport-routing
callback, all friendly fleets pass through a final fleet callback, and the raw
basic-task and production-queue finalizers are represented as ordered
callbacks after those scans. The turn input also records the raw fleet-role
scan totals for colonizer, mission, support-design, attack-design, and armed
fleets. Planet and fleet records now optionally carry their original raw
record pointer and map point, so these planners can bind directly to the
recovered AI tables and order helpers.

The source probes pass the early-turn and late-turn threshold paths, the typed
mission-design fleet group, the typed planet/fleet policy checks including the
queue-full boundary, the owned production-selection boundary, the production
resource-gate boundary (including equal-cost and underfunded cases), the
queued-cost-plus-hull-batch gate, and the typed task recommendation boundary
under strict host C89; the production event dispatcher is covered by a
separate three-check probe, and the fleet action boundary by a separate
three-check probe. A fresh packed integration probe binds the recovered
56-byte `_planet` and 124-byte `_fleet` views, including owner, flags, packet,
planet, cord, and sixteen ship-count fields, and passes 31 host/native checks
over design-mask splitting, production queueing, fleet action dispatch,
transport routing, and finalization order. Production/table choices and
detailed order decisions remain explicit service results rather than invented
raw offsets. The remaining per-planet table selection, exact
retarget/colonizer/transport decision predicates, direct packed order
mutation, and complete original-behavior evidence remain open. The typed
planet and fleet records now carry optional raw-record pointers and map points
so those remaining predicates have an explicit table-integration input.
