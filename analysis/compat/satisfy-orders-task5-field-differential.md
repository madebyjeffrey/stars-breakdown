# SatisfyOrders task-5 field differential

Status: **the service-order and field-level differential is complete**. The
live current-source binding for the exact high-offer race-bit-C input remains
open under SO-E2.

## Original and current-source comparison

| Field or service | Original Stars! evidence | Current-source VC1.52c evidence | Comparison |
|---|---|---|---|
| Planet scrap technology result | Matched task-0/task-5 runs retain the same shared input H2 and high-offer SHDEF. The positive case raises player-0 tech; the negative case sets the trader-tech bit without changing battle-tech levels. The fresh bit-C pair adds a positive `0x13D`, player-0 tech 0 `3→4`, and target-fleet retirement. | `tests/satisfy_orders_task5_services_probe.c`, native 30/30, exercises positive `0x13D` and negative `0x13C` results through the packed task-5 path. | Both signed branches, result index placement, message recipients, and fleet/order retirement agree. The exact high-offer current-source input is still a separate binding gate. |
| Resource-conversion branch | The pinned race-bit-5 original turn adds 10/7/6 minerals, emits `0x5D/0x143`, and removes fleet 0. | `tests/satisfy_orders_task5_resource_probe.c`, native 75/75, checks mark-techs/reprice/resource/message/removal order, `0x5D/0x143`, both signed extensions, extra-resource mutation, and retirement. The verified-input `CResourcesAtPlanet` probe is native 22/22. | Message IDs, resource argument placement, mineral credit, and retirement agree. Verified planet/player fields feed reconstructed `CResourcesAtPlanet`, which returns 35 base resources and adjusted task-5 amount 14. |
| SHDEF costs | The cost oracle pins the original executable and design-0 HST. The v4 original pair pins the actual 11-component hull-9 high-offer design, player-1 tech `[0,0,1,0,5,0]`, race stat 1, and race bits `0x80003045` with bit C set. | `tests/satisfy_orders_shdef_reprice_probe.c`, native 67/67, resolves real original executable part records with reconstructed `FLookupPart` and `GetTruePartCost`, then invokes repricing from task 5. | Recovered part-table addressing, four cost lanes, and repriced task-5 accounting are checked against original records. This native run uses design 0 and forces the branch; it does not yet exercise the exact high-offer v4 design under its live race condition. |
| Deep-space salvage | The natural original control/action pair retires the fleet and stores the 5/0/2 salvage thing. The human-message pair preserves the original `0x5B` frame with `-6`, salvage id `0x2200`, and fleet word 0. | `tests/satisfy_orders_smoke_deepspace_probe.c`, native 21/21, matches all 18 saved thing bytes after `ThingDecay`, mineral lanes, callback frame, and fleet retirement. | Salvage creation, message fields, post-decay thing state, and retirement agree. |
| Callback and mutation order | Original branch order is retained from the decompiled task-5 path at `10b0:8207-8e65`. Original M1 returns preserve the resulting message sequence for each matched run. | Native traces assert `MTMRD` for planet positive/negative tech paths, `KUUUUCMTMRD` for resource conversion, and `SmRD` for deep-space salvage. The service probe also checks that message removal sees the fleet alive and colonists unchanged, before dead-bit storage and transfer. | Current-source order matches the recovered sequence: mark/reprice/resource services as applicable, message, tech result, old-message removal, fleet dead bit, then completion/transfer. |

The v4 paired return set and reproducible validator are in
[`satisfy-orders-reprice-v4-bitc-original-oracle.json`](satisfy-orders-reprice-v4-bitc-original-oracle.json),
with per-side hashes and decoded state in the [control run](../../runs/vmctl-sytech-reprice-v4-bitc-control/RUN.JSON)
and [action run](../../runs/vmctl-sytech-reprice-v4-bitc-action/RUN.JSON).
This differential closes the SO-E2 field-comparison check. The exact high-offer
v4 SHDEF, verified player-1 table/race state, and matching original
planet/message inputs now also pass current-source task 5 in the fresh
[VC1.52c native run](../../runs/vmctl-soe2s/MANIFEST.JSON), including live
repricing. SO-E2 is complete. SO-E3 separately owns the surrounding full-turn
and RNG comparison.
