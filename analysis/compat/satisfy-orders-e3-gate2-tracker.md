# SO-E3 Gate 2 tracker

Status: **closed — 0 of 14 work packages remain** (2026-09-26).

Gate 2 closed when both pinned inputs executed the complete selector-1 turn
through current-source services with one correctly seeded main RNG per run,
in original call order, without runtime answers supplied by retained turn-1
outputs or unresolved witnesses. Expected original values may be used in
assertions only. Gate 1 is closed. Complete stream assembly/comparison remain
Gates 3 and 4.

These numbered packages track workload, not additional SO-E3 acceptance items.
The last package audits the whole gate; a passing slice does not bypass it.
Newly found defects belong to the relevant package and can reopen it.

| ID | Status | Required to close | Current evidence or gap |
|---|---|---|---|
| G2-01 | **Done** | Complete AI startup, designs, research, fleet decisions, and queue changes from current state. | The [AI closure audit](satisfy-orders-e3-ai-callback-audit.md) accounts for every reached AI callback, original policy/role exclusions, source design/queue/order decisions, generated X2/H2 and live-state destruction/reload. Both pinned seeded runs and 100 scout cases pass with exact existing body/RNG assertions. G2-14 final whole-turn provenance audit passed. |
| G2-02 | **Done** | Player logs, waypoint validation, all four order passes, transfers, battles, colonist/research effects update the shared world. | The [order/log closure audit](satisfy-orders-e3-orders-audit.md) covers generated X2 loading/replay, serial/design/fleet preparation, source waypoint checks, all four passes in fleet-table order, task-5 and passive handling, drop/transfer queues, research, thing interactions, battle/bombing exclusions and shared-world publication. Both pinned integrated runs and 100 scouts pass; nine focused source/framing regressions pass. G2-14 final provenance audit passed. |
| G2-03 | **Done** | Move every surviving fleet with source-derived orders, speed, fuel use/capacity, and arrival effects. | The [movement closure audit](satisfy-orders-e3-movement-audit.md) covers current survivors/orders, rounded travel, source fuel/capacity and zero scoop gain, branch exclusions, shared cargo/position/direction/flag state, and source waypoint completion. Both pinned original fleet/order comparisons pass. Full raw messages remain G2-12. |
| G2-04 | **Done** | Complete mining, population, production queues, resource budgets, research allocation, and builds for all applicable planets. | The [production closure audit](satisfy-orders-e3-production-audit.md) accounts for empty-queue initialization, first-pass source queue insertion/finalization, full-map mining/production, current resource/cost/research inputs, partial build, population/fractions, and state write-back before downstream events. Both pinned runs pass original planet/queue/research checks. Inapplicable build/research branches are explicitly guarded; nested events and raw messages remain G2-05/G2-12. |
| G2-05 | **Done** | Execute random-event services, including nested effects or source-proven skipped branches. | The dispatcher now calls source meteor, climate, mineral-discovery, and trader services with current planet/game state and the shared RNG. Both pinned runs miss all three random gates; the loaded turn proves the trader early return. Nested effects are source-proven absent and unexpected eligible effects fail explicitly. See closure evidence below. |
| G2-06 | **Done** | Complete thing movement/interactions, decay, minefield paths, and post-production wormholes against current fleets/world. | Current fleet/planet/thing inputs feed source movement, decay, sweeping, and waypoint validation. Sweep power now derives from current ship/starbase designs and original beam records. All four things are wormholes, proving minefield/packet effects inapplicable. Exact moved records and shared RNG match both runs; nonzero sweep/relationship adapter checks pass. See closure audit below. |
| G2-07 | **Done** | Bind `FuelFleets`, including current capacity, friendly starbase eligibility, and fuel generation. | Source fueling runs for all 8/7 survivors, with current designs/counts, source capacity calculation, loaded directional relations, and current starbases. Both seeded replays and separate fuel-generation/refill/write-back checks pass. |
| G2-08 | **Done** | Bind `HealShips` over surviving ships and starbases with loaded race/hull data. | Corrected source service runs over all 8/7 surviving control/action fleets and all 32 planets; loaded race/current design/hull data determine rates. Damage writes back to runtime state and fleet writers. Both seeded replays and separate nonzero-damage adapter checks pass. |
| G2-09 | **Done** | Bind remote terraforming; complete existing auto-terraform/breed/race-gated stage audit. | Source remote stage receives all survivors and 32 planets. Current ship components yield zero capability. Loaded races 9/1 prove original auto-terraform/breeding early returns; fixture race values and zero-length dummy tables were removed. Newly eligible unsupported effects fail explicitly. |
| G2-10 | **Done** | Calculate population/defense guesses from current runtime planets and players across the full map, using the shared RNG. | All 32 runtime planets feed `stars_update_guesses`. Turn-1 initializer structs were removed. Loaded race/technology, source defense lookup/limits/survival, and current population feed the calculation. Both seeded replays pass. |
| G2-11 | **Done** | Compute complete current scores, score history, and player summary fields from the resulting world. | Source calculation now uses current planets/resources, fleets/design power, loaded technology, and victory settings. Generated 24-byte records match both original player records in both runs; player ranks are written back. Writer no longer copies retained score bodies. |
| G2-12 | **Done** | Generate runtime message buffers and player/log-write state through source services, with all resulting mutations retained. | The [writer lifecycle audit](satisfy-orders-e3-writer-lifecycle-audit.md) accounts for the `0x0202` generation flag scope, guarded pre-view order preparation, source message and player mutations, current section writers, backup/log move and M1 append versus M2/HST create decisions, BOFs and three closes. The integrated control/action and 100 scout cases plus five focused tests pass. Complete encrypted streams remain Gate 3; G2-14 final provenance audit passed. |
| G2-13 | **Done** | Audit one main RNG per run through every bound stage; expected values only verify draws, never supply them. | The [RNG closure audit](satisfy-orders-e3-rng-audit.md) checks every one of 57 control / 39 action draws, both seed words across all callbacks and five BOF handoffs, exact call order, and final state. Effective startup ticks and writer clock residues are explicitly recovered external replay inputs, not historical clock captures. Other package changes must preserve this audit. |
| G2-14 | **Done** | End-to-end source/state provenance audit and regression for both complete inputs, with no unresolved runtime fixtures or skipped required services. | The [final provenance audit](satisfy-orders-e3-gate2-provenance-audit.md) inventories every reached coordinator stage and nested package audit, separates runtime input from expected output assertions, removes three output-to-runtime leaks and publishes task-5 technology. The 36-test Gate 2 regression passes (one expected Unicorn skip). Exact file assembly/whole-record comparison remain Gates 3–4. |

## G2-10 evidence

`tests/satisfy_orders_turin_binding_e3_probe.c` builds each `StarsGuessPlanet`
from the current `turn_planet_states` owner, population, defenses, and guess
word. All 32 entries pass through `stars_update_guesses` and return to the
runtime planet table. Zero-population planets are checked for zero guesses.
The two populated planets reproduce original guess words and both 90%
survival values using reconstructed part lookup, technology checks,
habitability, defense limits, and survival calculation.

The full runtime PLANET packing used by resource preparation is shared with
the defense adapter. Player race and technology fields come from the loaded
runtime tables, with the source AI research selection used for player 1.
Random draws come from `trace.seeds`. Retained output words/limits/values are
assertions, not initialization. The generated header no longer defines
`retained_guess_planets_control`, `retained_guess_planets_action`, or
`retained_guess_planet_records`.

Verification:

```sh
python3 -m unittest tests.test_satisfy_orders_input_corpus tests.test_satisfy_orders_turin_binding -q
```

**19 passed, 1 skipped** (20 tests), 15.060 seconds. Both seeded runs report
`GUESS_INPUTS=32/32`; all 100 existing scout-roll cases pass. The skip is the
original x86 player comparison disabled on this host. No new guest run was
needed. Upstream turn-state correctness remains covered by the other open
packages; G2-10 closes this calculation's binding, not the complete turn.

## G2-08 evidence

`turn_heal_ships` replaces the no-op coordinator callback. It enumerates the
complete host fleet identities and selects current surviving movement state;
retired owner-1 fleets must be absent/dead, while inactive owner-0 fleets are
excluded. Unsupported identities fail explicitly. Both runs check survivor
counts (control 8, action 7) and unchanged RNG seeds.

Every current host planet supplies its owner, starbase presence, packed
damage, and no-heal flag. Current design banks and original hull tables supply
repair-ship hull kinds and starbase cargo capability. Race statistic 14 comes
from the complete loaded player. The source service's original-rule correction
is detailed in `analysis/reconstruction/heal-ships.md`.

Owner-1 fleet loading now preserves all 32 ship-count/damage words. Healing
writes fleet damage into the mutable host state and owner-1 raw fleets; output
fleet writers consume those current words. Starbase damage returns to the
current planet table, preserving the other packed bits.

The retained replays are accompanied by a separate copied-state perturbation
check: damage 80 becomes 75 in both owners' fleet state (and owner-1 raw bytes),
and starbase damage 80 becomes 30. Original oracle replay states are untouched.
The focused service probe covers 22 eligibility/rate/saturation conditions.

Verification: input/selector suite **19 passed, 1 skipped** (20 tests,
14.992 seconds); final selector suite including perturbations **1 passed**
(11.697 seconds), covering both seeded replays and all 100 scout-roll cases.
The selector report includes `HEALING=PASS`. This closes the healing service
binding for the pinned pair. Upstream movement/battle state and the final
whole-pipeline audit remain their own open packages; no native run is claimed.

## G2-07 evidence

`turn_fuel_fleets` replaces the coordinator no-op and shares the current
surviving-fleet lookup with healing. Each fleet uses current movement fuel,
ship counts, and design slots. The capacity callback binds original hull
fuel/cargo fields and current components to production fleet/design stat
services. The refill callback uses current planets and the base owner's
loaded relation toward the fleet owner. Fuel writes back to movement state,
mutable host fleet state, and owner-1 raw bytes used by subsequent services.
Both runs assert survivor coverage and unchanged RNG seeds.

The source service now documents the legacy capacity callback name and only
queries capacity when the original branch requires it. Its eight-condition
strict-C89 probe passes. Separate copied-state integration checks exercise
permission direction, non-refueling orbital forts, transport/generator fuel,
capacity saturation, and write-back for both owners. Oracle replay states
remain untouched by these perturbations.

Input/selector verification: **19 passed, 1 skipped** (20 tests), 15.276
seconds, including both seeded replays and all 100 scout-roll cases. The
selector reports `FUELING=PASS`; fleet records and 57/39-draw RNG traces still
match. Details: `analysis/reconstruction/fuel-fleets.md`. Upstream movement,
new ship creation, and the whole-pipeline audit remain their own open packages.

## G2-09 evidence

For both pinned runs, loaded player race statistic 14 is 9/1. The recovered
`AutoTerraform` returns before its planet loop unless some player has race 3;
`BreedColonistsInTransit` returns before its fleet loop unless some player has
race 4. Both coordinator adapters now query complete loaded player state and
pass current tables (32 planets and all surviving fleets), replacing retained
race constants and empty dummy arrays. A newly eligible input fails explicitly:
this closure does not claim the unexecuted climate/cargo/message branches.

`turn_remote_terraform` replaces the no-op with `stars_remote_terraform` over
all 8/7 surviving fleets and all 32 current planets. The amount callback builds
source `StarsTerraFleetInput` from current ship counts and complete design
slots, then calls `stars_terra_from_fleet`. Every survivor has zero capability;
the original owned-orbit gate and zero-amount branch therefore prove that no
planet mutation, desirability calculation, message, or RNG draw is required.
All survivors are audited even if their orbit gate would skip them. Positive
capability or a call into an unsupported nested effect fails the replay.

The legacy remote-planet field `mineral_concentration` carries environment
axes: original `rgMinConc[axis+3]` aliases the planet environment. Header and
source comments now document that mapping. The adapter verifies all 32
planet environments remain unchanged and that RNG seeds do not advance.

A separate copied-state test sets three ships with two matching terraformer
components for each owner and verifies capacity 6; changing the subtype removes
the capacity. Loaded race changes to 3/4 verify explicit rejection of newly
eligible unbound branches. Original oracle replays are not modified.

Verification: input/selector suite **19 passed, 1 skipped** (20 tests), 15.049
seconds, including both seeded replays and 100 scout-roll cases. The selector
reports `TERRAFORM_GATES=PASS`. This is source-proven branch coverage for the
pinned pair, not general terraforming/breeding compatibility. Upstream design,
fleet, and world correctness remain covered by the open packages and G2-14.

## G2-11 design-power progress (still open)

`turn_prepare_score_designs` replaces the score-stage no-op with preparation
of both current ship-design banks (32 slots). It respects free/hidden bit 9,
resolves relevant weapon/component values from the pinned original part table,
derives combat speed through corrected `stars_speed_of_ship`, and calculates
power through `stars_compute_design_power`. Engine fuel rows are read from
original component offsets 54 onward; hull weight comes from current design
state. No expected output supplies power or speed.

Current surviving ships are classified into noncombat, small-combat, and
large-combat counts. Packed counts match original score-history fields at
16/18/20 for both players in both runs. Expected records are assertions only
for this calculation. The score writer still uses retained bodies; these
counts do not yet close complete score generation or output provenance.

Verification: input/selector suite **19 passed, 1 skipped** (20 tests),
15.286 seconds; final selector with category comparisons **1 passed**, 11.568
seconds, covering both seeded replays and 100 scout-roll cases. Corrected
strict-C89 ship-speed probe also passes. **10 packages remain.**

## G2-11 closure

The coordinator now calls `turn_update_scores`. Planet population, ownership,
starbase cargo capability, and resources are derived from current runtime
state. Resources use source habitability, population ceilings (with loaded
race attributes), factory limits, and `stars_cresources_at_planet`. Current
surviving ship counts and the source design-power calculation feed
`stars_calc_player_score`; technology comes from loaded mutable player tables.
The original heavy-ship bonus formula is supplied when applicable.

Source `stars_update_player_scores` receives decoded victory values/checks
from the loaded game record (including its global-game offset semantics),
current technology levels, and score totals. It computes rank/achievement
metadata. `turn_score_record` writes PLAYER.wScore at offset 10 and packs the
current summary into a fresh 24-byte record. `turn_data_file_scores` consumes
only `turn_score_records`, not retained original bodies.

All 24 bytes match each original M1/M2 score record for both control/action
runs (four comparisons), and each current player rank equals its emitted
score-history rank. These inputs retain inhabited planets and do not trigger
death/victory messages or dirty-state changes; unexpected callbacks fail the
replay instead of silently omitting effects. AR resource handling is likewise
explicitly rejected, since neither loaded race is AR. No RNG draws occur.

Separate copied-state tests increase planet-10 population and verify changed
resource/score totals and serialized score bytes; increasing player-1
technology updates both the technology sum and score. Original oracle replay
states remain untouched. Existing victory-rule and packed-score probes cover
the corrected category threshold logic separately.

Verification: input/selector suite **19 passed, 1 skipped** (20 tests), 15.082
seconds; final selector including PLAYER.wScore write-back **1 passed**,
11.744 seconds, covering both seeded runs and 100 scout-roll cases. The report
includes `SCORES=PASS`. This closes the score service binding for the pinned
pair; upstream production/research/world correctness and complete player-file
assembly remain their open packages and Gates 3/4. **9 packages remain.**

## G2-04 production resource progress (still open)

Production now shares the score resource adapter: source habitability and
population ceilings use current planet state and loaded player attributes,
including the race modifier bit. Both owned planets call
`stars_cresources_at_planet`; the owner-0 zero-budget fallback is removed.
Planet 10 contributes 35 resources through the original null-queue branch.
Planet 24 produces 32 before its loaded 15% research allocation, leaving 28
for its queue and crediting 4. Cheater eligibility comes from loaded flags.

The stage resets PLAYER.lResLastYear and accumulates current planet credits
into mutable host players at offset 58, giving 35/4. Original order transfers
supply planet 24's current pre-growth population. The old constant
habitability/population-limit resource callbacks are removed. Downstream
research advancement and other production services remain open.

Input/selector verification: **19 passed, 1 skipped** (20 tests), 15.296
seconds. Both seeded original comparisons and 100 scout-roll cases pass.
**9 packages remain; G2-04 has not closed.**

## G2-04 current build-cost progress (still open)

`turn_produce_costs` now calls source `stars_get_true_hull_cost` on the current
owner/design bank. The generated `retained_owner1_design0_costs` array and its
oracle-derived initializer are removed. The pinned queue still receives
mineral/resource costs [16,2,8,35] and reproduces its original progress, but
those amounts now come from the Gate 1 initialized runtime design.

Separate copied-state tests change all four cost words in both owners' banks
and verify the mineral/resource order returned by the adapter. The queue's
habitability callback now reads the current planet environment and complete
loaded player record rather than selected fixture arrays.

Verification: input/selector suite **19 passed, 1 skipped** (20 tests), 15.080
seconds, including both seeded original comparisons and 100 scout-roll cases.
**9 packages remain.** This replaces cost inputs only; finished-build effects,
queue finalization, research, and the other production gaps keep G2-04 open.

## G2-04 full-map mining input binding (still open)

Mining now packs all 32 current host planets into the production turn loop.
Unowned planets take the source estimator's no-owner return. The two owned
planets derive operating mines through `stars_cmines_operating`, population
limits, and loaded mining attributes; efficiency comes from race attribute 4,
the special-race gate from attribute 14, and the concentration floor from
planet flag bit 10. Concentrations, depletion levels, and stockpiles come from
current planet state. Pre-mining task-5 minerals and transferred population
are synchronized into that state before the pass.

The eight generated mining input arrays (IDs, owners, race, efficiency,
operating mines, concentrations, depletion, stockpiles) are removed. Expected
random values and output yields remain assertions only. Both replays visit
32 planets, operate on the two owned planets in original order, and match the
six original rounding draws and yields. AR remote-mining effects are explicitly
unsupported rather than silently skipped; neither pinned race is AR.

Verification: input/selector suite **19 passed, 1 skipped** (20 tests), 15.262
seconds, including both seeded comparisons and 100 scout-roll cases.
**9 packages remain; G2-04 is open.**

## G2-04 growth binding and queue-finalization audit (still open)

Population growth now reads full loaded race records and current planet
population, fractional remainder, and environment. Race-stat and race-bit
modifiers and ideal growth rates replace fixture constants. Both seeded runs
still reach populations 287/256 with fractional remainder 50/50. The existing
unseeded scout cases use the same complete input records plus current transport
state. Input/selector verification: **19 passed, 1 skipped**, 15.145 seconds.

The `FinishProduction` audit found a separate prerequisite defect: the source
adapter conflated player ID, selected planet ID, and AI mode. They are now
separate inputs. Lookup uses planet ID; UI refresh uses AI mode. Cancel updates
planet bit 23 from the global flag (including clearing it), and tutorial state
updates a separate tutor flag word. Four strict-C89 source cases pass, including
player 1 / planet 24, AI suppression, and clearing the planet bit. The actual
turn callback still requires binding; historical native probes predate the fix.
**9 packages remain; G2-04 stays open.**

## G2-04 queue-finalization binding (still open)

`turn_seed_finish_production` now calls corrected `stars_finish_production`.
Its write path releases old queue ownership, moves the pending queue to selected
state, clears the pending pointer, and performs the original lookup sequence
(-1 save, then planet 24 reload). The adapter installs that queue as the
planet's current queue; `stars_produce` consumes the installed pointer/count,
rather than bypassing finalization with a direct scratch-array assignment.
The fixed queue allocation belongs to the probe arena, so release clears its
ownership without freeing stack/embedded storage.

AI mode comes from the loaded player flags. UI/tutor callbacks fail if invoked;
the pinned owner-1 AI path does not invoke them. The callback asserts both
lookups, queue count, pending-pointer clearing, and installed queue identity.
Current resources only fund a partial hull, so the build callback explicitly
rejects nonzero completion until creation/message effects are bound.

Input/selector verification: **19 passed, 1 skipped** (20 tests), 15.193 seconds,
including both seeded replays and all 100 scout-roll cases. Queue progress and
original output record comparisons remain unchanged. **9 packages remain.**
The production `update_research` callback is still unbound, and moving the
late queue replay into the complete AI lifecycle remains G2-01/G2-04 work.

## G2-04 production research handoff

`startup_write_research` now writes the source AI decision into the shared
host player. `turn_produce_research` binds Produce's research callback before
random events. It clears and accumulates each player's current planet credits
before calling `stars_update_research_status`, replacing the previous late
credit copy after Produce returned.

The adapter reads all six research balances and technology levels, selection
and mode, player flags, race attributes, and game flags from current state.
Costs call `stars_get_tech_level_cost`; its 27-entry table is extracted from
the SHA-256-pinned original executable at the `rglTechCost` debug symbol.
Technology totals and race modifiers come from current players. The AI
selection is 0x32; the human selection remains 0x60. Credits are 35 and 4.
The source pool distribution derives 8 points in energy for player 1 and
retains its 0x159 callback payload (player 1, goto -2, field 0, amount 8).
Raw message-buffer integration remains G2-12; AI message filtering is not
claimed closed by retaining this callback.

Both players' entire byte range 26..61 (36 bytes each) is compared with each
matching original HST: six technology levels, six research balances, research
percentage/selection, and last-year resources. Original output bytes are used
only as assertions. No breakthrough occurs for these inputs; a newly reached
level-up/unlock branch fails explicitly pending its integration. This does
not certify arbitrary research scenarios or the source's slow-tech branches.

Verification: `python3 -m unittest tests.test_satisfy_orders_input_corpus
 tests.test_satisfy_orders_turin_binding -q`: **19 passed, 1 existing skip**,
