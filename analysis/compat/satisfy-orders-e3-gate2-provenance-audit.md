# SO-E3 Gate 2 — final source/state provenance audit

Status: **closed for pinned `vmctl-sye3k0` control and `vmctl-sye3k1`
action, 2026-09-26**. This closes complete selector-1 turn execution and
current-state services. It does not claim complete generated HST/M1/M2
streams or every-record file comparison (SO-E3 Gates 3–4).

## Input and output boundary

The [Gate 1 audit](satisfy-orders-e3-gate1-audit.md) verifies both complete
starting XY/HST/M1/M2 sets, 274 saved-file frames, all runtime destinations,
and absent starting X2/H2. `InputReader.load_run` verifies sizes/hashes before
transferring the complete aggregate to the strict-C89 probe. Generated X2/H2
come from the current Turin services, and host reload/replay consumes that X2.
The test selects control/action only to select its pinned *input* and expected
assertion arrays. The older `syorig1/syact3` experiment's unresolved guest
directory listing is historical supporting evidence and is not either pinned
input set.

Retained original turn-1 bodies, IDs, messages, and draw values occur in
assertions and diagnostics. The final review removed three runtime uses:

1. Host planet writing now enumerates IDs from the live 32-planet table; the
   original output ID list is checked only after selection.
2. Owner-1 task-5 learning now reads the live host player technology (as the
   owner-0 path already did) and publishes both services' resulting levels to
   the shared player records.
3. Turin local supply now passes the source `ai.c:2208` amount `0x19` through
   `STARS_TURIN_LOCAL_SUPPLY_COLONISTS`, not a macro calculated from the
   original output's cargo delta. The old output delta remains an assertion in
   the fixture generator.

The output BOF game ID now comes from the loaded runtime game record; the
retained ID verifies it. Effective startup tick and five writer clock
residues are separately documented recovered **external clock inputs**, not
turn-result fields; their limitations and exact RNG handoffs are in the
[G2-13 RNG audit](satisfy-orders-e3-rng-audit.md). Original executable
part/hull/race tables and registered disk identity are immutable environmental
inputs, not generated turn answers. Nonseeded scout probes retain explicit
witnesses; both seeded control/action executions take the shared main RNG
branch and audit its entire 57/39-call sequence.

## Coordinator callback census

The integrated `run_generate_turn` binds every game-affecting stage reached by
the original pinned path. `write_player_file` is an optional reconstruction
hook with no corresponding extra original `FGenerateTurn` call; original
output goes through `FWriteDataFile`. Progress is UI only. The callback groups
below are checked in source order with current state published to downstream
consumers; the linked package audits give the branch and field details.

| Stage callbacks | Runtime source and result |
|---|---|
| `destroy_game`, `load_game`, `initialize_turn`, `random_value` | AI heaps are released, HST/M1/M2 and generated-X2 state reload independently, `0x0202` turn flags enter scope, and the single seeded RNG shuffles both players. The [AI audit](satisfy-orders-e3-ai-callback-audit.md) checks stale-state poisoning and reload; [RNG audit](satisfy-orders-e3-rng-audit.md) checks every draw. |
| `load_player_log`, `validate_player_logs`, `validate_waypoints` | Both logs load in shuffled order; generated X2 mutations update shared fleets, orders, cargo, designs, queues and players. Source serial/design/fleet preparation and waypoint services run on the merged current table. The [order/log audit](satisfy-orders-e3-orders-audit.md) requires their visit counts and effects. |
| `do_orders` modes 0/1 | Each mode executes two original ordered passes, including task-5 scrap, passive fleets, research, drop/transfer consumers, thing interactions and combat gates. All reached effects publish to shared world; newly eligible unbound effects reject. See [order/log audit](satisfy-orders-e3-orders-audit.md). |
| `unmark_mine_fields`, `move_things` modes 0/1, `move_fleets`, `thing_decay` | Full current thing/fleet tables feed source services. All four things are wormholes, proving packet/minefield branches inapplicable. Surviving fleets move with live orders, capacity and fuel; arrival/waypoint effects publish. See [movement audit](satisfy-orders-e3-movement-audit.md) and the G2-06 evidence in the [tracker](satisfy-orders-e3-gate2-tracker.md). |
| `breed_colonists`, `produce` | Loaded race traits prove the breeding exclusion. Full-map mining/production, two applicable queues, partial build, growth, research, and event dispatch run against current planet/player state and shared RNG. Original event gates miss in both runs; unexpected nested eligibility rejects. See [production audit](satisfy-orders-e3-production-audit.md) and G2-05 evidence. |
| `fuel_fleets`, `sweep_for_mines`, `heal_ships`, `auto_terraform`, `remote_terraforming` | All survivors and 32 planets are visited. Current designs and original part tables determine fuel/healing/sweep capability; loaded races and zero capability prove terraforming exclusions. Mutated fuel/damage state reaches writers. See G2-06–09 evidence in the [tracker](satisfy-orders-e3-gate2-tracker.md). |
| `spank_cheaters`, second `validate_waypoints`, `update_guesses` | Current player/turn flags prove cheater messages absent. Waypoint validation repeats at the source position. All 32 current planets feed guesses, with loaded race/tech/defense inputs and the shared RNG. See G2-10 and G2-13 evidence. |
| `mark_backup`, `update_scores`, `prepare_output_files`, `write_data_file`, `cleanup` | Source scores use the resulting world and publish ranks. Backup/log moves, visibility, message preparation, each file writer and three closes run in original order. Output BOFs and live player/design/planet/fleet/thing records are individually compared. Temporary flags and memory-log cursor clear at exit. See [writer lifecycle audit](satisfy-orders-e3-writer-lifecycle-audit.md) and G2-11–13 evidence. |

The source-specific AI callback census is in the [G2-01 audit]
(satisfy-orders-e3-ai-callback-audit.md); nested callbacks for orders,
production, movement, events and writers are in their package audits. Each
inapplicable branch is decided from loaded/current fields or fails if newly
eligible. No reached game-state stage uses an unresolved runtime witness.

## Regression

The [Gate 2 regression report](satisfy-orders-e3-gate2-regression-test.txt)
records **36 tests: 35 passed, one expected skip**. The skip is original x86
Unicorn execution on macOS ARM; the independent wire input audit and strict-C89
integrated control/action test pass. The integrated test executes both pinned
seeded turns and all 100 scout cases; exact RNG, generated log, current state,
and individual original output body assertions pass. `git diff --check` passes.
No original executable or guest run was performed for this closure.
