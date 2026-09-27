# SO-E3 Gate 2 — movement closure audit

Status: **G2-03 closed for the pinned control/action pair, 2026-09-25**.
This is a package within the complete-turn service gate. Gate 2 remains open.

## Requirements and evidence

| Requirement | Current evidence |
|---|---|
| Enumerate surviving fleets from current state | All four owner-1 slots are considered using runtime presence and source task-5 dead flags. All six owner-0 slots are loaded from current host state. Owner-0 task-5 dispatch uses the loaded order; its flags/order bytes write back before movement. The source mover receives these activity states. The pair produces 8/7 survivors. A copied-state check proves the oracle case label cannot determine survival. |
| Read actual movement orders | Owner-1 selection uses the first-order task and current next order. Fleets with fewer than two orders or zero next warp skip. All surviving owner-0 fleets have one order and therefore skip. The only moving fleet is owner-1 scout 0; its current destination is a planet and its warp is 5. |
| Derive travel and fuel | Source distance is rounded with the original `(long)(distance + 0.9999)` rule. The limit is current next-order warp squared, not the first waypoint's warp. Current ship counts, design mass, hull capacity, cargo, engine fuel table, and race discount feed source EstFuelUse. Source fleet statistics supply fuel capacity. The result is positive and affordable; insufficient/zero-cost paths fail explicitly rather than using the semantic mover's different fallback behavior. Both pinned scout outputs retain fuel 298. |
| Account for optional movement effects | Loaded player flags exclude cheater refusal. Warp 5 skips cheap-engine failure, warp-10 damage, and stargates. Current planet targets exclude pursuit, wormhole traversal, and nearest-object arrival search. The moving fleet has zero colonists, excluding AR/radiation deaths. Both conditions are checked rather than supplying zero effects. All four current things are wormholes, as established by G2-06, so minefield travel cannot produce an interaction. |
| Account for ram-scoop gain | Current engine fuel entries and current component/ship counts feed source stars_fuel_gain_from_ram_scoops; its result is zero. Newly nonzero gain fails explicitly until a corresponding cargo/message path is bound. |
| Commit movement state | Positions, planet, fuel, fHereAllTurn, fDone, and direction/warp-valid fields update host fleet state and owner-1 packed state. Direction scaling follows original MoveFleets 10b0:4686..4782. Stationary fleets retain their loaded position/planet and zero direction. |
| Commit arrival orders | The coordinator updates waypoint zero as original 10b0:4e12..4e6d, calls corrected source KillUsedWaypoints and source DeleteWpFar, and writes complete 18-byte orders/counts into host and packed state. The scout's completed waypoint is removed. Subsequent waypoint checks see 8/7 orders for 8/7 survivors. |
| Preserve message side effects | The source completion callback records owner 1's fleet-0 0x4e event. Its generation is checked. Full message-buffer serialization/filtering is explicitly G2-12, which remains open. |
| Preserve transport cargo across stages | Source ChangeCargo writes the AI supply fleet's current five cargo lanes into both packed and host fleet state. Movement therefore observes current cargo. Writers use current cargo/design-plan/order-count state instead of restoring retained input lanes or supplying a separate colonist exception. |
| Verify results independently of runtime inputs | Both pinned original fleet/order record comparisons pass with these state mutations. Expected destinations, fuel, and order bytes are assertions only. The original RNG traces continue to match; movement adds no random calls for these inputs. Broader RNG provenance is G2-13. |

## Verification

`python3 -m unittest tests.test_satisfy_orders_input_corpus tests.test_satisfy_orders_turin_binding -q`

Result: **19 passed, 1 existing skip**, 15.863 seconds. The integrated test
runs the two complete initialized inputs and the existing 100 scout-roll cases.
The existing skip concerns original-x86 execution through Unicorn on this host.

The corrected waypoint source/integration/packed/turn probes passed strict C89
compilation in the preceding package work. The additional DeleteWpFar source
probe passes ordinary deletion, recycling, same-position fallback, and the
last-waypoint recycle fallback. `git diff --check` passes.

No new original/guest run was made. The original comparison outputs remain
runs/vmctl-sye3k0 and runs/vmctl-sye3k1. Historical native waypoint reports
predate the corrected source and do not certify it.

## Scope

This audit establishes the applicable movement path for both pinned inputs.
It does not certify arbitrary pursuit, stargate, shortage, zero-fuel, partial
travel, radiation, or positive scoop cases. Source/input guards and matched
endpoints establish those branches are absent here. Full startup/order/message
integration and end-to-end provenance remain G2-01/02/12/13/14. Complete output
stream assembly and comparison remain Gates 3 and 4.