16.119 seconds. Both seeded turns and all 100 scout-roll cases pass; main RNG
expectations remain unchanged. No guest/original execution was performed.

**Remaining: 9/14 Gate 2 packages; 3 grouped SO-E3 gates.** G2-04 remains
open for the full queue/lifecycle and production integration audit.

## G2-04 first-pass queue lifecycle and full-map production

Removed `turn_seed_turin_queue`, the second, planet-only Turin invocation at
production time. The probe now has one `stars_do_turin_drone_ai_turn` call
site. In that first pass, queue eligibility comes from the loaded M2 planet's
starbase flag and population >=200, matching the original Turin branch.
The turn and planet maximum come from loaded GAME offsets 18 and 10.
Planet desirability uses source calculation and loaded M2 environment/player
fields. No queue decision is inferred from the original turn-1 queue.

Queue requests invoke source `stars_add_item_to_queue` in append mode and
translate its packed records to Produce items. Source `stars_finish_production`
commits that queue. The AI log callback verifies finalization already happened
and production has not begun. Produce consumes the same committed pointer;
it no longer clears and reconstructs the queue. Gate 1 establishes that both
starting queue sections are empty, so preparation currently allocates empty
storage; a final InitProduction side-effect audit remains before G2-04 closes.

Produce now receives all 32 current planets. Ownership, mineral banks,
improvements, research percentage/exemption, and cheater flags are loaded
from runtime state. All 32 population callbacks are visited; unowned planets
are checked for zero population, and the two owned growth paths retain their
source calculations. Research accumulation scans the full planet array.

Verification: **19 passed, 1 existing skip** in 15.705 seconds via the focused
input-corpus/selector suite. Both seeded replays, all 100 scout-roll cases,
production queue output checks, research bytes, and shared RNG expectations
pass. `git diff --check` passes. No VM run.

**Remaining: 9/14 Gate 2 packages; 3 grouped SO-E3 gates.** G2-04 still needs
its final initialization/state-propagation audit; G2-01 retains wider AI gaps.

## G2-04 closure: initialization and state propagation

The [production audit](satisfy-orders-e3-production-audit.md) accounts for each
applicable stage and the original initialization work that cannot affect this
empty-queue, turn-zero AI path. Saved research flags now follow the original
initialization rule. Both scrap adapters use loaded race flags and the
per-planet extra-resource table consumed by Produce. Host-turn load initializes
that table as the original coordinator does.

The final integration change commits production minerals and the queue count
before population/research/events; no stale bank is copied after random events.
Mining now propagates concentrations as well as depletion levels and stocks.
Research asserts all 32 population visits and the state commit occurred.
The output queue writer uses the committed queue and its current count.

Final verification: `python3 -m unittest tests.test_satisfy_orders_input_corpus
 tests.test_satisfy_orders_turin_binding -q`: **19 passed, 1 existing skip**,
15.971 seconds. Both seeded runs and 100 scout-roll cases pass. The first
compile attempt exposed an adapter return-type mismatch; it was corrected
before this passing run. No original/VM experiment was performed.

**G2-04 closed. Remaining: 8/14 Gate 2 packages; 3 grouped SO-E3 gates.**

## G2-05 closure: source event gates

The four dispatcher callbacks now call `stars_meteor_strike`,
`stars_planetary_climate_change`, `stars_discover_new_minerals`, and
`stars_mystery_trader`. The first three receive 32 current planet states;
turn, player count, and map size come from loaded GAME fields. Random calls
advance the same `trace.seeds` used throughout the turn. Original values are
compared afterward, never supplied to the source services.

| Run | Main draw positions | Bounds | Generated values | Result |
|---|---|---|---|---|
| Control | 37–39 | 20,20,15 | 19,18,10 | All three source gates return without nested effects. |
| Action | 19–21 | 20,20,15 | 11,7,2 | All three source gates return without nested effects. |

The trader receives a non-null allocation callback and RNG callback; it
returns from its source turn<=39 guard without either call. Thus it is the
loaded turn, not a missing service pointer, that proves inactivity. Unexpected
nested draws, messages, queue removal, or trader allocation fail this pinned
adapter explicitly. Positive event effects are not claimed integrated for
arbitrary other seeds; they are not reached by either required run.

The source audit found and corrected two defects against original
`decompiled/all/turn2.c`:

- MeteorStrike must consume Random(20), and on success Random(cPlanet),
  before checking turn>9. The previous source returned before these draws.
- PlanetaryClimateChange permits every selected planet after turn 19.
  The previous source incorrectly returned for all turns after 19. Before
  turn 20, owned planets with population>=51 remain ineligible.

Strict-C89 meteor and climate probes pass with new cases exercising early-year
RNG consumption and the turn-19/20 climate boundary. Earlier native evidence
predates these fixes. The integrated input/selector regression passes
**19 tests, 1 existing skip**, 15.890 seconds, including both seeded runs and
100 scout-roll cases. `git diff --check` passes. No guest run was performed.

**G2-05 closed for the pinned pair. Remaining: 7/14 Gate 2 packages;
3 grouped SO-E3 gates.** Whole-turn RNG provenance remains G2-13.

## G2-06 current-world wormholes and decay

`retained_wormhole_validate` now enumerates all current survivors through
`turn_current_moving_fleet`: 8 control / 7 action fleets, with current positions.
Planet positions come from loaded coordinates. Removed generated
`retained_wormhole_fleets_control`, `retained_wormhole_fleets_action`, and
`retained_wormhole_planets` runtime arrays. The output-derived fleet positions
are no longer supplied to the source validator.

Both MoveThings and wormhole validation now receive GAME.mdSize at offset 4.
The previous fixture macro incorrectly used the planet count (32) as map size.
The tested short movements still match after correcting this latent boundary
error. Post-production movement starts from current `turn_thing_bytes`, and
writes the resulting records to both that buffer and shared host thing state.
The original 18-byte records remain assertions only.

ThingDecay now receives all 8/7 survivors instead of two owner-1 fleets.
Current positions/flags populate the source input. Cleared fleet marks write
back to host state and owner-1 packed fleets. All four current things are
wormholes; the existing guard rejects newly eligible unsupported kinds.

Verification: focused input-corpus/selector regression **19 passed, 1 existing
skip**, 16.012 seconds. Both seeded outputs, shared RNG expectations, and all
100 scout-roll cases pass. `git diff --check` passes. No guest run.

**Remaining: 7/14 Gate 2 packages; 3 grouped SO-E3 gates.** G2-06 remains
open: the waypoint-validation callback still only counts calls, and remaining
thing-interaction applicability must be audited. The source waypoint service
is `stars_validate_waypoints` in `reconstructed/src/common.c`.

## G2-02/G2-06 source waypoint validation

Replaced the two coordinator no-op validation callbacks and the post-wormhole
callback with `turn_validate_current_waypoints`, which invokes current source
`stars_validate_waypoints`. The adapter reads current packed owner-1 orders,
loaded owner-0 orders, current surviving fleet coordinates/flags, current
things, and the loaded map size. It preserves full packed order bytes while
writing the validated coordinate/target/type fields back. Fleet coordinate
clamps also update the first order as the original does.

The wormhole callback now publishes moved things before validation, rather
than waiting for MoveThings to return. The original ValidateWaypoints entry
repair and later fleet/thing retargeting branches are inapplicable: all current
orders target planets (type 1). Newly eligible non-planet targets fail
explicitly because message delivery and replacement tie-breaking are not yet
bound. Thus this is a complete visit of the applicable pinned path, not a
claim of generic waypoint retargeting support.

Coverage assertions:

| Checkpoint | Control fleets/orders | Action fleets/orders |
|---|---|---|
| After logs, before order execution | 9 / 10 | 9 / 10 |
| After wormhole movement | 8 / 9 | 7 / 8 |
| Final coordinator validation | 8 / 9 | 7 / 8 |

Focused regression: **19 passed, 1 existing skip**, 15.484 seconds. Both
seeded replays and all 100 scout-roll cases pass; original output/RNG checks
remain unchanged. No VM run.

**Remaining: 7/14 Gate 2 packages; 3 grouped SO-E3 gates.** G2-06 still has
constant mine-sweeper power callbacks and retained sweep-input fields. Those
are the next binding/audit work; waypoint counting is no longer a gap.

## G2-06 closure: thing services and mine-sweeper power

Replaced both `return 1L` sweep-power callbacks. Fleet power now calls
`stars_mine_sweep_from_fleet` with current ship counts and current design
banks, then `stars_mine_sweep_from_hull`. The component callback resolves
original EXE beam rows through source part lookup and applies original
range-squared × component count × damage, including beam ability precedence
and the starbase range bonus. Planet power requires a current starbase and
uses its loaded slot/current design. Attack eligibility reads the attacker's
loaded relation toward the owner. Retained sweep planet owners/coordinates
and owner-0 fleet flags were replaced with current runtime fields.

### Requirement audit for the pinned pair

| Service | Evidence / applicable effects |
|---|---|
| UnmarkMineFields | All four current records are passed to source. Their kind is 2 (wormhole), so mine marking is inapplicable; unchanged bytes are checked. |
| Pre-production MoveThings | Source visits all four current things. Wormholes do not move in this phase; fields and both RNG seeds remain unchanged. Packet/minefield paths are source-ineligible. |
| ThingDecay | Source receives all 8/7 survivors and four things, clears fleet mine-contact marks, and returns flags to host/packed fleet state. No thing is a packet or minefield, and no free callback occurs. |
| Post-production MoveThings | Source movement/chance/position validation uses current things, surviving fleet positions, loaded planet coordinates, and GAME.mdSize. All four generated 18-byte thing records match original results. Shared RNG supplies all 12 movement draws per run. |
| ValidateWaypoints | All three checkpoints call source with current fleets/orders/things. Current orders exclusively target planets; unsupported newly eligible retargeting fails explicitly. Moved things are published before the inner callback. Fleet/order census assertions cover every checkpoint. |
| SweepForMines | Source receives 32 current planets, 8/7 survivors, and four current things. Power is computed from current ships/starbases and original components. All things are wormholes, so no attack/message/free/strength effect occurs. |
| State propagation | Wormhole records update host and writer state, fleet decay flags update shared/packed state, and waypoint coordinate/type/target results update current orders. No output-derived fleet-position initializer remains. |

Independent copied-state checks verify that two pinned Lasers give 20 hull
power, three such ships give 60 fleet power, and a starbase gives 80 through
the range bonus. Removing the starbase gives zero; changing directional
relations changes attack eligibility. These catch binding errors that the
no-minefield original runs alone could not expose.

Final regression: **19 passed, 1 existing skip**, 15.395 seconds, via the
focused input-corpus/selector suite. Both seeded replays and 100 scout-roll
cases pass. `git diff --check` passes. No VM run. This closes the package for
the pinned pair, not arbitrary newly created minefields/packets or generic
waypoint retargeting. Whole-turn upstream state/RNG provenance remains open.

**G2-06 closed. Remaining: 6/14 Gate 2 packages (01,02,03,12,13,14);
3 grouped SO-E3 gates.**

## G2-03 source movement fuel and capacity

Removed the callback returning `TURIN_RETAINED_FUEL_SPENT` and removed that
generated macro. The callback now constructs `StarsEstFuelInput` from current
fleet counts/cargo, current design mass and components, loaded race bit 0,
and original engine fuel tables. It checks engine presence/count against the
original hull slot requirement. Source `stars_wt_max_ship_design_stat` supplies
cargo capacity; `stars_est_fuel_use` calculates consumption. The original
2-unit cost is an assertion, not a runtime input. A copied-state mass increase
must increase fuel use, guarding against reintroducing a constant answer.

Both owners' movement fuel capacities now call the existing current-design
source capacity adapter rather than setting capacity equal to current fuel.
Owner-0 movement also binds the fuel service even though all its surviving
fleets stay put in this pair. Owner-1 movement orders come from current packed
PLORD entries (next order when present, otherwise current order); retained
order field arrays no longer supply that callback. Distance now calls source
`stars_get_distance` and uses the original integer truncation.

Corrected comments on the legacy EstFuelUse API names: `cargo_count` means
ship count, `ship_stat` means cargo capacity, `hull_fuel_weight` means design
mass, and `cargo_weight` means available fuel for range estimation. No API
layout changed.

Verification: **19 passed, 1 existing skip**, 15.488 seconds, using the
focused input-corpus/selector suite. Both seeded runs, all 100 scout-roll
cases, original fuel/position expectations, and the mass perturbation pass.
`git diff --check` passes. No guest run.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates.** G2-03 remains
open for survivor selection, arrival effects, and movement write-back;
startup ideal-warp/design adapters also remain part of G2-01's audit.

## G2-03 owner-0 retirement propagation

The owner-0 task-5 adapter now dispatches from the loaded order kind rather
than `seeded_action`. Source task-5 fleet flags and completed-order bytes
write back to the shared host fleet. Movement reads that current retirement
flag instead of selecting the surviving fleet set from the oracle case label.

A copied-state regression flips the control/action label and verifies that
survival is unchanged; flipping the actual retirement flag changes survival.
Both pinned coordinator runs retain their expected results. Focused input and
Turin regression: **19 passed, 1 existing skip**, 15.498 seconds;
`git diff --check` passed. No guest execution was needed.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates.** G2-03 still
requires owner-1 survivor enumeration, arrival semantics, and complete
movement write-back; this change does not close the package.

## G2-03 owner-1 enumeration and shared movement state

Movement storage and traversal now cover all four owner-1 input slots.
Deleted slots are skipped by runtime presence, and source retirement flags
control activity. The shared fleet lookup no longer special-cases IDs 2/3
as the expected casualties. Both pinned runs still yield survivors 0/1.

After movement, all surviving host fleets receive current coordinates,
planet, fuel, and fHereAllTurn. Owner-1 packed fleets receive the same
fields. The original first-pass initialization and travel clearing of
fHereAllTurn are in stars-decompile/turn.c (MoveFleets, original addresses
10b0:333e..335d and 10b0:465d onward). Fleet serialization now starts from
current flags instead of retained input flags.

Arrival/order completion and movement-limit semantics still need audit.
In particular, the generic movement fallback reads fleet warp while the
original uses the next order's warp; this must be bound before closure.
**Remaining: 6/14 Gate 2 packages; G2-03 remains open.**

Verification: focused input/Turin regression **19 passed, 1 existing skip**,
15.551 seconds; `git diff --check` passed. No guest run.

## G2-03 next-order movement limits and stationary skips

The movement-limit callback now reads the selected next waypoint's warp and
returns warp squared (original MoveFleets 10b0:4240..426c). It rejects
unsupported pursuit/stargate/high-warp paths explicitly. The retained scout
uses warp 5 (limit 25); a copied-state check changes warp to 3 and verifies
limit 9, independent of the fleet's initial first-order warp.

Owner-1 order selection now implements the original first-order task skips,
requires a second waypoint with nonzero warp, and prevents repeated planet
arrival processing in later pursuit passes. Owner-0 fleets with fewer than
two orders likewise skip. Stationary fleets preserve their loaded planet
instead of acquiring it through synthetic zero-distance arrival callbacks.
Both original movement endpoints and fuel checks continue to pass.

Arrival closure is still outstanding: the coordinator does not yet call
source KillUsedWaypoints. Existing writers select the retained second-order
slot directly. This must become real order-state mutation, including the
original first-waypoint position update, and downstream consumers must read
the resulting order list before G2-03 can close.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates.**

Verification: **19 passed, 1 existing skip**, 15.664 seconds in the focused
input/Turin regression; `git diff --check` passed. No new guest run.

## G2-03 waypoint-service correction before binding

Inspection of original KillUsedWaypoints exposed incorrect typed indices
and unconditional deletion in the reconstructed service. Corrected PLORD
byte-offset interpretation, arrival gating, ORDER 1 to ORDER 0 copying,
retargeting/recycle/message indices, and ten-byte payload preservation are
now in the source. Four affected strict-C89 host probes pass: source,
integration (9), packed (6), and turn (6). Historical native validation
predates these fixes and does not prove the corrected behavior. See the
[correction audit](../reconstruction/kill-used-waypoints.md).

Coordinator binding, DeleteWpFar mutation, and downstream current-order
consumption remain outstanding. **6/14 Gate 2 packages remain; G2-03 is open.**

## G2-03 source waypoint completion bound to the coordinator

After shared movement position/fuel write-back, the coordinator enumerates all
8/7 surviving fleets into current typed order lists. Moving fleets receive the
original first-waypoint position/planet update (10b0:4e12..4e6d). Source
KillUsedWaypoints then copies the reached next order into waypoint zero and
calls source DeleteWpFar to remove waypoint one. Both header and fleet order
counts, complete 18-byte orders, and canonical host orders are updated.

The scout now actually has one order in runtime state. Fleet writers read that
current first order instead of selecting the former second slot. The later
waypoint checkpoints consequently contain 8/7 orders for 8/7 survivors, rather
than retaining the scout's completed waypoint. Both pinned fleet/order output
comparisons still match. The source completion callback records owner 1's
0x4e fleet-0 event; raw message assembly/filtering remains G2-12.

DeleteWpFar's two-order recycle path was also corrected to disable recycling,
as the original does, avoiding an uninitialized saved-order copy. Its strict
C89 source probe now checks that case as well as deletion and recycling.

The arrival adapter explicitly bounds these inputs to at most two current
planet-target orders per survivor. It fails unsupported paths instead of
inventing target/planet responses. G2-03 still needs the remaining movement
side-effect and state-provenance audit before closure.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates.** No VM run.

Verification: **19 passed, 1 existing skip**, 15.742 seconds in the focused
input/Turin suite, plus DELETE_WP_FAR_SOURCE=PASS; `git diff --check` passed.

## G2-03 closure: movement effects and shared state

The [closure audit](satisfy-orders-e3-movement-audit.md) accounts for every
applicable movement effect in the pinned pair. Final corrections bind supply
cargo to the fleet seen by movement; round travel upward as original code does;
write source direction/fDone fields; calculate zero scoop gain from current
engine data; and use current cargo, design-plan, and order counts in writers.
Cheater/colonist/cheap-engine/unsupported fuel paths fail if unexpectedly live.

Focused input/Turin regression: **19 passed, 1 existing skip**, 15.863 seconds.
DeleteWpFar source probe and `git diff --check` pass. No VM run.

**G2-03 closed. Remaining: 5/14 packages (01,02,12,13,14);
3 grouped SO-E3 gates.** Gate 2 remains open.

## G2-01 current design decisions and early initialization

Removed generated hull-capability, ideal-warp design, and engine arrays from
the coordinator fixture header. FleetMightHaveTeeth now resolves the current
SHDEF hull through the original part table. FindIdealWarp resolves current
engine components and original engine fuel curves. Latest-starbase selection
calls source IshdefAiSbLatest over the current starbase bank instead of
returning the expected answer.

The first regression exposed a real initialization dependency: the runtime
SHDEF bank was populated after AI and fixture arrays had hidden that gap.
It is now populated before AI starts and subsequent consumers preserve it.
Copied-state checks remove an engine, substitute an unarmed hull, activate a
newer starbase, and revisit the loader; resulting capability/warp/latest
choices change and the modified bank survives subsequent access.

Focused input/Turin regression: **19 passed, 1 existing skip**, 15.373 seconds;
`git diff --check` passed. No VM run. G2-01 remains open: startup capability
gates, full SHDEF creation/commit, names/history, and other startup decisions
still need binding/audit. **Remaining: 5/14 packages; 3 grouped SO-E3 gates.**

## G2-01 startup starbase slot gates

Original EnsureAiStarbaseDesigns (1090:7222) checks SHDEF free bits at
starbase-bank offsets 0x1a1, 0x10e, and 0x234: flags in slots 2, 1, and 3.
The coordinator now reads these current fields instead of the generated
`{1,1,1}` capability fixture. It also supplies loaded GAME.turn and each
current design's flags, turn, and 131/133 words to the source service.
The original turn <=49 return excludes rotation for this turn-zero pair.

A copied-state source test verifies the original four requests (slots 1–4),
then clears the relevant free bits and verifies no creation request. Both
pinned coordinator replays preserve their four startup name/RNG draws.

Verification: **19 passed, 1 existing skip**, 15.846 seconds in the focused
input/Turin regression; `git diff --check` passed. No native/guest run.

G2-01 remains open. In particular, startup_create_shdef still constructs a
minimal work record, and startup_change_shdef records naming evidence without
committing a complete source-built SHDEF. Those callbacks must bind original
starbase part alternatives, current technology, quantities, costs, names,
and source publication/log mutations before closure.
**Remaining: 5/14 packages; 3 grouped SO-E3 gates.**


## G2-01 original-template starbase construction (2026-09-25)

The startup adapter now calls source `stars_f_create_ai_shdef`,
`stars_f_get_ai_part`, `stars_lookup_part`, and `stars_tech_status` with
current player technology/race and original executable hull/component data.
The source constructor now accepts choice zero: it is a valid AI component
family, not an absent template entry. The dedicated constructor probe covers
this case.

Original binary operands locate the starbase templates at segment 19:0x7688
and the packed choices at segment 19:0. FGetAIPart's `8a 87 50 14` instruction
reads the count table at DS:0x1450; the debug symbol's 0x1440 address is not
used. The first 39 families contain 125 choices, checked during header
extraction. These are original executable inputs, not turn-1 answer fields.

The source starbase constructor scales original hull slot quantities using
the loaded player mode. Bitmap occupancy checks the active current starbase
bank and designs already constructed this startup. In creation order
18,20,17,19, the chosen bitmaps are 137,138,128,129. The previous synthetic
100–103 bitmap range is removed.

