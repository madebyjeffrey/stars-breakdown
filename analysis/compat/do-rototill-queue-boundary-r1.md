# Rototill final queue population boundary, r1

The retained Rototill backup X2 contains a final type-29 mutation body
`18 00 03 1c 02 00`: planet 24 receives a planet-group factory item
(kind 7) of quantity **3**. The turn-1 M2 queue ends with one factory item
at progress 89, while the planet's factory count rises from 10 to 12.
These are separate AI-queue and post-production observations.

The selector-3 coordinator now emits all seven original X2 mutations byte
for byte, including source `XferAiSupply` moving 25 colonists from planet 24
to fleet 1 and the final factory queue mutation. Its transfer adapter
observes planet cargo population 250 to 225.

## Source-backed boundary diagnostic

A retained strict-C89 diagnostic in the
[selector-3 coordinator](../../tests/satisfy_orders_rototill_coordinator_e4_probe.c)
composes the retained planet and player input with current-source
`InitProduction`, `GetResourcesAvailable`, `GetProductionCosts`, factory
operating/capacity helpers, and `FFillProdMinesAndFactories`. A recording
callback captures the proposed additions without mutating the actual queue
or memory log. It uses the live four-design starbase catalog already created
by `IroEnsureAi`. The two population states produce:

| Planet 24 population used by queue | Selected “no research” bit | Base resources | Available resources | Factory unit cost | Operating / maximum | Source additions |
|---:|---:|---:|---:|---:|---:|---|
| 225, after transfer; live turn-0 research 0% | 0 | 32 | 32 | 10 resources, 3 germanium | 10 / 22 | 3 factories only |
| 250, retained loaded planet; live turn-0 research 0% | 0 | 35 | 35 | 10 resources, 3 germanium | 10 / 25 | 3 factories, then 1 mine |
| 225, after transfer; live turn-0 research 0% | 1 | 32 | 32 | 10 resources, 3 germanium | 10 / 22 | 3 factories only |

The catalog has 18 entries in these cases. The diagnostic is now asserted by
`tests.test_satisfy_orders_turin_binding`; its strict-C89 coordinator build
and full test pass. The same test pins the original X2 type-29 body
`1800031c0200`. The diagnostic rows are host compositions, not an original
internal state trace. The coordinator now also binds the actual final queue
callback and compares its logged type-29 body to that original record.

The original `XferAiSupply` delegates to `ChgCargo`, which commits the
planet change through `FLookupPlanet(-1, ...)`. A later
`ChangeMainObjSel(1, planet)` normally reloads the canonical planet.
`InitRandomPlanetList` builds `vrglpplAi` from pointers directly into the
canonical `lpPlanets` array, so the final queue's AI planet pointer aliases
the post-transfer canonical record. The 250-population diagnostic is not a
valid live-state binding. Original `GetResourcesAvailable` reads the
no-research bit from `sel.pl`; `FinishProduction(0)` can carry a game flag
into it, but the bound retained path leaves that bit clear.
The original Rototill caller also runs
`HandleBasicAiTasks` **between** this transfer and `FillProductionQueue`.
The coordinator now binds that retained-input stage through source
`KeepFleetsMoving`, the existing-starbase `QueueAiStarbases` skip, source
`stars_handle_basic_ai_tasks` planet-action order, and source
`FinishProduction(0)`. All four owner-1 fleets are supplied; speed handling
visits fleet 0's two-order scout route and preserves its planet-26 warp-5
order. The planet action loop invokes source upgrade, fling, scanner,
defense, and terraforming helpers, all returning without a queue addition.
Upgrade consumes the next source `Random(100)=12`. The turn-0 attack-fix
time gate and absent-production blocked-queue gate skip their downstream
work. The selected planet remains at 225 colonists with its no-research bit
clear after finish. This is a retained-input host binding, not original
internal-state evidence.

The retained coordinator now checks four bounded basic-task gates with the
current-source helpers at the post-transfer 225-population state. The
available mineral vector is `[538,964,506]`, whose sum 2008 is below
`FAIFling`'s 3000 threshold. The population 225 is below the defense
planner's 1600 threshold. The source `InitProduction` catalog has no
planet-group scanner (kinds 18–26) or terraforming item (kind 12), and the
source scanner/terraforming helpers both return without an addition. The
latest starbase slot is 0. The strict-C89 coordinator regression passes
these checks. The four no-addition checks are now part of the bound
basic-task callback. The original input player's research percentage is 15
and remains 15 in the original turn-1 output; the original research target
byte also stays 96. The retained loaded planet improvement word is
`0x0000000a`, so its no-research bit is initially clear. The original
witness regression pins the player bytes, and the coordinator pins the
improvement word.

