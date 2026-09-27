# Phase 2 evidence checklist

**28 of 31 numbered evidence units remain open.** Numbers stay fixed; use
them when directing work. The 9 remaining procedure promotions are separate.
[TODO7.md](TODO7.md) is the acceptance ledger and
[tracking/reviews.json](tracking/reviews.json) holds the detailed review
inventory. A passing subprobe does not close a unit whose stated comparison
or live binding is still missing.

For each unit, retain the input fixture, host test, native VC1.52c build
status and logs where required, probe report, and decoded original output
used for a comparison. The **Needed to close** column states the gate;
existing support is summarized alongside it.

## Orders — `SatisfyOrders`

| # | ID | Evidence already recorded | Needed to close | Done |
|---:|---|---|---|:---:|
| 1 | SO-E1 | [Original cost oracle and corrected native 19/19 scrap probe](analysis/compat/satisfy-orders-task5-e1.md) | Complete: verified design costs, isolated 9/6/5 mineral return, message amount 20, and packed fleet/order retirement. The full-turn stockpile difference belongs to SO-E3. | [x] |
| 2 | SO-E2 | [Task-5 progress](analysis/compat/satisfy-orders-task5-e2-e3-progress.md); corrected [service-order native 30/30](runs/vmctl-soe2h/MANIFEST.JSON) and [resource native 75/75](runs/vmctl-soe2g/MANIFEST.JSON). A pinned [original race-bit-5 resource turn](analysis/compat/satisfy-orders-racebit5-original-oracle.json) proves 10/7/6 mineral return, resource messages 0x5D/0x143, and fleet retirement. The [verified-input resource native probe](runs/vmctl-soe2l/MANIFEST.JSON) binds original player/planet/design scalars and reconstructed `CResourcesAtPlanet` for 22/22 checks. The [integrated original-part SHDEF repricing native probe](runs/vmctl-soe2n/MANIFEST.JSON) uses recovered executable part records with reconstructed `FLookupPart` and `GetTruePartCost` in task 5 for 67/67 checks. A natural [original deep-space control/action pair](analysis/compat/satisfy-orders-smoke-deepspace-original-oracle.json) proves fleet retirement and a 5/0/2 salvage thing; a [native 21/21](runs/vmctl-soe2q/MANIFEST.JSON) matches all 18 saved thing bytes after `ThingDecay`, the 0x5B callback, and retirement. A paired [original human-message turn](analysis/compat/satisfy-orders-smoke-human-message-original-oracle.json) exposes the suppressed 0x5B frame. The slot-verified v2 [positive `0x13D` pair](runs/vmctl-sytech-correct-v2-action/RUN.JSON) raises player-0 tech index 2 from 3 to 4. The matched v3 [task-0 control](runs/vmctl-sytech-correct-v3-seed1-control/RUN.JSON) and [task-5 negative action](runs/vmctl-sytech-correct-v3-seed1-action/RUN.JSON) share H2 SHA-256 `69dcea347053bfc511760f793895f98447eff68e17ad02fd018c3f559112bb89`; control retains owner-1 fleet 0 with no point-tech message, action retires it and emits `0x13C` with exact args `[512,1147,0,10]`. Both retain raw HST/M1/M2 returns and record slot-0 Battleship design 41. Five v2 shared-H2 seed pairs are also retained: seeds 1 and 2 produced no tech result; seeds 3–5 produced `0x13D` and raised player-0 tech indices 0, 4, and 2 from 3 to 4. A matched [v4 race-bit-C high-offer original pair](analysis/compat/satisfy-orders-reprice-v4-bitc-original-oracle.json) now verifies the actual player-1 bit-C input, slot-0 hull-9 eleven-part SHDEF, shared H2, fleet retirement, raw `0x13D` frame, and player-0 tech 0 increase; see the [control](runs/vmctl-sytech-reprice-v4-bitc-control/RUN.JSON) and [action](runs/vmctl-sytech-reprice-v4-bitc-action/RUN.JSON) returns; exact-input current-source repricing passes host/native 74/74 in the [native report](runs/vmctl-soe2s/SOE2SRES.TXT) and [run manifest](runs/vmctl-soe2s/MANIFEST.JSON), documented in the [closure note](analysis/compat/satisfy-orders-task5-v4-current-source-native.md). | Complete: the exact v4 high-offer SHDEF, player-1 race/technology state, planet inputs, and original part records pass current-source task-5 host and VC1.52c native checks. Matched original task-0/task-5 `0x13D` and `0x13C` outcomes also pass with slot-verified definitions, shared H2, raw returns, decoded tech flags/levels, and exact message arguments. `0x13E/0x13F` remain a separate extension if closing the planet-target resource branch. The [field-level differential](analysis/compat/satisfy-orders-task5-field-differential.md) compares original service mutations, callback order, fleet removal, and message variants. | [x] |
| 3 | SO-E3 | [Original action and same-input oracle](analysis/compat/satisfy-orders-task5-e2-e3-progress.md); fresh [four-pass VC1.52c report](runs/vmctl-soe3j/SOE3RES.TXT) matches the strict-C89 host report after newline normalization. The [isolated original pair](runs/vmctl-sye3k-pair-r1/PAIR.JSON) and [audit test](tests/test_satisfy_orders_fresh_ai_pair.py) pin absent starting `.X2`/`.H2`, identical owner-1 M2 inputs, and the task-0→task-5 HST mutation; selector word `0x2797` dispatches to Turin. The production [M2 input fleet reader slice](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-m2-input-fleet-reading-2026-09-25) loads all four retained owner-1 type-16/type-20 fleet/order pairs through `stars_read_fleet` before Turin AI mutates their state. The [owner-0 HST input reader slice](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-owner-0-hst-fleet-reading-2026-09-25) uses the same production reader for six turn-0 fleet/order records and passes the decoded compact orders to movement. The host-only [Turin binding probe](tests/test_satisfy_orders_turin_binding.py) derives the unknown-planet candidate set from retained player mode, M2 planet ownership, and the original marker reset; it runs selector 1 through the reconstructed Turin turn, nearest-unknown target selection, packed MOVE write, and movement, reproducing fleet 0's point 26, warp 5, `(1313,1262)`, and fuel 298. Its production wormhole finder tests all 100 possible `Random(100)` results against retained HST entries; every value leaves fleet 0 at target 26. The original output BOF directly exposes one later `Random(8)` result: `vmctl-syorig1` and the same-input clean `vmctl-sye3k0` record 0 and 1, while task-5 action `vmctl-sye3k1` records 4. Since the identical clean inputs produce different values, the action/control outputs do not share an RNG stream and their full-turn differences cannot all be attributed to the task edit. The oracle's static callgraph audit counts 10 RNG-bearing functions reachable from Turin and 28 from `FGenerateTurn` (over-approximate guarded sites); it distinguishes the two-call per-player shuffle from `Random(8)` and conditional earlier draws. The pinned `CLEAN.XY` type-7 game record gives `cPlayer=2`, fixing the shuffle arguments at `Random(2)` and `Random(1)`; the latter advances both seeds despite returning zero. The retained streams pin five successful `Random(2000)` writer calls: two from the AI X2/H2 files and three from FGenerateTurn's M1/M2/HST files; M1's turn-0 BOF is inherited. The full-map `MineMinerals` stage has only owned planets 10 and 24; each has ten operating mines, with all three mineral products carrying nonzero remainders, so it consumes six `Random(100)` draws when rounding is enabled. The retained player-1 `InitRandomPlanetList` has one owned planet, so its `vclpplAi - 1` shuffle consumes zero draws. The selector-1 Turin binding now derives fleet 1’s retained owned-planet transport decision through `stars_turin_plan_local_transport`, using decoded current planet ownership/population, then runs production `stars_xfer_ai_supply` and `stars_change_cargo`; it reproduces planet/fleet colonists 250/0 → 225/25 with the original query/add/debit call order. It then binds retained player/planet environment and growth data through `stars_planet_desirability`, `stars_calc_planet_max_pop`, and `stars_change_population`: +31 growth with fraction 50 reaches the original 256 colonists. The fleet-1 transport choice and 25-unit capacity are derived for this retained case: the planner binds the original owned, inhabited current-planet branch, and capacity comes from retained design/hull/cargo fields through reconstructed `stars_wt_max_ship_design_stat`, `stars_get_fleet_stat`, and `stars_get_cargo_free`. Complete production and the wider full-turn differential remain open. The pinned original SHDEF/part audit and source trace explain both missing fleets: `MergeAllShdefs(0xC)` deletes fleet 3 into fleet 2, Turin writes task 5 to the turn-0 survivor, and pre-movement `SatisfyOrders(1)` scraps it. The updated strict-C89 [production integration probe](tests/satisfy_orders_fresh_ai_merge_scrap_probe.c) now obtains task 5 from reconstructed `stars_do_turin_drone_ai_turn` via `stars_ai_set_first_order_kind`; it no longer seeds the action order by hand. It reads the retained M2 type-26 SHDEF and type-13 planet through production readers, resolves original EXE hull/part rows, and runs production merge, Turin assignment, and packed task-5 scrap. It reproduces fleet 3 merging into fleet 2, then fleet 2 retirement (leaving original output IDs 0/1), design cost [123,41,0,12], planet-24 stock [532,956,500,250] → [597,956,519,250], and messages 0x5A/0x141 with first-message mineral total 84. Technology marking now calls production `stars_mark_techs_seen_shdef` with technology bytes from the pinned EXE part rows. The packed scrap callback now supplies the recovered action `Random(100)=43` at main-stream draw 11 to production `stars_itech_learn_a_tech`. The merge callback also invokes production `stars_fleet_transfer_cargo_balance` with retained fleet records and original-EXE hull/design weight data; it conserves all five cargo lanes, leaves the deleted fleet empty, and gives fleet 2 the observed 420 fuel. Both the combined route replay and separate task-5 replay now call production cargo balancing, fleet lookup/deletion, and report invalidation. Raw message serialization remains a fixture service; scoped callbacks use exact recovered RNG values or explicit witnesses, while an end-to-end shared-stream adapter remains open. The fresh-pair audit now bounds the retained `UpdateGuesses` outputs to four possible random values per owned planet and run, proves all seven `SpankTheCheaters` random sites skip at turn 0, and bounds turn-zero `RandomEvents` to 3–10 calls per run (three gates plus conditional planet and climate draws). The seven-module focused host suite passes 38/38. This is still a bounded host merge/scrap slice, not a full-turn or native integration. A separate fresh StarDev VC1.52c [turn-zero miner task-write probe](runs/vmctl-turin-miner-task-e3-r1/MINER.TXT) passes 24/24 and matches its [host report](runs/vmctl-turin-miner-task-e3-r1/HOST.TXT); compile/link and process exit pass, with returned build artifacts in the [manifest](runs/vmctl-turin-miner-task-e3-r1/MANIFEST.JSON). At that earlier native-run checkpoint, this validated task assignment only; native merge/scrap binding and the full-turn comparison remained open. Fleet weapon capability now uses reconstructed `stars_fleet_might_have_teeth` over retained M2 design IDs joined to original EXE hull-slot records. Ideal warp now uses reconstructed `stars_find_ideal_warp` with retained design 0/engine 1 and the original EXE fuel-use row. The combined strict-C89 route/merge/scrap probe passes 81/81 for each of 100 injected outer `Random(100)` values. It calls production `stars_merge_two_fleets` on runtime fleet records derived from the retained M2, then production readers for the retained SHDEF and planet, reconstructed Turin task assignment, and `stars_satisfy_orders_packed(pass=1)` for the merged task-5 fleet. It checks the 2+2 ship merge, 420 fuel, fleet-3 deletion, fleet-2 retirement, planet-24 minerals `[532,956,500,250]` → `[597,956,519,250]`, and messages `0x5A/0x141` with first-message total 84. The packed task-5 replay now supplies recovered action draw 11=43 to production `stars_itech_learn_a_tech`. That replay also calls production `stars_fleet_transfer_cargo_balance` using retained fleet snapshots and original EXE hull/design weights; all five cargo lanes conserve, deleted fleet cargo is zero, and the merged destination retains 420 fuel. The combined selector-1 route probe now uses production `stars_fleet_transfer_cargo_balance`, `stars_lookup_fleet`, `stars_delete_fleet`, and `stars_invalidate_report`; it conserves all five cargo lanes, empties fleet 3, and checks table compaction and report invalidation. Raw message serialization, an integrated shared RNG adapter, and the full-turn comparison remain open. AI-planet status bytes for this retained route are now generated from the original initialization rules and M2 state; the outer random result remains a fixture input. The supplied screenshot shows Stars! rejecting `C:\SOTC\CLEAN.X2`; the [2026-09-24 read-back](runs/diagnostic-soe3-x2-20260924/CLEAN.X2) and [live 2026-09-25 read-back](runs/vmctl-e3-livecheck-20260925/READBACK.JSON) both match the archived 355-byte file. The guest input manifest excludes X2, but the current listing contains it and no pre-run listing exists, so its starting presence remains unknown. The current [desktop capture](runs/vmctl-e3-livecheck-20260925/GUEST-SCREEN.png) shows the connected agent and Explorer with no Stars! window. Keep this case separate from the isolated fresh-pair `.X2` files. The retained source/XY/RNG audits now pin at least 37 calls per run, including four starbase-name draws in Turin AI’s `IroEnsureAi` prelude (accepted `Random(13)` values by creation order: control `[4,9,7,0]`, action `[9,5,6,10]`; both runs have zero retries, and exhaustive matching recovers effective low-14-bit `Randomize2` tick residues 13,747/9,953, seed words `[19,279]`/`[443,67]`, main-stream draw positions 1–4, and scout `Random(100)` values 99/4 at positions 5/5), followed by player shuffle, two-planet mining, 3–10 `RandomEvents` calls, four post-production wormhole loops (at least three calls each), two output-bounded `UpdateGuesses` draws, saved flag, and five BOFs. A new type-table audit proves that `MoveThings(0)`, `FTravelThroughMineFields`, and the type-3 `DoThingInteractions` random paths are skipped for the retained four-wormhole input. A new retained-input regression narrows fleet 0’s scout subpath to exactly one direct `Random(100)`, proves the conditional wormhole/fallback draws are skipped, and audits both earlier `FinishProduction` calls: `fAi = 1` suppresses their random-bearing `FillPlanetProdLB` path. It also proves the earlier queue `Random(4)` and slot-12-gated `Random(3)` sites, plus nested draws, are skipped by retained tech/marker values. The scout values are control 99/action 4 at draw 5; draw 6 is the retained starbase-upgrade roll, with values 7/75 that both miss its <6 gate. The X2 and H2 `WriteBOF` calls then consume draws 7/8 with `Random(2000)` values 1240/1434 and 1922/267. The FGenerateTurn shuffle then consumes draws 9/10 with `Random(2), Random(1)` values 0/0 and 1/0. At that checkpoint, the later full-turn draw sequence remained unknown. A host-only strict-C89 [integrated replay](tests/test_satisfy_orders_fresh_ai_pair.py) now runs merge → Turin task assignment/queue → `DoOrders` passes 1–2 and packed task-5 scrap → mining/`Produce` → passes 3–4 in one control/action probe; bounded mining witnesses reproduce both retained planet-24 endpoints and the two-item queue. The current seven-module focused host suite passes 38/38, including the full-record wormhole replay, source-bound player-log shuffle, output-bounded `UpdateGuesses` draw regression, turn-0 `SpankTheCheaters` gate regression, the 3–10-call turn-zero random-event bound, and the new zero-draw thing-table audit for pre-production minefield paths. At that checkpoint, full-turn service comparison and the complete RNG sequence remained open; the effective low-14-bit startup residues and first ten draw positions are recovered: name draws 1–4, scout values 99/4 at draw 5, starbase-upgrade values 7/75 at draw 6, X2/H2 writer values 1240/1434 and 1922/267 at draws 7/8, and player-shuffle values 0/0 and 1/0 at draws 9/10. The [E3 oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md) records the bounds and remaining gaps. The new [control/action output-record census](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#retained-controlaction-output-record-census-2026-09-24) pins changed body counts across HST/M1/M2/H2, decodes owner-0 player fleet count 6→5 and score-history `_score.rgcsh[1]` 2→1; it remains a partial field check, not a complete production-turn comparison.  The recovered original trace establishes all 57 control and 39 action main-stream draws. A current-source host continuation now passes production `stars_update_guesses` and three `stars_write_bof` calls against retained guess words, RNG values, saved flags, and BOF salt/player/type fields; the focused eight-module suite passed 41/41, and the selector-1 probe plus retained RNG-trace regression pass 2/2 with exact BOF records and per-writer tick-residue cross-checks. Production growth now derives owner-0 planet 10 from retained HST inputs through desirability, maximum-population, and population-change services, then feeds it into UpdateGuesses and CMaxOperableDefenses: 250/fraction 0 becomes 287/fraction 50. Planet 24 also reaches its original 256 through production transfer and growth and feeds that value into the same services. Production best-defense lookup, operable-defense limits, and CalcPctSurvive reproduce both 90% survival results from retained HST player/planet and pinned EXE part data; turn-1 guess fields remain retained inputs. All three turn-1 BOF records compare byte-for-byte, but complete HST/M1/M2 files are not serialized or compared. See the [seeded pipeline evidence](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#seeded-selector-1-startup-to-post-production-wormhole-rng-pipeline-2026-09-25). The earlier fixture-budget and fCheater hypotheses are superseded. The current selector derives planet 24's pre-research pool as 32 through production `stars_cresources_at_planet` from retained HST planet/player inputs, then applies the retained 15% research deduction (runtime flags 0; planet bit 23 clear) to reach 28. The selector no longer injects the 28 witness. The production/resource, selector-binding, task-5/Produce, and four-pass delta suite passed 10/10; this closes the planet-24 resource subissue, not SO-E3. | All four grouped gates are closed for the pinned control/action pair. The [Gate 1 input audit](analysis/compat/satisfy-orders-e3-gate1-audit.md) verifies complete source-initialized inputs; the [Gate 2 provenance audit](analysis/compat/satisfy-orders-e3-gate2-provenance-audit.md) verifies all reached current-source turn services and one seeded main RNG in original call order. The [Gates 3–4 output audit](analysis/compat/satisfy-orders-e3-gates3-4-audit.md) verifies complete turn-1 HST/M1/M2 serialization for both runs: all 274 turn-1 records decode and match their original counterparts, and all six complete encrypted files match byte for byte. One retained regression covers every output record. SO-E3 has 0 of 4 grouped gates remaining. | [x] |

The separate rejected `C:\SOTC\CLEAN.X2` dialog is retained as a
[user-supplied screenshot](runs/diagnostic-soe3-x2-20260924/stars-corrupt-player-log.png)
(SHA-256 `254a4c6baf7cc560dde3976da7e16d6bb26396cae43a26c0749463b1ed4db780`).
It documents the rejection only; the missing pre-run directory listing
leaves the file's starting presence unresolved. It is separate from the
fresh isolated pair and does not close SO-E3.
The screenshot attached again on 2026-09-25 has the same SHA-256, confirming
it is this already archived rejection image rather than new guest-state
evidence.
The rejected `.X2` and the paired control/action `BACKUP/CLEAN.X2` files all
decode to the same 13 record kinds and share BOF game ID, version, turn,
player, and flags. Their salts differ; the rejected file differs in the BOF,
type-9, and four type-27 bodies. This is a framing/header comparison only: it
does not reveal the rejected field or establish that Stars! accepted either
reference file. Details and exact lengths are in the [SO-E3 oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.json).
The selector-1 regression was rerun on 2026-09-25 and passes 1/1 (8.1
seconds). It now derives the 90% survival inputs through production defense
lookup, operable-defense limits, and `stars_calc_pct_survive`; complete-turn
serialization and comparison remain the SO-E3 closure gate.
The retained selector-1 regression was rerun on 2026-09-25 and passed 1/1
(8.3 seconds); this validates its route and seeded control/action cases, not
the complete HST/M1/M2 turn required to close SO-E3.

The retained type-26 output census is now field-decoded against `_rtshdef`:
design slot 0 changes only `cExist` (1→0), while AI design slots 17–20 keep
their headers and components and differ only in compressed-name tails. The
codec regression decodes control names `Cube`, `Monolith`, `Guardian Angel`,
and `Hip to be Square`; action names are `Potato`, `Hip to be Square`,
`Sphere of Doom`, and `Rock of Gibraltar`. Each name re-encodes to the exact
retained bytes. Their changes across separate launches with identical starting
M2 inputs are consistent with launch-dependent RNG state, but do not recover
draw values or positions. The current-source turn still lacks full SHDEF and
AI-name output comparison.


Latest SO-E3 supporting replay: the strict-C89
`test_fresh_pair_owner0_action_runs_all_four_orders_passes` regression starts
from the isolated original task-0/task-5 pair and executes production
`DoOrders` passes 1–4 with `Produce` between passes 2 and 3. It matches owner-0
planet 10 in both retained turn-1 outputs: control `[476,902,383,287]`, action
`[484,908,388,287]`; the raw M1 messages (`0x3F` for control and
`0x5A,0x141,0x3F` for action); and surviving owner-0 fleets 1–5 field by field.
The separate bounded owner-1 planet-24 integration covers fleet merge, Turin
task assignment, task-5 scrap, four order passes, mining, and production.
Their mining `Random(100)` inputs are output-bounded witnesses, not recovered
original draws. The selector-1 route/merge/scrap probe sends both retained
owner-1 survivors through reconstructed `MoveFleets`: fleet 0 reaches point 26
with fuel 298; fleet 1 remains at point 24 with fuel 200. It passes 69/69
checks for each of 100 outer `Random(100)` witnesses.

The post-production `MoveThings(1)` replay now also matches all four complete
18-byte wormhole HST records in both retained runs, including their packed
destination updates. The four `PctWormholeMoves` chance fields are zero, so
every gate value selects local jitter. Output-derived `Random(25)` coordinate
witnesses are control `[[8,17],[9,0],[6,18],[11,13]]` and action
`[[15,13],[5,19],[16,23],[14,23]]`. They do not recover rejected retry counts
or global RNG positions. The [wormhole regression](tests/test_satisfy_orders_wormhole_move_e3.py)
replays these records against current source.

The reconstructed `stars_generate_turn` coordinator now uses the original
forward player-log shuffle and shared RNG callback. The retained two-player
game shape invokes limits `[2,1]`; the strict-C89 source probe verifies those
limits and shuffled loader order with injected witness draws. Its three-player
case also verifies `[3,2,1]`, and absent/out-of-range RNG values fail before
logs load. It asserts the complete coordinator callback sequence, including
both order passes, both movement modes, production, output writes, cleanup,
and the failed-log early return. The updated seven-module focused suite passes
32/32. This fixes one direct coordinator/RNG-accounting gap; actual pair draw
values, full-turn services, and complete output-state comparison remain
unresolved. See [shuffle regression](tests/test_generate_turn_source.py) and
[turn coordinator evidence](analysis/reconstruction/turn-generation.md).

The original `ValidateWaypoints` has four possible random draws, all within
its fleet-target waypoint branch. The new retained-order audit checks every
fleet/order pair in both fresh HST inputs and both turn-1 outputs: each has
`object_kind=1`, so no serialized retained order enters that branch. This
does not prove the transient order state after log replay or close the other
random-bearing turn services. See the [waypoint RNG gate](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md)
and its [regression](tests/test_satisfy_orders_fresh_ai_pair.py).

The retained player attribute at index 14 is `[9,1]` for owners 0 and 1 in
both fresh input HSTs. The original `BreedColonistsInTransit` scans fleets
only if any player has attribute value 4; its only `Random(3)` call is inside
that scan. The [retained-input regression](tests/test_satisfy_orders_fresh_ai_pair.py)
checks the player layout, original `GetRaceStat` access, and guarded draw.
This removes that service's direct draw for this pair, while other turn-stage
draws and the full state comparison remain open.

The same retained stat-14 values `[9,1]` exclude the `AutoTerraform` trait
gate, which must equal 3 before its planet scan and `Random(3/10/1000)` sites.
The [regression](tests/test_satisfy_orders_fresh_ai_pair.py) verifies the
player records and original source guards. Remaining turn-service RNG and
full output-state comparison stay open.

The seven-module focused suite previously passed 38/38. The latest fresh-pair
and wormhole modules pass 28/28, including the new seeded-stream regression. It
recovers all main-stream values and positions through the final turn-1 BOF
writers: 57 calls for control and 39 for action. The six mining draws match
independent stock/remainder bounds; event gates, first wormhole candidates,
UpdateGuesses, saved flags, and writer tick mixes also agree with retained
outputs. The current-source task-5 slice now consumes action gate draw 11=43
and planet-24 mining draws 16–18 `[38,26,18]`. Its merge stage calls
production `stars_fleet_transfer_cargo_balance`, `stars_lookup_fleet`, and
`stars_delete_fleet`; lookup commits merged records into a four-slot service
table with retained IDs 2/3, while deletion marks fleet 3 dead and compacts
that table. It also calls `stars_invalidate_report`, which
clears the fleet-report cache and sets view flag `0x0200` without window
callbacks. The 28/28 fresh-pair/wormhole suite and both focused global-table
lookup/deletion tests pass. These bindings are in the task-5 merge/scrap probe;
the separate selector-1 route probe still has fixture cargo and fleet/report
callbacks. Raw message serialization, shared-stream integration, and the
complete production-turn differential remain open. **SO-E2 has 0 closure
checks left. SO-E3 remains 1 named acceptance item with 1 unresolved
component.** See the [E3 oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md)
and [four-pass regression](tests/test_satisfy_orders_task5_four_pass_delta.py).

**SO-E2 remaining: 0 closure checks.** The exact v4 high-offer original inputs
are now bound through the current-source packed task-5 path and pass host and
VC1.52c native 74/74 checks. See the [closure note](analysis/compat/satisfy-orders-task5-v4-current-source-native.md),
[native report](runs/vmctl-soe2s/SOE2SRES.TXT), and [run manifest](runs/vmctl-soe2s/MANIFEST.JSON).
SO-E3 remains one named full-turn differential acceptance item with 1 unresolved component: complete original/reconstructed production-turn state and service comparison. The complete retained-pair RNG stream is now recorded separately in the E3 oracle. The legacy `SYORIG1`/
`SYACT3` outputs still have unresolved `.X2`/`.H2` starting provenance. The
fresh isolated [original pair](runs/vmctl-sye3k-pair-r1/PAIR.JSON) establishes
that both files were absent in its clean directories and records their
generated returns. A host-only selector-1 route probe derives its input
fleet/order records from the retained original `BACKUP/CLEAN.M2`, map points
from `CLEAN.XY`, and expected fleet-0 turn-1 position/order/fuel from the
original output `CLEAN.M2`; the data are not manually duplicated in C. Its
unknown-planet candidate mask is source-derived: Turin clears `fMarkedPlanets`,
player mode 1 initializes absent M2 planet IDs as unknown, the input M2 has
only owned planet 24, and all four fleet orders target 24. This identifies
31 unknown candidates and derives planet 24's status byte as 1. The four
retained fleets each have one order, so the original multi-order fleet-marker
overwrite does not fire in this case. The probe connects the
current Turin turn, packed order writer, and movement implementation. The
production wormhole finder is tested against the four type-2 things in the
retained original input HST. They are all outside its four-times-nearest-planet
distance cutoff; the host probe exhausts values 0–99, covering both sides of
the 5% gate, and target 26 is unchanged for every value. This removes the outer
draw as a target-selection uncertainty but not its later RNG-state effects. The hull-teeth gate now calls the reconstructed fleet/hull predicate over retained M2 designs and original EXE hull slots. The ideal-warp callback now calls reconstructed
`stars_find_ideal_warp` using retained design 0's engine 1 and its original EXE
fuel-use row; the route probe verifies the returned warp 5 and one engine-1
lookup. These are bounded source bindings, not a full-turn differential or a
native VC1.52c route run. The retained case's AI-planet status bytes are generated from source rules and
M2 data; the outer random result remains a fixture input. Source audit of
the recorded `-g` route establishes
`gd.fTutorial` clear before `FGenerateTurn`, so its conditional fixed reseed is
skipped. `Randomize2(GetTickCount())` uses the tick count's low 14 bits; the
four retained starbase-name draws now recover each run's effective residue,
initial seed words, and stream positions 1–4. The full 32-bit tick count and
later draw sequence remain unknown. One later draw is directly observable:
the original `FGenerateTurn` `Random(8)` value is stored in turn-1 HST BOF
flags. Two clean runs with identical starting HST/M1/M2 hashes record 0 and 1;
the task-5 action records 4, confirming that these separate launches do not
share the same observed RNG stream.
The retained type-16 fleet record places `iplan=0` and `cord=1` in its final
two bytes; the original reader consumes one compact order. The previous
`cord=0` note was incorrect. For fleet 1, the original Turin source calls
`XferAiSupply(1, planet24, 2, fleet1, 3, 25)`. The new host probe
[`satisfy_orders_turin_supply_e3_probe.c`](tests/satisfy_orders_turin_supply_e3_probe.c)
uses retained 250- and 0-colonist inputs with the production transfer and
cargo helpers; it returns 25 and produces the immediate 225/25 state. The
combined selector-1 binding also carries this state through production
`stars_planet_desirability`, `stars_calc_planet_max_pop`, and
`stars_change_population`: +31 growth with fraction 50 reproduces the final
256 colonists. The retained fleet-1 transport decision now runs through
`stars_turin_plan_local_transport`, using the retained current planet owner
and population; packed-record probes verify the source fields decode from the
original runtime layouts. This binds the known owned, inhabited current-planet
path; neutral, empty-population, and off-planet fallback branches remain open.
The accepted 25-unit capacity comes from retained M2 design 1, the original
EXE hull-15 cargo maximum, and reconstructed fleet/free-cargo helpers. The
wider full-turn service sequence remains open. The input
HST has owner-1 fleet IDs 0–3;
both original output HSTs and player M2 files contain only IDs 0 and 1. Raw
Turin source has a conditional `MergeAllShdefs(0xC)` pass for design slots 2
and 3, and input fleets 2/3 both carry one slot-2 ship at the same point. The
new [pinned SHDEF audit](tests/test_satisfy_orders_fresh_ai_pair.py) decodes
flags `0x0907` (bit-9 gate clear), hull 20 (`Midget Miner`), and the group-`0x80`
item-0 part (`dp=5`, quantity 2 per ship). `CMineFromLpfl` therefore returns
10, not its 4000 sentinel. In M2 record order, fleet 2 becomes the merge
destination and fleet 3 is deleted; Turin then writes task 5 to the turn-0
survivor, and pre-movement `SatisfyOrders(1)` marks fleet 2 dead. This source
chain explains both missing output IDs. The full production turn and RNG
accounting remain open. Strict-C89 [current-source production tests](tests/test_produce_source.py)
and [`CBuildProdItem` tests](tests/test_c_build_prod_item_source.py) now bind
the retained design-0 cost through `stars_produce`: with two supplied
zero-progress queue items, it writes progress 81%/0% in place and yields
planet-24 minerals 592/964/520. This matches the retained output queue and
stock witness. The turn-0 HST does not contain that starting type-28 queue;
the bounded current-source seed-to-Produce replay is recorded below, while the
original full-turn path remains open. Adjacent schedule
integration/differential/mutation/resource-queue probes pass 34/34.
The original call path is separately proven by the missing `.X2` files and
source chain in the [E3 oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md). The current-source [selector-routing probe](tests/test_do_ai_turn_source.py)
passes with player word `0x2797`. The newer host-only
[Turin binding probe](tests/test_satisfy_orders_turin_binding.py) continues past
that dispatcher and runs the reconstructed no-enemy scout route, packed MOVE
order write, and fleet movement. It matches point 26, warp 5, position
`(1313,1262)`, and fuel 298. The generated 32-byte status column follows the original initialization
rules and retained M2: absent planets are `0x10` under mode 1, owned planet 24
is `1`, and none of the one-order fleets overwrites that byte. Point 26 is the
nearest eligible target. Neutral-planet desirability and multi-order marker
writes are outside this retained case. The actual outer random value and its
later RNG effects, a native selector-1 route/full-turn comparison, and the
complete turn remain open; a separate
task-write-only native check is recorded below.
The attached `C:\SOTC\CLEAN.X2` error is separately retained in the oracle and
must not be confused with the fresh `C:\SYE3K0`/`C:\SYE3K1` runs.

The earlier SO-E3 original-output audit checks the unchanged player-1 side of
the `SYORIG1` control / `SYACT3` task-5 action run: the control/action M2 input
backups are byte-identical (SHA-256
`588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b`),
and all four owner-1 fleet/order pairs in each M2 input match its turn-0 HST
snapshot. Input fleets 0–3 resolve to output fleets 0/1;
their complete decoded ship counts, cargo, ownership, location, flags, and
compact orders match in original action/control HST and M2. Planet 24's
owner, mineral levels/concentrations, and environment agree, and the M1 message
bodies are pinned (`0x5A` task-5 action versus `0x3F` control). Planet 24 is
decoded through its complete type-13 record and duplicated HST/M2 output: its
owner, flags, levels, concentrations, environment, mineral stocks, population
growth, and auxiliary `uGuesses` are checked. Control stockpiles are
**592/963/520/256** with `uGuesses=8257`; action stockpiles are
**591/964/520/256** with `uGuesses=8255`. The current source now also has a
paired owner-0 control/action replay and a representative failure-message
argument frame; fresh VC1.52c report `runs/vmctl-soe3j/` matches its
strict-C89 host output exactly after newline normalization.
The owner-1 player record has word `0x2797` in the control/action HST and M2
inputs, decoding as `fAi=1`, `lvlAi=1`, and `idAi=1`. The original `DoAiTurn`
switch maps selector 1 to `DoTurinDroneAiTurn`, and the reconstructed
dispatcher routes selector bits 13–15 to `personalities[selector]`. The
retained `.M2` proves the scout enemy list is empty for this pair. Both output
HSTs and M2 files retain fleets 0/1 and omit 2/3, so the latter are deleted
during the turn rather than filtered only from M2. The pinned SHDEF, original
hull/part table, and Turin source show the merge deletes fleet 3 into fleet 2;
the turn-0 mining branch writes task 5 to fleet 2, and the pre-movement
`SatisfyOrders(1)` pass marks it dead. This explains both missing fleet IDs in
the original outputs. The host-only route slice reproduces fleet 0's MOVE
order and movement result. The selector-1 binding also routes fleet 1's
0-to-25 transfer through a source-derived local transport decision and
production supply/cargo helpers, then carries planet population through
production growth to the original 256 colonists (net +6). Its retained-case
transport decision and capacity are source-derived; the remaining
fleet/planet/message and surrounding order, movement, transfer, battle, merge,
and other service effects still need a complete production comparison. The full
neutral-planet and multi-order marker cases remain open; the retained-case
status bytes and wormhole finder inputs are source-bound. The unknown-candidate
set explains why point 26 is eligible. The original `-g` source trace proves
the tutorial reseed is skipped. The effective low-14-bit `Randomize2` startup
residues and seed words are recovered; the full 32-bit `GetTickCount` value
and complete draw sequence remain unknown. The retained four type-2 wormholes
also prove that `KeepFleetsMoving` adds no fleet-speed RNG draws: it excludes
type-2 things, and `SetAiFleetIdealSpeed` reaches its two `Random(10)` calls
only when a listed minefield matches. This source/fixture guard is pinned by
[`test_retained_thing_table_skips_minefield_random_paths`](tests/test_satisfy_orders_wormhole_move_e3.py).
The first retained AI planet-task roll is draw 6 (7/75), and both runs miss its <6 upgrade gate; later AI planet-task draws and the full stream remain open. **SO-E3 remains at
one named open check, with two unresolved subissues: (1) complete
production/full-turn differential and (2) exact RNG seed/draw accounting.**

The owner-1 mineral stage is now bounded from the fresh pair's hash-pinned
planet/player records through current-source `stars_est_minerals_mined` in a
strict-C89 host probe. The original inputs resolve to 10 working mines,
efficiency 10/10, concentrations 68/87/68, and possible yields 6–7/8–9/6–7;
explicit draw witnesses reproduce mineral levels 247/241/247. Both original
turn-1 HSTs retain the same type-28 queue after planet 24: two design-0 ship
entries, with the first at 81% progress. The hash-pinned original hull/part
table rows for design 0 are [35,16,2,8] in resource/ore table order;
`GetProductionCosts` supplies them to production as ores/resources
[16,2,8,35]. At 81%, this prorates to [12,1,6,28], spending [12,1,6] ore.
Combined with source-valid mining witnesses,
this exactly accounts for original planet-24 stocks [592,963,520] in control
and [592,964,520] in action from the integrated task-5 stock [597,956,519].
This is an arithmetic witness, not the recovered historical draw sequence or
a complete production replay. The turn-zero Turin seed branch explains the
two design-0 queue entries for the retained 32-point map: `32 → 2 → -28`
requests two hulls when the planet has an open production slot and no existing
ship queue. A strict-C89 [seed-to-Produce probe](tests/test_satisfy_orders_turin_seed_produce.py)
passes 11/11 and matches the original ending queue and action mineral stocks.
The retained type-13 record decodes to runtime planet flags `0x0f07` and
population 250, satisfying raw production bit 9 and the 200-population gate.
Current-source `stars_planet_desirability` returns 100 from the retained player
and planet environments. Later AI queue gates still use typed inputs; the
original RNG stream and surrounding turn services remain unbound. The SO-E3
acceptance count is unchanged. See the [owner-1 mining and queue audit](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#owner-1-planet-24-mining-bound)
and its [retained-input regression](tests/test_satisfy_orders_task5_four_pass_delta.py).

- [x] Bind the exact high-offer design, player-1 race/technology state, planet
      input, and original part costs through current-source task 5. The
      [v4 original pair](analysis/compat/satisfy-orders-reprice-v4-bitc-original-oracle.json)
      pins the live task-0/task-5 inputs; the current-source native probe
      verifies twelve part lookups, four repricings, costs 1930/494/681/261,
      message order/recipients, planet mutation, technology result, and fleet
      retirement.
- [x] Capture a complete original deep-space salvage case from a consistent
      input and compare the salvage thing, minerals, fleet retirement, and
      `0x5B` message. The natural SMOKE control/action pair establishes the
      thing at (1276,1380), 5/0/2 minerals, and fleet retirement. A second
      paired run clears only the owner-1 AI message-suppression bit and saves
      the original `0x5B` frame (`-6`, salvage ID `0x2200`, fleet word 0).
      The [VC1.52c native 21/21 probe](runs/vmctl-soe2q/MANIFEST.JSON)
      matches the complete saved 18-byte thing after `ThingDecay`, the minerals,
      callback frame, and retirement. The paired [original message oracle](analysis/compat/satisfy-orders-smoke-human-message-original-oracle.json)
      and [original salvage oracle](analysis/compat/satisfy-orders-smoke-deepspace-original-oracle.json)
      pin the two original observations.
- [x] Capture matched original positive and negative technology-result cases
      from ship-definition slots proven to belong to the tasking fleet. Retain
      the exact task-0/task-5 inputs (including a shared H2 seed), raw HST/M1/M2
      outputs, decoded learned-tech changes, and exact message arguments.
      The slot-verified v2 task-5 run emits `0x13D` and raises player-0 tech
      index 2 from 3 to 4. The matched v3 task-0/task-5 shared-H2 pair retains
      fleet 0 in control and retires it in action; action emits `0x13C` with
      args `[512,1147,0,10]`, sets trader-tech bit 2 and `fLearned`, and leaves
      player-0 tech levels unchanged. Both runs retain raw HST/M1/M2 outputs,
      and no error dialog appeared. Five additional v2 shared-H2 seeds are
      retained; seeds 1 and 2 had no tech result, and seeds 3–5 emitted
      `0x13D`. The `0x13E/0x13F` planet-target resource branch remains a
      separate extension. The attached corrupt-`CLEAN.X2` screenshot is from
      an earlier invalid candidate and is excluded; no corrected run produced
      `CLEAN.X2` or showed an error dialog.
- [x] Run matching current-source native cases and publish a field-level
      differential for mutation and service/message order, including the
      original resource case. The [differential](analysis/compat/satisfy-orders-task5-field-differential.md)
      links the original oracles, current-source callback traces, and fresh
      VC1.52c reports; only the exact v4 high-offer current-source binding above
      remains open.

## Combat — `FAttack` and `FDoCoolBattle`

| # | ID | Evidence already recorded | Needed to close | Done |
|---:|---|---|---|:---:|
| 4 | FA-E1 | Native selector, offset, and context subprobes; see [TODO7 combat criteria](TODO7.md#combat--6). | Bind live original weapon/hull/target tables and target resolution, including `FLookupPart` race, hidden, and technology behavior. Show native beam and torpedo cases covering hull, resource, shield, jam, and capacity effects with before/after state. | [ ] |
| 5 | FA-E2 | Isolated table and attack-boundary probes. | Compare representative original beam and torpedo damage, boundary values, and side-effect order against the native attack call; retain original inputs/outputs and a field-level mismatch report. | [ ] |
| 6 | FA-E3 | Narrow attack service probes. | Run a fresh complete native battle with the attack service, retain full battle input/output and logs, and compare the resulting state to the original battle. | [ ] |
| 7 | FB-E1 | Packed-token-to-attack callback replays. | Bind exact `_tok`/`vrgtok` layout, live DGROUP/hull memory, global compaction and production callers; prove the complete native token field map with representative records. | [ ] |
| 8 | FB-E2 | Partial mechanics probes. | Reconcile movement geometry, speed, initiative, damage, shield, casualty, salvage, bombing, and report-record writes against source and native before/after cases. | [ ] |
| 9 | FB-E3 | Scoped battle replays. | Replay a complete battle natively and in the original executable; retain packed before/after state, event logs, reports, and a decoded differential. | [ ] |

## AI personalities

Each complete-turn gate below requires the same deterministic starting state
in the reconstructed and original turns, saved raw records, and an explicit
field/callback comparison. Earlier narrow probes remain supporting evidence.
The [AI-turn burndown](analysis/compat/ai-turn-burndown.md) divides the 20
open gates and 6 promotion audits into currently 84 open smaller checks without changing
these acceptance boundaries.

| # | ID | Evidence already recorded | Needed to close | Done |
|---:|---|---|---|:---:|
| 10 | RB-E2 | First-order Robotoid repair and route-plan/apply callback support. | Verify colonizer, transport, and attack target searches and predicates against original behavior; show the resulting route and order writes. | [ ] |
| 11 | RB-E3 | First-order packed repair. | Mutate and compare multi-order raw `PLORD` records and dependent AI-table branches, including before/after bytes and native results. | [ ] |
| 12 | RB-E4 | Scoped Robotoid order probes. | Bind live tables and compare a deterministic complete native/original AI turn: fleet, order, resource, and callback state. | [ ] |
| 13 | TU-E1 | Host/native repair, role queues, seed, technology, minelayer, support-fleet, packed planet writes, and production-gate subchecks. | Bind live production, AI, player, design, and strength tables; cover remaining growth/resource services, queue/finalization side effects, and original production-policy outcomes. | [ ] |
| 14 | TU-E2 | Turin route-plan/apply seam. | Verify target searches, predicates, and final order writes for each fleet action family against original behavior. | [ ] |
| 15 | TU-E3 | First-order packed mutation support. | Compare multi-order `PLORD` and dependent AI-table branches as raw before/after records in host/native cases. | [ ] |
| 16 | TU-E4 | Scoped Turin production and routing probes. | Compare complete native/original turns over orders, production, resources, RNG/callback order, and AI-table state in representative cases. | [ ] |
| 17 | AU-E1 | Host support for first-match colonizer selection, RNG/queue sequence, AI-planet writes, production census and queue cases, live desirability calls, and packed queue scan. | Finish remaining colonizer/mineral/planet action branches in original decision order, bind live services, obtain Win16 results, and compare original turn outcomes. | [ ] |
| 18 | AU-E3 | Host support for owned-fleet flag sweep, direct MOVE/COLONIZE first-order writes, and scout low-fuel task-5 gate. | Verify target search and remaining route families; show multi-order raw `PLORD` and AI-table mutation, native execution, and original-turn comparison. | [ ] |
| 19 | AU-E4 | Scoped Automitron branch probes. | Compare a deterministic complete native/original AI turn across fleet, order, resource, and message state. | [ ] |
| 20 | RT-E1 | Host two-pass census, AI-planet writes, packed queue bypass, production lifecycle, [focused VC1.52c r9](analysis/compat/do-rototill-production-e1-native-packet.md) 49/49, 11/11 caller-to-shared-writer checks for exact pre-finish PLPROD item bytes, and a [retained original selector-3 turn](analysis/compat/do-rototill-original-turn-r1.md) with type-28 queue body `011c9205` (one planet factory, quantity 1, progress 89). A [retained four-fleet AI phase](analysis/compat/do-rototill-retained-ai-phase-r1.md) passes 11/11 host/native and shows planet 24's 250 colonists do not enter Rototill's direct queue-add branch; the final queue service is invoked. Original backup X2's final type-29 mutation queues three factories before production. A [source-backed boundary diagnostic](analysis/compat/do-rototill-queue-boundary-r1.md) now applies original turn-zero `IroEnsureAi`'s 0% research write to the live player table. Post-transfer canonical population 225 yields 32 resources and three factories alone, matching the original X2 quantity; stale population 250 would also queue a mine. The bound retained-input basic-task callback runs fleet speed, existing-starbase skip, five planet actions, and `FinishProduction(0)` without a queue addition, leaving population 225 and no-research clear. The final source callback now runs selection, catalog, resource/queue costs, filler, packed addition, `FinishProduction(1)`, and planet logging; its type-29 body matches the original byte for byte. A focused source `Produce` slice and a 32-planet source mining/production pass build two factories and emit the original turn-1 one-factory/89% type-28 body; after the two player-shuffle RNG draws, mined stocks `539/965/506` become the original final `539/965/498` with factory count 12, using the retained later-phase 15% player value. The selector-3 coordinator now restores that HST percentage, loads its generated X2 with source `stars_load_log_file`, and applies all seven mutations through source `stars_run_log_file`/`stars_run_log_record` to a fresh HST planet view; the replayed three-factory queue, planet/fleet 225/25-colonist split, fleet-0 second order, and unchanged 15% research agree. The 32-planet `Produce` pass also reaches the original 256 colonists/fraction 50, matches player-1 turn-1 M2 research bytes 26–61, and executes all four random-event callbacks with the Rototill four-draw turn-zero path. | Bind complete turn-state materialization, native callback, and full resource/mineral and file outcomes against original output. | [ ] |
| 21 | RT-E2 | Host 17/17 packed fleet entry and selected routes; the [original selector-3 turn](analysis/compat/do-rototill-original-turn-r1.md) pins fleet 0's planet-26 order. Four witness tests derive 31 unknown-status candidates. The [scout caller packet](analysis/compat/do-rototill-scout-dispatch-e2.md) passes 10/10 host/native (r15 combined 103/103); the [retained route composition](analysis/compat/do-rototill-scout-route-e2.md) passes 180/180 host/native with original-hull-derived tooth result, engine-derived warp 5, retained wormhole exclusion, synthetic diversion, source-order AI status, full second PLORD order, growth/relocation, no-unknown outcomes, corrected human-only retry, controlled near/distant-enemy outcomes, and reserved-planet exclusion. The [retained-turn RNG audit](analysis/compat/do-rototill-main-rng-audit-r1.md#retained-turn-execution-context-closure-2026-09-27) closes RT-E2.e with direct caller dispositions and original X2 indirect-path guards. | The [original design-name seed audit](analysis/compat/do-rototill-rng-seed-r1.md) recovers effective tick residue 6844 and scout `Random(100)=31`; source `IroEnsureAi` now consumes those four name draws and publishes the four original names; all seven original backup X2 mutations now match byte for byte, including the four designs, scout order, 25-colonist transfer, and factory queue, while turn-1 M2 design flags/points change later; the [output-tail audit](analysis/compat/do-rototill-rng-tail-r1.md) matches source-written M1/M2/HST BOFs with inferred tick inputs. Original enemy/wormhole/fallback outcomes, live allocator/global identity, and complete native/original output comparison remain. | [ ] |
| 22 | RT-E3 | [Three-order caller/native probes](analysis/compat/do-rototill-multi-order-e3.md) compare all 58 PLORD bytes and AI table across second-order marker, occupied-route count repair, complete first/second transport reroute writes, and bounded deep-space/selected-planet/starbase first-kind fallback. The [owned-planet colonizer marker branch](analysis/compat/do-rototill-multi-order-e3.md#colonizer-ai-byte-15-marker-continuation-2026-09-27) follows original select/nearest/supply/refresh/colonize order and passes 9/9 host/native checks in a fresh 112/112 VC1.52c packet. The [selector-3 coordinator](analysis/compat/do-rototill-coordinator-e4-r1.md) reproduces fleet 1's source-backed 25-colonist transfer and its exact original backup X2 type-1 mutation. | Bind other colonizer entry states and second-sweep routes, resolve absent-target/live service and selected-pointer semantics, and compare internal original PLORD/AI-table before/after state; compact turn files alone cannot show it. | [ ] |
| 23 | RT-E4 | Scoped Rototill production and route probes plus [retained original selector-3 turn-1 outputs](analysis/compat/do-rototill-original-turn-r1.md). The [serialized replay oracle](analysis/compat/do-rototill-turn-r1-oracle.json) pins six file hashes, all four owner-1 fleet/order pairs, planet 24, queue, 32 points, and 26 changed leaves; HST/M2 owner-1 views agree. A [variant audit](analysis/compat/do-rototill-turin-variant-r1.md) identifies reusable full-turn records. One [retained AI-phase call](analysis/compat/do-rototill-retained-ai-phase-r1.md) passes 11/11 host/native. A [selector-3 coordinator](analysis/compat/do-rototill-coordinator-e4-r1.md) now passes strict-C89 host loading, personality dispatch, live AI-table allocation, history callback, AI-session cleanup, the four original starbase design names and five-design count through source `IroEnsureAi`, the original fleet-0 scout destination/warp/packed second order with recovered scout RNG state, and fleet 1’s 25-colonist transfer and the final three-factory queue; all seven backup X2 mutations and the complete 322-byte source-written backup X2 match byte for byte (with retained serial/environment/tick inputs); the source-written 95-byte H2 also matches original after the next BOF RNG draw; a focused planet-24 source `Produce` slice and a 32-planet source mining/production pass match the original type-28 queue, factory count 12, planet-24 mineral stocks `539/965/498`, population 256/fraction 50 and player-1 research bytes; source movement, waypoint cleanup and `WriteFleet` match all four turn-1 owner-1 M2 and six owner-0 HST fleet/order pairs byte for byte, including the planet-26 arrival at 298 fuel, source postproduction `MoveThings` matches all four turn-1 HST wormhole records, and source `UpdateGuesses` plus `WritePlanet` match all 32 HST planet bodies, M1 planet 10, and the original 37/37-byte planet-24 M2 body. The [source file integration](analysis/compat/do-rototill-coordinator-e4-r1.md) now matches all 147 turn-1 records and all three complete encrypted M1/M2/HST files byte for byte. The [first full-turn VC1.52c compile packet](analysis/compat/do-rototill-full-turn-native-r1.md) exposes an oversized monolithic trace and compiler internal error; no native executable resulted. | Split the complete coordinator into Win16-sized state and fixture segments, then run it natively and compare the same-input native/original queue, route, order, planet, resource, callback, and full-file state. | [ ] |
| 24 | MA-E1 | Partial Macinti planet-table selection support. | Bind live planet and AI-table inputs; compare original selection predicates, chosen records, and table/output writes. | [ ] |
| 25 | MA-E2 | Partial retarget/colonizer/transport support. | Reconcile exact fleet predicates, candidate choice, and route/order sequence for these action families against original behavior. | [ ] |
| 26 | MA-E3 | Host first-order repair support. | Compare direct multi-order raw `PLORD` before/after mutation and dependent AI-table state in native cases. | [ ] |
| 27 | MA-E4 | Scoped Macinti branch probes. | Replay remaining personality branches and compare a complete native/original turn's orders, fleets, resources, and messages. | [ ] |
| 28 | CY-E3 | E10 action dispatch, E12 replacement/append replays, and the [source-bound colonizer second-order marker](analysis/compat/do-cyber-colonizer-second-order-e3.md): 200/200 host and VC1.52c caller checks. | Bind the remaining raw Cyber branch predicates and complete multi-order `PLORD`/AI-table mutation to original behavior, with native/original before/after evidence. | [ ] |
| 29 | CY-E4 | Scoped Cyber personality probes. | Replay the complete native/original personality turn and compare AI tables, fleet, production, order, resource, and callback state. | [ ] |

## Reports and popups — `ExecuteReportClick`

| # | ID | Evidence already recorded | Needed to close | Done |
|---:|---|---|---|:---:|
| 30 | ER-E2 | Report/click host support; see [TODO7 report criteria](TODO7.md#reports-and-popups--2). | Exercise actual Win98 USER/GDI report selection, geometry, controls/messages, draw and refresh; retain screenshots or captured events and compare with original observations. | [ ] |
| 31 | ER-E3 | A 33-check host callback-chain replay. | Drive planet, starbase, defense, resource-exempt, and modal caller/layout branches through production report/click/popup integration; retain native and original results for each case. | [ ] |

## Separate Step-6 promotions — 9 open

A procedure promotion starts only when all its numbered evidence units above
are closed. For each one, synchronize the final scope in
[tracking/reviews.json](tracking/reviews.json), regenerate and check
`analysis/coverage.json` and TODO5, update reconstruction notes and
TODO6/TODO7, and verify that the procedure is genuinely 6/6.

| Promotion | Procedure | Prerequisite | Done |
|---|---|---|:---:|
| SO-P | `SatisfyOrders` | [Promotion audit](analysis/compat/satisfy-orders-step6-promotion.md) | [x] |
| FA-P | `FAttack` | FA-E1–E3 | [ ] |
| FB-P | `FDoCoolBattle` | FB-E1–E3 | [ ] |
| RB-P | `DoRobotoidAiTurn` | RB-E2–E4 | [ ] |
| TU-P | `DoTurinDroneAiTurn` | TU-E1–E4 | [ ] |
| AU-P | `DoAutomitronAiTurn` | AU-E1/E3/E4 | [ ] |
| RT-P | `DoRototillAiTurn` | RT-E1–E4 | [ ] |
| MA-P | `DoMacintiAiTurn` | MA-E1–E4 | [ ] |
| CY-P | `DoCyberAiTurn` | CY-E3–E4 | [ ] |
| ER-P | `ExecuteReportClick` | ER-E2–E3 | [ ] |

## Latest SO-E3 source progress (2026-09-25)

The reconstructed `stars_f_create_ai_starbase` now performs the missing
`stars_pick_ai_name_and_bitmap` step before the change/publish callback. A
strict-C89 probe runs the retained owner-1 research bytes through
`stars_do_turin_drone_ai_turn` / `stars_iro_ensure_ai`, uses the original EXE
research-requirement table, and reproduces HST turn-1 current technology
`0x32` at 15 percent. The same recovered control/action seed then drives four
production starbase name picks and scout draw 5 (99/4); the original-output
name regression maps the selected IDs through the embedded EXE string table.
See [the detailed probe record](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#retained-turin-starbase-startup-rng-is-source-bound-2026-09-25)
and [source test](tests/test_ai_starbase_name.py).

This probe still uses fixture starbase capability/work inputs; it does not
publish the four SHDEF records or execute a full turn. **SO-E2: 0 closure
checks remain. SO-E3: 1 full-turn differential component remains.** The
current focused suite passes 41/41.

## SO-E3: recovered startup RNG enters the combined selector-1 route

`tests/test_satisfy_orders_turin_binding.py` now has seeded `control` and
`action` modes in addition to its 100 injected route values. These modes start
from the recovered effective `Randomize2` ticks and pass the same seed through
the production outer `stars_do_ai_turn` and Turin `stars_do_turin_drone_ai_turn`
calls. The Turin `ensure_ai` callback runs production `stars_iro_ensure_ai`,
the starbase-design selector, `stars_f_create_ai_starbase`, and
`stars_pick_ai_name_and_bitmap` before the selector-1 scout planner consumes
draw 5. Input research fields and the 31-byte Turin requirement table come from
retained HST and the symbol-resolved pinned EXE; expected names and RNG values
come from the retained outputs and recovered seed trace.

Both seeded modes check four name draws, IDs `[1078,1083,1081,1074]` for
control and `[1083,1079,1080,1084]` for action, then scout `Random(100)` values
99 and 4 at draw 5 and target planet 26. The original HST tech state transitions
from `0x60` to `0x32` at 15% with tech levels unchanged. The existing 100
scripted route cases and both seeded cases pass in the focused eight-module
suite (**41/41**).

The starbase capability bits, latest-design index, hull/work fields, and
publication storage are still test fixtures; the run does not materialize the
four SHDEF records in M2 or compare a complete reconstructed turn with both
original outputs. The screenshot error for `C:\SOTC\CLEAN.X2` remains a
separate corrupt-input diagnostic and is not part of the clean isolated pair.
**SO-E2: 0 checks left. SO-E3: 1 full-turn differential component left.**

## SO-E3 shared startup-to-post-production wormhole RNG pipeline (2026-09-25)

The seeded selector-1 probe now continues the same recovered `StarsSeeds`
state beyond the four starbase names and scout draw. Production
`stars_f_upgrade_ai_starbase` consumes draw 6; two production
`stars_write_bof` calls consume draws 7–8; production `stars_generate_turn`
consumes the two-player shuffle at draws 9–10; and production
`stars_itech_learn_a_tech` consumes the task-5 technology draws. The probe
checks serialized BOF record framing, type flags, and the `Random(2000)` salt
values, along with shuffle order and each technology draw's limit/value.
The action's draw 11 (`43`) is bound to the retained owner-1 task-5 call.
Draw 12 (`12`) now follows the retained owner-0 planet-10 task-5 fleet/order
replay through `stars_satisfy_orders_packed`, then player-0
`stars_itech_learn_a_tech` in that production callback.

It then calls production `stars_mine_minerals` for the only two owned planets
in the original HST input, IDs 10 and 24. Their retained player/planet mining
inputs feed production `stars_est_minerals_mined`; the shared seed supplies
the next six `Random(100)` values, and the probe checks each draw and each
planet's three yields.

Then production `stars_random_events` dispatches Meteor, Climate,
New-Minerals, and Mystery-Trader callbacks. The first three test adapters call
the retained `Random(20)`, `Random(20)`, and `Random(15)` entry gates; all
values are nonzero, so nested event paths are skipped. At turn zero the
Mystery-Trader callback is also skipped before any random call.

Without reseeding after those gates, the probe calls production
`stars_move_things(post_production=1)` on all four retained type-2 HST things.
Its `Random(100)` and `Random(25)` calls match the original recovered values at
control draws 40–51 and action draws 22–33. Production
`stars_pct_wormhole_moves` selects the zero-chance local-jitter path, while
`stars_validate_wormhole_position` checks candidates against the retained XY
planet coordinates, other moving things, and each run's output fleet points.
Each first candidate succeeds; the probe compares all 18 bytes of all four
wormhole records with the corresponding original turn-1 HST output.

The control stream matches through draw 51: upgrade 7, BOFs 1240/1434,
shuffle 0/0, 20 task-5 draws beginning with 74, then mining draws
`[67,11,28,32,99,3]`. They yield planet 10 `[7,9,7]` and planet 24
`[7,8,7]`, then event gates `[19,18,10]`, then wormhole draws
`[29,8,17,21,9,0,28,6,18,77,11,13]`. The action stream matches through
draw 33: upgrade 75, BOFs
1922/267, shuffle 1/0, task-5 gates 43/12, then mining draws
`[95,17,59,38,26,18]`; yields are planet 10 `[6,9,7]` and planet 24
`[7,9,7]`, then event gates `[11,7,2]`, then wormhole draws
`[62,15,13,20,5,19,78,16,23,69,14,23]`. One host test covers the 100 injected route cases and both seeded
streams; the surrounding eight-module focused suite passes **41/41**.
See the [probe record](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#seeded-selector-1-startup-to-post-production-wormhole-rng-pipeline-2026-09-25),
[source probe](tests/satisfy_orders_turin_binding_e3_probe.c), and
[regression](tests/test_satisfy_orders_turin_binding.py).

Random-event effects beyond the three retained entry gates, final output
writers, full-turn services, and complete output-file comparison remain
outside this source harness. The wormhole waypoint-validation callback remains
a fixture adapter; the candidate collision/scoring check uses production
`stars_validate_wormhole_position`.

### Evidence still required to close SO-E3

1. Run the complete current-source selector-1 turn for the retained control
   and action inputs, preserving each run's recovered initial seed and the
   original stage order. The current harness does enter production
   `stars_generate_turn`, but uses minimal turn state and fixture callbacks
   for several services; it does not yet run the complete retained game state
   through every current-source turn service.
2. Retain raw production HST/M1/M2 and related player-log outputs plus the
   build/run reports and hashes. Parse each record and keep the original pair
   returns beside them; do not replace or normalize the source artifacts.
3. Compare both output runs field by field against the matching original
   outputs: fleet identities, orders and four-pass retirement; cargo and
   ownership; planet population/resources and mining; technology/research;
   SHDEF/design/name records; scores/history; and complete message IDs,
   order, arguments, and serialized bytes.
4. Account for the known launch-specific RNG and BOF tick fields, then resolve
   every remaining output difference. The control/action delta must agree with
   the original pair, with no unexplained state or serialized-record changes.
5. Preserve the runnable regression and its full report so another run can
   reproduce the same-input comparison. The isolated corrupt
   `C:\SOTC\CLEAN.X2` screenshot is a separate diagnostic and is not evidence
   for or against this clean-pair differential.

**SO-E2: 0 checks left. SO-E3: 1 full-turn differential component left.
TODO7: 39 open items (29 evidence, 10 promotions).**

## Latest SO-E3 seeded turn-coordinator integration (2026-09-25)

The seeded control/action harness now runs through production
`stars_generate_turn` instead of calling it only for player shuffle. Both order
phases use production `stars_do_orders`. Before production it calls
`stars_unmark_mine_fields` and pre-production `stars_move_things` on the four
retained type-2 wormholes; both preserve their bytes/coordinates and consume no
RNG. The `ThingDecay` stage calls production `stars_thing_decay` on those
wormholes and the retained fleet flag subset.

The produce stage now generates planet 24's two design-0 items through
production `stars_do_turin_drone_ai_turn`, then passes them through
`stars_produce` and `stars_build_production_item`. The 81% first item spends
the retained EXE-backed `[12,1,6]` mineral cost. Action pass 1 also replays the
retained owner-0 planet-10 task-5 order through `stars_satisfy_orders_packed`
before mining. The production HST writer path now emits all 32 type-13 planet
records: the 30 unchanged planets round-trip through `stars_read_planet` and
`stars_write_planet`, and planets 10/24 use their production-updated state.
Every record body matches the corresponding original control/action output.
Control stocks match as planet 10 `476/902/383/287` and planet 24
`592/963/520/256`; action stocks match as planet 10 `484/908/388/287` and
planet 24 `592/964/520/256`. This exposed and fixed `stars_read_planet`'s
absent-tail default for unowned planets. The strict-C89 selector-1 regression
passes 1/1 with `PLANET_RECORDS=32/32` in both seeded runs. The current
three-file output callbacks now run production `stars_write_data_file`; its
create callbacks write the retained BOFs, its game-file planet callback emits
the 32-record HST table, and its turn callback writes the final turn-1 marker.
All three `0100` type-0 markers match both original control/action M1/M2/HST
outputs. The eight-module SO-E3 regression passes 41/41 in 105.289 seconds.

This adds a production save-coordinator and turn-marker slice alongside the
type-13 planet-record slice. It still does not generate complete streams: the
load state and planet-24 resource budget remain fixtures; many turn stages
remain no-op; and the save writer omits its player, battle, salt, message,
SHDEF, fleet, and battle-plan sections. The M1/M2 player-log planet callback
now emits all five original records: M1 planets 7, 10, and 12 plus M2 planets
24 and 26. Planet 26's visibility transition is bound to production
`stars_set_vis_pf_init` after the moved owner-1 fleet reaches its retained XY
coordinates. Nested event effects, type-28 queue output, M1/M2 message output,
and complete file serialization remain open. The probe compares BOFs, final
markers, the complete HST planet table, and all five player-log planet records,
not complete HST/M1/M2 files. The attached SOTC corruption image repeats the archived
`CLEAN.X2` load rejection and adds no new guest-state evidence. **SO-E2: 0
checks left. SO-E3: 1 full-turn differential component left. TODO7: 39 open
items (29 evidence, 10 promotions).**

### SO-E3 player-log planet serialization (2026-09-25)

The production data-file writer now sends all five retained M1/M2 planet
records through production serialization. M1 planets 7/12 type 14 are read
from retained M1 input; planet 10 and M2 planet 24 use production-updated
type-13 states. M2 planet 26 is read from HST and marked at detail 3 by
production `stars_set_vis_pf_init` using the arrived fleet-0 state. All five
outputs match both retained original runs byte-for-byte. Coverage is M1 3/3,
M2 2/2, 5/5 total. The focused selector test passes 1/1 and the eight-module
SO-E3 regression passes 41/41 in 105.715 seconds. This does not close SO-E3:
**SO-E2: 0 checks left; SO-E3: 1 full-turn differential component left;
TODO7: 39 open (29 evidence, 10 promotions).** See the [SO-E3 oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-player-log-planet-records-2026-09-25).

### SO-E3 owner-1 M2 fleet and order serialization (2026-09-25)

The current-source `stars_write_data_file` callback now visits the two retained
owner-1 M2 output fleets and uses production `stars_write_fleet` and
`stars_write_orders`. Fleet 0 takes its destination, fuel, and newly assigned
Turin order from the current production movement/PLORD state. Fleet 1 takes
its current supply cargo and stationary movement state plus its current compact
order. Both 23-byte type-16 records and both following 8-byte type-20 orders
match the original control and action outputs byte-for-byte (2/2 fleets,
2/2 orders). The targeted strict-C89 selector test passes 1/1, and the full
eight-module SO-E3 suite passes 41/41 in 104.809 seconds.

This is a serialized M2 slice, not full file generation: the coordinator still
uses a minimal load state and has fixture/no-op stages, and other HST/M1/M2
records and the complete record-by-record differential remain. The attached
Stars! dialog has the same SHA-256 as the archived `CLEAN.X2` rejection image;
it confirms that Stars! rejected that file, while its contents and starting
presence do not identify the cause. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**

### SO-E3 owner-1 HST fleet and order serialization (2026-09-25)

The production game-file writer now emits owner-1 HST fleets 0 and 1 through
the same `stars_write_fleet` and `stars_write_orders` path used for M2. In both
original runs these HST fleet/order bodies are byte-identical to their M2
counterparts. Current-source output matches both 23-byte type-16 fleet records
and both following 8-byte type-20 compact orders (HST 2/2 fleets, 2/2 orders;
M2 remains 2/2 and 2/2). The targeted strict-C89 selector test passes 1/1;
the eight-module suite passes 41/41 in 105.636 seconds.

Other HST fleet records and the rest of the HST/M1/M2 streams still need
current-source generation and full comparison. The SO-E3 acceptance count is
still one component. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential
component left. TODO7: 39 open (29 evidence, 10 promotions).** See the
[HST fleet/order oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-hst-owner-1-fleet-order-records-2026-09-25).

### SO-E3 complete HST fleet/order record slice (2026-09-25)

The current-source turn now supplies owner-0 retained fleets to production
`stars_move_fleets`; their unchanged point/fuel state feeds production
`stars_write_fleet` and `stars_write_orders`. The action task-5 path retires
owner-0 fleet 0, so it is omitted from the action HST. Together with the
already bound owner-1 fleet movement/supply state, the writer now matches every
HST fleet and following compact order: control 8/8 fleets and 8/8 orders,
action 7/7 fleets and 7/7 orders. M2 remains 2/2 fleets and 2/2 orders. The
targeted strict-C89 test passes 1/1; the full eight-module regression passes
41/41 in 105.209 seconds.

This closes the HST fleet/order record slice, not SO-E3: M1 fleet records and
other save sections, full state loading, and the complete HST/M1/M2
differential remain. The newly attached corruption dialog is still the exact
archived screenshot and provides no new cause evidence. **SO-E2: 0 checks left.
SO-E3: 1 full-turn differential component left. TODO7: 39 open (29 evidence,
10 promotions).** See the [complete HST fleet/order comparison](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-complete-hst-fleet-order-records-2026-09-25).

### SO-E3 M1 fleet and order serialization (2026-09-25)

The current-source `stars_write_data_file` path now serializes the retained
owner-0 M1 fleets with production `stars_write_fleet` and
`stars_write_orders`, using the same post-movement state and task-5 retirement
as HST. All M1 owner-0 type-16 fleet bodies and following type-20 compact
orders match the retained original outputs byte-for-byte: control IDs 0–5
(6/6 fleets and orders), action IDs 1–5 (5/5). These M1 records also match the
corresponding HST owner-0 records. In the same run, M2 remains 2/2 and HST
matches 8/8 control and 7/7 action fleets and orders. The focused selector
test passes 1/1 in 11.307 seconds; the eight-module regression passes 41/41
in 105.462 seconds.

This binds these fleet/order slices only. Remaining sections, complete current
turn-state loading, and the full HST/M1/M2 file differential remain. **SO-E2:
0 checks left. SO-E3: 1 full-turn differential component left. TODO7: 39 open
(29 evidence, 10 promotions).** See the [M1 fleet/order comparison](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-m1-fleet-order-records-2026-09-25).

### SO-E3 planet-24 production queue serialization (2026-09-25)

The current-source turn now serializes planet 24's two live production queue
items after its type-13 planet record in both M2 and HST. The new production
serializer packs each 4-byte `_prod` item from current quantity, design ID,
object kind, and progress fields. The resulting 8-byte type-28 body is
`0100140501000400`, matching the original M2 and HST control/action outputs
exactly. The two current items are design 0 ship entries, count 1 each, at 81%
and 0% progress. This follows current Turin queue seeding and production; the
planet-24 resource witness is still injected from retained evidence.

The focused selector test passes 1/1 in 11.330 seconds, and the eight-module
regression passes 41/41 in 105.606 seconds. This adds the M2/HST queue record
slice but does not close SO-E3: other save sections, complete current-state
loading, and full output-file comparison remain. **SO-E2: 0 checks left. SO-E3:
1 full-turn differential component left. TODO7: 39 open (29 evidence, 10
promotions).** See the [type-28 queue oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-planet-24-type-28-production-queue-2026-09-25).

### SO-E3 HST type-43 wormhole records (2026-09-25)

The `stars_write_data_file` coordinator now follows the original save-section
order around designs and fleets: design bank 0, fleets, design bank 1, score
records, things, then battle plans. The new `stars_write_thing_records`
serializer emits a type-43 count body (`0400`) followed by four 18-byte type-43
records. In the selector-1 turn, production `stars_move_things` state is copied
into the current thing table, and all four HST bodies match the retained
control and action HST wormhole records byte-for-byte. M1 and M2 emit no
type-43 records for this retained pair.

The focused selector test passes 1/1 in 11.265 seconds. The combined
eight-module SO-E3 regression and writer-order regression pass 42/42 in
107.747 seconds. `baseline.py --check`, oracle JSON validation, Python syntax,
and whitespace checks pass.

This binds the HST type-43 slice and coordinator ordering only. The score
callback is still a placeholder; other records and complete retained-state
loading are incomplete, and full HST/M1/M2 files are not yet assembled or
compared. The screenshot shows only that Stars! rejected `C:\SOTC\CLEAN.X2`;
it does not establish the cause. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**
See the [type-43 oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-hst-type-43-wormhole-records-2026-09-25).

### SO-E3 type-45 score-history serialization (2026-09-25)

The production file module now has `stars_write_score_records`. It emits each
selected `_scorex` body unchanged as a 24-byte type-45 record. Selection
matches the retained `FWriteDataFile` predicate: player logs only; include
the current player, visible players, dead players, and all players after
turn 19 when the victory-history flag is set. Game HST output has no type-45
section.

The strict-C89 source probe passes six checks for raw-byte preservation,
visible/dead/current-player selection, the player-log-only rule, and the
turn-19/20 boundary. The selector replay passes 1/1 in 11.263 seconds, the
writer-order test passes 1/1, and the expanded ten-module SO-E3 suite passes
49/49 in 114.801 seconds. `tools/build_core.py` and
`tools/baseline.py --check` pass.

The selector's data-file callback now invokes `stars_write_score_records` for
each output. It emits one retained 24-byte score body for M1 and one for M2
in both control and action, and emits no type-45 record for HST. The combined
selector/pair/score-writer regression passes 28/28 after this integration.

This binds serializer selection in the turn coordinator, but not score
values: the score bodies still come from retained output fixtures and
`turn_update_scores` remains a no-op. Complete HST/M1/M2 files are not yet
assembled or compared. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).** See the
[type-45 score-history oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-type-45-score-history-selection-2026-09-25).

### SO-E3 score calculation correction (2026-09-25)

While preparing to generate score-history values, comparison with the original
`CalcPlayerScore` export exposed a mismatch in `stars_calc_player_score`.
The original adds three points per owned planet whose starbase hull has cargo
capacity; the prior helper instead multiplied that planet's mineral score by
three when a generic `developed` flag was set. The helper now takes an explicit
`score_starbase` input, reports the qualifying starbase count, and sums the six
technology levels into the score summary. Its score remains based on the
original mineral, resource, race, design-power, and fleet-category stages.

The full common-source suite passes 221/221, the retained selector-1 SO-E3
test passes 1/1, and both save-writer source tests pass. The core build and
baseline check pass. This corrects a production scoring primitive but does not
yet bind player/planet/fleet/design state into `turn_update_scores` or emit
current `_scorex` values. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential gate. TODO7: 39 open (29 evidence, 10 promotions).** See the
[score-calculation audit](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#calcplayerscore-starbase-bonus-correction-2026-09-25).

### SO-E3 type-45 score-summary packing (2026-09-25)

The production `stars_pack_score_history_record` now packs a current
`StarsScoreSummary` with caller-maintained flags/rank into the 24-byte
type-45 `_scorex` body. The packed control and action bodies match the retained
M1 records byte-for-byte, including score 25, resources 38, one planet, one
starbase, technology 18, and the `[4,2,0]` / `[4,1,0]` fleet-history lanes.

The combined score, retained-pair, and writer regressions pass 29/29.
`tools/baseline.py --check`, JSON parsing, the core build, and
`git diff --check` pass. This adds a source-backed record packer; the
`turn_update_scores` callback still lacks complete current state inputs and
does not produce turn-1 type-45 bodies. The full HST/M1/M2 turn differential
remains open. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential
component left. TODO7: 39 open (29 evidence, 10 promotions).** See the
[score-history packing oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#production-packing-for-type-45-score-history-2026-09-25).

### SO-E3 production turn-zero anti-cheating stage (2026-09-25)

The selector-1 coordinator now invokes production
`stars_spank_the_cheaters` at turn 0. The production gate exits before the
fleet and planet penalty loops; the callback verifies the RNG seeds/call count
stay unchanged and no penalty message is emitted. The strict-C89 integrated
selector test passes across 100 injected outer random values.

This removes one no-op turn stage for the retained pair. Other service stages,
complete state loading, and full HST/M1/M2 generation/comparison remain open.
**SO-E2: 0 checks left. SO-E3: 1 full-turn differential component left.
TODO7: 39 open (29 evidence, 10 promotions).** See the
[SpankTheCheaters integration note](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-turn-zero-spankthecheaters-stage-2026-09-25).

### SO-E3 production race-gated turn services (2026-09-25)

The selector-1 coordinator now calls production transit breeding and
auto-terraforming. Turn-0 HST player records provide race stat `0x0e` values
`[9,1]`, which enable neither service. Both return before fleet/planet
processing; the integration callbacks confirm no RNG calls or seed changes.
The strict-C89 selector test and retained-pair suite pass 27/27, including 100
injected outer `Random(100)` values.

This removes two no-op turn stages for the retained pair. Other service
callbacks, state loading, and complete HST/M1/M2 generation/comparison remain
open. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component
left. TODO7: 39 open (29 evidence, 10 promotions).** See the
[race-gated service note](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-turn-zero-race-gated-services-2026-09-25).

### SO-E3 production mine-sweeping stage (2026-09-25)

The selector-1 replay now calls production `stars_sweep_for_mines` with the
current 8 control / 7 action fleets, all 32 planets, and all four moved HST
things. The retained things are all wormholes (kind 2), so production skips
them at the minefield-kind check. The test confirms no attack, message, or
free callback and no change to thing activity or owner masks. Planet ownership
comes from input HST and is checked against both turn-1 outputs; map coordinates
come from `CLEAN.XY`. Positive power witnesses exercise the production scan;
their exact game calculations remain outside this slice.

The focused selector test passes for control/action and 100 injected outer
random values. The combined selector/pair suite passes 27/27; core build,
baseline, oracle JSON, and whitespace checks pass. This replaces one no-op
stage but does not close SO-E3. Fuel, healing, remote terraforming, current
score generation, other full-state services, and complete HST/M1/M2 comparison
remain. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component
left. TODO7: 39 open (29 evidence, 10 promotions).** See the
[mine-sweeping stage note](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-mine-sweeping-on-the-retained-pair-2026-09-25).

### SO-E3 all 32 HST planet inputs (2026-09-25)

The selector-1 loader now reads all 32 turn-0 HST type-13 planet records
through production `stars_read_planet` and stores current planet state by ID.
HST serialization uses that table for all 32 planet records instead of
re-reading unowned planets from the retained input during the writer. Player-1
visibility works on a copy, so its planet-26 visibility bits do not leak into
the HST output. The two seeded HST outputs match all 32 retained turn-1 planet
bodies byte-for-byte; the probe asserts `HST_INPUT_PLANETS=32/32` and
`PLANET_RECORDS=32/32`.

The focused selector test passes for control/action and 100 injected outer
`Random(100)` values; the combined selector/pair suite passes 27/27. Build,
baseline, JSON, syntax, and whitespace checks pass. This closes HST planet
loading and planet-record serialization only. Other input records, player-log
state, service boundaries, complete HST/M1/M2 assembly, and full decoded
comparison remain open. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**
See the [full-turn oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-loading-of-all-hst-planet-inputs-2026-09-25).

### SO-E3 M1/M2 player-log planet inputs (2026-09-25)

The player-log load callback now decodes M1 planets 7, 10, and 12 and M2
planet 24 through production `stars_read_planet` before turn processing. The
owned planet 10 and 24 inputs are checked against their loaded HST state. M1's
type-14 planets 7 and 12 are serialized from the loaded player-log states;
planet 10 and M2 planet 24 use their current production-updated shared state.
All 5/5 M1/M2 output planet bodies still match, with explicit input assertions
`M1_INPUT_PLANETS=3/3` and `M2_INPUT_PLANETS=1/1`.

The selector test passes for control/action and 100 injected outer
`Random(100)` values; the combined selector/pair suite passes 27/27. This binds
these player-log planet inputs only. Other input sections, complete HST/M1/M2
assembly, and the full decoded differential remain open. **SO-E2: 0 checks
left. SO-E3: 1 full-turn differential component left. TODO7: 39 open (29
evidence, 10 promotions).** See the [full-turn oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-m1-and-m2-planet-inputs-2026-09-25).

### SO-E3 retained M2 SHDEF records (2026-09-25)

The owner-1 `load_player_log` path examines all four retained turn-0 type-26
SHDEF records in stream order. Designs 0, 1, and 2 decode through production
`stars_read_shdef`; design 2's decoded cost fields are checked before order
processing. Design 16 refers to hull 34, which the retained type-0x4000 hull
lookup cannot resolve because it accepts item IDs only through 31. The probe
records this as one unresolved SHDEF input and does not invent a hull record.
At task-5 pass 1, the production scrap adapter uses loaded design 2 and the
already-loaded M2 planet 24 state rather than reparsing those inputs during
the order pass. The isolated non-turn scrap probe loads the same three
resolvable designs.

The selector test passes for control/action and 100 injected outer
`Random(100)` values; the combined selector/pair/score-writer run passes
28/28. The probe
reports `M2_SHDEFS=3/4 INVALID=1`. This binds the three resolvable designs;
design 16/hull 34, other player-log records, complete HST/M1/M2 assembly, and
the full decoded differential remain open. **SO-E2: 0 checks left. SO-E3: 1
full-turn differential component left. TODO7: 39 open (29 evidence, 10
promotions).**
See the [full-turn oracle](analysis/compat/satisfy-orders-full-turn-e3-original-oracle.md#source-bound-m1-and-m2-planet-inputs-2026-09-25).

### SO-E3 full-turn acceptance audit (2026-09-25)

I compared every retained turn-1 output stream with the probe's current writer
coverage. The original outputs contain 32/30 records in M1, 24/24 in M2, and
83/81 in HST for control/action. Counts below are `control/action`.

| Record kind | M1 | M2 | HST | Current probe evidence |
|---|---:|---:|---:|---|
| 0 turn marker | 1/1 | 1/1 | 1/1 | Written and checked |
| 6 player record | 1/1 | 1/1 | 2/2 | Not written; player-file callback is a no-op |
| 8 | 1/1 | 1/1 | 1/1 | Not assembled or compared |
| 12 | 1/1 | — | — | Not assembled or compared |
| 13/14 planets | 1+2 / 1+2 | 1+1 / 1+1 | 32/32 | Output bodies checked against retained records |
| 16/20 fleet/order | 6/5 each | 2/2 each | 8/7 each | Output bodies checked against retained records |
| 26 ship designs | 7/7 | 8/8 | 15/15 | Output designs not written or compared; the source input loader now initializes and checks all 44 design records across both input sets |
| 28 production queue | — | 1/1 | 1/1 | One planet-24 queue case; 32 base resources are now source-derived and 15% research leaves 28 |
| 30 | 5/5 | 5/5 | 10/10 | Not assembled or compared |
| 43 things | — | — | 5/5 | HST count and four thing records checked |
| 45 score history | 1/1 | 1/1 | — | Production selector runs, but body values are retained fixtures |

This confirms the concern at the acceptance-gate level: many passing
tests verify useful slices, but they do not reduce SO-E3 because the
probe does not assemble or compare complete streams. The current
selector regression now derives planet 24's production pool from the
pre-production population and research state; the original retained-witness
shortcut has been removed from that probe (details below). Unhandled kinds
6, 8, 12, 26, and 30,
fixture-backed scores, and
incomplete runtime design initialization are concrete additional gaps. The earlier M2 design-16/hull-34 lookup failure is now resolved; see the Gate 1 input-loading entry below. The next
work must complete the four gates in the SO-E3 row above.
**Remaining: SO-E2: 0 checks. SO-E3: 1 acceptance item / 4 grouped
closure gates. TODO7: 39 open (29 evidence, 10 promotions).**

### SO-E3 planet-24 production resource binding (2026-09-25)

The selector's planet-24 production input no longer uses the retained
28-resource output as its input. The retained Turin supply path now feeds its
current-source 250→225 colonist transfer into `CResourcesAtPlanet`; the
runtime planet and player-stat values are decoded from the retained HST
input. At 225 colonists, 10 factories, environment stat 10, and factory stat
10, production-source `stars_cresources_at_planet` returns 32. The retained
player's `pctResearch=15` and clear planet bit 23 then make `Produce` reserve
4 for research and pass 28 to the queue. The result is the retained 81%
design-0 progress and unchanged mineral stock; the player cheater flag is 0.

`StarsProducePlanet` now carries the research percentage and planet
exemption bit. `stars_produce` applies the original cheater scale first,
deducts the research share unless the planet exemption bit is set, and tracks
research plus leftover resources instead of counting the full pool twice.
The separate 35→28 fCheater regression remains as a check of that branch.

The focused production, Turin seed/production, four-pass, and selector
regressions pass **10/10 in 70.95 seconds**. The selector test covers its 100
outer random draws plus control/action seed cases. This closes the resource
budget subissue only; current selector/fleet fixtures, incomplete stages,
unwritten record kinds, and absent complete HST/M1/M2 differential remain.
**Remaining: SO-E2: 0 checks. SO-E3: 1 acceptance item / 4 grouped closure
gates. TODO7: 39 open (29 evidence, 10 promotions).**


### SO-E3 Gate 1 input boundary and player initialization (2026-09-25)

Gate 1 remains open. The current input boundary in
[`tools/satisfy_orders_input_corpus.py`](tools/satisfy_orders_input_corpus.py)
checks each retained input against its own run manifest, consumes every framed
record through `stars_read_record`, and preserves the XY coordinate section.
It reads only the starting XY/HST/M1/M2 files, with the manifest recording
absent starting X2/H2. It does not read turn-1 outputs.

Both runs contain 82 HST, 31 M1, and 21 M2 input records, plus three XY records
and 32 raw coordinate entries. Across both runs this is **274 records and 64
coordinates**. The action HST has one type-19 order and nine type-20 orders;
the control has ten type-20 orders. Complete initialization must preserve
that distinction. Framing/decryption coverage is not runtime-object coverage.
The [record inventory](analysis/compat/satisfy-orders-e3-input-corpus.json)
retains every input path/hash, record offset/type/body hash, coordinate,
and initialized player-state hash.

All eight type-6 input records (two HST players and one player in each M file,
per run) are initialized as full 192-byte player records through
`stars_read_player_record`. An independent wire/name decoder checks all bytes,
including relations, both names, and the cleared runtime flag. The selector
probe invokes the player reader itself from retained input bodies: two HST
reads, one M1 read, and two M2 reads (AI load and subsequent turn-log load).
AI personality now comes from the loaded player; coordinator player count
and turn come from the decoded game header and HST BOF.

The former M2 design-16/hull-34 rejection was a lookup adapter defect:
`LphuldefFromId` routes hulls 32–36 to the separate starbase table. The probe
now includes that original table, stores starbases in a separate bank, and
reads all four M2 designs without overwriting ship design 0. The source cost
adapter also selects the starbase table. A dedicated regression checks all
five original starbase hull costs and rejects hull 37 without writing output.
This establishes lookup/bank coverage; complete SHDEF field fidelity,
including names, flags, and component normalization, still needs verification.

Remaining Gate 1 work: initialize every remaining record group into complete
runtime state, verify both HST order formats, bind all HST/M1/M2 designs,
battle plans, messages and things, then audit every consumed record against
its initialized object and remove the remaining minimal/synthetic loading.
The general loader's dispatch also needs validation against these real input
streams before it can replace the probe's selected-record callbacks.

Verification: **8 tests passed, 1 skipped** across
`tests.test_satisfy_orders_input_corpus`,
`tests.test_satisfy_orders_turin_binding`, and
`tests.test_satisfy_orders_shdef_reprice`. Original-instruction player
comparison is skipped on this macOS ARM host because its Unicorn execution
is disabled; this skip is not counted as original-machine verification.
**Remaining: Gate 1: 1 acceptance item. SO-E3: 4 grouped gates open.
SO-E2: 0 checks. TODO7: 39 open (29 evidence, 10 promotions).**

### SO-E3 Gate 1 complete design, battle-plan, and thing states (2026-09-25)

The complete-input loader now initializes **8 player records, 44 design
records, 40 battle plans, and 8 things** across the two pinned input sets.
All 274 framed records and 64 XY coordinates are still retained. The
[generated inventory](analysis/compat/satisfy-orders-e3-input-corpus.json)
marks each record's runtime group and retains the design-bank hashes,
battle-plan bytes, and thing bytes. Reproduce it with:

    python3 -m tools.satisfy_orders_input_corpus --output analysis/compat/satisfy-orders-e3-input-corpus.json

The production `stars_read_shdef` now assembles the saved design into a
zeroed 147-byte candidate, preserves the full saved flag word, handles
partial-design names at the correct wire offset, and checks fFree at bit 9.
It decodes names, resolves the low-byte component item separately from the
high-byte quantity, applies original slot limits and engine fallback, and
uses actual component mass. The former hull-default copy and eight-byte
part-prefix mass interpretation were incorrect. Both affected integration
probe adapters now supply the original mass field and the source name codec.

`stars_initialize_shdef_cost` binds the remaining loader initialization:
source repricing, hull/installed-part mass, damage points including
regenerating-shield armor adjustment, and the -1 power-cache sentinel.
`tools/stars_design_input.py` assigns records in each player's saved ship
and starbase counts/order and retains separate complete banks, including
all unused entries marked free. The 44 occupied entries compare in full
against an independent wire/EXE-table oracle; a separate case verifies
partial-information retention and replacement when fFree is set.

Battle plans use `stars_unpack_battle_plan`; all 40 complete headers and
names match the pinned default plans. The eight 18-byte THING structures
retain the original direct-copy layout and pass source table lookup.
The older merge/production regression was also updated to the already
established 32-resource input with 15% research and four research resources;
its previous last-year-resource expectation predated that source correction.

Verification: **13 tests passed, 1 skipped** across the input-corpus,
selector, merge/scrap integration, and repricing tests. The strict-C89
standalone SHDEF reader probe also passes. Original x86 player execution
remains skipped on this host; these results do not claim new native guest
or original-machine execution.

**154 object records remain uninitialized in the complete-input loader:**
72 planets, 80 fleet/order records (including the action's type-19 order),
and two message records. The existing selector's focused readers remain
supporting work; the complete loader must initialize those groups and then
replace the coordinator's minimal/synthetic load state. Full integration
and record-by-record initialization coverage must pass before Gate 1 closes.
**Remaining: Gate 1: 1 acceptance item. SO-E3: 4 grouped gates open.
SO-E2: 0 checks. TODO7: 39 open (29 evidence, 10 promotions).**

### SO-E3 Gate 1 complete fleet/order input states (2026-09-25)

The complete-input loader now initializes all **40 fleets and 40 order
records** across the two pinned input sets through `stars_read_fleet`.
`tools/stars_fleet_input.py` bounds every ship, cargo, damage, and order
span; checks ownership, duplicate identities, and orphan orders; and supplies
the matching XY coordinates and BOF turn. Each runtime fleet retains its
original identity word, owner, and contributing record offsets. All fields
of the source fleet state, including unused order storage, are checked
against an independent wire decoder. The action's 18-byte type-19 task-5
order is loaded from its own HST and retains its complete task payload;
compact type-20 orders are expanded into zeroed 18-byte runtime entries.

The audit also corrected `stars_read_fleet`: `fNoAutoTrack` is bit 13 of
the ORDER word at offset 6, as recorded in the original CodeView layout.
The old implementation cleared bit 5 and could change the warp. It now
clears byte 7's bit 5; a separate regression verifies that warp 7 survives
while the no-auto-track flag is cleared. Malformed order sequences and
unconsumed fleet tails are rejected.

Verification: the complete-input and selector-binding suites report
**11 passed, 1 skipped** (12 tests). The skip remains the original x86
player comparison disabled on this macOS ARM host. These are host tests,
not new guest execution. The regenerated input inventory accounts for
the 80 fleet/order records by their actual initialized record offsets.

**74 object records remain: 72 planets and 2 messages.** Coordinator
integration and the final record-to-runtime audit also remain. The planet
source review identified another pending correction: first-year handling
currently tests the planet owner, whereas original `FReadPlanet` tests
the loading player. That difference must be resolved with planet loading,
including discovery-message side effects, before those records are counted
as initialized. Gate 1 and all **4 SO-E3 grouped gates remain open**.

### SO-E3 Gate 1 complete planet and message input states (2026-09-25)

The complete-input loader now initializes all **72 planet records and both
saved message records**. Its regenerated inventory reports **0 uninitialized
object records** across the 274 framed records and 64 XY coordinates. This
does **not** close Gate 1: the complete loader still needs to replace the
partial coordinator's selected-record loading, followed by a final audit.

`tools/stars_planet_input.py` bounds each complete planet record and invokes
`stars_read_planet` with the correct loading context: -1 for HST, 0 for M1,
and 1 for M2. Every resulting state field is checked against an independent
wire-to-runtime oracle, including runtime flags, mineral levels/stocks,
environment/original environment, guesses, improvements, starbases, and
routing. Truncated records, extra tails, and invalid environments fail.

The source reader's first-year test now uses the loading player, matching
original `FReadPlanet`. Its discovery-message path now multiplies
habitability by true maximum growth and places the growth amount and planet
ID in the correct argument positions. Both M1 views discover planets 7/12
with source habitabilities 33/17 and message 0xAC. The source callbacks use
the complete loaded player records. The optimal-terraform callback is not
needed by these inputs and fails explicitly if reached by another input.
The supporting selector probe now distinguishes host saved-state flags from
fresh client first-year flags; its host serializer no longer depends on the
reader's former owner/loader confusion.

`tools/stars_message_input.py` obtains the 387-entry argument-count table
from the SHA-pinned original executable (logical NE segment 7, offset 0x5B0E).
The source `ReadPlayerMessages` adapter now skips the full four-byte MSGHDR,
including its goto word. Each retained M1 message record initializes five
saved messages; the planet callbacks append two discoveries through source
`stars_send_player_message2_xgen`. All seven decoded messages, exact packed
buffer bytes, sent bits, and initial selection are checked. HST/M2 message
tables remain empty. Incomplete headers/arguments fail before entering C.

Verification: **15 tests passed, 1 skipped** across the complete-input and
selector-binding suites. The standalone strict-C89 planet reader and
message reader probes also pass. The skip remains original x86 player
execution on this host; no new native guest execution is claimed.

**Remaining: 0 uninitialized object records; 2 Gate 1 work areas remain:
coordinator integration and the final record-to-runtime coverage audit.
Gate 1 remains open, and all 4 SO-E3 grouped gates remain open.**

### SO-E3 Gate 1 coordinator input bridge and load finalization (2026-09-25)

`tools/satisfy_orders_input_bridge.py` now exports both source-initialized
input sets into portable C89 aggregate initializers. It accepts only the
complete-input loader's results, with no turn-1 oracle access. The mutable
coordinator state retains separate HST/M1/M2 views containing complete
players, normal/starbase design banks and occupancy, battle plans, planets,
fleets/orders, things, message bytes/sent bits/selection, game metadata, and
XY coordinates. A compiled bridge regression compares every initialized
field/table for both runs against the loader, including the action order.

The selector coordinator now loads players, HST/M1/M2 planets, M2 designs,
M2 fleets/orders, owner-0 HST fleets/orders, coordinates, and wormholes from
that state. Its old duplicate selected-record parsers and the redundant
hand-built fleet initialization were removed. The action task-5 path reads
its fleet, expanded order, and design from the action HST state. Movement
starts from the current loaded fleet coordinates/fuel/orders. `DoOrders`
now receives all 32 host planets, replacing the artificial two-entry table
that incorrectly assigned planet 24 to owner -1.

The integration audit caught a further `FLoadGame` requirement: saved player
counts must be rebuilt from runtime objects. HST's saved planet counts are
32/0, and M1's saved count is 3; loaded ownership is one planet per included
player. The loader now retains both the raw `ReadRtPlr` result (for wire
verification) and finalized runtime players, rebuilding planet, fleet,
ship-design, and starbase-design counts. The C coordinator receives the
finalized players. Every other player byte is checked unchanged for this
corpus. The generated inventory now distinguishes these two player states.

Verification: **17 passed, 1 skipped** across the complete-input and selector
tests, including complete C bridge field comparisons and both seeded
control/action replays. Finalized player counts and bridge transfer were
also checked after adding starbase-count reconstruction. Original x86
player execution remains the sole skip. This is host verification.

**Remaining: 0 uninitialized object records; 2 Gate 1 work areas remain:
finish reviewing the coordinator's remaining initialization adapters and
complete the final record-to-runtime/load-finalization audit.** The bridge
and passing replay do not certify the remaining turn services or output
fixtures; those remain Gate 2–4 work. Gate 1 has not yet been marked closed.

### SO-E3 Gate 1 closed: both complete inputs loaded (2026-09-25)

**Gate 1 is closed.** The [requirement-by-requirement audit](analysis/compat/satisfy-orders-e3-gate1-audit.md)
supersedes the earlier open-Gate-1 progress entries. Both pinned control/action
input sets are source-initialized and transferred to complete mutable C
coordinator state. Every one of the **274 records and 64 XY coordinates** is
accounted for; the [inventory](analysis/compat/satisfy-orders-e3-input-corpus.json)
now includes a concrete runtime reference for every framed record.

The final review checked distinct XY/save EOF semantics, stamped THING turns,
finalized player counts, host/client planet flags and discovery messages,
complete C field/table transfer, and the coordinator's load adapters. Resource
planet preparation and AI research initialization now use loaded fields.
The action task-5 path uses its own initialized HST fleet/order/design.

Final verification: **19 passed, 1 skipped** (20 tests) across
`tests.test_satisfy_orders_input_corpus` and
`tests.test_satisfy_orders_turin_binding`, including the complete compiled C
bridge comparison and both seeded coordinator replays. The sole skip is
original x86 player execution through Unicorn on this host; no new native
guest execution is claimed.

**Remaining: Gate 1: 0 work areas. SO-E3: 3 grouped gates open (2, 3, 4).**
SO-E3 remains one open acceptance item. The remaining gates cover complete
source-bound turn execution, complete HST/M1/M2 serialization, and every-record
comparison against the matching originals. TODO7 remains **39 open** because
closing this workload gate does not close the SO-E3 acceptance item.

### SO-E3 Gate 2 tracking and full-map guess binding (2026-09-25)

Gate 2 now has a [numbered closure tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md):
**14 work packages, 1 complete, 13 remaining**. It covers AI, orders/logs,
movement, production/research, nested events, things, fuel, healing,
terraforming, guesses, scores, messages/player writes, RNG, and the final
whole-pipeline audit. These are workload counts; SO-E3 remains one open
acceptance item and **3 grouped gates remain open**.

**G2-10 is complete for the pinned pair.** `UpdateGuesses` now receives all
32 current runtime planets instead of two turn-1-derived initializer structs.
Population, owner, defenses, and initial guess words come from current state;
race/technology and defense-service inputs come from loaded players and
source part tables. All 32 results return to the runtime world. The old
retained guess-planet structs and packed-record initializers were removed
from the generated header. Original output guess words remain assertions.

Verification: **19 passed, 1 skipped** across the complete-input and selector
suites (20 tests). Both seeded replays report `GUESS_INPUTS=32/32`, reproduce
the original guess words/survival values, and retain their 57/39 main-stream
draw traces. The sole skip remains original x86 player execution on this
host. **Gate 2 remains open: 13 packages left.**

### SO-E3 Gate 2 healing source correction (2026-09-25)

Original-code inspection found that the existing healing service treated the
no-heal flag as a repair-rate selector and omitted several original rules.
The [healing audit](analysis/reconstruction/heal-ships.md#original-rule-audit-correction-2026-09-25)
records corrections for eligibility, movement/orbit/starbase rates, race and
repair-ship bonuses, and packed damage saturation. The expanded strict-C89
host source probe passes all 22 conditions. The historical native report
predates this correction.

The coordinator healing stage still needs binding and integrated verification.
**Gate 2: 13 of 14 packages remain; SO-E3: 3 grouped gates remain open.**

### SO-E3 Gate 2 healing binding complete (2026-09-25)

**G2-08 is closed; 12 of 14 Gate 2 packages remain.** The coordinator calls
corrected source `stars_heal_ships` over all surviving control/action fleets
(8/7) and all 32 current planets. Loaded race data, current designs, original
hull tables, and movement flags determine repair eligibility and rates.
Damage propagates into host fleet state, owner-1 raw fleets, current planets,
and fleet serialization. Owner-1 input loading now retains damage words too.

Both seeded replays retain exact original fleet records and RNG traces. A
separate copied-state test proves nonzero damage write-back for both owners
and a starbase; it does not alter either oracle replay. The input/selector
suite passes 19 tests with 1 existing skip; the final selector run including
the mutation check passes both seeded cases and all 100 scout-roll cases.
See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-08-evidence).

**SO-E3: 3 grouped gates remain open.** Fuel, the remaining services, and the
whole-pipeline provenance audit still prevent Gate 2 closure.

### SO-E3 Gate 2 refueling binding complete (2026-09-25)

**G2-07 is closed; 11 of 14 Gate 2 packages remain.** Source `FuelFleets`
replaces the coordinator no-op for all surviving fleets (8 control / 7 action).
Current designs/counts and original hull tables feed source capacity services;
current starbases and loaded directional relations determine refill access.
Transport hulls and generator components supply fuel production. Fuel is
written back into movement, host fleet, and owner-1 raw state.

Both seeded replays and all 100 scout-roll cases pass, preserving original
fleet records and RNG traces. Separate copied-state checks cover allied-base
permission direction, orbital-fort exclusion, fuel generation, capacity
saturation, and both owners' write-back. The input/selector suite passes
19 tests with 1 existing skip; the eight-condition strict-C89 fueling probe
also passes. See the [fueling audit](analysis/reconstruction/fuel-fleets.md)
and [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-07-evidence).

**SO-E3: 3 grouped gates remain open.** Terraforming, other incomplete turn
services, and the final provenance audit remain Gate 2 work.

### SO-E3 Gate 2 terraforming/race-gate audit complete (2026-09-25)

**G2-09 is closed for the pinned pair; 10 of 14 Gate 2 packages remain.**
Automatic terraforming and transit breeding now use loaded race statistics
and complete current planet/survivor tables. The loaded races 9/1 take the
original early returns; the retained race initializer and dummy empty tables
were removed. Newly eligible races fail explicitly until their nested effects
are bound.

Remote terraforming now calls source `stars_remote_terraform`, with capability
derived through source `stars_terra_from_fleet` from every survivor's current
design components and ship counts. All 8/7 survivors have zero capability;
therefore the original source requires no remote environment/message effects.
All 32 planet environments and the shared RNG remain unchanged. Separate
copied-state checks prove the component adapter detects added terraforming
capacity and that newly eligible race paths are rejected.

The input/selector suite passes 19 tests with 1 existing skip (15.049 seconds),
including both seeded runs and all 100 scout-roll cases. See the
[Gate 2 audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-09-evidence).
**SO-E3: 3 grouped gates remain open.**

### SO-E3 Gate 2 score-rule audit correction (2026-09-25)

The original score procedure counts technology categories meeting victory
setting 1 and compares that count with setting 2. The reconstructed procedure
and earlier E2 test oracle incorrectly compared total technology with setting
1. The source, packed adapter, and oracle are corrected; three additional
source cases distinguish equal technology totals with different category
coverage. Current host probes pass **37/37 source, 6/6 packed, 36/36 E2**.
Historical native reports predate the fix. Details are in the
[score audit](analysis/reconstruction/update-player-scores.md#technology-victory-correction-2026-09-25).

**G2-11 remains open; 10 of 14 Gate 2 packages remain.** Current-world score
calculation, player/metadata effects, and replacement of retained score writer
bodies still need binding. **SO-E3: 3 grouped gates remain open.**

### SO-E3 Gate 2 current design power and score categories (2026-09-25)

The score-stage no-op is replaced with current design preparation: all 32
ship-design slots pass through hidden/free eligibility and reconstructed
combat-speed/design-power calculation with original part data. Original
inspection corrected the speed service's confusion between engine count and
maneuver bonuses, zero-count slots, and weight divisor. Its host probe passes.

Current surviving ships reproduce all three original score-history ship-count
categories for both players in both seeded runs. The input/selector suite
passes 19 tests with 1 existing skip; the final selector with category checks
also passes. See the [tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-11-design-power-progress-still-open)
and [speed audit](analysis/reconstruction/spd-of-ship.md#gate-2-design-power-dependency-audit-2026-09-25).

**G2-11 remains open; 10 of 14 Gate 2 packages remain.** Planet/resource/tech
score totals, current player metadata, and replacement of retained score
writer bodies are still required. **SO-E3: 3 grouped gates remain open.**

### SO-E3 Gate 2 current-world score binding complete (2026-09-25)

**G2-11 is closed; 9 of 14 Gate 2 packages remain.** Current planets, source
resource/habitability/population-limit services, current ship counts/design
power, and loaded technology feed production score calculation. Loaded game
victory settings feed source rank/achievement calculation. Player rank is
written back to PLAYER.wScore, and fresh score-history records feed the writer.
Retained score bodies are now comparison oracles only.

Both player records match all 24 bytes in each control/action run. Separate
copied-state changes to population and technology change generated scores,
proving the runtime dependency. No death/victory effects apply to this pair;
unexpected effects fail explicitly. Shared RNG seeds remain unchanged.

The input/selector suite passes 19 tests with 1 existing skip (15.082 seconds),
and the final selector/rank-writeback regression passes both seeded runs and
all 100 scout-roll cases (11.744 seconds). See the
[closure audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-11-closure).
**SO-E3: 3 grouped gates remain open.**

### SO-E3 Gate 2 both-player resource budgets (2026-09-25)

Production now derives both owned planets' resource budgets with source
habitability, population-limit, factory, and resource services using loaded
player data. The owner-0 zero-budget fallback is removed: planet 10 credits
35 through the null-queue branch. Planet 24 still derives 32 before research,
with 4 allocated and 28 available for production. Loaded player fields now
supply research percentage and cheater eligibility. Credits propagate into
mutable PLAYER.lResLastYear (35/4).

The input/selector regression passes **19 tests with 1 existing skip**
(15.296 seconds), including both seeded cases and all 100 scout-roll cases.
See the [production tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-04-production-resource-progress-still-open).
**G2-04 remains open; 9 of 14 Gate 2 packages remain.** Queue/build/finish,
full research effects, and remaining state propagation still need binding.

### SO-E3 Gate 2 current production costs (2026-09-25)

Production now reads build costs from the current owner/design bank through
source `stars_get_true_hull_cost`. The retained cost array and its oracle-based
initializer are removed. Both original replays still reproduce the queue's
costs and progress. Separate copied-state checks change all four cost words
for both owners and verify the adapter reads those changes. Queue habitability
now uses current planet environment and loaded player state too.

The input/selector suite passes **19 tests with 1 existing skip** (15.080
seconds), including both seeded runs and 100 scout-roll cases.
**G2-04 remains open; 9 of 14 Gate 2 packages remain.** See the
[production tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-04-current-build-cost-progress-still-open).

### SO-E3 Gate 2 full-map mining inputs (2026-09-25)

Mining now runs over all 32 current host planets. Source mine-capacity services
and loaded race attributes derive operating counts and efficiency; current
planet flags, concentrations, depletion levels, and stockpiles supply the
remaining inputs. Task-5 minerals and transport population are synchronized
before mining. Eight retained mining-input arrays have been removed.

Both seeded replays preserve original owned-planet yields and all six rounding
draws. The input/selector suite passes **19 tests with 1 existing skip**
(15.262 seconds). See the [mining audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-04-full-map-mining-input-binding-still-open).
**G2-04 remains open; 9 of 14 Gate 2 packages remain.**

### SO-E3 Gate 2 current growth inputs and finalization prerequisite (2026-09-25)

Growth now reads current planet population/fractions/environment and loaded
race settings, replacing fixture constants. Both original population endpoints
and score records still match. Input/selector verification passes **19 tests
with 1 existing skip** (15.145 seconds).

Queue-finalization inspection found that `FinishProduction` conflated player,
planet, and AI mode. The source API now distinguishes them, preserves separate
tutorial flags, and correctly sets or clears the cancel-path planet flag.
Its expanded strict-C89 source probe passes four cases. Turn binding remains
unfinished; historical native reports do not validate this correction.

**9 of 14 Gate 2 packages remain. G2-04 remains open.** See the
[production tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-04-growth-binding-and-queue-finalization-audit-still-open).

### SO-E3 Gate 2 queue-finalization binding (2026-09-25)

The production queue now passes through corrected source `FinishProduction`.
Its save/reload callbacks install the current queue, which production then
consumes. Pending ownership is cleared, planet ID and player ID remain distinct,
and loaded AI mode suppresses UI work. The partial-build path explicitly
rejects a completed build whose creation effects are not yet bound.

Both original replays retain queue progress and output-record matches.
Input/selector verification passes **19 tests with 1 existing skip**
(15.193 seconds), including all 100 scout-roll cases. See the
[queue audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-04-queue-finalization-binding-still-open).
**G2-04 remains open; 9 of 14 Gate 2 packages remain.** Production research
and complete AI queue lifecycle integration are still required.

### SO-E3 Gate 2 production research binding (2026-09-25)

The integrated selector now writes the AI research decision back to the host
player and binds Produce's research callback before random events. Current
planet resource credits reach the source research service before it runs.
The service uses loaded player/race/game fields and original executable
research-cost tables through `stars_get_tech_level_cost`. It spends 35/4
resources and derives player 1's 8-point intelligence-sharing credit.

All 36 technology/research/resource bytes for both players match each
original control/action HST. The sharing callback is retained; complete raw
message integration remains open. Neither pinned run reaches a breakthrough;
unbound level-up effects fail explicitly. Full queue/lifecycle integration
still requires audit. See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md#G2-04-production-research-handoff).

Focused host regression: **19 passed, 1 existing skip**, 16.119 seconds.
No new guest run. **Remaining: 9/14 Gate 2 packages; 3 grouped SO-E3 gates;
SO-E2: 0 closure checks.**

### SO-E3 Gate 2 first-pass production queue (2026-09-25)

Removed the late Turin replay that previously supplied the production queue.
The first AI pass now derives queue eligibility from loaded planet state,
uses source `stars_add_item_to_queue`, and commits through source
`stars_finish_production` before AI log writing. Produce consumes that same
queue. Its input now contains all 32 current planets, with loaded ownership,
improvements, research fields, and mineral banks.

Both seeded turns and all 100 scout-roll cases pass: **19 tests passed,
1 existing skip**, 15.705 seconds. Queue outputs and research fields still
match original returns. Initialization side effects and final state propagation
remain to audit before closing the production package. Details are in the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Remaining: 9/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 production package closed (2026-09-25)

**G2-04 is closed for the pinned pair.** The [production closure audit](analysis/compat/satisfy-orders-e3-production-audit.md)
accounts for initialization, first-pass queue creation/commit, full-map mining
and production, resource budgets, partial construction, growth, research,
and final state propagation. Production banks and queue count now commit
before population/research/events, and the old post-event copy is removed.
Both scrap paths feed a current per-planet resource-credit table. Mining
propagates concentrations, depletion levels, and stockpiles.

The source-proven absent branches are documented and guarded; raw message
handling and nested events retain their separate open packages. Host regression:
**19 passed, 1 existing skip**, 15.971 seconds. No guest run.

**Remaining: 8/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 random-event package closed (2026-09-25)

**G2-05 is closed for the pinned pair.** All four dispatcher callbacks now
invoke current source event services with loaded game/current planet state.
The shared RNG produces control values 19/18/10 and action values 11/7/2 for
bounds 20/20/15; each source gate returns before nested effects. The trader
source returns at loaded turn 0 without randomness or allocation.

Corrected source meteor RNG ordering before the age check and climate
eligibility after turn 19, verified against original decompilation. Both
strict-C89 source probes pass with boundary cases. Focused integrated suite:
**19 passed, 1 existing skip**, 15.890 seconds. Earlier native probes predate
these changes. No new VM run. See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md)
for scope and source-proven skipped branches.

**Remaining: 7/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 current-world wormhole/decay inputs (2026-09-25)

Wormhole validation now uses current surviving fleets and loaded planet
coordinates. Removed runtime fleet-position arrays derived from original
outputs. Corrected the map-size argument from planet count to loaded
GAME.mdSize. Moved thing records write back to shared host state.

ThingDecay now visits all **8 control / 7 action survivors** and retains its
fleet-flag changes. Focused regression: **19 passed, 1 existing skip**,
16.012 seconds. G2-06 remains open for waypoint validation and the remaining
interaction audit; see the [tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Remaining: 7/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 waypoint validation binding (2026-09-25)

All three waypoint checkpoints now invoke source validation on current fleet
and order state. Coverage is 9 fleets/10 orders before orders execute, then
8/9 for control and 7/8 for action at both later checkpoints. Moved wormhole
state is published before validation. The pinned orders all target planets;
unsupported newly eligible retargeting paths fail explicitly.

Focused regression: **19 passed, 1 existing skip**, 15.484 seconds. Remaining
G2-06 work includes constant mine-sweeper powers and retained sweep fields.
See the [tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Remaining: 7/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 thing-service package closed (2026-09-25)

**G2-06 is closed for the pinned pair.** Sweep power now comes from current
fleet counts, ship/starbase designs, and original beam data through source
fleet/hull services. Relations, planet fields, and fleet flags use current
state. Nonzero adapter checks verify hull/fleet/starbase power 20/60/80,
missing-starbase zero, and directional relations.

The [tracker's closure audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md)
accounts for unmarking, pre/post movement, decay, waypoint validation,
sweeping, and state write-back. All four things are wormholes, so other
thing interactions are source-proven absent. Exact thing records and RNG
checks match both originals. Focused suite: **19 passed, 1 existing skip**,
15.395 seconds. No guest run.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 movement fuel binding (2026-09-25)

Movement now calculates its 2-unit fuel use through source EstFuelUse from
current ship mass/counts/cargo, loaded race flags, and original engine data.
Source capacity calculations replace capacity=current-fuel initialization.
Current packed orders and source distance replace the scoped order/distance
adapters. Increasing mass in a copied state increases fuel use.

Focused regression: **19 passed, 1 existing skip**, 15.488 seconds. G2-03
remains open for survivor/arrival/write-back auditing; see the
[tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 movement retirement state (2026-09-25)

Owner-0 task-5 dispatch now reads the loaded order; its resulting fleet
retirement flag and order bytes update shared runtime state. Movement uses
that flag instead of the control/action label. A copied-state check proves
label changes cannot alter survival and retirement-flag changes do.

Focused regression: **19 passed, 1 existing skip**, 15.498 seconds.
`git diff --check` passed. No new VM run. Owner-1 survivor enumeration,
arrival effects, and remaining movement write-back keep G2-03 open.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 complete owner-1 movement enumeration (2026-09-25)

Movement now examines all four owner-1 slots and derives survivors from
runtime presence/dead flags. Shared host and owner-1 packed fleet records
receive movement coordinates, planet, fuel, and here-all-turn flags before
later services. Fleet serialization reads current flags.

Focused input/Turin regression: **19 passed, 1 existing skip**, 15.551
seconds. `git diff --check` passed. No VM run. G2-03 remains open for
arrival/order semantics, movement limits, and remaining state propagation.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 movement limits and stationary skips (2026-09-25)

Movement limits now use the next waypoint's warp squared, verified at warp
5 and warp 3. Fleets without a next waypoint preserve their loaded planet
and skip movement; completed planet movement is not repeated in later passes.
The existing original endpoint/fuel comparisons pass for both runs.

The arrival audit identifies the next concrete gap: bind KillUsedWaypoints,
update the first waypoint, and replace direct second-slot selection in writers
with the resulting current order list. G2-03 remains open.

Focused regression: **19 passed, 1 existing skip**, 15.664 seconds;
`git diff --check` passed. No VM run.
**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 waypoint-source correction (2026-09-25)

The arrival audit found incorrect PLORD offset interpretation and unconditional
waypoint deletion in KillUsedWaypoints. The source now checks actual arrival,
copies ORDER 1 to ORDER 0 with all 18 bytes preserved, and uses the correct
order for retargeting/recycle/message checks. Updated source, integration,
packed, and turn host probes pass strict C89 compilation. Historical native
reports predate this correction and cannot validate it; the reconstruction
note now states that limitation explicitly.

The focused input/Turin regression also passes: **19 passed, 1 existing skip**,
15.360 seconds. `git diff --check` passed. No guest run. Actual coordinator
binding and downstream current-order consumption remain G2-03 work.

**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 runtime waypoint completion (2026-09-25)

The coordinator now performs the original first-waypoint movement update,
then invokes source KillUsedWaypoints with source DeleteWpFar. Current order
counts and all 18 order bytes write back to shared host and owner-1 packed
state. The scout's completed second waypoint is truly removed. Fleet writers
consume current waypoint zero; later validation sees 8/7 orders for 8/7
survivors. Both pinned original fleet/order comparisons pass.

The completion-message callback records owner 1's 0x4e fleet-0 event; full raw
message propagation remains G2-12. DeleteWpFar's last-waypoint recycle case
now follows the original fallback and has a passing strict-C89 source check.
The focused regression passes **19 tests, 1 existing skip**, 15.742 seconds;
`git diff --check` passed. No VM run.

G2-03 remains open for remaining movement side-effect/state-provenance audit.
**Remaining: 6/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 movement package closed (2026-09-25)

**G2-03 is closed for the pinned pair.** The [movement audit](analysis/compat/satisfy-orders-e3-movement-audit.md)
accounts for current survivors/orders, rounded travel, source fuel/capacity,
zero scoop gain and source-proven skipped effects, shared movement state,
and source waypoint completion. AI transport cargo now reaches the actual
fleet state; fleet writers consume current cargo/design-plan/order counts.
Movement direction, fDone, and here-all-turn state are committed downstream.
Both original fleet/order output comparisons pass.

Focused regression: **19 passed, 1 existing skip**, 15.863 seconds. The
DeleteWpFar source probe and `git diff --check` pass. No VM run. Historical
native waypoint reports predate the source correction and are not current
validation. Full raw message propagation remains G2-12.

**Remaining: 5/14 Gate 2 packages (01,02,12,13,14);
3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 AI current-design decisions (2026-09-25)

Scout capability and ideal warp now use current design state and original
hull/engine data; the generated fixture tables were removed. Latest-starbase
selection calls the source selector over the current starbase bank. The bank
now loads before AI execution and is preserved for subsequent stages.
Copied-state tests demonstrate changed capability/warp/latest choices and
verify later loader access does not restore old designs.

Focused regression: **19 passed, 1 existing skip**, 15.373 seconds;
`git diff --check` passed. No guest run. G2-01 still needs full startup
capability gates, design creation/commit, and remaining startup services.

**Remaining: 5/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**

### SO-E3 Gate 2 startup starbase slot gates (2026-09-25)

EnsureAiStarbaseDesigns now receives current SHDEF free-slot bits, loaded
GAME.turn, and current design metadata instead of the capability fixture and
constant turn. A copied-state source check verifies that occupying the
relevant slots suppresses creation requests. Both original seeded replays
retain the correct four startup name/RNG draws.

Focused regression: **19 passed, 1 existing skip**, 15.846 seconds;
`git diff --check` passed. No guest run. Full source-built SHDEF creation,
publication/log mutation, and remaining startup services keep G2-01 open.

**Remaining: 5/14 Gate 2 packages; 3 grouped SO-E3 gates; SO-E2: 0 checks.**


### SO-E3 Gate 2 original-template starbase construction (2026-09-25)

Startup now selects hull components through the reconstructed AI constructor,
AI part lookup, part lookup, and technology check, using original executable
templates/tables and current player state. Valid AI choice zero is accepted.
The four created starbases' component group/item/quantity lists match both
original M2 outputs; bitmap allocation uses occupied current designs and
matches 137,138,128,129 in creation order. Expected output lists are used only
in assertions. The integrated selector test passes in 12.292 seconds and the
strict-C89 constructor probe passes 2/2. Full design publication, actual
names, repricing, and history remain unfinished; this does not close G2-01.
See the [construction audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-original-template-starbase-construction-2026-09-25).
**Remaining: 5/14 Gate 2 packages; 3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 original starbase names (2026-09-25)

The startup adapter now uses reconstructed compressed-string decoding over
original executable string tables, replacing fake ID names. Name collision
checks examine active same-hull designs. All four generated names compare
exactly with each original M2 output; component, quantity, bitmap, and RNG
checks also pass in the integrated regression (12.047 seconds).
See the [name audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-original-starbase-names-2026-09-25).
Full design publication, costs, counts, and history remain unfinished.
**Remaining: 5/14 Gate 2 packages; 3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 publication integration — regression open (2026-09-25)

Full created starbase records now reach the current bank with source cost
calculation and player count updates. A recovered-layout correction separates
cargo capacity (offset 52) from damage (56); input corpus tests pass 18 with
one existing skip. Regenerate the input inventory after this correction.
The strengthened integrated regression is **currently failing**: original
final starbase damage reflects armor halving absent in current created state.
Original LogChangeShDef changes the design type flags before writing the log;
that mutation and the subsequent reload/reprice remain to bind. See the
[publication investigation](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-publication-integration--unfinished-regression-2026-09-25).
**Remaining: 5/14 Gate 2 packages; 3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 design-log reload — regression restored (2026-09-25)

The prior starbase-damage failure is resolved by source design-change logging,
serialization, and source design reload/repricing at the player-log load stage.
All four final damage values now match both original outputs, along with
names, component lists, and bitmaps. Published designs and player counts are
retained in current state. The design writer's missing count byte was fixed
against original WriteRtShDef; its strict-C89 probe passes. The regenerated
input inventory has zero uninitialized object records. Combined regressions
pass 19 tests with one existing skip in 15.949 seconds. See the
[design-log audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-design-log-reload-closes-damage-mismatch-2026-09-25).
Complete file-log dispatch, history, and remaining AI callback coverage remain
open; this does not close G2-01. **5/14 Gate 2 packages remain; 3 grouped
SO-E3 gates.** No VM run.


### SO-E3 Gate 2 history/upgrade input binding (2026-09-25)

Startup history now clears the version-2 count and follows loaded player-mode
and turn guards; the original detailed history walk is ineligible at turn 0.
Upgrade eligibility now reads planet, design, mode, minerals, and queue state
instead of fixed mineral witnesses. Both retained upgrade rolls reject the
upgrade, and the integrated regression passes in 12.756 seconds. See the
[history/upgrade audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-history-validation-and-upgrade-inputs-2026-09-25).
Four DoAiTurn preparation callbacks remain no-ops requiring audit/binding;
history writing and complete log dispatch are also open.
**Remaining: 5/14 Gate 2 packages; 3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 AI preparation callbacks (2026-09-25)

The four no-op DoAiTurn preparation callbacks are replaced: source planet-list
collection/shuffle, source minefield preparation on the loaded player view,
source attack marking over all four own fleets, and source component/speed
power calculation with current-bank writeback for three ship designs.
Absent design banks are explicitly excluded. The integrated regression passes
in 12.289 seconds; retained RNG and downstream comparisons remain passing.
See the [preparation audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-doaiturn-preparation-callbacks-2026-09-25).
History writing, complete log dispatch, and remaining Turin callback coverage
still prevent G2-01 closure. **5/14 Gate 2 packages remain; 3 grouped SO-E3
gates.** No VM run.


### SO-E3 Gate 2 history and design-log body comparisons (2026-09-25)

Source history writing now produces all four non-BOF H2 records and matches
the original types, lengths, order, and bodies for both runs. The four generated
design-change bodies also match the original X2 files retained under BACKUP.
X2/H2 BOF draws now occur inside their writer callbacks. The integrated
regression passes in 12.243 seconds. See the
[history/log audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-history-bodies-and-original-design-log-comparison-2026-09-25).
Full non-design log generation/dispatch and remaining AI callback coverage
still prevent G2-01 closure. **5/14 Gate 2 packages remain; 3 grouped SO-E3
gates.** No VM run; complete encrypted stream assembly remains separate.


### SO-E3 Gate 2 shared design/research log dispatch (2026-09-25)

Generated design and research records now share an ordered runtime log and
run through the source typed record dispatcher. Research bytes match both
original X2 returns. A copied-state mutation check proves replay restores
research state. Integrated regression passes in 12.214 seconds. See the
[dispatch audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-shared-designresearch-log-dispatch-2026-09-25).
**5/10 X2 mutation records are generated/dispatched; 5 remain.** Full header,
file-log loading, and remaining AI coverage are still open.
**5/14 Gate 2 packages remain; 3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 production-log generation/replay (2026-09-25)

The AI queue now generates a source type-29 log record that matches both
original X2 returns. Source dispatch replaces the current production queue;
a copied-state mutation check verifies it restores changed quantities and
progress. The logger's undersized variable-record buffer was fixed, and the
two-entry source probe passes AddressSanitizer. Integrated regression passes
in 12.128 seconds. See the [production-log audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-production-memory-log-generation-and-replay-2026-09-25).
**X2 mutations: 6/10 handled; 4 remain. Gate 2: 5/14 packages remain;
3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 fleet-balance log replay (2026-09-25)

Generated fleet-balance bytes match both original X2 records. Source dispatch
reconstructs the merge from saved pre-AI input records, preventing duplicate
ship-count addition; a poisoned-count regression passes. Merge capacity now
uses current designs and the correct hull fuel-capacity field. Integrated
regression passes in 12.279 seconds. A source mask-decoder over-read was also
fixed and verified with AddressSanitizer (22 checks, zero failures). See the
[fleet-balance audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md#g2-01-fleet-balance-log-generation-and-replay-2026-09-25).
**X2 mutations: 7/10 handled; 3 remain. Gate 2: 5/14 packages remain;
3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 order-log generation/replay (2026-09-25)

Source `stars_log_change_fleet` now generates the AI type-4 insertion and
type-5 replacement; their 12-byte bodies match both original backup X2 files.
Typed `stars_run_log_record` dispatch applies them to restored input orders,
updates counts, and zero-expands compact payloads. Copied-state poison checks
verify replay restores both affected arrays. A source trimming off-by-one
was corrected; the full 22-byte insertion passes AddressSanitizer.
The integrated selector regression passes 1/1 in 12.585 seconds. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**X2 mutations: 9/10 handled; 1 remains (cargo transfer). Gate 2: 5/14 packages
remain; 3 grouped SO-E3 gates.** Whole-file log loading and the remaining
service/provenance audits are still open. No VM run.


### SO-E3 Gate 2 cargo-log generation/replay (2026-09-25)

Source `stars_log_make_valid_xfer` now generates the seven-byte cargo record
from the actual AI fleet delta. Its body and position match both original X2
mutation sequences. The typed replay adapter applies source cargo changes
from loaded input state and publishes fleet/planet cargo; poison checks prove
it restores 25/225 without double application. Both seeded runs pass the
integrated strict-C89 regression (1/1, 12.395 seconds). See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**X2 mutations: 10/10 handled; 0 remain. Gate 2: 5/14 packages remain;
3 grouped SO-E3 gates.** Whole-file log/header handling, remaining service
bindings, messages/player-write state, and RNG/provenance audits still prevent
Gate 2 closure. No VM run.


### SO-E3 Gate 2 log-file header corrections (2026-09-25)

The original `RTLOGHDR` stores memory-log length at byte 0, serial at byte 2,
and configuration at byte 6 in a 17-byte type-9 record. Corrected the source
writer's missing length/shifted fields and the loader's incorrect record kind
and BOF-derived length. Updated source probes and added an original-file
regression: both retained X2 files load and reproduce all non-BOF records via
the source codec under AddressSanitizer. The codec and integrated turn tests
pass 2/2 in 16.133 seconds. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Gate 2: 5/14 packages remain; 3 grouped SO-E3 gates.** Coordinator file-path
integration and runtime serial/config provenance remain open. Original
serial/config bytes are used only as explicit codec-test inputs. This result
does not diagnose the old original-game rejection. No VM run.


### SO-E3 Gate 2 source memory-log runner (2026-09-25)

The coordinator now packs generated mutation records and executes source
`stars_run_log_file` rather than a handwritten replay loop. Typed dispatch
checks logging suppression; copied-state checks verify restoration of the
prior logging state and clearing of the pending-log bit. The integrated
regression passes 1/1 in 12.234 seconds. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Gate 2: 5/14 packages remain; 3 grouped SO-E3 gates.** Complete file
writer/loader integration and serial/config provenance remain open. The
pinned manifests do not establish the historical INI and disk-environment
inputs; older INI snapshots have not been assumed equivalent. No VM run.


### SO-E3 Gate 2 current transport capacity (2026-09-25)

Removed prebuilt transport component/hull/count tables from the runtime.
Transport and cargo replay now calculate capacity from current fleet/design
state and original hull records through the same source-backed adapter used
by fueling. Mutation checks verify capacities 50, 0, and 75 after changing
live counts/components; the original 25-capacity transfer still matches both
runs. The integrated regression passes 1/1 in 12.307 seconds. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Gate 2: 5/14 packages remain; 3 grouped SO-E3 gates.** G2-01 remains open;
full file integration and the remaining provenance/service audits are still
required. No VM run.


### SO-E3 Gate 2 order-phase planet propagation (2026-09-25)

Connected source DoOrders planet flag/turn changes to the shared planet
state, refreshing the input projection each phase and preserving intervening
order-handler mutations. Both pinned replays and existing planet record
comparisons pass (integrated regression 1/1, 12.278 seconds).

The [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md) now
explicitly lists five unbound DoOrders callbacks and incomplete full-fleet
SatisfyOrders pass handling. **G2-02 remains open; Gate 2: 5/14 packages
remain; 3 grouped SO-E3 gates.** No VM run.


### SO-E3 Gate 2 order-phase thing interactions (2026-09-25)

Bound both DoOrders interaction phases to source `stars_do_thing_interactions`
with current world views. Original pre-movement and kind-3 trader guards
prove no effects for the four current kind-2 wormholes. A mutation test
verifies newly eligible trader input fails explicitly. Both original turn
replays pass (integrated regression 1/1, 12.617 seconds). See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Unbound DoOrders callbacks: 4 remain, down from 5. Gate 2: 5/14 packages
remain; 3 grouped SO-E3 gates.** Full order enumeration and other service
bindings still prevent G2-02 closure. No VM run.


### SO-E3 Gate 2 order-phase research (2026-09-25)

Bound both DoOrders research callbacks to source `stars_update_research_status`
using shared current player state. Production retains its separate resource
handoff; order phases use no pool. Checks verify placement after passes 1/3,
call counts, and original research outcomes. Integrated strict-C89 regression
passes 1/1 in 12.616 seconds. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Unbound order callbacks: 3 remain, down from 4. Gate 2: 5/14 packages
remain; 3 grouped SO-E3 gates.** G2-02 still requires battles, drops,
transfers, and complete fleet task/pass handling. No VM run.


### SO-E3 Gate 2 battle fleet-linking prerequisite (2026-09-25)

Reconstructed original `LinkFleets` circular position grouping, including
singleton self-links, reverse insertion after the first fleet, flag updates,
and retired-fleet exclusion. The source probe passes strict C89 and
AddressSanitizer across 70 groups and after a fleet moves (1/1, 0.385 seconds).
See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Unbound order callbacks: 3 remain; Gate 2: 5/14 packages remain.** The
battle callback still needs grouping/eligibility/bombing integration and
shared-state publication. No VM run.


### SO-E3 Gate 2 fleet-link integration correction (2026-09-25)

An older index-based `stars_link_fleets` helper exists in `common.c`; the prior
search missed it. Renamed the new record/flag API to
`stars_link_fleet_records`, resolving the integrated declaration conflict.
The post-movement boundary now computes current fleet rings and retains
intermediate flags for eligibility. Final flag publication remains deferred
until CplrBattle/bombing are connected; publishing the partial stage alone
caused three replay failures. Integrated and source-link tests now pass 2/2
in 12.708 seconds. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Unbound order callbacks: 3; Gate 2: 5/14 packages remain.** Battle eligibility,
bombing, and final shared-state publication remain open. No VM run.


### SO-E3 Gate 2 battle callback integration (2026-09-25)

Bound source DoBattles, current fleet rings, CplrBattle eligibility, and
bombing guards; final flags now publish to shared fleet state. Corrected a
source no-opponent flag-cleanup error and prevented stale host flags from
reviving retired AI fleets. Both original fleet-record comparisons pass;
a mutation check confirms newly eligible bombing fails explicitly. The
source eligibility probe passes AddressSanitizer; integrated and grouping
tests pass 2/2 in 12.751 seconds. Details are in the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Unbound order callbacks: 2 remain, down from 3. Gate 2: 5/14 packages
remain; 3 grouped SO-E3 gates.** Colonist drops, transfers to others, and
complete fleet-order enumeration still prevent G2-02 closure. No VM run.


### SO-E3 Gate 2 complete no-task fleet visits (2026-09-25)

All four order passes now enumerate the current fleet table and invoke source
SatisfyOrders for remaining task-0 waypoints, publishing fleet/turn flags.
Each pass verifies 8 active control fleets or 7 action fleets after task-5
retirements. Newly eligible task kinds fail explicitly, verified by a mutated
waypoint test. Integrated regression passes 1/1 in 12.500 seconds. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Unbound order callbacks: 2 remain; Gate 2: 5/14 packages remain.** Source
drop/transfer consumers, their producer audit, and final order-path audit are
still required. No VM run.


### SO-E3 Gate 2 drop/transfer consumers (2026-09-25)

All DoOrders callbacks are now bound. Source DropColonists runs twice and
TransferToOthers once against explicitly initialized runtime queues. The
same-owner cargo log and current waypoint/task census establish empty queues
for the pinned pair; injected nonempty queues fail explicitly. The integrated
strict-C89 test passes **1/1 in 12.418 seconds**, including both seeded runs
and the existing 100 scout cases. See the
[queue producer/consumer audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Unbound callbacks: 0. Gate 2: 5/14 packages remain; 3 grouped gates remain.**
G2-02 still needs its final state/order audit; this is not general drop/transfer
support or complete Gate 2 closure. No VM run.


### SO-E3 Gate 2 immediate scrap-state publication (2026-09-25)

The order audit found and fixed a stale planet input: coordinator task-5 scrap
now reads the current shared planet after log cargo changes, then publishes
minerals, colonists, and flags immediately. Mining no longer overwrites shared
state from the older scrap/supply snapshots. A modified-current-state check
proves the adapter reads current values; both seeded original comparisons and
100 scout cases pass in the integrated test (**1/1, 12.573 seconds**).
See the [state audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Gate 2: 5/14 packages remain; 3 grouped gates remain.** G2-02's final
execution/provenance audit and log integration remain open. No VM run.


### SO-E3 Gate 2 runtime message serialization (2026-09-25)

Task-5, waypoint, research-pool, and production callbacks now generate runtime
message bytes through the source packer, using original EXE argument counts
and current player suppression flags. Fixed the packer's loss of message-ID
bit 8, which affected IDs such as 0x141/0x159. Source message writing now runs
inside FWriteDataFile, replacing the probe's count-only player-write hook.

Both original message sections match: M1 retains its 21-byte starting section
and appends source-generated messages (26 control / 42 action bytes total);
M2 has no emitted messages. This accounts for the original no-X1 append rule
in the comparison; complete append-file execution remains open. See the
[runtime message audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

The integrated test passes **1/1 in 12.488 seconds**, including both seeded
runs and 100 scout cases. The strict-C89/AddressSanitizer message probe passes
**6/6**. **Gate 2: 5/14 packages remain; 3 grouped gates remain.** G2-12 still
needs player-record and pre-write state integration. No VM run.


### SO-E3 Gate 2 pre-write message state (2026-09-25)

FWriteDataFile now runs source sender marking and planet-loss filtering
against runtime queues. Fixed a source bug that incorrectly treated ordinary
production/scrap messages as planet-loss notifications; original behavior
selects only IDs 7, 0x23, 0x40, and 0x8f. Newly eligible unsupported outgoing
or loss-message effects fail explicitly. See the
[pre-write state audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

Both pinned original message comparisons and all scout cases pass in the
integrated test (**1/1, 12.578 seconds**); the strict-C89/AddressSanitizer
planet-loss probe passes **5/5**. **Gate 2: 5/14 packages remain; 3 grouped
gates remain.** G2-12 still requires player-record, complete visibility, and
file-lifecycle integration. No VM run.


### SO-E3 Gate 2 current visibility inputs (2026-09-25)

Player-1 visibility initialization now receives all current fleet rows and
both current ship/starbase banks, replacing its single retained scout and
zeroed player/design tables. Source-generated view counts are retained; a
changed-current-ship-count check proves they depend on runtime state. The
integrated regression passes **1/1 in 12.642 seconds**, including both seeded
runs and all 100 scout cases. See the
[visibility-input audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**Gate 2: 5/14 packages remain; 3 grouped gates remain.** Scanner/mining,
thing visibility, other viewers, subsequent visibility passes, and complete
view publication remain before G2-12 can close. No VM run.


### SO-E3 Gate 2 visibility scanner binding (2026-09-25)

The player-1 visibility callback now derives scanner ranges from current
ship components and original part records through source scanner services.
Removing a scanner in a copied runtime state changes the computed result to
-1. Restored the original conditional mining-capability call and derive its
gate from current fleet flags, waypoint task, and planet ownership. See the
[visibility scanner audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

The integrated regression passes **1/1 in 12.423 seconds**; the strict-C89
AddressSanitizer visibility probe passes. **Gate 2: 5/14 packages remain;
3 grouped gates remain.** Other viewers, remaining visibility passes,
player-record publication, and file lifecycle are still open. No VM run.


### SO-E3 Gate 2 host player-record generation (2026-09-25)

Both host player records now generate through the source writer from current
shared state and source host visibility, with resulting player fields retained.
All four control/action record bodies match the originals byte-for-byte
(130/124 bytes per run). The audit fixed visibility count reset, starbase-design
counting, and accidental loss of player bitmap bits. See the
[host player-record audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

The integrated regression passes **1/1 in 12.402 seconds**; the expanded
strict-C89/AddressSanitizer visibility probe passes. **Gate 2: 5/14 packages
remain; 3 grouped gates remain.** Client player records and full visibility/
file lifecycle remain open; complete encrypted streams remain Gate 3.
No VM run.


### SO-E3 Gate 2 M2 player-record generation (2026-09-25)

M2's complete player record now generates from current state and matches both
original 124-byte bodies. HST player comparisons remain passing. Source
visibility finalization now retains/recounts visible starbase designs, and
the planet-mark adapter calls source MarkPlanet so newly visible planets
update player counts. See the
[M2 player/visibility audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

The integrated regression passes **1/1 in 12.392 seconds**; the expanded
visibility-finish source probe passes under AddressSanitizer. **Gate 2:
5/14 packages remain; 3 grouped gates remain.** M1, complete client visibility,
and file lifecycle remain open. No VM run.


### SO-E3 Gate 2 M1 writer — visibility regression open (2026-09-25)

Bound M1 visibility initialization and source player writing, including the
loaded race/hull/technology built-in scanner path. The new complete M1 record
comparison exposes a missing visibility effect: **1 visible planet generated,
3 in the original** (byte 2 of the 130-byte player record). The remaining
fleet/planet visibility passes must produce that count; expected output is
not used to supply it. See the
[M1 investigation](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

Latest combined verification: **2 passed, 1 failed in 15.535 seconds**. The
current integrated regression fails at the new M1 comparison. **Gate 2:
5/14 packages remain; 3 grouped gates remain.** No VM run.


### SO-E3 Gate 2 M1 visibility regression resolved (2026-09-25)

Source fleet-centered visibility now runs for both clients using current
fleets, computed scanner ranges, XY coordinates, and wormhole state. It derives
the two additional M1 planets and restores the exact player-count match.
**All eight turn-1 player record bodies across control/action now match the
originals.** See the [visibility audit](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

The integrated regression is passing again: **1/1 in 12.561 seconds**, including
both seeded runs and all scout cases. The fleet-visibility source probe passes
under AddressSanitizer. **Gate 2: 5/14 packages remain; 3 grouped gates remain.**
Planet/thing visibility passes, full state publication, and file lifecycle
remain open; matching player records alone does not close G2-12. No VM run.


### SO-E3 Gate 2 planet/thing visibility and client planet writers (2026-09-25)

Both clients now execute the source fleet-, planet-, and thing-centered
visibility passes in original order. Planet scanner selection uses current
technology/race/improvements and original executable part records through
source lookup and range services. Loaded races prove the special stargate
and thing-scanning branches inapplicable to this pair, with explicit guards.

Client planet writers now enumerate the computed visibility of all 32
planets, replacing retained M1 ID selection and hard-coded M2 planet choices.
All resulting client planet bodies and all eight player bodies continue to
match both originals. The integrated regression passes **1/1 in 12.660s**
(two seeded runs plus 100 scout cases); the planet-visibility source probe
passes strict C89 and AddressSanitizer. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** G2-12 still needs
fleet/thing writer integration, remaining visibility state publication, and
append/log lifecycle handling. No VM run.


### SO-E3 Gate 2 fleet/thing output selection (2026-09-25)

Fleet writing now enumerates the current fleet table and source-computed
visibility, taking identity and flags from runtime state. Expected output
IDs are used only for comparisons. Source visibility initialization now
includes current wormholes; client thing writing tests calculated visibility
instead of skipping clients unconditionally. Updated wormhole visibility and
knowledge are published between views, and host visibility is restored.

Both seeded runs still match all checked fleet/order and host thing bodies;
client thing selections are empty as in the originals. The integrated
regression passes **1/1 in 12.613s**, including 100 scout cases. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** G2-12 still requires
the complete visibility/design state lifecycle audit and append/log handling.
Complete file assembly remains Gate 3. No VM run.


### SO-E3 Gate 2 design publication and complete design records (2026-09-25)

Current-source design writing now matches **60/60 turn-1 design bodies**
across both control/action outputs: 7 M1, 8 M2, and 15 HST per run. Visibility
publishes current player/design flags and source-recounted existing ship and
starbase counts into shared state. New comparisons exposed and resolved
missing AI-starbase publication and a stale count on the scrapped design.

The source save coordinator now enumerates all players for both design
banks, matching the original visibility-filtered traversal. Its focused
client regression verifies other-player design enumeration. The integrated
selector and save-coordinator tests pass **2/2 in 12.769s**, including both
seeded runs and 100 scout cases. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Pre-write view
preparation, remaining state lifecycle audit, and append/log handling remain
open in G2-12. Complete stream assembly remains Gate 3. No VM run.


### SO-E3 Gate 2 pre-write preparation audited (2026-09-25)

The client save preparation callback now visits all surviving owned fleets:
**8 control / 7 action** across the two clients. Each has ships and one order;
no fleet-target conversion, patrol, or later-waypoint repair branch is
eligible. Guards enforce these exclusions without assuming an external
session flag is off. Integrated regression passes **1/1 in 12.740s**, including
both seeded runs and 100 scout cases.

The append audit identifies the next source defect: `FGenerateTurn` currently
passes append=0 for every client; original source derives append from missing
X logs during backup. The existing source append helper is not yet bound to
the integrated save path. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Append/log lifecycle
and the final state audit remain open. No VM run.


### SO-E3 Gate 2 append decision connected (2026-09-25)

The source turn coordinator now carries backup-phase append decisions to
saving. The pinned replay invokes the source append/mark helpers for M1
using its verified starting header; M2 and HST use create. It checks one
append, one header mark, and two creates per run, with unchanged output
record comparisons. The coordinator regression checks mixed decisions.

Integrated selector/coordinator tests pass **2/2 in 12.749s**; strengthened
append assertions pass the focused coordinator test **1/1 in 0.212s**.
Both starting M1 files were reverified against their manifest SHA-256s.
See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md)
for the implementation boundary and repaired generator-path collision.

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Complete persisted
X2/header integration and file lifecycle are still open: X2 presence is
currently tracked by the AI writer callback, and append operates on header
metadata rather than a complete output stream. No VM run.


### SO-E3 Gate 2 replay consumes saved mutation snapshot (2026-09-25)

AI log writing now captures the ten generated mutations at write time, and
source replay reads those saved bytes instead of reconstructing them from
live AI buffers. A perturbation check destroys the live buffers before
replay and still recovers the expected runtime state. Integrated regression
passes **1/1 in 12.381s**, covering both seeded runs and 100 scout cases.

**5/14 Gate 2 packages remain; 3 grouped gates remain.** This verifies the
saved mutation section, not a complete X2 file. Serial/environment header
provenance and whole-file persistence/loading remain open. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md). No VM run.


### SO-E3 Gate 2 complete battle-plan records (2026-09-25)

The source save coordinator now writes battle plans from current loaded
state through the reconstructed writer and name compressor. **40/40 full
battle-plan bodies match** across both runs: five M1, five M2 and ten HST per
run. Count, length and every byte are checked against the matching original.

Integrated regression passes **1/1 in 12.529s**, covering both seeded runs
and 100 scout cases. **5/14 Gate 2 packages remain; 3 grouped gates remain.**
Complete log metadata/file lifecycle and final provenance audits remain open.
See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).
No VM run.


### SO-E3 Gate 2 task dispatch order corrected (2026-09-25)

The order audit found that the coordinator executed both scrap handlers
before traversing fleets, in owner-1/owner-0 order. Original source dispatches
each task at its fleet-table position. The adapter now follows that order
and publishes each handler's state before visiting the next fleet.

Explicit task-5 visit assertions are control `[514]`, action `[0, 514]`.
Both output and RNG checks continue to pass. Integrated regression:
**1/1 in 12.626s**, both seeded runs and 100 scout cases.

**5/14 Gate 2 packages remain; 3 grouped gates remain.** G2-02 still needs
whole-file log integration and its final provenance audit. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md). No VM run.


### SO-E3 Gate 2 archived environment candidates match (2026-09-25)

Both retained older STARS.INI files decode successfully through current
source with the pinned executable's shuffle table. Their serial and all
**11 registered environment bytes match both original X2 headers**. The
[metadata audit](analysis/compat/satisfy-orders-e3-environment-candidate-audit.json)
records hashes and comparison results without exposing registration values;
`tools/satisfy_orders_environment_audit.py` reproduces it.

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Actual capture/run
provenance and registered-versus-current disk environment remain distinct
requirements. No expected header bytes were adopted as runtime inputs.
See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).


### SO-E3 Gate 2 disk-derived environment candidates (2026-09-25)

A read-only FAT metadata extraction feeds the source disk-identity function.
Both retained Win98 and Win95 images reproduce the C-drive identity and
capacity bytes of both pinned log headers; with D unavailable, all eleven
environment bytes match. These signatures are nonunique, so this establishes
an equivalent archived replay environment rather than a unique historical
machine attachment. The [disk audit](analysis/compat/satisfy-orders-e3-disk-candidate-audit.json)
retains inputs, sector hashes and comparisons; reproduce it with
`tools/satisfy_orders_disk_environment_audit.py`.

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Whole-file log
integration still needs completion. No VM run and no disk-image writes.


### SO-E3 Gate 2 generated encrypted X2 replay (2026-09-25)

The current coordinator writes a complete X2 into a bounded memory-backed file
through source log/record writers, then decrypts and loads that file through
source record/log readers before replaying mutations. Header metadata derives
from the archived INI and the documented equivalent disk profile. Startup BOF
identity now derives from complete loaded inputs, fixing the initial loader
failure. A regression destroys both live log buffers before replay and rejects
wrong-game/truncated files, proving the generated file is authoritative.

The selector regression passes **1/1 (13.291 seconds)**, covering both pinned
runs and 100 scout cases; main RNG totals remain **57/39**. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md) for scope:
global/dirty/serial state and file lifecycle audit remain, and startup BOF
clock provenance remains G2-13. This does not claim guest disk persistence.

**5/14 Gate 2 packages remain; 3 grouped gates remain.** No VM run.


### SO-E3 Gate 2 log state publication (2026-09-25)

Generated-log replay now retains serial/environment records and absent-log
sentinels, publishes loaded header/player flags, and uses source DirtyGame
for shared dirty-state transitions on mutation/write/load. The integrated
regression passes **1/1 (12.981 seconds)** for both pinned runs and 100 scout
cases, including a dirty reload/title-notification check.

The audit found the post-log serial-validation stage missing from the turn
coordinator; this is the next concrete G2-02 integration gap. It is separate
from the already audited turn-zero SpankTheCheaters exclusion. Details are in
the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** No VM run.


### SO-E3 Gate 2 post-log serial validation (2026-09-25)

The source coordinator now invokes post-log serial validation before waypoint
processing. The retained adapter applies original validity/duplicate flags
from loaded player state and generated serial records; current branches prove
no cheater messages are eligible in either pinned run. The coordinator test
checks stage order and failed-load early exit.

The header adapter's status-word offset was corrected from +6 (detail/mode)
to +84 (status flags), with a regression proving header flags cannot alter the
mode word. Source coordinator and integrated selector tests pass **2/2
(13.038 seconds)**. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Post-log design/fleet
preparation and the remaining lifecycle audit are still open. No VM run.


### SO-E3 Gate 2 post-log design/fleet preparation (2026-09-25)

Source preparation now visits 32 ship-design slots and ten fleet entries
before waypoint validation, repairs first engine components under the original
guard, publishes fleet flags, and proves the follow-fleet loop ineligible from
current orders. Both pinned turns pass the coordinator/integration regression
(**2/2, 13.484 seconds**); focused mutation checks pass (**1/1, 0.215 seconds**).
See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Final lifecycle and
callback/state provenance audits remain open. No VM run.


### SO-E3 Gate 2 battle-output and salt branches (2026-09-25)

Connected source battle output for all three files per run. Verified input
record absence and current source combat eligibility establish an empty
runtime battle list. The optional password-salt section now derives from the
verified starting HST, with an explicit failure for newly eligible unsupported
nonzero salt. Both pinned runs pass the integrated regression **1/1
(12.826 seconds)**. Details are in the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** File/global-state
lifecycle and final provenance audits remain open. No VM run.


### SO-E3 Gate 2 saved-file RNG ordering (2026-09-25)

Moved the saved-file Random(8) from early backup marking to a new source
coordinator output-preparation stage after score updates and before writes,
matching original service order. Its result now updates shared GAME flags as
well as BOF state. Coordinator and integrated tests pass **2/2 (13.510 seconds)**;
main draw totals remain 57/39. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Backup move/copy state
and pre-write scanner/cloak/buildability refresh still require lifecycle work.
No VM run.


### SO-E3 Gate 2 pre-write buildability (2026-09-25)

Current ship-design buildability now refreshes through source services using
loaded technology/race and original part records; mutations publish to shared
banks. Corrected the helper to accept lookup result exactly 1. Integrated
replay passes **1/1 (13.125 seconds)**; the focused lookup-result regression
passes **1/1 (3.208 seconds)**. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Scanner/cloak refresh,
backup handling, and final lifecycle/provenance audits remain open. No VM run.


### SO-E3 Gate 2 scanner/cloak output preparation (2026-09-25)

Pre-write preparation now computes ship scanner caches and starbase cloak
values through source services from current components, player data, and
original part records, then publishes the results to shared design banks.
Visibility and output preparation share the scanner adapter. Integrated replay
passes **1/1 (12.784 seconds)** for both pinned turns and 100 scout cases.
See the [Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Backup handling and
final lifecycle/provenance audits remain open. No VM run.


### SO-E3 Gate 2 backup namespace integration (2026-09-25)

Source HST marking and memory-backed backup operations now preserve full
starting-file bytes: X2/M2/HST move, M1 copies and remains available for append.
Append/create callbacks check the resulting namespace. The moved generated X2
also passes restore/replay checks. Integrated regression passes **1/1
(12.493 seconds)** for both pinned turns and 100 scout cases. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Global flags, cleanup,
clock provenance, and final audits remain open. Full output streams remain
Gate 3. No VM run.


### SO-E3 Gate 2 exact startup BOFs (2026-09-25)

Replaced zero-tick X2/H2 writing with explicitly recovered per-writer clock
residues. Exact comparison exposed and fixed the missing EnsureAis submit-flag
scope. All five BOFs now match their corresponding original records; source
RNG totals remain 57/39 and encrypted X2 backup/replay still passes. Integrated
regression: **1/1 (12.827 seconds)**. Clock residues are inferred replay inputs,
not independently captured historical clock readings. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**5/14 Gate 2 packages remain; 3 grouped gates remain.** Final whole-stream,
global-state, cleanup, and package audits remain open. No VM run.


### SO-E3 Gate 2 RNG package closed (2026-09-25)

**G2-13 is closed for the pinned pair.** Every main RNG draw now has continuous
seed-state verification and exact original limit/value comparison: **57/57
control, 39/39 action**, including five source BOF writer handoffs. The
[RNG audit](analysis/compat/satisfy-orders-e3-rng-audit.md) records the recovered
clock-input limitation and separates file crypto/INI state from gameplay RNG.
Integrated regression passes **1/1 (12.898 seconds)**.

**4/14 Gate 2 packages remain: G2-01, G2-02, G2-12, G2-14.** Three grouped
SO-E3 gates remain open. No VM run.


### SO-E3 Gate 2 Turin design maintenance (2026-09-25)

Bound previously missing source design-status and ship-maintenance callbacks.
Current designs and technology drive seven role-status checks and one complete
maintenance pass per pinned run; source guards establish no eligible ship
creation/replacement. Integrated regression passes **1/1 (12.360 seconds)**,
including unchanged 57/39 RNG transcripts. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**4/14 Gate 2 packages remain: 01,02,12,14.** AI lifecycle and remaining
callback audits continue. No VM run.


### SO-E3 Gate 2 complete AI history and allocation lifetime (2026-09-25)

Both generated H2 streams now compare exactly with their originals, using
source framing/encryption. AI temporary allocations/releases are tracked and
must finish after log/history writes with zero live allocations. Integrated
regression passes **1/1 (12.618 seconds)**. The audit found final
FillProductionQueue still unbound; this remains a concrete G2-01 gap. See the
[Gate 2 tracker](analysis/compat/satisfy-orders-e3-gate2-tracker.md).

**4/14 Gate 2 packages remain: 01,02,12,14.** No VM run.


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

### SO-E3 G2-01 AI package closed (2026-09-26)

The [AI closure audit](analysis/compat/satisfy-orders-e3-ai-callback-audit.md)
now covers current-input policy/role reachability, generated X2/H2, both
source DestroyCurGame transitions, host reload, selection/session identity,
and the complete pinned control/action integrated checks. The focused source
probes pass; the strict-C89 control/action and 100 scout cases pass. G2-01 is
closed for the pinned pair. G2-14 will still audit whole-turn provenance and
may reopen defects. **Gate 2: 3 of 14 packages remain (G2-02, G2-12, G2-14).**
SO-E3 grouped Gates 2–4 remain open. No VM run.

### SO-E3 G2-02 order/log package closed (2026-09-26)

The [order/log closure audit](analysis/compat/satisfy-orders-e3-orders-audit.md)
now traces generated-X2 replay, serial and preparation, waypoint validation,
all four source order passes, task-5 and passive fleets, battle eligibility,
research, and empty drop/transfer consumers through shared current state.
Merge replay publishes the complete merged/deleted fleet state into the host
world and verifies it before downstream use. The [integrated test]
(analysis/compat/satisfy-orders-e3-orders-integration-test.txt) passes for
both pinned cases and 100 scouts; nine focused source/framing tests pass.
G2-02 is closed for the pinned pair, subject to G2-14's final provenance
audit. **Gate 2: 2 of 14 packages remain (G2-12, G2-14).** SO-E3 grouped
Gates 2–4 remain open. No VM run.

### SO-E3 G2-12 writer lifecycle package closed (2026-09-26)

The [writer lifecycle audit](analysis/compat/satisfy-orders-e3-writer-lifecycle-audit.md)
checks the original `0x0202` generation-flag scope, bit-1-gated view order
preparation, live message/player and section-writer state, and source backup,
append, create, BOF, and close transitions. The memory backend retains the
old M1 bytes with its marked header and new BOF at the old EOF; M2/HST begin
with current-source BOFs and all three destinations close. Complete generated
section streams remain Gate 3. The [six-test report]
(analysis/compat/satisfy-orders-e3-writer-lifecycle-test.txt) passes for both
pinned runs and 100 scouts, with focused log/writer/score/planet checks.
G2-12 is closed for the pinned pair, subject to G2-14's final provenance
audit. **Gate 2: 1 of 14 packages remains (G2-14).** SO-E3 grouped Gates
2–4 remain open. No VM run.

### SO-E3 Gate 2 final provenance audit closed (2026-09-26)

The [G2-14 audit](analysis/compat/satisfy-orders-e3-gate2-provenance-audit.md)
accounts for every reached coordinator callback, current-state handoff,
source-proven branch exclusion, retained-output assertion and external clock
input for the pinned `sye3k0/sye3k1` pair. It removed three runtime oracle
dependencies: planet writer IDs from the turn-1 output list, owner-1 task-5
technology from a retained starting array, and Turin's 25-colonist transfer
amount from the observed output delta. The latter is now the original source
literal `0x19`. Both task-5 technology services publish their resulting
levels; the output BOF game ID comes from the loaded game record.

The [Gate 2 regression](analysis/compat/satisfy-orders-e3-gate2-regression-test.txt)
passes **36 tests (35 passed, one expected Unicorn skip)**, including both
pinned seeded turns, 100 scout cases, and focused source/input/log/writer
checks. `git diff --check` passes. **Gate 2: 0 of 14 packages remain; grouped
Gate 2 is closed. Two grouped SO-E3 gates remain (complete stream assembly
and every-record differential).** No VM run.

### SO-E3 Gates 3–4 complete six-file stream differential (2026-09-26)

The integrated strict-C89 selector-1 probe now serializes every turn-1
section through the source record writer for both pinned inputs. The retained
[Gates 3–4 audit](analysis/compat/satisfy-orders-e3-gates3-4-audit.md) lists
all six HST/M1/M2 files and their 274 turn-1 records. The regression decodes
each generated file, checks the full ordered record count and every kind/body
against its matching original, then compares the entire encrypted file byte
for byte. Both M1 files retain their 31-record turn-0 prefix. The control and
action seeded runs and 100 scout rolls pass in
`python3 -m unittest tests.test_satisfy_orders_turin_binding -q`.

**SO-E3: 0 of 4 grouped gates remain; the acceptance item is closed.** No new
original Stars! VM run was made.

### SatisfyOrders Step-6 promotion (2026-09-26)

The [six-part promotion audit](analysis/compat/satisfy-orders-step6-promotion.md)
links the recovered boundary, readable buildable source, strict-C89 host
coverage, retained VC++ 1.52c/Win98 packets, and pinned original full-turn
differential. The `0017:6798-9eb8` review record and generated coverage/TODO5
scope now carry SO-E3 closure and Step-6 status; reconstruction notes and
TODO6/TODO7 are synchronized. **SatisfyOrders: 6/6; SO-P closed.**

### RT-E4 source player, design, and score bodies (2026-09-26)

The [selector-3 coordinator](analysis/compat/do-rototill-coordinator-e4-r1.md)
now replays X2 design changes, prepares player-1, player-0, and host
visibility, and runs source score updating. Source writers reproduce all four
complete M2 turn-1 starbase design bodies; all four HST/M1/M2 player bodies
(including player-1 rank 2); and both M1/M2 type-45 score bodies byte for byte
against the retained original.
The focused strict-C89 regression passes. At that checkpoint, complete
HST/M1/M2 streams, native VC1.52c execution, and a same-input complete-turn
differential remained; the later file update below supersedes the stream gap.

### RT-E4 complete source-backed record inventory (2026-09-26)

The [selector-3 coordinator](analysis/compat/do-rototill-coordinator-e4-r1.md)
now generates every turn-1 M1/M2/HST record body using bound reconstructed
services: 147 records in original section order compare byte for byte. Source
XOR re-encoding of that inventory matches complete M2 and HST encrypted
files, plus M1 when its retained turn-0 append prefix is included. The
strict-C89 focused regression passes (one test, 43.902 seconds). This was an
assembled output inventory; the later integrated writer below supersedes
that limitation.

### RT-E4 complete host source file streams (2026-09-26)

The [selector-3 coordinator](analysis/compat/do-rototill-coordinator-e4-r1.md)
now passes its source-generated state bodies through reconstructed
`stars_write_data_file`, `stars_append_file`, `stars_mark_file`, and
`stars_write_record`. The source coordinator selects sections; source record
encryption produces complete M1 (1,700 bytes), M2 (870 bytes), and HST
(2,081 bytes). All three full files match their retained original byte for
byte in the focused strict-C89 regression (one test, 44.132 seconds). The
input M1 append prefix is explicitly checked as byte-identical to the
retained Rototill input. **RT-E4.b and RT-E4.c are complete. RT-E4 remains
open** for remaining state-stage binding, native VC1.52c execution, and the
same-input native/original turn differential. The 26 parent AI items remain
open; **88 of 91 small units remain**.

### RT-E4 turn-service integration (2026-09-27)

The [selector-3 coordinator](analysis/compat/do-rototill-coordinator-e4-r1.md)
now invokes source waypoint validation at both explicit turn points and
inside postproduction wormhole movement. It also runs all four source
`DoOrders` passes and the surrounding movement, decay, breeding, fuel,
sweep, healing, terraforming, and cheat services in turn order. The focused
strict-C89 regression still matches the three complete encrypted output
files byte for byte (one test, 44.451 seconds). The post-AI phase now runs
under source `stars_generate_turn`, including its shuffle, stage sequencing,
turn increment, and per-file output callbacks; the complete file comparison
still passes (one test, 44.036 seconds). The player-log callback
now replays X2 and source `stars_mark_file` marks the
exact selector-3 input HST. All three complete backup files compare byte for
byte to the retained originals; output preparation consumes the saved RNG
draw and the M1 append path uses the live prepared prefix. The focused test
still matches complete output files (one test, 44.679 seconds). A fresh HST
runtime restore, source `DestroyCurGame` at driver entry, and generation
flag entry/exit now also pass the same full-file test (one test, 44.151
seconds). The reload still reuses decoded retained input rather than a
complete source file-reader pass; that broader file loading behavior is
outside this retained-input source-state small unit. **RT-E4.a is complete.**
Native VC1.52c execution and the same-input differential keep parent RT-E4
open. **87 small units and all 26 parent AI items remain open.**

### RT-E1 integrated source production and original outcome (2026-09-27)

The [selector-3 coordinator](analysis/compat/do-rototill-coordinator-e4-r1.md)
now drives the retained AI queue through live production/resource callbacks,
the HST reload and X2 replay, and the 32-planet `Produce` stage in source
`stars_generate_turn` order. It checks 32 available resources, the original
three-factory X2 queue, two completed factories, the third at 89%, final
planet-24 mineral stocks `539/965/498`, population 256/fraction 50, and
complete source-written encrypted M1/M2/HST files against the original.
The focused strict-C89 regression passes (one test, 44.151 seconds).
**RT-E1.a and RT-E1.c are complete; RT-E1.b native integrated execution
remains.** Across the AI burndown, **85 small units remain, 6 closed of 91**.

The [fresh VC1.52c r4 production boundary](analysis/compat/do-rototill-production-turn-e1-native-r4.md)
passes Win16 compile/link, process exit, and returned report. It runs source
queue selection/fill, resource availability, factory addition, `Produce`,
and build-item stages against the retained 225-colonist input. Unlike r3,
the native callbacks calculate 32 resources and the factory cost through
source resource, maximum-population, and production-cost services using
retained race/planet fields. **RT-E1.b is complete; 84 small units remain,
7 closed of 91.** Parent RT-E1 still awaits full native `DoRototillAiTurn`
caller and same-input file outcomes under RT-E4.d/e.

### RT-E2 later main RNG source trace (2026-09-27)

The [selector-3 RNG audit](analysis/compat/do-rototill-main-rng-audit-r1.md)
pins 33 ordered callback draws and five source `WriteBOF` draws, maps the
later stages to original raw call sites, and retains the matching turn-file,
wormhole, planet, and saved-flag outcomes. The focused host full-turn test
passes with an asserted draw transcript. Retained HST race stats, turn zero,
empty starting queues, and the original factory queue dispose four reachable
`Random` callers. The retained ten fleet orders, sole warp-5 X2 route, live
player flags, and kind-2-only thing inventories dispose three movement callers.
The original trader interaction and minefield-visibility RNG sites are also
excluded by the retained kind-2-only thing inventory, turn-zero trader guard,
and player race stats 9/1. The retained pre/post-movement fleet locations and
planet owners rule out the original post-movement battle and bombing random
sites, including battle-only descendants. The exact X2 pickup and original
single task-0 order guards now dispose the four remaining named direct
callers. All 28 static direct caller families have a fixture-specific
disposition. A reproducible [indirect inventory](analysis/compat/do-rototill-rng-indirect-r1.json)
finds 35 statically reachable indirect transfers, including eight unresolved
ones. The retained design names exclude the fixed tutorial seed reset at turn
entry: it would yield `[9,11,12,1]`, rather than the observed four name draws
`[12,6,12,2]`. Later UI reentry and those indirect paths, along with
distinct call contexts still need review for RT-E2.e. The parent gate and **84-unit** burndown
are unchanged.

### RT-E3 owned-planet colonizer marker (2026-09-27)

The [colonizer marker evidence](analysis/compat/do-rototill-multi-order-e3.md#colonizer-ai-byte-15-marker-continuation-2026-09-27)
binds the original second-sweep positive-target write to source
`DoRototillAiTurn`. The 9-check host probe compares callback order, AI-table
bytes, and all 58 `PLORD` bytes. Fresh VC1.52c package r16 compiled, linked,
and returned a 112/112 PASS report including those nine checks; the unique
return directory retains logs, map, objects, executable, report, and agent
log. **RT-E3.a is complete; 83 small units remain, 8 closed of 91.** Parent
RT-E3 remains open for other route states, live service identity, and an
action-bearing original internal state comparison.

### RT-E2 indirect RNG transfer resolution (2026-09-27)

The [original-binary audit](analysis/compat/do-rototill-main-rng-audit-r1.md#indirect-callsite-resolution-2026-09-27)
resolves the eight previously unidentified indirect sites by checking their
callsite arguments, original NE callback descriptors, and compiler runtime
paths. Five are `_bsearch`/`_qsort` comparator calls, one reads a 13-entry
`DrawPlanShip` table with no direct `Random` target, and two are runtime
jumps. The direct generation call graph does not reach `StartTutor`; the
retained four design-name draws already exclude its fixed seed reset at
entry. A focused audit test passes. Drawing-callback descendants and
synchronous UI reentry remain unproved, so RT-E2.e and the parent gate stay
open. **83 small units remain, 8 closed of 91.**

### RT-E2 retained-turn RNG path closure (2026-09-27)

The [original X2 execution guard audit](analysis/compat/do-rototill-main-rng-audit-r1.md#retained-turn-execution-context-closure-2026-09-27)
shows that four design additions, no fleet deletion/merge record, and ten
task-0 planet orders cut all direct paths to the drawing callback, report
sort, and tutorial seed reset on the retained selector-3 generation. The
focused original-binary, guard, and seed tests pass (3 tests), and the
complete source-turn test passes (1 test, 43.609 seconds) with its pinned
38-position RNG sequence and exact retained output files. **RT-E2.e is
complete; 82 small units remain, 9 closed of 91.** Parent RT-E2 still
requires original route outcomes, live identity, and native/original route
differentials.

### RT-E4 complete-turn native compile boundary (2026-09-27)

The [first VC1.52c full-turn compile packet](analysis/compat/do-rototill-full-turn-native-r1.md)
transferred the existing complete Rototill coordinator and generated fixture
to StarDev. The compile-only batch exited, but the returned status is
`COMPILE=FAIL`: `TurinTrace` exceeds Win16 structure limits, followed by
compiler C1001. The failed logs and agent return are retained. Removing an
unused 65,536-byte trace field preserves the complete strict-C89 host turn
regression; the trace remains 195,488 bytes on the host. RT-E4.d requires a
segmented native coordinator, link, execution, and returned state. **82
small units and all 26 parent items remain open.**