The regression independently extracts the four original output component
lists from each run's M2 and compares all group/item/quantity bytes with
constructed snapshots. Expected output components are assertion-only data.
Both runs match all four lists. These snapshots are not yet full published
SHDEF records: fake ID names, full bank publication, repricing, and history
remain outstanding.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passes (1 integrated test, 12.292 seconds, including both seeded runs and
existing scout cases). The strict-C89 constructor probe passes
`CREATE_AI_SHDEF_CHECKS=2 FAILURES=0 RESULT=PASS`.
No original executable or VM run was performed.

**Remaining: 5/14 packages; G2-01 remains open; 3 grouped SO-E3 gates.**


## G2-01 original starbase names (2026-09-25)

`startup_name_get` now calls reconstructed `stars_get_compressed_string`
using the original executable's segment 3 compressed-string tables. The
`IDxxxx` placeholder and parsing that placeholder back into a string ID are
removed. The callback records the requested ID separately for RNG assertions.
The generated name buffer is bounded and zero-filled.

The name-collision callback checks active current starbase records of the
same hull, as the original PickAiNameAndBmp loop does, plus designs already
constructed in this startup while full publication remains pending. It no
longer unconditionally reports that every name is unused. Both retained
runs accept their first choices, preserving the four original name draws.

The regression decodes names saved in each original M2 design record and
compares all 32 bytes against the four generated names. Original output names
are assertion-only data. Component/quantity/bitmap and RNG checks also pass.
`python3 -m unittest tests.test_satisfy_orders_turin_binding -q` passes in
12.047 seconds. No VM run was performed.

Full bank publication, cost calculation, player count changes, and design
history remain G2-01 work; the source change-design operation and its original
count/write/log sequence have been inspected for the next binding step.
**Remaining: 5/14 packages; G2-01 remains open; 3 grouped SO-E3 gates.**


## G2-01 publication integration — unfinished regression (2026-09-25)

The startup callback now invokes `stars_f_change_ai_shdef`, recalculates
cost/physical fields through `stars_initialize_shdef_cost`, increments the
current player's starbase count, publishes the full 147-byte records into the
current bank, and preserves four change snapshots. Published occupied slots
suppress duplicate creation in the current-state adapter check. Count is five
(one original plus four created). Snapshots currently substitute only for
retaining changes, not for the required LogChangeShDef serialization.

A stronger original output comparison exposed a layout bug: HUL cargo
capacity is at 52 and damage at 56 (types.h HUL offsets 0x34/0x38).
`reprice_shdef.c` now reads original hull damage and writes design damage at
56; `stars_read_shdef` puts the full wire record's damage there as well.
The independent input oracle was corrected to the recovered layout.
Input corpus tests: 19 tests, 18 passed / 1 existing skip, 3.498 seconds.
The earlier input inventory/runtime hashes need regeneration after this fix.

**Integrated regression currently fails.** Created damage values are
1300/2100/250/700 versus final original 900/1300/175/400, in creation order.
Component quantities and names still match. The difference equals the
regenerating-shields armor halving. Current created flags have low byte zero;
original saved full records have seven. Original `LogChangeShDef`
(planet.c:4229) sets that low byte to seven before WriteRtShDef/WriteMemRt.
The current adapter does not yet implement that flag/log mutation or the
subsequent serialized-design reload/reprice. This is the next concrete
integration target; do not treat the current full-record comparison as passing.
The test reports six failed assertions, including dependent consistency checks.

**Remaining: 5/14 packages; G2-01 remains open; 3 grouped SO-E3 gates.**
No VM run. The goal remains active.


## G2-01 design-log reload closes damage mismatch (2026-09-25)

This supersedes the failing publication regression above. The adapter now
calls `stars_log_change_shdef` and `stars_write_rt_shdef` with the generated
147-byte design. The LogChangeShDef adapter applies the original low-byte
flag mutation to seven before writing. Four type-27 bodies are retained,
including the owner/design/action header and source-compressed names.
At the existing owner-1 player-log load stage, their design payloads run
through `stars_read_shdef`, original hull/part lookup, name decompression,
and source cost/physical-field initialization. They update the same bank
used by downstream services. No original output body supplies runtime state.

The reload's full-design flags enable the original regenerating-shields
armor adjustment. Final damage now matches 900/1300/175/400 in creation
order for both pinned runs. Names, all component group/item/quantity bytes,
bitmaps, turn/built/existing fields, and player starbase count also pass.
Pre-reload change snapshots remain distinct from normalized runtime designs.

Binding exposed and corrected WriteRtShDef's missing count byte. The original
io.c:2719 writes count at 6, turn at 7, built at 9, existing at 13, then
components at 17; the reconstructed writer had shifted the full prefix by
one byte. The short form's name begins at 6. Its strict-C89 probe now checks
the corrected full prefix and passes `WRITE_RT_SHDEF_SOURCE=PASS`.
The earlier HUL damage-offset correction remains in place. The complete
input inventory has been regenerated with zero uninitialized object records.

Verification: combined input-corpus and integrated selector regressions run
20 tests: 19 passed, 1 existing skip, 15.949 seconds. No original/VM run.
The generated log records are not yet assembled into full X2 files or replayed
through a complete file-log dispatcher. The adapter handles this pinned
create-only design path; replacement deletion still fails explicitly.
Startup history validation and remaining AI callbacks still need audit/binding.
**Remaining: 5/14 packages; G2-01 remains open; 3 grouped SO-E3 gates.**


## G2-01 history validation and upgrade inputs (2026-09-25)

The count-only history callback now implements the original pre-turn-20
path from aiu.c ValidateStarbaseHistory: personalities 4/5 return; otherwise
AI-data version below 3 clears the history count at byte 2. DoAiTurn source
initializes version 2; both pinned inputs have turn 0, so the detailed
planet/transport history walk is skipped. Later eligible turns fail explicitly
until that walk is bound. Copied-state checks cover version-2 count clearing,
version-3 count preservation, and personality-4 early return. This closes
this pinned history-validation subpath, not history-file writing.

The starbase upgrade callback no longer supplies fixed family/mineral values.
It reads the loaded M2 planet's owner, starbase flag/index and minerals, current
player mode/turn, the newly published next-design active flag, and current
production queue. It implements the original normal-personality family-0
eligibility guards before the source upgrade roll. Existing queue items only
suppress that roll if object kind is fleet and design exceeds 15, matching the
original starbase-build check. Both original rolls (7 and 75) reject the <6
upgrade branch, so no queue mutation is eligible. Mineral values are saturated
at 65535 for the helper's 16-bit threshold test, preserving the original
32-bit comparison against 200. Other eligible families/recycle paths and a
successful upgrade fail explicitly; this is bounded retained-pair support.

Verification: integrated selector regression passes in 12.756 seconds,
including both seeded runs, original RNG/BOF checks, and history adapter
checks. No VM run. Remaining concrete G2-01 audit targets include the four
DoAiTurn preparation callbacks still assigned to `simple_stage`:
ComputeShdefPowers, MarkPlanetsUnderAttack, IncreaseMinefieldSizes, and
InitRandomPlanetList. Their state effects must be bound or proved absent;
one-owned-planet zero shuffle draws alone is not that proof. History writing
and complete generated-log dispatch also remain open.

**Remaining: 5/14 packages; G2-01 remains open; 3 grouped SO-E3 gates.**


## G2-01 DoAiTurn preparation callbacks (2026-09-25)

All four `simple_stage` assignments have been removed:

- InitRandomPlanetList packs loaded M2 planets, runs the source collection /
  shuffle with the shared RNG and current stable-order flag, and retains the
  resulting IDs/count. That count feeds Turin. The result is planet 24 only,
  with zero random calls. Extra owned planets fail explicitly because the
  coordinator currently supports one Turin planet slot.
- IncreaseAIMinefieldSizes copies the current M2 thing table through the
  source service and writes the results back to that player view. It does
  not substitute the broader host thing table for the AI's loaded view.
- MarkPlanetsUnderAttack runs the source service over all four loaded raw
  fleets. All are player 1, so the source rejects them before enemy hull or
  planet lookup. Unexpected eligible enemy callbacks fail explicitly; enemy
  threat effects are not claimed implemented for arbitrary inputs.
- ComputeShdefPowers checks loaded bank presence and active flags, derives
  weapon components and speed through the existing source power adapter,
  then writes all three current ship-design power fields at offset 135.
  Absent banks are skipped, as the original null-pointer guard requires.
  Zero-filled storage for an absent bank must not be treated as 16 active
  definitions. Starbases are not part of this original ship-bank loop.

The integrated regression asserts the owned list, zero shuffle draws, one
mine preparation call, four checked fleets, and three power calculations.
It passes both seeded runs and existing scout cases in 12.289 seconds.
Original RNG/BOF, movement, production, and score checks still pass. No VM run.
History writing, complete generated-log dispatch, and the remaining Turin
callback/state audit remain necessary before G2-01 closes.
**Remaining: 5/14 packages; G2-01 remains open; 3 grouped SO-E3 gates.**


## G2-01 history bodies and original design-log comparison (2026-09-25)

The history callback now runs `stars_write_hist_file`. Its inputs come from
the current loaded M2 planet/player/design view, surviving AI fleet count,
reset message filters, initialized AI data, and loaded game/player/turn.
Source `stars_write_planet` writes history-form planet data. Header, planet,
filter-mask, and EOF records are retained in runtime output buffers.
The original ResetMessages (msg.c:98D6) clears 49 filter bytes; there is no
starting H2 filter record to replace them. Initial score-history tables are
empty. Other-player detection gates suppress player/design records for the
pinned view. Unexpected eligible player/design/score callbacks fail explicitly.
Version 2 AI data is omitted, matching original FWriteHistFile's >2 gate.

The original H2 files contain BOF plus exactly four bodies: type 32 (4 bytes),
type 14 (16 bytes), type 33 (49 bytes), EOF (0 bytes). All four generated
bodies, their types, lengths, and order compare byte-for-byte for both runs.
The runtime header reports one planet and three surviving AI fleets.
This is complete history-body coverage for the pair, not encrypted file
assembly or a claim that every possible history section is implemented.

The actual generated X2 returns are retained under each run's BACKUP folder.
All four source-written type-27 design-change records now compare byte-for-byte
with those original records, including flags, pre-reload damage, names, and
component arrays. This directly confirms the previously inferred log/reload
boundary: final M2 damage differs from the earlier design-log payload.

BOF calls for X2/H2 now execute inside write-log and history-create callbacks,
respectively. The artificial two calls after DoAiTurn were removed; the main
RNG sequence and existing original BOF checks remain passing.
Verification: integrated regression passes in 12.243 seconds. No VM run.
Complete non-design log generation/dispatch, remaining Turin callbacks, and
the G2-01 provenance audit still remain.
**Remaining: 5/14 packages; G2-01 remains open; 3 grouped SO-E3 gates.**


## G2-01 shared design/research log dispatch (2026-09-25)

The four generated design changes and the generated research change now enter
one ordered runtime memory-log collection. Research writes type 34 with the
current percent/technology bytes at the source IroEnsure callback; both bytes
match each original BACKUP/CLEAN.X2 record. No original payload is used to
construct runtime records.

The player-log stage now calls source `stars_run_log_record` for each captured
record. Typed design callbacks invoke the source design reader; typed research
callbacks apply the decoded research state to the current host player. The
adapter rejects any record type whose effect has not been bound, avoiding the
source dispatcher's permissive missing-callback default. Five records are
captured and five dispatched for each run. A copied-state regression poisons
both research bytes before replay and confirms the generated log restores
them. The integrated regression passes in 12.214 seconds.

The original X2 has ten mutation records after its BOF and type-9 header:
four type-27 designs, type-34 research, type-23 fleet balance, type-29
production, type-4 order insertion, type-1 cargo transfer, type-5 order
replacement, followed by EOF. Thus **5/10 mutation records are generated and
dispatched; 5 remain**. This count describes this log subtask, not Gate 2
packages. Header/environment state, whole-file writing/loading, and full
mutation replay still need integration. In particular, future fleet/cargo
replay must avoid applying effects twice to state already changed by AI.

**Remaining: 5/14 Gate 2 packages; G2-01 remains open; 3 grouped SO-E3 gates.**
No VM run.


## G2-01 production memory-log generation and replay (2026-09-25)

Finishing the first AI production queue now invokes source
`stars_log_change_planet` with the proven empty starting queue and the current
packed source-generated queue. Other snapshot fields come from the loaded
planet and are unchanged across this queue-only operation. The emitted type-29
record enters the same ordered runtime log as designs/research. Its ten bytes
match each original BACKUP/CLEAN.X2 production record.

Source `stars_run_log_record` decodes the queue replacement at player-log
load. Its typed callback replaces the runtime production entries, resetting
quantity/type/object-kind/progress from the generated packed words. The
selected shared queue pointer/count feeds the later production stage.
A copied-state test changes both quantities to 99, replays the generated log,
and verifies quantities return to one, progress returns to zero, and queue
ownership points to the copy's current arena. Existing production later still
reaches the original 81 percent progress.

Binding found a real buffer overflow in `log_change_planet.c`: its six-byte
stack record was also used for variable-length queue records. Two entries
require ten bytes. The buffer now accommodates 255 four-byte entries plus
the two-byte planet ID, with a count bound. The focused source probe now
includes two entries and checks the final payload byte. It passes strict C89
with AddressSanitizer: `LOG_CHANGE_PLANET_SOURCE=PASS`.
Integrated regression passes in 12.128 seconds. No VM run.

**X2 mutation records: 6/10 generated/dispatched; 4 remain** (fleet balance,
order insertion, cargo transfer, order replacement). Complete header/file-log
loading and remaining AI coverage remain open.
**Gate 2: 5/14 packages remain; G2-01 open; 3 grouped SO-E3 gates.**


## G2-01 fleet-balance log generation and replay (2026-09-25)

The AI merge retains its two pre-mutation input fleet records. Source
`stars_log_make_valid_xferf` receives the actual ship-count delta and encoded
owner/fleet IDs and appends type 23 at the original point between research
and production. Its nine bytes match both original X2 records.

Source log dispatch now handles that record through a bounded complete-merge
adapter. It validates every decoded delta against the source fleet's complete
pre-AI ship list, then runs source `stars_merge_two_fleets` and cargo balancing
on an isolated copy restored from those input records. It publishes the
resulting ship/damage/cargo fields and presence state into the current bank,
retaining independently changed orders. This avoids adding ships twice to
already-mutated AI state. Partial transfers fail explicitly rather than being
treated as complete merges. A copied-state regression poisons both design-2
ship counts to 99; replay restores the input-derived sum, zeroes the source,
and retires it.

Merge capacity now reads current runtime designs and original hull/component
records rather than the retained SHDEF fixture or a slot-2-only shortcut.
The hull fuel capacity offset is corrected from bitmap offset 50 to 54.
The integrated original comparisons still pass in 12.279 seconds.

The source balance decoder's mask at byte 5 is a 16-bit word. It previously
read four bytes despite accepting a seven-byte record, potentially reading
past an empty-mask payload. This is corrected to a word read. A seven-byte
case added to the source probe passes strict C89 with AddressSanitizer:
`RUN_LOG_CHECKS=22 FAILURES=0 RESULT=PASS`.

**X2 mutations: 7/10 generated/dispatched; 3 remain** (order insertion,
cargo transfer, order replacement). File header/environment handling,
whole-file log dispatch, and remaining AI provenance checks are still open.
**Gate 2: 5/14 packages remain; G2-01 open; 3 grouped SO-E3 gates.** No VM run.


## G2-01 order-log generation and replay (2026-09-25)

The Turin fleet-visit adapter now compares loaded M2 orders with the current
AI-mutated order arrays through `stars_log_change_fleet`. Fleet 0 produces
the type-4 insertion and fleet 2 the type-5 replacement. Both 12-byte bodies
match their respective original `BACKUP/CLEAN.X2` records for both pinned
runs; original bodies are assertion inputs only.

The shared `stars_run_log_record` dispatcher now binds the order callback.
Before replay it restores the loaded M2 order arrays, preventing duplicate
insertion into the already AI-mutated arrays. The callback applies insertion
or replacement, expands compact payloads into zeroed 18-byte orders, and
updates both order-list and fleet counts. A copied-state check poisons both
order arrays before replay and verifies the resulting logged bytes, counts,
and zero-filled tails. Unsupported operations and bounds fail explicitly.

The source order writer had an extra length increment after trimming trailing
zeroes. Removing it matches original `LogChangeFleet` and fixes a potential
23-byte read from its 22-byte buffer. The full-length insertion source probe
now requires exactly 22 bytes and passes strict C89 with AddressSanitizer:
`LOG_CHANGE_FLEET_SOURCE=PASS`.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passes 1/1 in 12.585 seconds, including both seeded original comparisons and
the copied-state checks.

**X2 mutations: 9/10 generated/dispatched; 1 remains (cargo transfer).**
File header/environment handling, whole-file log dispatch, and remaining AI
provenance checks are still open. **Gate 2: 5/14 packages remain; G2-01 open;
3 grouped SO-E3 gates.** No VM run.


## G2-01 cargo-log generation and replay (2026-09-25)

The AI supply-transfer adapter snapshots the current fleet cargo before
`stars_xfer_ai_supply` and passes the resulting delta and both object IDs to
source `stars_log_make_valid_xfer`. Its memory-log writer generates the
seven-byte type-1 body, matching both original backup X2 records. This occurs
between the type-4 insertion and type-5 replacement, preserving the complete
original ten-mutation sequence. A fresh temporary log is valid at this
boundary because the preceding record is an order insertion, not a transfer
that could coalesce. Header/file-environment records remain separate work.

`startup_replay_memory_logs` now binds the typed cargo decoder. The bounded
adapter validates the same-owner fleet/planet IDs and colonist-only positive
delta. It restores the loaded M2 cargo into a copied runtime, then applies
source `stars_change_cargo` in original dispatcher order: fleet gain followed
by planet debit. Capacity, clamp, or unsupported branches fail explicitly.
It publishes the result to supply objects, raw fleet cargo, the canonical host
fleet, and the host planet. The copy keeps replay checks from altering the
AI-stage call counters. This is not a general cargo-transfer implementation.

A poison check sets cargo to 99 before replay and verifies fleet 25 and planet
225 across the shared representations; the normal turn still reaches the
original post-growth planet value. All expected original log bytes remain
assertion-only inputs. The strict-C89 integrated regression passes 1/1 in
12.395 seconds, covering both seeded runs and existing scout cases.

**X2 mutations: 10/10 generated/dispatched; 0 mutation records remain.**
This does not close G2-01: the source whole-file log writer/loader and
header/environment handling remain, followed by the remaining AI callback
and state-provenance audit. **Gate 2: 5/14 packages remain (01,02,12,13,14);
3 grouped SO-E3 gates.** No VM run.


## G2-01 log-file header/source correction (2026-09-25)

Inspection of original `FWriteLogFile`/`FLoadLogFile` and the recovered
`RTLOGHDR` layout exposed two source defects. The 17-byte type-9 payload is
`cbLog` (word at 0), serial (long at 2), and 11 configuration bytes at 6.
The reconstructed writer previously omitted `cbLog` and shifted the other
fields two bytes early. The reconstructed loader treated 17 (the payload
length) as the record type and read `cbLog` from the BOF magic. Existing source
probes used the same incorrect layout and therefore missed both defects.

Both source functions and their source probes now use the recovered layout.
The loader consumes the header after the BOF and preserves the original
17-byte serial/config callback gate. Missing/wrong-kind/short headers fail.
The writer now writes all 17 bytes at their recovered offsets.

New `tests/test_satisfy_orders_log_files.py` independently decodes each pinned
original backup X2, checks the ten-record mutation census and `cbLog`, and
compiles a strict-C89 AddressSanitizer probe. The probe loads the original
records through `stars_load_log_file`, compares the complete packed memory
log, and uses `stars_write_log_file` to reproduce every subsequent record
(type-9 header, all ten mutations, EOF). It also rejects record kind 17 in
place of the type-9 header. The original serial/config bytes are explicit
codec-test inputs only, not coordinator runtime inputs or evidence of their
provenance. BOF generation is outside this codec test.

Verification: both standalone source probes pass with AddressSanitizer;
`python3 -m unittest tests.test_satisfy_orders_log_files
 tests.test_satisfy_orders_turin_binding -q` passes 2/2 in 16.133 seconds.

**Remaining: coordinator integration of the corrected file writer/loader,
runtime serial/config provenance, and remaining AI services/audit.**
**X2 mutation records: 0 remain; Gate 2: 5/14 packages remain; 3 grouped SO-E3
gates.** No VM run. This corrects a reconstructed format defect; it does not
establish the cause of the previously archived original-game rejection.


## G2-01 source memory-log runner integration (2026-09-25)

The coordinator's handwritten record loop has been replaced with
`stars_run_log_file` (original `FRunLogFile`, 1048:A2D6). The adapter bounds
and packs the ten generated records into the memory-log format, restores the
input order lists, and calls the source runner. Its callback binds the typed
`stars_run_log_record` services and explicitly rejects unbound record types.
The source runner owns iteration and aggregate success, suppresses logging
while dispatching, restores `fLogOff`, and clears the pending-log flag. The
adapter retains the returned state. A copied-state check uses nonzero flags
to verify restoration and preservation of unrelated bits; sentinels are not
used in the normal turn. A final audit must still connect this adapter's
log-state fields to the complete file writer/loader lifecycle.

Verification: the final integrated regression passes 1/1 in 12.234 seconds,
including both pinned seeded turns, all ten original mutation-body checks,
state-poison replay checks, and logging-state checks. The original-file codec
and intermediate integration regression also passed 2/2 in 15.980 seconds.