## Turn-zero research write resolves the quantity discrepancy

The earlier 28-resource diagnostic accidentally used the **loaded** player
research percentage, 15%, for the live AI queue. Original `DoRototillAiTurn`
passes `pct=0` to `IroEnsureAi` when `game.turn < 20`; original `IroEnsureAi`
writes this value to the live `rgplr[idPlayer].pctResearch` before any fleet
or production work. The reconstructed coordinator now applies that source
write to its live player table, and its source resource callback reads 0%.
At canonical population 225 with the no-research bit clear, available
resources are 32. The factory filler therefore proposes exactly three
factories with two resources left and no mine, matching the original type-29
body. At stale population 250 and the same 0% research, it would propose a
mine too, so the original queue also discriminates in favor of the canonical
post-transfer planet. The original turn-1 serialized player percentage is
15%; that later output does not show the live AI-phase percentage.

The original phase handoff is now source-bounded. [`EnsureAis`](../../../stars-decompile/decompiled/all/mdi.c)
runs [`DoAiTurn`](../../../stars-decompile/decompiled/all/ai.c) on the
player-view M2, then destroys that game and reloads HST.
[`FGenerateTurn`](../../../stars-decompile/decompiled/all/turn.c)
destroys/reloads HST again before it replays each player's X2 through
`FLoadLogFile`/`FRunLogFile`. The retained Rototill X2 has only
seven non-header mutations, with kinds `27,27,27,27,4,1,29`; it has no
`rtLogResearch` (kind 34) record. Original
[`FRunLogRecord`](../../../stars-decompile/decompiled/all/planet.c) changes
`pctResearch` only on that research record. Thus the temporary AI 0% write
does not overwrite the reloaded host player's 15%, explaining the later
Produce input and output. The focused host probe uses those two phase values
from source and retained file bytes. The later
[selector-3 coordinator](do-rototill-coordinator-e4-r1.md) now runs the
retained reload/replay and turn stages together through source
`stars_generate_turn`.

## Bound final queue and mutation record

The retained selector-3 callback now runs source `stars_fill_production_queue`
over the post-transfer planet, with source selection, `InitProduction`,
`GetResourcesAvailable`, empty `GetProdQCost`, factory/mine capacity and
`GetProductionCosts` helpers, `FFillProdMinesAndFactories`, packed
`AddItemToQueue`, `FinishProduction(1)`, and `LogChangePlanet`. It asserts
the 32-resource budget, 10 operating and 22 maximum factories, a single
three-factory queue item `03 1c 02 00`, and the emitted type-29 body
`18 00 03 1c 02 00`. The original backup X2's ordered seven mutation
types and bodies all match the generated host stream. This is a source
composition with the retained input and recovered main RNG seed; it is not
yet a native full-turn result or a persisted `Produce` comparison.

## Focused post-queue Produce boundary

A follow-on retained-input host check starts from the generated three-factory
packed queue and runs source `stars_produce`, `stars_build_production_item`,
`stars_get_production_costs`, and `stars_build_object` for planet 24. At this
later turn-generation boundary it reads the retained M2 player percentage
**15%**, while the preceding AI callback has 0%. Source resources are 32;
15% research spends four, leaving 28. Two factories consume 20 resources,
and the remaining eight yield **89%** progress on the third under the
original partial-build rounding rule. Factory count reaches **12** and the
source queue writer emits `01 1c 92 05`, compared directly with the retained
original turn-1 M2 type-28 body. This check isolates the one-planet build
path: it does not yet run all 32 planets' mining, research, messages,
population growth, or serialized full-turn output. The 0% AI-to-15% Produce
handoff follows the source reload/replay order above. The later source
coordinator binds that handoff for the complete retained turn.

The [original AI4 decompilation](../../../stars-decompile/decompiled/all/ai4.c)
at `FFillProdMinesAndFactories` divides the resource balance by factory
cost, truncates to an integer, and caps it by factory capacity. It does not
round this branch upward. The reconstructed shared filler follows that
rule; changing it to ceiling division would be inconsistent with the
original routine.

## Required next evidence

The integrated host turn now compares the 0%-to-15% handoff, post-`Produce`
mineral and population deltas, research, messages, planet records, and full
output files with the retained original. A
[VC1.52c boundary](do-rototill-production-turn-e1-native-r3.md) passes for
source queue/Produce arithmetic, but its resource and cost callbacks still
use host-proven values. Bind those native live services for RT-E1.b, then
run the complete selector-3 turn natively and compare it with original for
RT-E4.d/e.
