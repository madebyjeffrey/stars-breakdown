# SO-E3 G2-01 AI closure audit

Status: **closed for the two pinned complete starting inputs**, 2026-09-26.
Scope is the selector-1 AI turn and its transition to the host generation load.
This package does not claim arbitrary Stars! games, complete output streams, or
closure of G2-02, G2-12, or G2-14.

## DoAiTurn entry and exit

| Requirement | Bound evidence |
|---|---|
| Fresh entry | Source DestroyCurGame runs in nine heap-reset steps before M2 load. The runtime reader loads the current player, design banks, planet, fleet records and orders. |
| Personality | Loaded player mode and dead bit choose the source Turin dispatcher. The active player is published during callbacks and restored to the caller afterward. |
| Temporary ownership | Three allocated AI buffers have source initialization and checked release after the generated X2 and H2 are closed. Host destruction rejects live allocations and removes runtime aliases to the released AI planet table. |
| Preparation | Design power, attack marking, minefield growth and planet-list preparation execute source-backed adapters over current records. Newly eligible unsupported effects reject. |
| Log and history | Current mutations generate the complete encrypted X2 and H2 in memory. The generated X2 is read through source file/log services and replays ten mutation records; original record bodies and H2 files verify the result. |
| Host transition | A second source DestroyCurGame runs before host load. Nine ordered heap callbacks clear live planet, fleet, design, order, queue, message and selection state. The decoded starting view reloads the world, and generated X2 replay reapplies mutations. A poisoned pre-AI merge snapshot still passes replay. |
| Session | Shared flag words, loaded game ID, player, counts and selection are published. The original save-selection callback sees the selected object before source cleanup clears it; host load restores identity and counts. Reset assertions include log/message sentinels, salt and tick. |

## Turin decision and callback census

| Path | Current result or exclusion for the pinned pair |
|---|---|
| Ensure AI, four merges, design status, ship maintenance | Source-backed callbacks operate on current research and design banks. Four created starbases have source names, parts, counts, costs, damage and design-change logs. Newly eligible recycling or unsupported creation rejects. |
| Split designs | Original turn > 60 gate excludes it at loaded turn 0. |
| Planet marking and desirability | One M2 planet is owned by player 1. The packed reader supplies visibility, minerals and flags; source desirability governs production admission. No visible unowned or viable enemy planet reaches colony demand. A focused test proves the enemy count is order sensitive. |
| Production policy | All five validity switches are enabled from current input. Integrated assertions prove tech level 2 <= 4, no viable colony target, inactive design 12, and no selected support or combat/transport production role. The reached turn-zero seed queue uses source selection, catalog, addition and finalization; blocked-queue ETA and final budget use current planet/resources. Unbound branches have explicit failure guards. |
| First fleet pass | Source attack classification uses current hull IDs; all three surviving AI fleets are visited in original order. Bit 15 is cleared in raw and lookup state. Newly eligible destination cleanup/drop effects reject. |
| Second fleet pass | Turn-zero miner task 5, local transport and armed scout move are bound. Current fleet roles/order counts guard dispatch. Enemy candidates come from the current compacted fleet table; newly eligible attack, repair, support, obsolete-scout scrap and fallback paths reject instead of succeeding silently. The reached speed, fuel and blocked-task services run from current state. |
| Optional hooks | `merge_needed`, `visit_planet`, `visit_fleets` and `apply_fleet_action` are adapter observers/optimizations alongside direct execution. Their absence does not skip an original service. |
| Outputs and RNG | Both seeded inputs match original AI H2 and X2 record bodies, fleet/order/queue effects, and all 57 control / 39 action draws through the integrated continuation. Expected originals assert results; they do not supply runtime branch values. |

The direct source fleet predicate, policy conditions and original call order are
checked in `reconstructed/src/do_turin_drone_ai_turn.c` and the integrated
`tests/satisfy_orders_turin_binding_e3_probe.c`. The strict-C89 test executes
100 scout cases plus both seeded complete inputs. The source ordering, AI
lifecycle, minelayer and packed-service probes cover nontrivial boundaries.

## Verification

- `python3 -m unittest tests.test_generate_turn_source tests.test_satisfy_orders_turin_binding -q` — 2 tests pass (12.868 s before the final selection/policy assertions).
- `python3 -m unittest tests.test_do_ai_turn_source tests.test_ai_final_queue_source tests.test_turin_minelayer tests.test_turin_packed_service -q` — 4 tests pass (2.888 s).
- Integrated test after the final selection/policy assertions — 1 test passes (12.639 s). Session identity reset/reload assertions then pass with the source ordering test (2 tests, 12.868 s).
- `git diff --check` — pass.

The generated output record and writer lifecycle audit remains G2-12; full
order/log integration remains G2-02. G2-14 must independently audit all
runtime provenance across the whole turn and can reopen G2-01 if it finds a
defect. Complete stream assembly and comparison are SO-E3 Gates 3 and 4.