Header-input provenance remains unresolved. Original InitInstance reads the
28-character GlobalSettings INI value into the serial/machine configuration;
`GetDiskSerialNumber` derives the current 11 environment bytes from C:/D:
volume labels, DOS date/time fields, and disk geometry. The pinned RUN.JSON
manifests do not retain these inputs. Older STARS.INI files exist under
`runs/step4/guest` and `runs/step4/guest98`, but their presence does not prove
they were used by either pinned run. No original output fields were added as
runtime metadata. Full file writing/loading, metadata provenance, and the
remaining AI callback audit are still required.

**X2 mutation records: 0 remain; Gate 2: 5/14 packages remain (01,02,12,13,14);
3 grouped SO-E3 gates.** No VM run.


## G2-01 current transport-capacity inputs (2026-09-25)

The remaining AI callback audit found transport capacity still bound to
prebuilt `retained_supply_*` component/hull/ship-count tables. Those tables
came from retained input decoding, but did not follow mutations to the
coordinator's current fleet or design banks. They have been removed from the
C runtime and generated header.

Transport now uses current raw fleet-1 ship counts and flags, current owner-1
design slots/components, and original hull records through the shared
`current_fleet_capacity` adapter. That adapter calls source
`stars_get_fleet_stat` and `stars_wt_max_ship_design_stat`; the existing fuel
path now calls the same adapter with the fuel statistic. Cargo replay also
uses this current-state path when checking available capacity.

Copied-state checks double the transport count and verify capacity 50, remove
it and verify 0, then change the live design's mechanical component to a cargo
pod and verify capacity 75. The normal pinned capacity remains 25 and both
original X2 cargo records/downstream turns still match. The strict-C89
integrated regression passes 1/1 in 12.307 seconds.

**Gate 2: 5/14 packages remain; G2-01 remains open; 3 grouped SO-E3 gates.**
Whole-file integration, metadata provenance, and remaining callback audits
are unchanged. No VM run.


## G2-02 shared planet state and callback census (2026-09-25)

`DoOrders` was mutating a separate `StarsOrdersPlanet` array initialized only
at game load. Those flag and turn writes were asserted on that array but
never published into the canonical planet table. Each order phase now reads
current shared planet identity/owner/flags/turn. Before each SatisfyOrders
pass, the source flag/turn writes are published to the shared table; order
handler mutations are copied back before the next pass, and final state is
published on return. This preserves source bit-13 ownership marking and the
post-movement turn reset for downstream consumers.

The integrated regression passes 1/1 in 12.278 seconds, including both pinned
turns and the existing complete planet record comparisons. This is a state
propagation fix, not completion of the missing callbacks.

The audited `turn_do_orders` binds only battle-ID assignment and
`turn_satisfy_orders`. Five action callbacks are still null: `do_battles`,
`do_thing_interactions`, `drop_colonists`, `update_research`, and
`transfer_to_others`. `turn_satisfy_orders` currently runs the two task-5
paths on pass 1 and records passes 2–4 without full fleet task enumeration.
Each required service needs a source binding and propagation, or a
source/input-backed proof of inapplicability. These are explicit remaining
G2-02 requirements; successful current replay does not prove them complete.
The thing interaction implementation is available in `reconstructed/src/thing.c`.

**G2-02 remains open; Gate 2: 5/14 packages remain; 3 grouped SO-E3 gates.**
No VM run.


## G2-02 thing interactions bound for the pinned pair (2026-09-25)

`turn_do_orders` now binds `do_thing_interactions` to source
`stars_do_thing_interactions`. Both phases enumerate the current host thing
array, current surviving fleet views, current planet states, and loaded
player technology. The integrated build explicitly compiles `thing.c`.

Original DoThingInteractions returns before movement and, after movement,
only enters interaction effects for `idFull >> 13 == 3` (mystery trader).
All four current objects are kind 2 wormholes, so both source/input guards
prove there are no interactions to publish for this pair. The adapter fails
explicitly for any different thing kind; unsupported trader callbacks are
not silently treated as success. Fields used only inside that excluded
branch remain outside this bounded binding.

The coordinator asserts one call per phase and four current things per
call. A copied-state test changes a current wormhole to trader kind 3 and
verifies the adapter reports failure. The strict-C89 integrated regression
passes 1/1 in 12.617 seconds, including both original turn comparisons.

**Unbound DoOrders action callbacks: 4 remain** (battles, colonist drops,
research updates, transfers to others), down from 5. Complete fleet-order
pass handling remains separately open. **Gate 2: 5/14 packages remain;
G2-02 open; 3 grouped SO-E3 gates.** No VM run.


## G2-02 order-phase research binding (2026-09-25)

Both DoOrders `update_research` calls now execute source
`stars_update_research_status` with `use_pool=0`. The production research
adapter was split into a shared current-player service and a production-only
resource handoff. All three service invocations read/write the same canonical
player technology levels, selection/mode, and six research balances; race
and cost callbacks retain their existing source bindings. The order stages
do not add the production resource pool a second time.

The coordinator verifies research calls occur after order passes 1 and 3,
with production called zero and one times respectively, and verifies exactly
two order-research calls. The two pinned runs reach no research breakthrough;
newly eligible level/unlock messages still fail explicitly pending binding.
The production pool message and original player research comparisons still
pass. Original values remain assertions, outside the common service.

Verification: strict-C89 integrated regression 1/1 in 12.616 seconds.
**Unbound DoOrders callbacks: 3 remain** (battles, colonist drops, transfers to
others), down from 4. Full fleet task/pass handling remains open.
**Gate 2: 5/14 packages remain; G2-02 open; 3 grouped SO-E3 gates.** No VM run.


## G2-02 battle prerequisite: source fleet linking (2026-09-25)

The battle audit initially missed an older index-only `LinkFleets` helper in
`common.c`; the follow-up below corrects that search result. Original
1038:1BB4 clears prior links and bit 11, skips retired fleets (bit 10), and
builds circular same-coordinate chains. Singleton fleets link to themselves;
therefore `DoBattles.has_next` cannot be approximated as "more than one fleet
at this position." Later insertions go immediately after the first fleet at
the coordinate. The routine sets game-flags2 bit 10.

Added `reconstructed/src/link_fleets.c` and its typed interface. It preserves
those mutations and ring order; a scan for prior roots replaces the original
63-entry search-cache optimization. A strict-C89 AddressSanitizer source
probe covers 70 coordinate groups (crossing that cache boundary), repeated
positions, a retired fleet, equal x/different y, preservation of unrelated
flags, and regrouping after movement. `python3 -m unittest
 tests.test_link_fleets_source -q` passes 1/1 in 0.385 seconds.

This is a prerequisite, not a bound battle callback. Remaining battle work:
connect current runtime fleets to these rings, bind `CplrBattle` with current
plans/relations/design capabilities, bind bombing or prove its input guards,
and publish flags/results. **Unbound order callbacks: 3 remain. Gate 2:
5/14 packages remain; G2-02 open; 3 grouped SO-E3 gates.** No VM run.


## G2-02 current fleet rings and integration correction (2026-09-25)

The integrated build exposed an existing `stars_link_fleets` index API in
`common.c`/`common.h`, missed by the earlier search. The new record/flag API
is now named `stars_link_fleet_records`; its standalone test had not caught
the conflicting declaration because it did not include `common.h`. The
integrated build is restored. The older API has not been replaced or claimed
as validated by this work.

The post-movement order boundary now runs the new source linker on current
host fleet positions and flags, retaining stable next indices and intermediate
flags for battle eligibility. It records one linking call and the full current
fleet-table count. Publishing only the linking flags initially caused three
replay failures: linking clears bit 11 and CplrBattle later sets it. Since the
latter is not bound yet, intermediate linking flags are now retained in the
battle workspace and are not presented as final shared-world battle results.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding
 tests.test_link_fleets_source -q` passes 2/2 in 12.708 seconds. This includes
the current-turn grouping integration and the AddressSanitizer ring probe.

Eligibility still needs current installed-weapon checks and battle-plan
attacker/target fields (`BTLPLAN` word at byte 2, low nibble and bits 8–12),
planet starbase state, and relations. Original singleton self-links cause
eligibility evaluation too. Bombing and final flag propagation also remain.
**Unbound order callbacks: 3; Gate 2: 5/14 packages remain; 3 grouped SO-E3
gates.** No battle closure is claimed. No VM run.


## G2-02 battle callback bound for the pinned pair (2026-09-25)

The DoOrders battle callback now executes `stars_do_battles`, including source
fleet grouping, allocations/frees, `stars_cplr_battle`, bombing, and the final
battle-current reset. Group inputs use current position rings, plan attacker
and target fields, directional player relations, installed component groups
and quantities through `stars_hull_has_teeth`, ship-design class byte 7 for
fleet weapon eligibility, and current planet/starbase slot state. The
coordinator builds those views from initialized/current banks, not original
turn-1 result fields.

Source `CplrBattle` incorrectly cleared bit 8 for a no-opponent group. The
original returns before spectator cleanup on that branch, retaining bits 8
and 11 set by its initial visit. Corrected the source early return and the
source probe expectation. The probe passes strict C89 and AddressSanitizer
(`CPLR_BATTLE_SOURCE=PASS`). This correction removed all 16 fleet-record
mismatches first exposed by the integrated binding.

A remaining retirement mismatch exposed an older host-view flag word:
publishing it could revive the retired AI task-5 fleet. Linking now takes
owner-1 presence/flags from the current raw fleet table. Final battle flags
publish to canonical host fleets and raw AI fleets only after eligibility.
Existing fleet retirement and original record comparisons pass.

`stars_do_bombing` receives current fleet planet/owner/flags and current
planet owner/starbase flags. Its source guards exclude all effects for the
pinned pair. Newly eligible damage calculation fails explicitly; a copied
state places a live foreign fleet over a planet without a starbase and
verifies that failure. Newly eligible combat resolution likewise fails
explicitly rather than silently accepting an unbound resolver. This closes
the callback for the pinned inputs, not arbitrary combat/bombing support.

Verification: integrated selector plus ring tests pass 2/2 in 12.751 seconds.
The integrated test retains both original seeded turns and final fleet/order
record comparisons. Assertion failures now include their C source line.

**Unbound DoOrders callbacks: 2 remain (colonist drops, transfers to others),
down from 3. Gate 2: 5/14 packages remain; G2-02 open; 3 grouped SO-E3 gates.**
Full fleet-order enumeration remains required. No VM run.


## G2-02 four-pass current-order enumeration (2026-09-25)

Before binding the empty drop/transfer consumers, the producer audit found
that the non-task-5 fleets were not visited by SatisfyOrders at all. Added
`turn_visit_remaining_orders` to every pass. It reads the current host fleet
view and the current owner-1 raw fleet/presence/order tables, applies retired
and already-done guards, and checks the current first waypoint. Original
SatisfyOrders reads that first waypoint, not every future route waypoint.

After the existing task-5 handlers, each pass contains only task-0/no-task
active waypoints for this pair. They now pass through source
`stars_satisfy_orders`; pass-1 turn-flag changes and fleet flags publish to
both host and raw owner-1 state. Retired rows also pass through the source
initial turn-flag clearing and exclusion. Any different active task or
missing first waypoint fails explicitly instead of being ignored by an
unbound callback.

The coordinator asserts all fleet-table rows visited in each of four passes,
and 8 control / 7 action active no-task fleets per pass. A copied-state test
changes a current no-task waypoint to task 1 and verifies rejection. This
establishes the absence of further order producers for this pinned case; the
task-5/log producer-to-queue audit and source drop/transfer consumers remain.
The existing task-5 callbacks are still separately dispatched, so complete
order-path provenance must be audited before G2-02 closure.

Verification: strict-C89 integrated regression passes 1/1 in 12.500 seconds,
including both original output comparisons and the new eligibility guard.
**Unbound order callbacks: 2 remain; Gate 2: 5/14 packages remain; 3 grouped
SO-E3 gates.** No VM run.


## G2-02 drop/transfer queue consumers (2026-09-25)

The coordinator initializes both queue counts to zero before player-log replay,
matching original `FGenerateTurn` (`turn.c`, initialization of `cColDrop` and
`cXferFull`). The two DropColonists calls and one TransferToOthers call now
invoke their source consumers with trace-owned queues and required callbacks.
Unexpected nonempty queues fail explicitly; these adapters do not implement
new hostile-drop or cross-owner-transfer effects.

Producer audit: the sole cargo log transfers colonists between owner-1 fleet
513 and owner-1 planet 24. The original RunLogRecord queue branches require
different owners (`planet.c` around lines 5208–5310); this transfer applies
directly. Current task-5 handling credits same-owner colonists directly.
Every remaining active first waypoint is task 0, verified on every pass.
Thus neither queue receives an entry for these pinned inputs. Copied-state
checks insert one entry into each queue and require explicit failure.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passes **1/1 in 12.418 seconds**, covering both seeded runs and the existing
100 scout cases. Exact invocation counts are asserted (drops 2, transfers 1).
**Unbound DoOrders callbacks: 0, down from 2. Gate 2: 5/14 packages remain.**
G2-02 remains open for the final order/state provenance audit and log integration.


## G2-02 immediate task-5 planet state (2026-09-25)

The final state audit found that the owner-1 task-5 adapter still loaded the
old M2 planet snapshot, then copied its mineral result into the shared world
only when mining began. This hid the scrap result from intervening order
services and used pre-transfer colonists in the scrap planet.

During coordinator execution, `scrap_load_inputs` now reads current
`turn_planet_states[24]`, including DoOrders flags and the log-applied
225 colonists. All four mineral/colonist lanes and flags publish immediately
after the source task-5 call. The delayed overwrite in `run_retained_mining`
is removed, so mining consumes the state already produced by prior services.
The standalone scout/scrap slice retains its own initial-input boundary.

A copied-state check changes current minerals to 701, colonists to 219, and
flags to 0x2f07 and verifies those exact values reach the scrap adapter,
independently of the original M2 values. The integrated test passes **1/1 in
12.573 seconds** with both seeded runs and all 100 scout cases. Original
mineral/fleet/planet comparisons remain passing. This fixes a real shared-state
gap; it does not certify remaining order execution/provenance requirements.

**Gate 2: 5/14 packages remain; G2-02 remains open.** No VM run.


## G2-12 runtime messages and source writer (2026-09-25)

The coordinator now owns a turn-message byte buffer and `StarsMessageState`,
reset before log replay as in original FGenerateTurn. Task-5 raw message
callbacks, waypoint completion, production, and research-pool callbacks call
`stars_send_player_message`. Parameter counts come from the pinned original
EXE's 387-byte `rgcMsgArgs` table (logical segment 7, offset 0x5b0e);
suppression reads current player mode at byte 6. The scrap remove-message
callback invokes the source removal service on this same buffer.

Integration exposed a source bug: `stars_package_player_message` overwrote
message-ID bit 8 while publishing the argument-width mask. Original PackageUpMsg
preserves the low nine ID bits. The fix preserves that bit, and a targeted
0x141 case checks the high ID bit together with a two-byte argument and signed
goto value. The strict-C89/AddressSanitizer message probe passes six checks.

`stars_write_player_messages` is now bound to FWriteDataFile's `write_messages`
callback. The previous count-only GenerateTurn `write_player_file` hook is
removed from this probe: original FGenerateTurn has no separate corresponding
player-file loop; message output belongs inside FWriteDataFile. This does not
remove or certify the generic coordinator hook for other callers.

The comparison retains the starting M1 type-12 records (21 bytes), because no
X1 is supplied/generated and original FGenerateTurn uses append mode. Newly
generated message bodies are added to that retained prefix only in the output
comparison buffer, not injected into the runtime message queue. M2 has a
generated X2 and is replaced. Both full original message sections match:
control M1 26 bytes, action M1 42 bytes, and no M2 message record in either run.
The added messages are production 0x3f, plus action scrap 0x5a and 0x141;
AI messages are suppressed by the source mode predicate. Expected output bytes
are used only for comparison.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passes **1/1 in 12.488 seconds**, covering both seeded runs and 100 scout
cases. `messages_source_probe.c` compiled with strict C89, warnings as errors,
and AddressSanitizer reports **6 checks, 0 failures**. Diff whitespace checks
pass.

Remaining G2-12 work includes source player-record generation, the complete
visibility/pre-write state mutation audit (including message-related marking),
and integration of actual append/log lifecycle state. The message-prefix
comparison is not complete file append execution. Gates 3–4 still require
complete stream assembly and whole-record comparison.

**Gate 2: 5/14 packages remain (01,02,12,13,14). G2-12 stays open.** No VM run.


## G2-12 pre-write message state (2026-09-25)

Bound FWriteDataFile's `mark_messages` and `mark_planets_lost` callbacks to
`stars_mark_players_that_sent_messages` and `stars_mark_planets_player_lost`.
Sender marking reads current player mode/detail words and publishes the
results to shared players. The pinned inputs contain no outgoing type-40
player messages, and the audited generated log mutations contain none; the
runtime outgoing queue starts empty. A newly nonempty queue fails explicitly
until its complete lifecycle is integrated.

Planet-loss marking consumes the actual generated turn-message buffer.
Comparing source with original `msg.c` (MarkPlanetsPlayerLost, 1030:93c6)
found a bug: source treated every ordinary message's goto as a lost planet.
Original only marks IDs 7, 0x23, 0x40, and 0x8f; 0x8f obtains the planet from
its second variable-width argument. Corrected the source selection. The
existing 7/0x8f probe now also tests both 0x23/0x40 and exclusion of ordinary
production/scrap messages. Pinned runs have no loss messages. The integrated
adapter rejects any newly eligible loss lookup until per-view loss marking
is supported, rather than silently discarding the event.

All three output calls execute both pre-write services. Copied-state checks
inject an outgoing message and a loss message and require explicit failure.
The original message-byte comparisons remain unchanged and pass.

Verification: integrated selector test **1/1 in 12.578 seconds** (both seeded
runs and 100 scout cases); strict-C89, warnings-as-errors, AddressSanitizer
planet-loss probe **5 checks, 0 failures**.

**Gate 2: 5/14 packages remain; G2-12 is open.** Player-record generation,
complete current-state visibility preparation, and append/log lifecycle
integration remain. These bounded branch checks do not establish support for
new loss or player-message cases. No VM run.


## G2-12 current fleet/design visibility inputs (2026-09-25)

The player-1 visibility initializer previously supplied a single scout with
flags and ship counts from retained input fixtures, plus zeroed player/design
tables. It now enumerates the complete current host fleet table, including
retired entries for source exclusion. Owner-1 flags/counts come from the
mutated raw fleet records and presence table; owner-0 fields and current
planet locations come from shared fleet state. It loads current player mode,
death state, fleet-count word, both ship/starbase banks' flags and existing
counts, and the loaded player's race stat.

The source initializer's resulting player/design counts and flags are retained
in a dedicated player-1 view, alongside the existing planet-26 visibility
result. A copied-state test changes the scout's current ship count to three
and verifies three ships in the corresponding source-generated design count,
two surviving owner-1 fleets, and enumeration of every current fleet row.
Both pinned planet/fleet/message regressions remain passing.

Verification: integrated selector test **1/1 in 12.642 seconds**, covering
both seeded runs and 100 scout cases. This removes runtime fixture inputs
from this visibility slice, but does not close visibility preparation:
scanner/mining callbacks, thing visibility, the other viewer modes, subsequent
visibility passes, and complete per-view publication are still unbound or
incomplete. These are explicit G2-12 requirements; the stored player-1 view
must not be mistaken for completed player-record generation.

**Gate 2: 5/14 packages remain; G2-12 stays open.** No VM run.


## G2-12 visibility scanner and mining gates (2026-09-25)

Player-1 visibility now supplies a real scanner callback. It reads current
fleet ship counts and installed components, resolves scanner range/abilities
from original executable part records (SCANNER offsets 52/54), and invokes
source design/fleet scanner calculations. The scanner-doubling race bit comes
from the current player. A copied-state check removes installed scanners from
the current scout design and verifies its result changes from a scanner range
to -1. The pinned player-1 race is 1; newly eligible race-9 built-in scanners
are explicitly rejected pending a hull/technology adapter. This is not a
closure claim for all viewers or scanner-cache lifecycle.

Original SetVisPFInit calls CMineFromLpfl only when fleet flag 0x2000, first
waypoint task 3, and an unowned planet make mining discovery eligible. Source
previously called the capability callback unconditionally. Restored the
short-circuit condition and populated `mine_special` from the current fleet
flag plus task, not a fixture. No current surviving player-1 fleet qualifies;
a newly eligible capability call fails explicitly until supported. The source
probe covers both no callback on an ineligible fleet and a positive eligible
mining detail-4 path.

Verification: integrated selector **1/1 in 12.423 seconds**, both seeded runs
and 100 scout cases; strict-C89/warnings-as-errors/AddressSanitizer visibility
initialization probe **PASS**. Existing original output comparisons pass.

**Gate 2: 5/14 packages remain; G2-12 remains open.** Other viewers, built-in
scanner/cache handling, thing visibility and subsequent visibility passes,
complete per-view publication, player-record generation, and append/log
lifecycle remain required. No VM run.


## G2-12 host visibility and player records (2026-09-25)

Visibility initialization now also executes for the host viewer (-1), including
FWriteDataFile's restoration calls. It consumes the same complete current
fleet/planet/design inputs, retains the resulting host player view, and applies
SetVisiblePlanFleet's host-map count assignment to player 0. Host mode avoids
the client scanner/visibility-refinement branches.

Source/original comparison and newly bound player serialization exposed three
source defects in SetVisPFInit, now corrected:

- Reset both packed fleet/starbase counts and planet counts before recounting.
- Increment the packed starbase-design count for every occupied slot, instead
  of OR-ing a constant bit into an old count.
- Preserve player bitmap bits 3..7 when setting visibility detail. Object
  detail fields use a different mask and must not be reused for players.

FWriteDataFile's player predicate/writer now emits both HST player records via
`stars_write_player_record`. Counts/detail come from the source host view;
research, technology, race, names, relations, and other fields come from the
current shared players. The writer's full 192-byte resulting player state is
published back. Both generated clear record bodies (130 and 124 bytes) match
their matching original HST records for both control and action, including
compressed names. The four complete host-player comparisons use expected
original bytes only as assertions.

The probe captures the source writer's clear storage buffer and verifies its
record header/type/length. It does not claim complete stream encryption or
assembly: those remain Gate 3. M1/M2 player predicates/writers are still
unbound, pending their complete current visibility views.

Verification: final integrated test **1/1 in 12.402 seconds**, including both
seeded runs and 100 scout cases. Strict-C89/warnings-as-errors/AddressSanitizer
visibility probe **PASS**, now checking stale-count reset, all ten occupied
starbase slots, and preserved player bitmap bits. Scoped diff checks pass.

**Gate 2: 5/14 packages remain; G2-12 remains open.** Client visibility and
player records, current thing visibility, remaining state publication, and
append/log lifecycle remain. No VM run.


## G2-12 M2 player record and visibility finalization (2026-09-25)

FWriteDataFile's source player writer now also emits the visible M2 player,
selected by current visibility/dead flags. Both 124-byte M2 player bodies
match their corresponding control/action originals in full. The four HST
player-body comparisons continue to pass. Source writer mutations remain in
shared player state.

Bound source SetVisPFFinish to the current player-1 view, with design knowledge
masks from the current ship/starbase banks. Original `io.c` showed that this
service also resets/recounts the visible starbase-design nibble while
preserving the low fleet count; the reconstructed interface lacked that field.
Added `fleet_count` to `StarsVisibleFinishPlayer` and implemented the original
reset/increment behavior. The source probe verifies a stale high nibble is
replaced and the viewer's own count is untouched.

The first M2 player comparison found planet count 1 instead of original 2.
The cause was the visibility mark callback only setting flags. It now calls
source `stars_mark_planet` with the actual current visibility flags and player
counts, then publishes resulting planet detail/count and player visibility.
The starbase-mark callback preserves existing detail, counts a newly visible
starbase once, and raises its detail to the original minimum of 3. Planet 26
now contributes its newly visible count; already visible planet 24 is not
counted twice. Counts are derived from execution, not an expected constant.

Verification: integrated selector test **1/1 in 12.392 seconds**, both seeded
runs and 100 scout cases. The visibility-finish probe passes under strict C89,
warnings-as-errors, and AddressSanitizer (with macOS deprecation warnings for
legacy C89 `sprintf` suppressed). Scoped diff checks pass.

Remaining: M1 player-record generation; full fleet/planet/thing visibility
passes before finish; scanner/cache handling for other viewers; full per-view
publication and append/log lifecycle. Calling finish on the current bounded
view does not prove those omitted passes irrelevant. Complete encrypted file
assembly remains Gate 3.

**Gate 2: 5/14 packages remain; G2-12 stays open.** No VM run.


## G2-12 M1 binding exposes missing visibility passes (2026-09-25)

M1 now invokes current-state visibility initialization and the source player
record writer. Its built-in scanner branch reads race 9, hull IDs 4/5/6,
electronics technology, and game flag bit 3; the ordinary installed-component
path remains shared with M2. Corrected the design scanner helper to recognize
positive built-in power even without an installed scanner. A focused test
checks built-in 60/30 range and the no-scanner result. The adapter explicitly
rejects zero or above-10 technology in the technology-derived branch because
the current signed-32-bit power interface cannot establish that wider domain.
This limitation must be addressed before claiming general scanner support.

The M1 writer compares its complete current turn-1 player body, using original
bytes only as expected results. That new check fails on byte 2: **generated
planet count 1, original count 3**. Record size is correct (130 bytes), and the
reported comparison identifies no other differing byte. Current initialization
only marks planets occupied by own fleets. The source fleet-centered and
planet-centered visibility passes have not yet been integrated; those must
derive the additional visible planets before this comparison can pass. Do not
replace the count with 3 or copy the saved player record.

Latest verification is **2 passed, 1 failed in 15.535 seconds** for the combined
integrated-selector and two focused design-scanner tests. The integrated
regression is currently **failing at the new M1 assertion**; the earlier passing
HST/M2 checkpoints do not establish a passing current full regression. The
strict assertion is retained as the next required gate. Scoped diff checks
passed before this run.

**Gate 2: 5/14 packages remain. G2-12 is open, with M1 visibility regression
pending.** Next: bind source SetVisPFFleets/SetVisPFPlanets against current
coordinates, scanners, and visibility state, then rerun both pinned cases.
No VM run.


## G2-12 M1 visibility mismatch resolved (2026-09-25)

Bound source SetVisPFFleets after initialization for both client viewers. The
adapter enumerates current fleet flags, positions, ownership and planet IDs,
all 32 input XY coordinates, and the four current wormhole records. Ordinary
and penetrating scanner ranges come from the existing current-component and
built-in-scanner source adapter. Planet discoveries call source MarkPlanet
and update the shared view counts/detail flags. The source pass derives M1's
two additional visible planets; the player count is no longer supplied by an
assertion or retained output.

The first integration attempt used the wrong XY tuple columns; inspection of
InputReader confirmed `(x, y, name)` layout, and the adapter now reads columns
0/1. The final regression passes: **all eight turn-1 player bodies across the
two runs (M1, M2, and both HST players) match their originals**. Existing
planet/fleet/message comparisons also remain passing.

Per-view wormhole results are retained for later visibility integration.
No enemy fleet enters the pinned scanner circles; an eligible cloak query or
enemy-fleet marking call fails explicitly until those services are bound.
Enemy-planet marking and tachyon-detector inputs also reject unsupported
paths. The wormhole-only input validates that no thing-owner discovery changes
player detail here. These explicit bounds do not establish general scanner,
cloak, or visibility support. Planet-centered/thing-centered passes and full
visibility publication still remain required, even though player bytes match.

Verification: integrated selector **1/1 in 12.561 seconds**, covering both
seeded runs and 100 scout cases; strict-C89/warnings-as-errors/AddressSanitizer
fleet-visibility source probe **PASS**. Removed unused source parameters/local
state warnings encountered when first compiling this service in the strict
coordinator build. No behavior was changed by that warning cleanup.

**Current integrated regression is passing again. Gate 2: 5/14 packages
remain; G2-12 stays open.** No VM run.


## G2-12 planet/thing visibility and client planet publication (2026-09-25)

The coordinator now runs source `SetVisPFFleets`, `SetVisPFPlanets`, and
`SetVisPFThings` in the original `SetVisiblePlanFleet` order before finish.
Planet scanning derives each owned planet's range from its current population,
improvement bits, race and traits, technology, and starbase hull. Ordinary
scanner selection calls source `LookupBestPlanetaryScanner`, `FLookupPart`,
and `TechStatus` against the pinned executable part records, including the
negative-dp/NAS exclusion. Source `GetPlanetScannerRange` resolves regular
and penetrating ranges. No expected output provides scanner inputs.

The current races exclude the original race-7 stargate scan and race-5/6
thing-centered branches. These exclusions are checked explicitly before the
source calls; changed inputs requiring those unbound services fail. Existing
cloak/enemy marking guards remain. This is evidence for the pinned pair,
not general visibility support. Thing visibility produced by the preceding
passes is passed directly into the thing-centered service.

All 32 resulting planet states are retained separately for each client.
Both client planet writers now enumerate these computed states using the
original known/detail predicates and temporary detail-2 conversion. They no
longer select M1 planets from retained input IDs or hard-code M2 planets
24/26. Current queues remain attached to the full-detail planet-24 write.
Every resulting client planet record still matches the matching original.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passes **1/1 in 12.660 seconds**, including both seeded runs and 100 scout
cases. The planet-visibility source probe passes strict C89, warnings as
errors, and AddressSanitizer. Its source change only marks two unused
parameters explicitly unused for that strict build.

**Gate 2: 5/14 packages remain; G2-12 remains open.** Remaining work includes
fleet and thing writer selection/publication, the complete visibility state
lifecycle, and append/log integration. Current thing results are retained
per view but are not yet consumed by the thing writer. No VM run.


## G2-12 fleet/thing writer visibility integration (2026-09-25)

`FWriteDataFile` now receives the complete current fleet-table length for
each output view. Its fleet predicate tests the current view's source-derived
known bit, matching original io.c's table traversal. Retired fleets are
excluded by source visibility initialization. Fleet writer identity, owner,
packed ID, and flags now come from that current table/view. Expected output
IDs only locate comparison assertions; they no longer choose runtime fleets.
The existing current movement/cargo/orders feed the source fleet writer.
All fleet and order bodies still match both originals.

Visibility initialization now includes all four current wormholes through
`SetVisPFInit`, replacing the separate manual client-bit clear. Planet and
fleet scans consume that initialized view. Each client thing writer applies
the original wormhole visibility-bit predicate instead of returning early
for every client. Both clients select zero things from their calculated
visibility. The host selects and writes all four current records, with exact
original body comparisons. Visibility flags and discovered-player masks are
published to the shared thing table and the turn's raw thing buffer before
subsequent views; host initialization restores the host visibility bit.
Non-wormhole inputs remain explicitly rejected in this pinned adapter.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passes **1/1 in 12.613 seconds** (both seeded turns and 100 scout cases).
`git diff --check` passes for the edited probe. No VM run.

**Gate 2: 5/14 packages remain; G2-12 remains open.** Remaining work includes
the complete visibility/design state lifecycle audit and append/log handling.
Whole output streams remain Gate 3; this work establishes current-state
selection and writer inputs, not complete file generation.


## G2-12 design state publication and complete design bodies (2026-09-25)

Visibility now publishes both player summaries and both design banks after
each client finish and host restoration. Owner-1's current AI design banks
are copied into the shared world, including newly created starbases. Source
`SetVisPFInit` resets/recounts SHDEF `cExist` (offset 131) from surviving
fleets and starbases; the adapter now publishes these calculated counts,
as well as design flags, to both shared and current AI banks. The old
adapter left the retired design's existing count at two despite calculating
zero. The new full-record comparison exposed that lost publication.

`FWriteDataFile` now binds design predicates and source `WriteRtShDef` with
source name compression. Every design input comes from current shared state.
The expected-output header supplies assertions only. Both ship and starbase
banks are enumerated for every player and filtered by player/design known
bits and empty-slot flags. This corrects a source coordinator defect: it
previously enumerated only the client's own player for these two banks,
where original io.c enumerates all players. Battle-plan enumeration keeps
its original own-player/host range. The source-isolation client test now
requires the visible other player's two designs to be written.

**All 60 turn-1 design bodies match**: M1 7, M2 8, HST 15 in each of the two
pinned runs. This covers record lengths, flags, hulls, components, names,
built/existing counts, and all other serialized fields. Player, planet,
fleet/order, message, score, and thing checks also continue to pass.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding
 tests.test_write_data_file_source -q` passes **2/2 in 12.769 seconds**,
