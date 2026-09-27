# SO-E3 Gate 2 production audit

Status: **closed for the pinned pair, 2026-09-25**.

Scope: G2-04 for the pinned `vmctl-sye3k0` control and `vmctl-sye3k1`
action starting states. This is a host integration audit, not certification
of production for arbitrary games. Original output values are assertions only.

## Required effects and bindings

| Requirement | Current binding / branch evidence |
|---|---|
| Initialize the production workspace at the original AI stage | The first Turin pass calls `startup_queue_prepare`. Gate 1 proves both input queue sections empty. The adapter allocates an empty queue and saves planet bit 23 into the original saved-research bit 5. |
| Account for InitProduction's remaining work | Original `InitProduction`, NE 1B:0167 onward, builds an available-item catalogue, reconciles existing queue entries, and optionally advances the tutorial. Empty input queues give zero reconciliation iterations. The player is AI player 1, so the player-0 tutorial path is absent. `AddItemToQueue` receives a non-null global queue and therefore does not query item capacity from the available catalogue. That temporary catalogue has no consumer on this turn-zero path. |
| Derive the queue decision | Loaded starbase/population fields determine production eligibility. Source desirability runs. Loaded GAME.cPlanMax=32 drives Turin's turn-zero loop, producing two single-ship requests. No second Turin invocation remains. |
| Insert and commit the queue | Source `stars_add_item_to_queue` packs both entries; source `stars_finish_production` installs the queue. The AI log boundary verifies the queue is committed before production begins. |
| Initialize and consume extra-resource credits | Host-turn load clears the 32-entry table as original FGenerateTurn does. Both task-5 adapters pass the target entry to the source scrap service and read loaded race bits. Produce's `planet_extra` callback consumes that table. The pinned races do not earn an extra resource credit. |
| Visit the complete map | Produce and MineMinerals receive all 32 current planets. Loaded ownership selects the two owned worlds; all 32 population callback visits are accounted for. Unowned worlds have zero population. |
| Mine from current state | Source operating-mine limits and mineral estimation use current population, improvements, race attributes, concentrations, depletion levels, and shared RNG. Stocks, concentrations, and depletion levels return to the shared world. |
| Calculate production resources and costs | Source resource/factory/population services calculate 35/32 resources. Loaded research fields allocate 4 of player 1's 32 resources. Source hull cost reads the current design, giving [16,2,8,35]. |
| Execute the applicable build path | Source CBuildProdItem uses the current mineral/resource bank. The 28-resource budget yields 81% progress and zero completed ships. A completed build fails explicitly until creation callbacks are bound. Neither queue contains auto-build, infrastructure, terraform, alchemy, packet, or separate-partial entries; their branch callbacks are not reached. |
| Commit before downstream effects | `turn_produce_commit` writes all working mineral banks and the queue count before the first population callback. Research asserts this commit and all 32 visits happened. The former copy after random events is removed, preventing stale production banks from overwriting later effects. |
| Grow population and preserve fractions | Source desirability, maximum population, and population change use current populations/fractions and loaded races. Both populated worlds' resulting populations/fractions are retained; other worlds are source-ineligible. |
| Allocate and apply research | Credits are reset after mining and accumulated before source research. Current selections, balances, levels, flags, and original executable cost tables drive research and intelligence sharing. All 36 research/technology/resource bytes per player match each original output. No breakthrough occurs; newly eligible unsupported breakthrough effects fail. |
| Preserve queue state for writing | The queue writer consumes the committed pointer and current count, not a hardcoded count or reconstructed late queue. Its eight output bytes match the original M2/HST queue records. |
| Account for messages and events | Production captures its queue-empty callback and research captures its intelligence-sharing callback. Raw message delivery/filtering is G2-12. Produce invokes the random-event stage after state commit/research using loaded game flags; nested events remain G2-05. These packages remain open. |

## Closure boundaries

The audit closes G2-04's applicable production state transitions for this
pair. Upstream AI/order correctness remains G2-01/G2-02; nested events are
G2-05; complete messages are G2-12; shared RNG and whole-turn provenance
remain G2-13/G2-14. Full output streams and their complete differential are
Gates 3/4. A newly eligible unsupported branch must fail rather than silently
reuse these conclusions.

Final focused verification: **19 passed, 1 existing skip**, 15.971 seconds.
Both seeded replays and all 100 scout-roll cases pass. See the Gate 2 tracker
for the command and remaining package count.