including both seeded turns and 100 scout cases. No VM run.

**Gate 2: 5/14 packages remain; G2-12 remains open.** The pre-write client
view preparation, remaining state lifecycle audit, and append/log handling
still require binding/audit. Complete encrypted files remain Gate 3.


## G2-12 pre-write fleet preparation audit (2026-09-25)

The save coordinator now binds `prepare_view` and enumerates every current
owned, nonretired fleet before messages/records. Original io.c:2858–3079
conditionally retires empty fleets, converts first-order fleet targets,
propagates patrol tasks, selects intercept targets, and repairs/tracks later
waypoints. The pinned runtime fleets all contain ships and exactly one
order; none has first-order target kind 2 or patrol task 7. Consequently
all mutations in this block are excluded by current input, independently
of the outer `gd.grBits` bit-1 setting. That external setting is not assumed
zero. Explicit guards reject newly eligible branches requiring services.

Both clients execute the callback once: player 0 enumerates six/five
control/action fleets and player 1 enumerates two in each run. These counts
are asserted. The integrated regression passes **1/1 in 12.740 seconds**,
including both seeded turns and 100 scout cases. No VM run.

The append audit found a concrete remaining source defect:
`reconstructed/src/generate_turn.c` passes zero as the append argument for
every player. Original turn.c:583–588 passes `rgfNoXFile[i]` for clients and
zero for the host. The backup phase derives this from X-file existence.
Original M1 is copied then appended when X1 is absent; M2 is replaced after
the generated X2. `stars_append_file` already binds the original mark-file
mode 4 and append BOF service, but the integrated coordinator has no append
callback. Fixing the decision requires carrying actual log/file lifecycle
state into the save phase, then binding mark/open/header handling; merely
adding an unconditional M1 append callback would leave its provenance gap.

**Gate 2: 5/14 packages remain.** G2-12 pre-write preparation is accounted
for this pair. Append/log lifecycle and the final state audit remain open.


## G2-12 backup append decision and source append helper (2026-09-25)

`StarsGenerateTurnState` now retains per-player append decisions from the
backup phase, and the source coordinator passes them to `FWriteDataFile`.
The host always receives zero. Source-isolation tests exercise mixed
append/create decisions across three players and the host, including the
existing two-player case. This corrects the unconditional zero argument.

The pinned adapter tracks initially absent X logs and marks X2 present when
its AI write callback runs. Backup captures absence before output writing:
M1 appends; M2 and HST create. This is explicit callback-lifecycle state,
not yet proof of complete persisted X2 bytes. G2-01's full log/header
integration remains required before that provenance can be closed.

M1 now invokes source `stars_append_file` and `stars_mark_file` using its
SHA-verified starting BOF (from the input backup, never the turn-1 output).
Mark mode 4 updates the starting header's bit 10 and retains the open state;
the source append helper invokes the BOF service with multi=1. Both append
and create paths share the existing source BOF/RNG adapter. Starting M1
message preservation now requires the append path. Assertions require one
append, one mark, and two creates per run. Actual stream storage/open/seek
and complete log-file persistence remain outstanding; no complete file
assembly is claimed by these metadata adapters.

Verification: integrated selector plus coordinator tests pass **2/2 in
12.749 seconds**. After strengthening the per-player append assertions, the
focused coordinator test passes **1/1 in 0.212s**. Both input M1 SHA-256
values were reverified after testing. An initial test-generator local-name
collision overwrote the action backup with generated header text; it was
restored byte-for-byte from the manifest-recorded source and the collision
was corrected before successful verification. Neither original corpus
content nor expected hashes were changed. No VM run.

**Gate 2: 5/14 packages remain.** Append decision/header marking is now
connected; complete log persistence, file lifecycle and the final state/
provenance audit remain open. BOF environment tick provenance remains part
of the broader open audit.


## G2-01 write-time mutation snapshot consumed by replay (2026-09-25)

AI log writing now captures all ten generated mutations into a bounded,
framed byte snapshot at write time. The append-presence marker is set after
that snapshot succeeds. The later source memory-log runner consumes the
saved bytes directly; it no longer rebuilds the stream from the live AI
record arrays at load time. This removes a dependency that could conceal
changes between writing and replay.

The replay perturbation check now destroys the copied live AI record buffer
and clears its record count before loading the saved snapshot. Design,
research, production, cargo, balance, and order changes still replay from
the saved bytes, preserving the source runner's log-offset/flag behavior.
Expected comparisons use the unchanged original trace, not the destroyed
working buffer. Both seeded runs and 100 scout cases pass the integrated
regression **1/1 in 12.381 seconds**.

This snapshot is explicitly the mutation section only. It is not a complete
X2 file, encrypted stream, or disk persistence test. The 17-byte log header's
serial/environment provenance, whole-file writing/loading, and backup file
operations remain open. The existing pinned run manifests do not capture
the original runtime serial/environment inputs; expected output bytes have
not been supplied as runtime replacements. Other open gate work remains
available while that metadata requirement is unresolved.

**Gate 2: 5/14 packages remain; G2-01 remains open.** No VM run.


## G2-12 complete battle-plan output bodies (2026-09-25)

The save coordinator now binds its battle-plan callback. It enumerates the
current loaded plan-presence table for the owner range selected by source
`FWriteDataFile` (client's own player; both players for HST). Every plan is
serialized by source `stars_write_battle_plan`, including original compressed
name encoding. Runtime bodies come from the complete input bridge's current
battle-plan state, not retained output arrays.

The generated oracle header now extracts turn-1 type-30 records solely for
length/body/count assertions. **40/40 complete records match:** five M1,
five M2, ten HST for each pinned run. Existing player, planet, fleet/order,
design, message, score, thing and RNG assertions remain passing.

Verification: integrated selector regression **1/1 in 12.529 seconds**, both
seeded runs plus 100 scout cases. No VM run.

**Gate 2: 5/14 packages remain; G2-12 remains open.** Remaining file lifecycle,
log metadata and final callback/state provenance audits are unchanged.
Whole stream assembly/comparison remain Gates 3–4.


## G2-02 task-5 dispatch restored to fleet-table order (2026-09-25)

Original `SatisfyOrders` (turn.c:2950 onward) visits the fleet table in order
and dispatches the current fleet's task within that visit. The coordinator
previously executed owner-1 scrap, then owner-0 scrap, before enumerating
remaining fleets. Final output matches did not justify this changed order.

Task-5 dispatch now occurs inside the current table traversal. It derives
eligibility from current retirement/turn flags and the current first-order
task. The two supported handlers publish retirement flags and planet results
before the next fleet is visited. Any other eligible task-5 identity fails
explicitly until its services are bound. Source no-task processing still
runs for the remaining active fleets, and all table visits are retained.

Assertions now record the actual task-5 IDs in visit order: control `[514]`;
action `[0, 514]`. Thus owner-0 action scrap precedes owner-1 scrap as the
loaded host table requires. The source packed handlers and the single RNG
stream remain in use. Both pinned outputs and their RNG checks still pass;
the changed ordering was previously concealed by this pair's independent
planet effects and owner-0's lack of an extra random draw.

Verification: integrated regression **1/1 in 12.626 seconds**, including both
seeded runs and 100 scout cases. No VM run.

**Gate 2: 5/14 packages remain; G2-02 remains open.** Whole-file log loading
and the final order/shared-state provenance audit are still required.


## G2-01 archived INI metadata candidates verified (2026-09-25)

A new read-only audit decodes both retained `STARS.INI` candidates through
`stars_serial_and_env_from_string`, using the original serial validator and
the pinned executable's 21-byte shuffle table (NE segment 5, offset 0x2870).
The table is a permutation of 0..20. Both INIs have identical SHA-256 and
decode successfully. **The serial and all 11 registered configuration bytes
match both pinned original X2 type-9 headers.** The two original environment
fields are also identical.

Reproduce with `python3 tools/satisfy_orders_environment_audit.py`.
The retained report is
`satisfy-orders-e3-environment-candidate-audit.json`; it records file/table
hashes and comparison booleans without printing registration values.
No coordinator runtime metadata was changed.

This supersedes the earlier statement that the older INIs were merely
unexamined candidates. They are now exact matching candidates. It does not
establish that either capture was the actual file read by the pinned run,
or that its registered environment is the contemporaneous disk environment:
original `FWriteLogFile` copies `vrgbEnvCur`, while the INI decoder populates
`vrgbMachineConfig`. The next provenance check is to establish current disk
identity inputs or an equivalent retained environment capture. Expected log
bytes remain comparison values only.

**Gate 2: 5/14 packages remain; G2-01 remains open.** This audit narrows the
metadata gap but does not close full log-file integration. No VM run.


## G2-01 retained disk identity extraction (2026-09-25)

A read-only FAT audit now extracts the MBR partition start, boot geometry,
root-directory volume-label entries, and DOS timestamps from the retained
Win98, Win95 and transfer disk images. It follows the FAT32 root chain or
reads the FAT16 fixed root without mounting or modifying the image. The
reconstructed `stars_get_disk_serial_number` consumes these extracted
inputs; expected X2 headers are comparison targets only.

Neither operating-system image has a volume-label entry. Their C-drive
identity follows the original findfirst-failure constants, and their disk
capacity calculation saturates at nibble 15. **Both independently reproduce
all C-drive identity bytes and its capacity nibble in both pinned headers.**
With D unavailable, both reproduce all eleven bytes. The small transfer
image has capacity nibble zero, so its C-drive capacity does not match.
An unlabeled small D disk would also produce the same D fallback/zero-nibble
bytes; the log signature is therefore not a unique physical-machine ID.

Reproduce using `python3 tools/satisfy_orders_disk_environment_audit.py`;
results and hashes of the examined identity sectors are retained in
`satisfy-orders-e3-disk-candidate-audit.json`. The hashes cover the MBR,
boot sector and root-directory data, not entire multi-gigabyte images.
No registration values or labels are printed.

Together with the successful archived-INI decode, this establishes archived
source inputs capable of generating the exact original header metadata,
without taking those runtime bytes from expected outputs. It does not prove
which physical disk attachment was used by the historical pinned runs.
The next integration can explicitly declare these equivalent archived replay
inputs, while preserving that provenance limitation in the final audit.

**Gate 2: 5/14 packages remain.** Whole-file log generation/loading remains
unfinished. No VM or original executable run; disk images were read only.


## G2-01/G2-02 generated X2 write/load integration (2026-09-25)

The coordinator now calls `stars_write_log_file` over the AI-generated memory
log. Its bounded memory-backed file contains a source BOF, the 17-byte type-9
metadata header, all ten mutation records, and EOF. Each record is framed and
encrypted through `stars_write_record`; close and presence are tracked before
the coordinator reloads the file.

Registration metadata comes from source decoding of the hash-pinned archived
INI and executable shuffle table. Current disk metadata comes from source
`stars_get_disk_serial_number` with the explicitly declared equivalent replay
profile (archived unlabeled Win98 C disk, D unavailable). Both generated type-9
headers match the originals; expected headers are assertion inputs only. This
profile does not uniquely identify the historical machine attachment.

Loading now decrypts the generated bytes with `stars_read_record`, then invokes
`stars_load_log_file` and the existing source memory-log dispatcher. Replay no
longer consumes the original write-time mutation snapshot directly. The
regression destroys both live AI records and the decoded mutation buffer before
reloading, then checks restored research, designs, production, fleet orders,
and cargo. Wrong-game and truncated generated files fail without stale-log
fallback.

The first integrated loader test exposed missing startup BOF game metadata in
the earlier isolated writer adapter. Startup BOFs now obtain marker, game ID,
turn, and turn flags from the complete loaded input. The corrected regression
passes both pinned runs and the 100 existing scout cases:

`python3 -m unittest tests.test_satisfy_orders_turin_binding -q`

**PASS: 1/1 test, 13.291 seconds.** The main draw totals remain 57/39.

Scope: this is an in-memory file backend, not guest disk persistence. X2/H2
startup BOFs still use the existing zero-tick replay convention; exact original
clock/salt provenance remains G2-13. Remaining log dirty-state callbacks,
serial-state publication, global flags, and backup/append lifecycle require
explicit audit before G2-01/G2-02/G2-12 closure. No gate is closed by this entry.

**Gate 2: 5/14 packages remain (01,02,12,13,14); 3 grouped SO-E3 gates remain.**
No VM or original executable run.


## G2-01/G2-02 log runtime-state publication (2026-09-25)

Audited original `planet.c` FLoadLogFile/FWriteLogFile/DirtyGame and `turn.c`
FGenerateTurn against the current log adapters. Added the omitted state
publication to the coordinator replay:

- Loaded BOF version and bit-12 flag publish to the session header state and
  the current host player's flags, following FLoadLogFile's host path.
- Each player gets FGenerateTurn's `0xffffffff` absent-log serial sentinel
  immediately before loading. The matching generated X2 replaces player 1's
  entry with its 15 serial/environment bytes and a zero padding byte; player
  0 retains the sentinel because its input has no X1.
- AI memory-log mutations now mark the shared GAME dirty through source
  `stars_dirty_game`. Source log write and load callbacks clear the same GAME
  field. AI mode comes from the running DoAiTurn coordinator; message-title
  notifications are counted at the UI boundary.
- A deliberately dirty reload verifies the transition to clean and exactly
  one title notification outside AI mode. Existing destroyed-buffer and
  wrong-game/truncated-file checks still pass.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
**PASS 1/1, 12.981 seconds**, both seeded turns and 100 scout cases.

The audit also identifies a concrete remaining coordinator omission:
FGenerateTurn's serial validity/duplicate-environment checks and associated
player flag/message handling occur after all player logs load, before waypoint
validation. The reconstructed coordinator currently goes directly from logs
loaded to waypoint validation. Retaining the serial table is now possible;
that consumer still needs reconstruction/binding or a complete current-input
branch proof. Do not confuse it with the later SpankTheCheaters stage, whose
turn-zero exclusion is already audited. Global flag lifetime and backup/append
lifecycle also remain open.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** No VM run.


## G2-02 post-log serial stage (2026-09-25)

Added `validate_player_logs` to the source turn coordinator immediately after
all logs load and before waypoint validation. The retained-pair adapter follows
FGenerateTurn's serial validity and duplicate-environment flag pass using
current player mode/status, the generated log metadata, and source
`stars_file_valid_serial_long`. Player 0's absent-log sentinel bypasses serial
validation; player 1's loaded AI mode selects the original clear-cheater branch.
The resulting current flags prove that neither pinned player enters the
following cheater-message loop; newly eligible messages fail explicitly.

The source coordinator sequence probe now requires this callback at the exact
post-load position, while failed log loading still exits before it. The
integrated callback additionally requires two completed loads and no waypoint
visits. Its execution count must be one per pinned turn.

Corrected the previous header adapter's player status offset: PLAYER.wFlags
is +84 (0x54); +6 is PLAYER.wMdPlr. The previous adapter incorrectly cleared
a detail bit at +6, a mistake later visibility updates could mask in final
output assertions. The header bit publication now targets +84. A focused
adapter check toggles BOF bit 12, verifies the player status/session flag, and
proves the detail/mode word stays unchanged.

This binds the serial stage for the pinned pair. It does not close all of
G2-02: the remaining post-log design/fleet preparation and full order/log
lifecycle audit must still be accounted for. No arbitrary invalid-serial
message-path support is claimed by the bounded adapter.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** No VM run.

Verification: `python3 -m unittest tests.test_generate_turn_source tests.test_satisfy_orders_turin_binding -q` — **PASS 2/2, 13.038 seconds**.


## G2-02 post-log design/fleet preparation (2026-09-25)

Reconstructed FGenerateTurn's preparation immediately after the serial/message
pass and before ValidateWaypoints:

- `stars_prepare_turn_design` visits all 16 ship slots for both players. For
  a non-free design whose first component group is not engine group 1, it sets
  that group and item 1, retaining nonzero quantity or supplying quantity 1.
  Existing engine components and free slots are preserved.
- `stars_prepare_turn_fleet_flags` clears bits 14/15 and sets bit 15 only for
  a single order targeting a fleet. The adapter visits all ten current table
  entries and publishes flags into shared world and owner-1 raw storage.
- Owner-1 design changes publish from its active bank into the host world
  bank. The integration asserts 32 design and ten fleet visits per run.
- No current entry is eligible for the subsequent follow-fleet loop. An
  unexpected eligible follow flag fails explicitly rather than skipping its
  message/order mutations.

The helpers are current-source functions in `generate_turn.c`. Focused checks
exercise invalid component repair, quantity retention/defaulting, free-slot
exclusion, existing-engine preservation, and single/multiple-order follow
flags while preserving unrelated fleet bits.

Verification: coordinator plus integrated selector tests **PASS 2/2, 13.484
seconds**. After adding the focused mutation checks, the coordinator test
passes **1/1, 0.215 seconds**. Both pinned turns retain their output-body and
57/39 main-RNG comparisons. No VM run.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** This accounts for the
identified post-log preparation; the final order/log lifecycle and complete
callback/state provenance audits remain open.


## G2-12 battle-output and password-salt branch provenance (2026-09-25)

Bound FWriteDataFile's previously absent battle-output callback to source
`stars_write_battles` for M1, M2, and HST. The fixture generator now rejects
any type-31/type-39 battle records in all verified starting HST/M inputs;
source combat eligibility already proves no battle in either pinned turn.
The combat callback now increments the runtime output-block count before
rejecting unexpected combat. Output requires that count to be zero and runs
the source writer's empty-list/client and host-return paths. The integration
requires three output callback visits and zero blocks.

The output salt condition now reads a value decoded from each verified starting
HST's type-36 section, following original FLoadGame's absent-section-to-zero
rule. Both pinned HSTs have no section. The admitted generated log mutations
exclude password changes, so this value remains zero through the current
turn. An explicit salt callback rejects any newly eligible nonzero section
instead of silently dropping it. This is a proof of the pinned zero branch,
not general password-change support. Password salt and BOF encryption salt
are separate values; the remaining BOF clock audit is unaffected.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
**PASS 1/1, 12.826 seconds**, both pinned turns and 100 scout cases.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** Remaining G2-12 work is
file/global-state lifecycle and its final audit; full streams remain Gate 3.
No VM run.


## G2-12/G2-13 output preparation order (2026-09-25)

The backup audit found that the replay consumed the saved-file `Random(8)`
inside its early `mark_backup` callback, before turn increment and score
calculation. Original FGenerateTurn consumes it after UpdatePlayerScores and
the file-backup loop, immediately before output writing. A matching draw value
alone did not prove correct service order.

Added a source coordinator `prepare_output_files` stage after score updates
and before all output writes. The shared-stream Random(8) now occurs there.
The callback requires the incremented turn, completed score work, prior backup
marking, and no output writes yet. It also publishes the selected three bits
to shared GAME.wCrap, preserving the other bits, rather than keeping them only
in the BOF scratch state. The source sequence probe requires this new stage
between score updates and file writing.

Verification: `python3 -m unittest tests.test_generate_turn_source tests.test_satisfy_orders_turin_binding -q`
**PASS 2/2, 13.510 seconds**. Both original saved flags and main-stream totals
57/39 remain unchanged.

This fixes the identified ordering/state issue. The in-memory backup namespace
still needs actual move/copy modeling: original X2 and M2 move to backup,
M1 is copied because X1 is absent, and HST moves after its header is marked.
The original pre-write scanner/cloak/buildability refresh also needs explicit
coverage in the lifecycle audit. These remain within G2-12; no package closes
from the RNG value match alone.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** No VM run.


## G2-12 pre-write buildability refresh (2026-09-25)

The output-preparation stage now visits occupied current ship designs for both
players, calls source `stars_can_build_design`, and sets SHDEF flag 0x8000
when hull/installed-part lookup rejects construction. The adapter binds
original executable part records, current player technology/race, source
FLookupPart, and TechStatus. Owner-1 updates publish to the shared host design
bank. The original only sets this bit on failure; successful checks do not
clear an already set bit.

Found and corrected a source helper mismatch: FCanBuildShdef accepts lookup
result exactly 1, whereas the helper previously accepted any nonzero result.
A focused regression now checks rejection of result 2 independently for the
hull and an installed component, in addition to existing lookup order and
empty-component checks.

Verification: integrated selector **PASS 1/1, 13.125 seconds** (both pinned
turns and 100 scout cases); focused CommonSource buildability test **PASS 1/1,
3.208 seconds**. No VM run.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** Scanner-cache and starbase
cloak refresh, backup move/copy handling, and final lifecycle/provenance audits
remain open. This entry closes only the identified buildability binding gap.


## G2-12 pre-write scanner/cloak refresh (2026-09-25)

Output preparation now refreshes current occupied ship-design scanner fields
and starbase cloak values before file writing. Ship fields at offsets
141/143/145/146 receive source scanner range, planet range, detection percentage,
and steal flags. Starbases receive `(100 - cloak_percent)^2` at offset 135,
following original FGenerateTurn. Both owner-1 banks publish to shared host
storage after mutation.

Extracted the existing current-design scanner adapter so fleet visibility and
pre-write refresh share source `stars_get_design_scanner_range`, original part
records, current race/technology, and trait handling. Starbase cloak uses source
`stars_pct_cloak_from_huldef`, current component groups/quantities, race/traits,
and original part points. Newly eligible unsupported tachyon/technology cases
retain explicit failure guards; no expected output supplies cache values.

Verification: integrated selector **PASS 1/1, 12.784 seconds**, both pinned
turns and 100 scout cases, including complete existing design-body comparisons
and 57/39 main-stream totals. These scanner/cloak values are runtime cache
fields; serialized design-body matches alone are not an independent cache
oracle. Source service binding and shared-bank writes establish their current
provenance; the final state audit still reviews all cache consumers.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** Backup move/copy handling and
final lifecycle/provenance audits remain open. No VM run.


## G2-12 memory-backed backup namespace (2026-09-25)

Added bounded active/backup file buffers containing the verified starting
HST/M1/M2 bytes and generated X2. Early backup marking now invokes source
`stars_mark_file` on the HST BOF (mode 1, value 0), clearing bit 9 before the
turn increments. After score/design refresh, the original backup decisions run:
X1 is absent, so M1 is copied and remains active; X2 exists, so X2 and M2 move;
HST moves. The regression requires three moves, one copy, no active X logs,
and preserved full backup bytes except the explicitly marked HST header bit.

Append flags now derive from log presence at that actual backup stage. The
M1 append opener requires retained active bytes, and its source mark writes
into that buffer. M2/HST create callbacks require their old files to have moved.
The generated-log replay perturbation restores the moved backup X2 into an
active test copy before checking reloading and stale-buffer rejection.

Verification: integrated selector **PASS 1/1, 12.493 seconds**, both pinned
turns and 100 scout cases. The full generated X2 survives move/reload. No VM
or original executable run; all file operations here use the bounded memory
backend. Complete newly written HST/M1/M2 contents remain Gate 3 work.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** The identified backup
move/copy gap is implemented. Global/session flag lifetime, cleanup, startup
clock provenance, and final package audits still require completion.


## G2-13 startup BOF clock replay and submit flag (2026-09-25)

Removed the zero-tick startup BOF convention. X2/H2 now use the retained
per-writer low-11-bit clock residues from the recovered writer audit: control
1560/1560, action 1858/1859. These are explicit recovered external replay
inputs, derived from original salt minus independently recovered Random(2000)
modulo 2048; they are not contemporaneous clock captures and do not identify
the original full 32-bit tick values. Main RNG calls still execute source
`stars_write_bof` using the shared evolving seeds. Expected BOF bytes are used
only for comparison, not as generated records.

The first complete startup BOF comparison exposed missing flag 0x0100 in both
files. Original EnsureAis (MDI 56BC) saves submit state, sets gd.grBits bit 4
around DoAiTurn, then restores it before host generation. The replay now
represents that scope in BOF session state; source WriteBOF produces the flag.
This is source-derived behavior, not an expected-header patch.

All five generated BOFs now compare byte-for-byte with their respective
original records, including X2/H2. Startup Random(2000) assertions remove the
clock contribution before comparing the recovered draw; the clock addition
does not advance or replace main RNG values. The generated encrypted X2 still
passes source read/load/replay after backup restoration.

Verification: integrated selector **PASS 1/1, 12.827 seconds**, both pinned
turns and 100 scout cases; main-stream totals remain 57/39. Earlier entries
stating startup zero ticks or unmatched startup BOFs are superseded.

**Gate 2: 5/14 packages remain (01,02,12,13,14).** G2-13 still needs its final
whole-stream input/callback audit, including the explicit recovered-clock
provenance boundary. Global session state and cleanup remain under audit.
No VM run.


## G2-13 main RNG package closed (2026-09-25)

The [closure audit](satisfy-orders-e3-rng-audit.md) now covers all runtime draw
callbacks, source initialization, five BOF seed handoffs, separate crypto/INI
state, exact transcript order, and the recovered external clock boundary.
Continuous pre/post seed checks and per-index limit/value comparisons pass for
all 57 control and 39 action draws. Expected data only asserts results; the
seeded scout-witness storage was removed and its assertions made explicit.

Verification: integrated selector **PASS 1/1, 12.898 seconds**, both pinned
turns and 100 nonseeded scout cases. All five BOFs compare exactly. The recovered
clock residues establish equivalent replay inputs, not independent historical
wall-clock evidence. No VM run.

**G2-13 closed for the pinned pair. Gate 2: 4/14 packages remain
(01,02,12,14); 3 grouped SO-E3 gates remain.** Other packages and the final
whole-gate audit remain open. New services must preserve this RNG evidence.


## G2-01 Turin status and ship-maintenance callbacks (2026-09-25)

The AI audit found two always-reached services still represented by null
callbacks: CheckAiShdefStatus and EnsureTurinDroneShdefs. Both are now bound.

The status adapter builds all 16 entries from current owner-1 design flags,
existing counts and creation turns, then calls source
`stars_check_ai_shdef_status` for each original role range. Current technology
index 3 supplies the original <7 miner-role exclusion. Seven calls execute in
each pinned run. Unexpected recycling fails explicitly; turn-zero current
designs require no recycling.

Ship maintenance now calls source `stars_ensure_turin_drone_shdefs` with current
flags, counts, hulls and all six current technology values. Its role guards
prove that neither pinned run needs a ship creation/replacement. Explicit
callbacks reject such work if future inputs make it eligible; a source-backed
random callback would enter the same audited main stream. This is distinct
from the four already source-created AI starbase designs.

Verification: integrated selector **PASS 1/1, 12.360 seconds**, both pinned
turns and 100 scout cases. Seven status calls and one maintenance call are
required. The complete 57/39 RNG transcript and five BOFs remain unchanged.
The initial link failure was resolved by including the maintenance source in
the probe build.

**Gate 2: 4/14 packages remain (01,02,12,14).** G2-01 remains open pending
remaining callback and allocation/destruction/history lifecycle audit. No VM run.


## G2-01 H2 stream and AI temporary ownership (2026-09-25)

The history writer now emits its complete BOF/framed/encrypted record stream
into a bounded memory file through source `stars_write_record`. At close, the
entire generated H2 is compared with each matching original H2. Expected bytes
are comparison-only inputs. Both files match exactly, extending the existing
individual history-body and BOF checks.

Replaced the AI temporary allocator's no-op release callback with ownership
tracking for the planet table, planet index, and AI data. Duplicate allocation,
unknown/wrong-tag release, double release, and release before both log/history
writers finish fail explicitly. Every integrated and scout run requires three
allocations, three releases, and no live AI temporary allocation. The bounded
backend retains storage for diagnostic inspection; the lifetime mask models
ownership, not host heap deallocation. This does not by itself prove every
later pointer consumer or whole-game DestroyCurGame cleanup.

Verification: integrated selector **PASS 1/1, 12.618 seconds**, both complete
H2 comparisons, seeded turns, and 100 scout cases. Main RNG 57/39 remains valid.

The callback audit also identifies Turin's final FillProductionQueue call as
still unbound. Original AI4 FillProductionQueue selects each owned AI planet,
initializes production, calls FFillProdMinesAndFactories, and finalizes the
queue. Its current-input behavior must be bound before G2-01 can close.

**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run.


## G2-01 final queue resource-budget correction (2026-09-25)

The pending FillProductionQueue binding audit found a missing early return in
`stars_fill_prod_mines_and_factories`. Original AI4 FFillProdMinesAndFactories
subtracts queued costs from available resources and returns zero immediately
when signed `rgResLeft[3]` is negative, before scanning queues or querying item
costs. The helper previously continued into mineral-shortage handling and could
add mineral-only-cost items despite a negative resource budget. It now returns
at the original boundary. Zero resource balance still enters the queue logic.

The strict-C89 focused probe passes **6 checks, zero failures**. Its new negative
budget case verifies both no cost callbacks and no queue additions; the zero
boundary case verifies continued processing. Removing the guard in a temporary
source copy makes the new regression fail. The integrated selector regression
also passes **1/1, 12.848 seconds**, including both seeded turns and 100 scout
cases. That integrated pass does not exercise the corrected helper yet: the
final queue callback remains unbound.

Remaining work here is to bind current selection/InitProduction, source
GetResourcesAvailable and GetProdQCost, the corrected fill routine, and
FinishProduction with proper temporary-queue ownership and state publication.
Do not infer the pinned negative-budget branch until those inputs are derived.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run; no package closed.


## G2-01 final queue reconciliation correction (2026-09-25)

Original InitProduction copies the selected queue, then reconciles in queue
order against the available-item catalog. The current source helper instead
iterated the catalog and retained only its first matching queue entry, losing
duplicate orders, order priority and packed progress bits. This matters to the
pending final pass because the AI has queued two separate scout entries.

The helper now traverses the original queue, retains duplicate entries and
non-quantity bits, removes unavailable items, clamps to remaining capacity,
and deducts finite capacity after each entry. Quantity 0x3ff remains unlimited.
Queue header bytes are preserved. The expanded probe checks reversed catalog/
queue order, duplicate finite and unlimited entries, progress bits, removal,
remaining availability and header preservation.

Added `tests.test_ai_final_queue_source` to compile and run both final-queue
helper probes under strict C89. It passes **1/1 (two service subtests), 0.415s**.
The integrated selector also passes **1/1, 12.362s**, both seeded inputs and
100 scout cases. Final queue binding is still absent, so the latter does not
prove execution of these helper corrections in the complete turn.

The input/catalog audit is not complete. In particular, the existing typed
InitProduction catalog builder's planetary group and item enumeration require
comparison with original produce.c before using it as a full faithful binding.
The original writes planetary group 1 (high-word mask/or 2); the helper currently
uses group 2. Its catalog-generation claims must not stand in for that audit.
Resource/queued-cost adapters and temporary queue finalization also remain.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.


## G2-01 production catalog correction (2026-09-25)

Compared the catalog portion of original produce.c InitProduction with the
current helper before binding the final AI pass. Corrected planetary group
from 2 to 1 and catalog sequence: eligible ship words, eligible starbase words,
planetary item 13 only for lookup result exactly 1, four packet items 14..17
for positive mass-driver warp, factory/mine/defense capacity, unconditional
alchemy item 11, conditional scanner item 27, terraforming item 12, then
race-gated automatic items 0..6. Unlimited quantities are 0x3ff; factory/mine
capacity retains the original 0x3fc cap. Tutorial notification now also requires
game flag 0x0800, not only player zero.

The typed boundary now accepts eligible starbase words, planetary lookup
result, scanner type and game flags. Eligible ship/starbase lists still need
to be derived from current flags, designs, hull cargo capacity and selected
starbase in the integration adapter. The old research-mask override no longer
replaces original race exclusions (AR omits 0..2; CA omits 4..5).

Expanded source checks compare exact packed catalog entries and ordering,
finite capacity, unlimited quantities, lookup result 1 versus 2, race exclusions,
scanner eligibility, and tutorial suppression without its flag. Queue duplicate/
progress regressions remain. `python3 -m unittest tests.test_ai_final_queue_source
 tests.test_satisfy_orders_turin_binding -q` passes **2/2, 12.793 seconds**,
including both seeded coordinator replays and 100 scout cases. No native run.

The final queue callback remains unbound; integrated green status does not
establish these helpers execute in the turn yet. Next bind current catalog
eligibility, resource/queue costs, and temporary queue finalization, then
validate the unchanged generated X2/H2 and RNG transcript.
**Gate 2: 4/14 packages remain (01,02,12,14).** No package closed.


## G2-01 final queue budget bound to current AI state (2026-09-25)

Turin's final fill_queue boundary is no longer null in the seeded replay. Its
current partial adapter executes source GetResourcesAvailable, GetProdQCost and
FFillProdMinesAndFactories. It derives mineral estimates from current loaded M2
concentrations, operating mines, race efficiency and rounding flag; resources
from source population/factory calculations; research withholding from current
player percentage and the selected planet flag; and queued costs from the two
runtime packed orders and current repriced design bank through source
GetProductionCosts. No original output supplies these values.

Both pinned runs prove available resource lane 3 is less than queued cost lane
3. The corrected source fill routine therefore returns zero before reading its
catalog or invoking additions. Newly eligible nonnegative budgets, unsupported
queue groups/progress/starbases, or AR mining explicitly fail. This binds the
reached budget/early-return behavior, not the entire FillProductionQueue service.
Original selection, InitProduction catalog/temporary allocation and
FinishProduction still need binding and ownership/state checks, even though
this fill branch makes no additions.

The first integrated attempt exposed an adapter mistake: turn_planet_states
is initialized by host turn loading after AI startup. The adapter now reads the
loaded M2 planet and current AI supply population, with its own max-population
context. It does not initialize or mutate the host turn table early. Research
exemption reads the second improvement word at packed offset 24. An execution
count requires exactly one final budget pass before seeded log writing.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
**PASS 1/1, 12.606 seconds**, both seeded turns and all 100 scout cases. This
includes the original output-body checks, complete H2 comparisons and the
continuous 57/39 RNG transcript. The initial failed attempts are not passes.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.


## G2-01 final queue initialization and release (2026-09-25)

The final Turin callback now invokes source `stars_fill_production_queue` over
the sole source-enumerated owned AI planet, with explicit selection,
initialization, budget/fill and finish callbacks. Initialization copies the
current two packed orders into a separately owned temporary queue and calls
source `stars_init_production`; reconciliation must retain both exact orders.

Catalog inputs now derive from current ship/starbase design flags and weight,
original hull cargo limits, exact-success source part lookup, source mass-driver
warp, factory/mine/defense capacity, source terraforming availability, current
scanner/race state and tutorial flags. Terraform immunity comes from PLAYER
minimum-environment bytes +19..21; centers come from +16..18. The adapter
publishes the original saved research-exemption bit into the AI session flags.

After the source fill routine's proven resource-deficit return, source
`stars_finish_production(write=0)` releases only the temporary queue. Checks
require one selection, initialization, budget pass, finish and release, no live
temporary ownership, unchanged selected queue pointer and unchanged planet
improvement bits. UI callbacks reject unexpected execution. This is bounded
memory ownership, not a native heap allocation claim.

The selection callback currently copies current AI planet/supply state at the
boundary. Its complete ChangeMainObjSel selection/report-state behavior still
requires audit/binding; do not treat this callback as that source service.
G2-01 also retains the wider session-flag/destruction/callback audit. The source
loop, initialization and finish gap is now implemented for the pinned pair.

Verification: `python3 -m unittest tests.test_ai_final_queue_source tests.test_satisfy_orders_turin_binding -q`
**PASS 2/2, 12.778 seconds**, including both pinned replays, 100 scout cases,
full H2, original record-body comparisons and continuous 57/39 RNG checks.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.


## G2-01 shared selection for basic tasks and final queue (2026-09-25)

Replaced final queue's independent planet copy with source
`stars_change_main_obj_sel` and a trace-owned selection record. The basic-task
adapter establishes its current planet selection before the upgrade check;
the final queue pass invokes the same source service and exercises its AI
same-kind/same-id early return. The selection loader copies the loaded M2
planet plus current supply population, resolves coordinates from complete
input XY, and builds fleet context from current compacted fleet identities and
mutable raw fleets/orders. Source report invalidation receives current AI mode
and takes its original AI early return. UI filling rejects unexpected calls.

The integrated check requires two selection-service calls, only one planet
load, and matching selected kind/id/planet. The queue loop no longer overwrites
the selected planet before the same-selection return. The existing source
queue initialization, budget and finalization checks remain in place.

Verification: integrated selector **PASS 1/1, 12.748 seconds**, both pinned
replays and 100 scout cases, complete H2 and continuous 57/39 main-RNG checks.
This accounts for these two selection boundaries. Earlier AI selection
history is not yet comprehensively modeled; this is not a complete selection
lifecycle closure.

The original-source audit also exposes a wider G2-01 gap: startup_handle_tasks
still implements only the starbase-upgrade slice of HandleBasicAiTasks. Original
AIU additionally calls KeepFleetsMoving, QueueAiStarbases, resource/cost admission,
production initialization, FAIFling, FQueueAiScanner, FQueueAiDefenses,
FQueueAiTerraforming, FinishProduction, guarded FixPlanetsUnderAttack and
AddMinesToBlockedQueues. These need current-input bindings or source-proven
exclusions. The existing generic stars_handle_basic_ai_tasks helper itself
omits several original guards and boundaries, so simply wiring that helper
would not prove coverage. Passing final outputs cannot substitute for this
service audit. Track this explicitly before G2-01 closure.

**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.


## G2-01 basic-task defense gate (2026-09-25)

Auditing FQueueAiDefenses exposed a source mismatch: original AIU requires
signed population >=1600, then compares current defenses against population/80.
The reconstructed helper mislabeled those inputs as mineral reserve/current
mines and could queue work below 1600. Corrected the guard and local names.
The existing test that expected construction at population 800 was incorrect;
it now uses 1600. Added rejection at 1599 and rejection when defenses already
meet the population/80 target. The focused probe passes all five checks.

The seeded basic-task adapter now invokes the corrected source service with
current AI supply population and loaded current defense count. Both inputs
prove population <1600, so the original early return precedes any queue lookup
or addition. A newly eligible population fails explicitly pending the complete
basic-task queue binding. Exactly one defense-gate visit is required per run.
Catalog placeholders are unreachable in this proven branch; they are not a
claim of arbitrary defense-queue support.

Verification: `python3 -m unittest tests.test_ai_final_queue_source tests.test_satisfy_orders_turin_binding -q`
**PASS 2/2, 13.332 seconds**, including the three helper probes, both seeded
turns and 100 scout cases. Full H2/record-body/RNG comparisons remain valid.
The initial focused link failure was fixed by discarding unused ai.c sections,
matching the existing host integration build's linker convention.

This closes the identified defense gate only. The basic-task resource admission,
initialization/finalization, fling/scanner/terraform checks, movement/starbase
preludes and blocked-queue tail remain to be accounted for in original order.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.


## G2-01 basic-task admission and queue decisions (2026-09-25)

The basic-task adapter now derives available resources and full queued costs
before its upgrade check. Current population passes the personality-dependent
39/59 threshold. Source resource and queue-cost services prove all three
mineral lanes affordable; the original deliberately permits a deficit only in
resource lane 3 (its rejection is i<3, not i<4). The current pair follows that
branch. Unexpected admission conditions fail rather than bypassing work.

Basic tasks now select the planet and call the same source InitProduction and
FinishProduction adapters used by the final fill pass. Separate phase and
begin/finish counts require two temporary queue initializations and releases
per seeded run, no overlapping ownership, and unchanged selected orders. The
final pass requires completion of the basic phase and retains its own budget
and fill invocation. These changes first passed in 12.689 seconds.

Bound packet, scanner and terraforming checks around the defense check in
original order after the failed upgrade:

- Packet resources come from the basic-task resource calculation. Their mineral
  sum is <=3000, proving the original packet branch cannot reach target search
  or Random(4). Source FAIFling also takes its resource gate; callbacks reject
  unexpected packet work.
- Scanner candidates derive from the current initialized catalog and temporary
  queue. Neither contains group-1 items 18..26; source FQueueAiScanner returns
  without querying costs or adding work. A newly eligible candidate fails
  explicitly pending its per-item cost adapter.
- All three current environment axes equal the loaded race centers, proving
  original FQueueAiTerraforming's no-axis branch. The source-built catalog
  also has no terraforming item; the current source service returns zero with
  that catalog and current population/personality. This is bounded evidence,
  not a claim that the generic helper models every original environment guard.

Verification after all changes: integrated selector **PASS 1/1, 12.585 seconds**,
both pinned replays and 100 scout cases; H2, output-body and 57/39 RNG checks
remain valid. No native run. KeepFleetsMoving, QueueAiStarbases, the guarded
FixPlanetsUnderAttack stage and AddMinesToBlockedQueues still require complete
current-input coverage. The latter is reached for the first queued scout and
needs a source ETA calculation; it must not be dismissed because final queues
already match. Earlier selection history and wider lifecycle audits remain.

**Gate 2: 4/14 packages remain (01,02,12,14).** No package closed.


## G2-01 fleet-maintenance integration exposes reached speed path (2026-09-25)

Added a basic-task prelude that feeds current compacted AI fleets, current
packed order counts and loaded M2 things into source KeepFleetsMoving. Added
current-input checks for QueueAiStarbases' existing-valid-starbase exclusion
and FixPlanetsUnderAttack's game-size/turn gate, plus execution counters.

**This work is not yet passing.** The integrated selector fails on control
(and must not count as closure): fleet 0 has two orders at this AI boundary,
so KeepFleetsMoving calls SetAiFleetIdealSpeed with parameter 16 and zero
minefield threats. The earlier expectation that all fleet order counts were
<=1 was wrong; later log reload normalizes the scout list. The explicit
unsupported-speed callback reports `fleet=0 speed=16 threats=0 orders=2`.
Original AIU SetAiFleetIdealSpeed selects the fleet, calls IWarpBestForWaypoint
on its second order, and only publishes a changed warp when necessary.

Next required binding: current fleet/order selection, source
stars_set_ai_fleet_ideal_speed -> stars_iwarp_best_for_waypoint, with both
IFindIdealWarp modes and current waypoint/fuel/planet context. Reusing the
existing scout warp assertion or simply returning its current warp would
supply an answer instead of proving this service. The generic reconstructed
best-waypoint helper and its callbacks need comparison with original gates.
Keep the failure explicit until this path is implemented. The new starbase/
attack checks are not independently closed by a failing integrated run.

Last attempted integrated run: **FAIL**, 12.520 seconds, control reports four
failures including the unsupported speed call and propagated failure checks.
Prior passing results remain historical evidence only. No VM run.
**Gate 2: 4/14 packages remain (01,02,12,14).** Goal remains active.


## G2-01 waypoint warp source correction (2026-09-25)

Compared IWarpBestForWaypoint with original scan.c before binding its reached
scout call. Corrected the source helper's branch structure: first-waypoint
return, AI-only second ideal-warp query, task/colonizer flat-out handling,
owned-destination AI-limit suppression, fuel/capacity admission, descending
trial-warp search against fuel on board, and the original optimization that
lowers warp only while rounded travel time stays equal. The old helper could
return the larger of its optimized and original warp, preventing the reduction.
Stargate override and the >11 clamp now follow the original final order.

The typed state now exposes fuel on board, target kind, colony-hull presence
and the owned-base condition; callers must derive these from current state.
Fuel callbacks receive a trial warp and must not leave the original order
mutated. The prior conflation of waypoint index and trial warp is removed.

Replaced the weak source probe (range checks) with eight exact checks covering
trial search counts, AI cap, travel-time reduction, fleet-target exclusion,
task flat-out behavior, human fuel-budget early exit, first waypoint and gate
override. Strict-C89 runner passes **1/1, 0.867 seconds**, including all four
source helper probes. No new native run. Historical native checks did not
validate these expanded semantics.

**The integrated selector is still failing at its explicit unbound
SetAiFleetIdealSpeed callback.** This change fixes the source helper needed
there; it does not bind current waypoint fuel/selection inputs yet. Next audit
LFuelUseToWaypoint (its current source divides distance by warp rather than
warp squared) and connect original-ordered current-state fuel trials. Do not
replace that consumer with the previously retained/current warp answer.
**Gate 2: 4/14 packages remain (01,02,12,14).** No package closure.


## G2-01 original waypoint-fuel instruction audit (2026-09-25)

Retained `analysis/compat/satisfy-orders-e3-fuel-waypoint-disassembly.txt` from
original EXE NE segment 11, A9F4..AE73, including executable SHA-256 and decoded
DGROUP doubles. AACB..AB29 proves the original adds 0.99999 to DGetDistance,
truncates that value for the segment distance, divides the floating value by
warp twice, adds 0.9999, then truncates for cYears. The decompiler lost these
floating operations; the source's distance/warp formula was incorrect.

Corrected source LFuelUseToWaypoint accordingly and added an optional precise
distance callback, retaining the old integer callback for integral-distance
callers. Also corrected the positive fuel-generation branch: when generation
is at least one year's consumption, the original segment requirement is one
year's consumption; it does not retain the entire route's cost. Smaller gains
still compare the adjusted multi-year cost against the prior maximum.

The focused source probe now checks exact estimator counts, corrected
multi-segment totals, invalid warp, both fuel-generation branches and a
fractional-distance boundary (7.0 versus 7.001) sensitive to the two original
constants. `tests.test_ai_final_queue_source` passes **1/1, 0.834 seconds**, all
five helper subtests. These are host checks; old native reports for the prior
fuel formula do not establish correctness of the revised arithmetic. The
reconstruction note records that limitation.

Capstone 5.0.9 was installed only in `/tmp/stars-capstone-audit` to inspect the
retained executable. The first sandboxed download failed DNS; the approved
network retry succeeded. No VM execution or original game run occurred.

**Integrated replay remains failing at the unbound speed callback.** Next
connect current designs, second order, fuel trials and fleet selection to the
corrected waypoint helpers, then rerun the complete pair. Existing movement
uses its separate source fuel estimator; this does not claim G2-01 closure or
new native evidence. **Gate 2: 4/14 packages remain (01,02,12,14).**

## G2-01 original ideal-warp scoop correction (2026-09-25)

Audited IFindIdealWarp directly in the pinned original executable, NE11
A76E-A9F3. The retained instruction listing is
`analysis/compat/satisfy-orders-e3-ideal-warp-disassembly.txt`, with EXE hash.
The existing reconstruction reversed the minimum-speed conditions for the
one- and two-step scoop reductions. Original A902-A95F tests warp >=5 and
>=6 before accepting the corresponding zero-fuel lower speed; the nested
non-reduction conditions therefore use warp <5 and <6. Original A96B-A97E
addresses fuel_used[warp-3] from engine offset 0x36. The decompiler's
ore_cost[warp+2] spelling aliases that same memory and must not be modeled
as an independent zero-initialized array.

Corrected the helper and replaced its old two-case probe (which expected
incorrect ordinary-engine behavior) with 15 checks: each reduction, minimum
speed boundaries, nonzero fuel, ignore-scoops mode, ordinary warp-10 cap,
and special engine exceptions. All six service subtests in
`python3 -m unittest tests.test_ai_final_queue_source -q` pass (1.437s).
The current-design ideal-warp builder is now reusable by either mode without
scout-specific expected-target assertions. Its existing scout wrapper retains
those assertions separately.

Integrated verification compiled after fixing a signed ship-count pointer
in the refactor, then reached the same unsupported fleet-speed callback:
**FAIL**, 12.471s, control four failures, fleet=0 speed=16 threats=0 orders=2.
This correction does not close the speed binding. Next: connect fleet
selection, waypoint state, trial fuel and planet callbacks to the corrected
helpers, then complete the blocked-queue ETA path and lifecycle audit.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.

## G2-01 fleet-speed maintenance bound (2026-09-25)

Replaced the failing KeepFleetsMoving speed callback with a current-state
adapter to stars_set_ai_fleet_ideal_speed, stars_iwarp_best_for_waypoint,
stars_fuel_use_to_waypoint, and stars_est_fuel_use. The integration test now
compiles both waypoint helpers directly. Both ideal-warp modes share the
current-design/engine builder; expected scout coordinates remain assertions
in the separate scout-order wrapper.

The adapter reads fleet 0's current two packed orders, ship counts, cargo,
fuel and designs at this AI boundary. Trial warp changes use local order
copies. Fuel estimation reuses the movement calculation without its movement
accounting or sufficient-fuel assertion, since trial warps may exceed fuel.
The EstFuelUse implicit-distance path truncates original Euclidean distance;
LFuelUseToWaypoint retains its separately audited floating constants. Fuel
transport hulls, ram-scoop gain, capacity, known planets and owned starbases
come from current AI inputs, not the later host planet table.

Selection now calls source ChangeMainObjSel for fleet 0 before speed planning,
then for the basic planet queue and final queue. The passing pair checks three
selection calls and two planet loads; the final same-planet selection returns
early. Ideal-engine checks include both maintenance modes. The prelude requires
one speed call and at least one trial-fuel call. Both runs retain their existing
speed, so changed-speed publication remains explicitly rejected if reached.
Zero minefield threats are checked. The stargate adapter implements the exact
planet-location lookup and source no-gate/no-jumpgate exclusion; newly eligible
jump branches fail explicitly. No RNG draw is added.

Verification:
- python3 -m unittest tests.test_satisfy_orders_turin_binding -q:
  **PASS**, 12.472s, both seeded complete-input replays and 100 scout cases.
  Existing original output-body comparisons and continuous 57/39 RNG checks pass.
- python3 -m unittest tests.test_ai_final_queue_source -q:
  **PASS**, 1.280s, six source-service subtests.

This supersedes the previously current unsupported-speed integrated failure.
G2-01 still requires AddMinesToBlockedQueues (ETA and resource admission) and
remaining selection/session/destruction callback lifecycle audit. G2-02,
G2-12 and G2-14 retain their closure requirements.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.

## G2-01 production ETA simulation prerequisite (2026-09-25)

The AddMinesToBlockedQueues audit reaches PszProductionETA, which delegates
to EstimateItemProdSched. Comparison against original produce.c:2168-2380
found the reconstructed scheduler replaced stored minerals with annual mining,
did not copy the input queue, failed to retain a shared partial item, called
the builder on a skipped non-final alchemy entry, and exposed no mutable
four-lane build budget. Those behaviors cannot establish the AI's ETA gate.

Corrected stars_estimate_item_prod_schedule to copy its queue and planet,
start with an empty partial item, accumulate newly mined minerals, retain
partial/queue mutations across simulated years, carry unspent mineral stock,
apply research before building, and skip non-final alchemy while setting the
next build's alchemy flag. A new mutable callback passes the current item,
optional partial output and all four budget lanes to the source build adapter.
The scheduler updates simulated mine/factory counts after applicable builds,
then advances population. The legacy build callback remains available, with
resource publication through the simulated planet. The explicit callback
contract returns new mining quantities without adding them itself.

Expanded the source probe with a two-year case checking 99+10 initial mineral
stock, spending then further mining to 114, 25% research deduction, shared
partial completion, simulated mine count and population, and byte-identical
caller queue/planet. A second case proves a non-final alchemy entry is skipped
and passes its flag to the following item. These complement the existing
first/last ETA, negative-index resource and final-alchemy checks.

Verification:
- python3 -m unittest tests.test_ai_final_queue_source -q: **PASS**, 1.158s,
  seven source-service subtests including the corrected scheduler.
- python3 -m unittest tests.test_satisfy_orders_turin_binding -q: **PASS**,
  12.854s, preserving both pinned replays, original body and RNG checks.

The scheduler is NOT yet connected to the integrated AI queue tail. Next is
its current-planet mining/resource/population and source CBuildProdItem adapter,
then AddMinesToBlockedQueues' first-item ETA and unit-resource admission check.
No package is closed by this prerequisite; wider lifecycle audits remain.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run.

## G2-01 blocked-queue ETA and resource gate bound (2026-09-25)

Added the original AddMinesToBlockedQueues tail after FixPlanetsUnderAttack's
turn gate. It selects the current AI planet, reads the current packed queue,
and skips the same first-item group/kind combinations as original aiu.c:801ff.
For the reached ship item it invokes corrected stars_estimate_item_prod_schedule
with current-state mining, resource, build and population adapters. The private
planet includes supply-adjusted population, starting fraction and improvements;
the simulation uses current research percentage, repriced ship costs and queue
progress. stars_build_production_item consumes each simulated budget. The real
selected planet and real queue are compared before/after to prove preservation.

Both pinned runs report:

    AI_BLOCKED_QUEUE first=2 last=2 builds=2 years=1 unit=35 resources=28

Thus source GetProductionCosts / CResourcesAtPlanet and research deduction
produce 35 > 28*(2-1), excluding the mine/alchemy insertion branch. No ETA or
resource answer is injected. Newly eligible insertion work fails explicitly.
Selection accounting now checks four selection calls and two planet loads;
the queue-tail and final-queue same-planet selections take source early returns.

Binding exposed an independent status defect: original turn2.c:740-759 returns
status 0 when finite production completes with zero quantity remaining. The
reconstruction returned 2, reserved by the original for an auto-build capacity
case. Corrected the finite completion status and the source probe's old
expectation. Partial ship progress/status is unchanged. No broader claim about
unreached auto-build branches is made.

Verification:
- python3 -m unittest tests.test_ai_final_queue_source tests.test_satisfy_orders_turin_binding -q:
  **PASS 2/2**, 14.487s; eight source-service subtests, both complete-input
  seeded replays and 100 scout cases, original body comparisons and 57/39 RNG checks.
- Reran the integrated test with successful subprocess diagnostic lines retained:
  **PASS**, 12.597s. Report: `analysis/compat/satisfy-orders-e3-blocked-queue-test.txt`.

G2-01's reached basic-task queue tail is now bound. Remaining G2-01 work is the
wider callback/selection/session-flag/destruction lifecycle audit. G2-02's
order/log audit, G2-12's writer lifecycle audit and G2-14's final provenance
regression remain open. **Gate 2: 4/14 packages remain (01,02,12,14).** No VM run.

## G2-01 AI initialization and runtime scout markers (2026-09-25)

Original DoAiTurn clears the allocated AI planet table before preparation and
restores saved idPlayer after success or failed load. The reconstructed
coordinator omitted both. Added the original table clear and an optional shared
current_player pointer whose loader-written value is restored at exit. Expanded
the coordinator probe with distinct allocation buffers, dirty planet storage,
AI-data header checks, saved-player restoration and a failed-load case. The
shared current-player pointer is source-tested but is not yet bound to the
integrated session's wider player/global lifecycle.

The table clear initially broke the integrated scout replay (41 failures):
its marker bytes were supplied before DoAiTurn by a Python-derived array.
Removed that runtime initialization. At the reached scout location boundary,
IdNearestUnknownPlanet's prerequisite now invokes source
stars_id_nearest_colonizable_planet with current loaded planet owners and
current fleet/order targets. No wormhole output is requested, matching the
original prerequisite. Results populate the live 16-byte AI planet records;
the retained array is now assertion-only. The initialization is performed once.
Known unowned planets are explicitly rejected in this bounded adapter until
the planet-optimum callback is bound; neither pinned input reaches that case.

Corrected the source colonizable helper to distinguish missing records
(owner sentinel -2) from known unowned planets (-1). Missing records retain the
original personality-dependent default (0x10 for Turin). Modes 0 and 5 rebuild
the classification even when marked, as in original aiu.c:485ff. Added focused
checks for missing records, marked-table reuse and mode-0 reclassification.

Verification:
- python3 -m unittest tests.test_do_ai_turn_source tests.test_satisfy_orders_turin_binding -q:
  **PASS 2/2**, 13.078s, including both pinned seeded replays and 100 scout cases.
- python3 -m unittest tests.test_ai_final_queue_source -q:
  **PASS**, 1.999s, nine source-service subtests.

The initial failing run is superseded by runtime marker initialization. All
existing record comparisons, blocked-queue ETA checks and 57/39 RNG checks pass.
G2-01 remains open: DestroyCurGame is still unbound at AI entry, and shared
current-player/session-state restoration plus the wider callback/lifecycle audit
remain. G2-02, G2-12 and G2-14 are unchanged.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run.

## G2-01 cleanup source-state corrections (2026-09-25)

Before binding AI-entry DestroyCurGame, compared stars_destroy_cur_game with
original io.c:1985-2110. The helper omitted current memory-log positions,
message index, AI-data reference, map capacity, turn, player-count reset,
selected coordinates and gd flag masks. Its game identity, salt and last-tick
fields were only 16 bits despite original 32-bit fields. Extended the typed
state and corrected these resets. Heap callbacks now observe planet/fleet/thing
counts cleared between the corresponding heap resets; battle/session masks are
applied before remaining heap resets. ResetMessages observes player=-1 and
cleared log/AI positions before game identity, salt and tick are cleared,
matching the original ordering.

The source probe now seeds full-width nonzero values and verifies preserved
unrelated flag bits, the exact nine-heap sequence, intermediate state visible
at heap callbacks, and ResetMessages ordering. Added it to the focused source
suite. Verification:

    python3 -m unittest tests.test_ai_final_queue_source tests.test_satisfy_orders_turin_binding -q

**PASS 2/2**, 14.045s: ten source-service subtests plus both seeded complete-input
replays and 100 scout cases. Existing original record-body, ETA and RNG checks
remain passing. This is corrected helper evidence, not yet integrated cleanup:
the AI-entry destroy callback remains unbound, and runtime arenas must be
cleared/reloaded without erasing the immutable starting-input corpus or retained
expected outputs. Shared session restoration still needs that integration.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.

## G2-01 AI-entry cleanup and reload bound (2026-09-25)

The DoAiTurn destroy callback now invokes stars_destroy_cur_game. Its heap
adapter clears owned runtime planet/supply, fleet identities/pointers/raw
records, ship/starbase design banks, orders, queue and AI temporary buffers.
The nine original heap resets are checked in order. Other heaps have no live
allocations at this fresh-input entry; retained input records remain the load
source, not objects owned by the disposable runtime arrays. The adapter rejects
a dirty/UI session, repeated destruction or preexisting AI temporary ownership.
It resets message state and publishes the source selection reset and gd flags.

Moved initial planet/supply/fleet loading (and its small independent transport
checks) out of main into the load callback, after destruction. Loading rejects
uncleared fleet/design ownership, rebuilds from the starting M2 records, and
publishes player/map/fleet/thing counts. The player's dead flag now comes from
the loaded player record rather than default-zero initialization.

Bound DoAiTurn.current_player to the shared session player. The replay explicitly
starts with no active UI player (-1); loading selects player 1. Personality,
log and history callbacks verify the active player and AI phase. At return the
coordinator must restore -1, with exactly one destruction, nine heap resets
and one message reset. This models the headless replay's caller state; it does
not claim a historical desktop selection capture. Wider consumers of session
fields and loaded-view transitions still require the final lifecycle audit.

Verification:

    python3 -m unittest tests.test_do_ai_turn_source tests.test_ai_final_queue_source tests.test_satisfy_orders_turin_binding -q

**PASS 3/3**, 15.019s: coordinator lifecycle test, ten focused source-service
subtests, both pinned seeded replays and 100 scout cases. Original body,
blocked-queue ETA, 57/39 RNG and temporary-allocation checks remain passing.
The first compile attempt found an unused main variable after moving the loader;
that was removed before these passing runs.

G2-01's previously unbound AI-entry destruction and player restoration are now
connected. Remaining closure work is the end-to-end callback/state audit,
including consumers and transitions of shared session fields. G2-02, G2-12 and
G2-14 retain their stated requirements.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.

## G2-01 shared flags and first production selection (2026-09-26)

The lifecycle audit found the submit flag was set only in rng_file_data while
AI services consumed trace.game_flags. Made the runtime low gd flag word the
source for the submit interval. The wrapper saves and restores its original
bit 4 rather than unconditionally clearing it. Cleanup now consumes/publishes
both runtime gd flag words, and all five BOF boundaries copy their current
values into the packed writer context. The second flag word remains distinct
from the first; its source log-writer and fleet-link updates are preserved.
The generated-log writer now reads the shared session player.

The first production queue's saved flag snapshot now preserves unrelated bits
while replacing only planet research-exemption bit 5, and publishes that word
back to runtime state. FinishProduction receives the actual coordinator AI flag
rather than inferring it from player personality flags.

Also bound source ChangeMainObjSel at the first production queue boundary,
matching original Turin AI's selection before InitProduction. The integrated
sequence now checks five selection calls with two actual planet loads: first
queue, fleet speed, basic queue, blocked-queue ETA, final queue. Same-planet
selections retain the source early-return behavior. First-queue catalog and
wider session transition provenance still need the final callback audit.

Verification: python3 -m unittest tests.test_satisfy_orders_turin_binding -q
**PASS**, 12.662s, both pinned seeded replays and 100 scout cases. Original body,
ETA, temporary ownership and continuous 57/39 RNG comparisons pass. Earlier
intermediate flag-only runs passed in 12.755s and 12.705s. No VM run.
**Gate 2: 4/14 packages remain (01,02,12,14).** No package closed.

## G2-01 first production catalog bound (2026-09-26)

The first queue previously selected the planet and initialized an empty packed
queue without executing InitProduction. Original Turin selects the planet and
calls InitProduction even when the selected queue is empty. Extracted the
existing current-state catalog builder into startup_build_catalog and reused
it at the first queue boundary. All three initialization passes now call source
stars_init_production with current ship/starbase eligibility, hull cargo limits,
planetary part lookup, warp equipment, factory/mine/defense limits, scanner,
terraforming, race and current session flags.

The first pass retains its catalog separately and checks that source
reconciliation returns the empty starting queue. Its packed global queue is
then filled by source AddItemToQueue and committed by the existing source
FinishProduction path. Original AddItemToQueue bypasses catalog capacity lookup
when the global queue is already initialized; the adapter preserves that
behavior. Later basic/final queue passes retain their separate temporary
ownership and counters. Final-pass acceptance now requires exactly one first
catalog initialization.

Verification:

    python3 -m unittest tests.test_ai_final_queue_source tests.test_satisfy_orders_turin_binding -q

**PASS 2/2**, 14.852s: ten source-service subtests and the integrated 100 scout
cases plus both complete-input seeded runs. Original body, ETA and 57/39 RNG
checks remain passing. An intermediate catalog-only integrated run passed in
12.714s. No VM run.

Optional-callback audit started: the repair_order callback is supplemental to
an existing direct stars_ai_repair_first_order call, so its absence alone is
not evidence of a skipped repair. Remaining work is the complete callback and
current-state input census, plus session transitions; no package is closed.
**Gate 2: 4/14 packages remain (01,02,12,14).**

## G2-01 callback census and runtime planet/enemy inputs (2026-09-26)

Created `analysis/compat/satisfy-orders-e3-ai-callback-audit.md`, listing every
Turin callback and the top-level DoAiTurn boundaries. It distinguishes bound
services, direct implementations with optional hooks, rejected unsupported
actions, and exclusions still needing proof. Four closure tasks remain within
G2-01: production-policy reachability, planner/repair/input census, AI-to-host
session transitions, and the integrated closure report. These do not change
the 14-package accounting.

Replaced the runtime planet's literal visibility value and manual partial copy
with pack_loaded_planet -> stars_turin_planet_from_packed. Loaded concentrations
and visibility now reach the semantic planet directly. Production-slot admission
continues to derive from loaded starbase presence and population >=200.

Replaced the scout adapter's default-zero enemy count with source
stars_enum_calc_enemy_fleet (original FEnumCalcEnemyFleets owner predicate) over
the current compacted fleet table. Current IDs and presence are validated;
newly eligible enemy candidates fail explicitly instead of accepting the scout
helper's successful no-action return. The pinned pair has no such candidates.

Verification: python3 -m unittest tests.test_satisfy_orders_turin_binding -q
**PASS**, 12.920s, both complete-input seeded replays and 100 scout cases;
existing original body, ETA, RNG and ownership checks remain passing.
**Gate 2: 4/14 packages remain (01,02,12,14).** No VM run or package closure.

## G2-01 production-policy inputs activated (2026-09-26)

The first production adapter now enables the cargo, colony, minelayer,
support and role-design policy paths with current inputs. Technology and
planet count come from the loaded M2 player; AI scale comes from the live AI
buffer. Design eligibility, existing/built counts and hull costs come from
the current design bank. The cargo count is retained from the source
CheckAiShdefStatus result at its original pre-maintenance boundary.

Previously these paths could be bypassed by zero-initialized validity flags.
Both pinned seeded cases now pass with those policies enabled. A newly
selected combat/transport production role explicitly fails until its full
resource/queue budget adapter is connected; no zero budget is accepted as
proof that such production should be skipped. Existing coordinator guards
also reject newly eligible minelayer/support callbacks that are unbound.

Validation: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passes (12.829s), including the 100 scout cases and both seeded complete-input
runs. This removes a concrete runtime-input omission. The original production
predicate/order audit remains open, including the colony condition's
previously visited enemy-planet count. Planner/repair and AI-to-host lifecycle
checks also remain. **Gate 2: 4/14 packages remain (01,02,12,14).** No VM run.

## G2-01 enemy-planet colony-production condition restored (2026-09-26)

Original `DoTurinDroneAiTurn` (`ai.c` 1717 and 1915–1921) queues four
slot-1 ships when either the first-pass viable unowned count or the
second-pass viable enemy count is nonzero, subject to the current existing
ship count. The reconstructed coordinator previously used only the former.
It now counts viable enemy planets as encountered, invokes the planet-option
service, and publishes AI marker bytes 10 and 3. Packed planet flags are
retained by the reader for the original bit-9 marker. Missing planet-option
services on this reached path reject instead of supplying an answer.

The focused probe verifies enemy-before-owned versus enemy-after-owned
ordering, positive versus zero option value, marker publication and counter
reset between runs. All 41 focused checks pass. The packed-service and
complete-input binding tests passed in the three-module run; that run's only
failure was the Python wrapper's obsolete expected check count (32), now
updated to 41 and rerun successfully. Both pinned seeded cases remain passing.
This fixes a source omission without claiming general fleet-planner or
session-lifecycle completion. **Gate 2: 4/14 packages remain (01,02,12,14).**

## G2-01 bounded fleet dispatch guards (2026-09-26)

The integrated planner now checks the original second-pass dispatch in
`ai.c:2079` onward before using local transport or scout adapters. Fleets
with at least two orders retain the original no-dispatch result. Reached
order-repair, cargo/support-role routing and obsolete high-tech scout scrap
paths reject until their full effects are connected. A transport must produce
the bound transport action; a scout must produce the bound move action.
Unarmed/no-target scout fallbacks can no longer silently succeed through the
bounded helper. Slot-0 obsolete-scout gating uses loaded technology and current
hull ID (original `ihuldefScout = 4`).

The strict integrated test passes (12.643s), including 100 scout cases and
both pinned seeded inputs. This establishes that the pinned route reaches the
bound actions under explicit role/order guards. It does not close the wider
planner census: the original first fleet pass (`ai.c:1943` onward), including
attack-list linkage, bit-15 clearing and destination-based order cleanup,
still needs its current-state effects/exclusions accounted for. The shared
AI-to-host lifecycle audit also remains. **Gate 2: 4/14 packages remain.**

## G2-01 first fleet pass and loaded order count (2026-09-26)

The coordinator now offers the original first-fleet-pass boundary after planet
production and before action dispatch. The integrated adapter traverses the
current surviving AI fleets, uses source FIsTurinDroneAiAttack with current
hull IDs to build the reverse attack list, and clears bit 15 in both raw and
lookup fleet state. Destination checks use live orders, planet owners and AI
markers. Newly eligible miner destination changes, foreign-target drops or
order clearing explicitly reject until their services are connected. Enemy
list consumers remain outside this no-enemy bounded adapter.

This exposed a genuine loader omission: the typed AI fleet `cord` was zero
although its packed order count was loaded. It now receives the loaded order
count. The first pass must visit all three surviving AI fleets before routing.
The strict integrated regression passes in 12.869s with that assertion, both
seeded complete inputs, existing record/RNG checks and all 100 scout cases.
Report: `analysis/compat/satisfy-orders-e3-first-fleet-pass-test.txt`.

Initial attempts failed on a const qualifier and then the missing order count;
neither is accepted evidence. The final report is passing. The wider planner
and AI-to-host lifecycle audit remains open. **Gate 2: 4/14 packages remain
(01,02,12,14).** No VM run.

## G2-01 host-entry destruction boundary (2026-09-26)

Original `FGenerateTurn` (`turn.c:91`) calls DestroyCurGame before the host
load; the reconstructed coordinator had no corresponding boundary. Added an
appended `destroy_game` stage to StarsGenerateTurnOps and invoke it before
load progress/load_game. The source ordering probe now requires destruction
before loading in both successful generation and failed player-log paths.

`python3 -m unittest tests.test_generate_turn_source tests.test_satisfy_orders_turin_binding -q`
passes (2 tests, 12.969s). The integrated adapter does not yet bind this new
callback, so this is a prerequisite rather than lifecycle closure.

Concrete remaining integration: release/reset the AI live planet/fleet/design/
order/message state and selection; reload host state and per-player views;
then replay the closed generated X2. Preserve generated file bytes and
observation snapshots separately from live state. Current startup_turn_load
only reloads players/planets and clears messages; startup_replay_memory_logs
resets orders and replays mutations, but these operations do not establish
DestroyCurGame equivalence. Existing AI-entry reset callbacks intentionally
reject post-AI use and cannot simply be reused. **Gate 2: 4/14 packages remain.**

## G2-01 host design/queue reload independence (2026-09-26)

Host load now clears the live ship/starbase banks, invalidates their loaded
flag, and removes the committed production queue and selected queue planet.
The existing source-input design loader must reload its bank before the
closed generated X2 is replayed. Production-log replay now resolves the
logged planet against current host planet ID/ownership and establishes the
queue target from that record; it no longer requires a surviving AI queue
planet as an input. All queues and four generated starbases are consequently
reconstructed without retaining those AI live buffers.

The integrated regression passes (12.816s), including both pinned seeded
cases, full existing body/RNG assertions and 100 scout cases. Report:
`analysis/compat/satisfy-orders-e3-host-reload-test.txt`. Host entry also checks
that temporary AI allocations are released and history/generated-log output
exists before discarding live state.

This completes only the design/queue portion of host reload independence.
The newly added coordinator destroy callback remains unbound pending complete
fleet/order/selection/session cleanup; the partial reset is currently inside
startup_turn_load. Do not count it as full DestroyCurGame equivalence.
**Gate 2: 4/14 packages remain (01,02,12,14).**

## G2-01 raw fleet/order reload and merge provenance (2026-09-26)

Host entry now clears raw fleet bodies, presence flags and packed orders.
A shared pack_input_fleet adapter materializes decoded M2 starting records
for both initial AI loading and host-side player state; generated-X2 replay
restores changed orders and merged cargo/ships. The merge replay now derives
its two starting fleet bodies directly from decoded input records rather
than consuming startup_merge_inputs captured by the earlier AI run.

The replay robustness check deliberately overwrites that old merge snapshot
with 0xA5 alongside the discarded live/decoded log buffers. Both pinned runs
still pass, including exact existing output-body/RNG assertions and all 100
scout cases. Final report: `satisfy-orders-e3-fleet-reload-test.txt`. `git diff --check`
passes.

Remaining host lifecycle work includes the lookup/typed fleet table (currently
also used by AI observation assertions), selection and session flags/counts,
and binding the complete reset to the coordinator destroy callback. Raw
fleet/order independence alone does not close that audit.
**Gate 2: 4/14 packages remain (01,02,12,14).**

## G2-01 host destruction bound and fleet-table reload (2026-09-26)

The integrated generator now calls source DestroyCurGame through its new
pre-load callback. All nine heap-reset callbacks are checked in original
order. The adapter clears live planet/supply state, raw and typed fleets,
lookup storage/pointers, design banks, orders and production queues; resets
messages and selection; and publishes source-cleared session flags. Released
AI buffers remain diagnostic snapshots with runtime planet-table aliases
removed. Closed generated files and recorded observations remain available.
Host load requires one completed destruction, nine resets and empty live
fleet/design/queue state, then reloads decoded inputs and session counts.

Merge replay now publishes ship counts, deleted typed-fleet ownership,
lookup storage and the compacted pointer list from the source merge result.
This eliminates reliance on AI's surviving lookup table. Clearing supply
state exposed a cargo replay omission: it restored minerals but inherited
fleet flags. It now loads those flags from the same decoded fleet input.

Final verification: generate-turn source ordering and complete-input binding
tests both pass (12.832s); both pinned runs and the 100 scout cases pass.
Report: `satisfy-orders-e3-host-destroy-test.txt`. Earlier failed reset runs
are diagnostic only. `git diff --check` passes.

G2-01 still requires a final field/consumer audit (including remaining session
identity/load fields and temporary aliases) and consolidated callback closure
report. The newly bound destruction is not, on its own, proof of the entire
AI lifecycle. **Gate 2: 4/14 packages remain (01,02,12,14).** No VM run.

## G2-01 selection and session identity publication (2026-09-26)

Each source ChangeMainObjSel call now publishes kind, full kind, ID, point,
selected planet and active waypoint to the same session consumed by the later
source DestroyCurGame. The loaded game ID is also published at the AI load
boundary. Host destruction invokes its save-selection callback before clearing
these fields. The callback checks that session and live selection agree and
records selected kind/ID, caller player and full game ID; it applies the
original selected-object mask (`ini.wFlags & 0xfe1f | kind << 5`) to its
tracked INI selection word. Both pinned cases assert that one nonempty
selection is saved with the loaded full game ID, then cleared by destruction.

The integrated strict-C89 regression passes (12.490s), including both seeded
inputs, exact existing output/RNG assertions and 100 scout cases. This fixes a
previously silent session-selection disconnect. The saved INI word is a local
state observation, not a claim that complete INI persistence has been proven.
Session salt/tick inputs and full callback/consumer coverage still require
final audit before G2-01 closes. **Gate 2: 4/14 packages remain.** No VM run.

## G2-01 closed: AI callbacks and host transition (2026-09-26)

The consolidated [AI closure audit](satisfy-orders-e3-ai-callback-audit.md)
accounts for current-input production policy, all reached fleet paths,
DestroyCurGame and load transitions, saved selection/session identity,
complete generated X2/H2 effects and exact RNG assertions in both pinned
cases. Source ordering, AI lifecycle, minelayer and packed service probes pass.
The integrated control/action run and 100 scout cases pass after the final
policy and session assertions. G2-01 is closed for this pinned pair; defects
found by G2-14 can reopen it.

**Gate 2: 3/14 packages remain (02,12,14).** No VM run.

## G2-02 closed: order/log integration (2026-09-26)

The [closure audit](satisfy-orders-e3-orders-audit.md) now accounts for
source log loading, serial and preparation stages, waypoint validation and all
four order passes over the current fleet table. Merge replay publishes ship
counts, cargo and deleted state into the host world; the integrated check
compares both merged host records to raw/presence state before downstream
services run. Source task-5, passive, research, thing, drop, transfer and
battle paths are accounted for in original order. The [integrated report]
(satisfy-orders-e3-orders-integration-test.txt) passes 1/1 (12.629 s); nine
focused four-pass/X2/coordinator tests pass (59.145 s). `git diff --check`
passes. G2-14 can reopen a newly discovered defect.

**Gate 2: 2/14 packages remain (12,14).** No VM run.
