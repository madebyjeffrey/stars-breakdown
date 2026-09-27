# Rototill selector-3 main RNG trace, r1

Status: **RT-E2.e closed for the retained selector-3 turn.** All 28 named
direct original callers have fixture-specific dispositions, and the
indirect/seed paths are closed by the execution-context audit below. No new
original `STARS.EXE` run was made.

The [selector-3 coordinator](../../tests/satisfy_orders_rototill_coordinator_e4_probe.c)
now records every call through its main-seed callbacks. Its
[`--dump-rng` transcript](do-rototill-main-rng-r1.txt) contains 33 ordered
bound/result pairs. The focused [host test](../../tests/test_satisfy_orders_turin_binding.py)
compares all 33 with that pinned transcript while still comparing complete
M1/M2/HST output files with the retained original. The trace ignores calls
on preview seed copies, so seed-audit computations do not enter the list.

| Full-stream positions | Stage | Callback draws |
|---|---|---|
| 1–4 | `IroEnsureAi` design names | `Random(13)=[12,6,12,2]` |
| 5 | scout wormhole gate | `Random(100)=31` |
| 6 | starbase upgrade gate | `Random(100)=12` |
| 7–8 | X2, H2 `WriteBOF` | `Random(2000)=[210,1059]` |
| 9–10 | player shuffle | `Random(2)=0`, `Random(1)=0` |
| 11–16 | two owned planets, three mineral lanes each | `Random(100)=[85,40,53,2,16,91]` |
| 17–20 | meteor, climate, minerals entry and planet choice | `(20,6),(20,19),(15,0),(32,24)` |
| 21–32 | four postproduction wormholes | per wormhole: `Random(100)`, `Random(25)`, `Random(25)`; values `82,20,3, 2,6,12, 83,3,5, 84,6,24` |
| 33–34 | `UpdateGuesses` on owned planets | `(71,45),(64,60)` |
| 35 | saved turn flag | `Random(8)=6` |
| 36–38 | M1, M2, HST `WriteBOF` | `Random(2000)=[1416,1017,1216]` |

The five `WriteBOF` calls draw inside the source writer, so they do not
appear in the callback transcript. Their position and values are recovered
from the same source seed sequence. X2/H2 complete bytes match the retained
original backup files; the three output BOFs match the retained original
M1/M2/HST after supplying their explicitly inferred low-11-bit tick
residues. Those tick residues are not independent guest-clock readings.

The original [FGenerateTurn raw listing](../decompiler/export/raw/10b0_0000_FGenerateTurn.c)
puts the player shuffle before logs, `Produce` before `MoveThings(1)`,
`UpdateGuesses` after movement, and the saved `Random(8)` immediately before
the three file writes. The original [Produce listing](../decompiler/export/raw/10b8_0000_Produce.c)
calls `MineMinerals`, then `RandomEvents` when the retained no-random flag is
clear. The [event dispatcher](../decompiler/export/raw/10b8_3064_RandomEvents.c)
calls meteor, climate, mineral discovery, and trader in that order.
The first two entry results are nonzero, so their nested calls are skipped.
The third is zero and draws planet 24; original
[`DiscoverNewMinerals`](../decompiler/export/raw/10b8_5e0c_DiscoverNewMinerals.c)
does not enter its later branch at turn zero. Original
[`MysteryTrader`](../decompiler/export/raw/10b8_5efa_MysteryTrader.c)
returns before its first draw when the turn is at most 39.
Original [`MoveThings`](../decompiler/export/raw/10b0_18f4_MoveThings.c)
draws once for chance and twice for position on each postproduction wormhole
when its chance is zero. The retained HST has exactly four type-2 things,
and the coordinator matches all four original turn-1 records. Original
[`UpdateGuesses`](../decompiler/export/raw/10b8_532c_UpdateGuesses.c)
has its random call on the two inhabited planets; the source result matches
the original guess words. The six mining calls follow original
[`EstMineralsMined`](../decompiler/export/raw/1028_5362_EstMineralsMined.c):
two owned planets, three nonzero mineral remainders, and the retained random
rounding flag. `Random(1)` in the shuffle advances both seeds despite
returning zero.

This establishes an inspectable source sequence and identifies the original
guards corresponding to its later stages. At that checkpoint RT-E2.e remained open because the
raw original call graph has not yet been exhaustively checked for every
conditional descendant between these stages. A breadth-first inventory of
the [original call graph](../decompiler/callgraph.json) finds
[28 direct `Random` callers reachable from `FGenerateTurn`](do-rototill-rng-callgraph-r1.txt).
Nine are the sites or stage families audited above: `FGenerateTurn`,
`EstMineralsMined`, the four event procedures, `MoveThings`, `UpdateGuesses`,
and `WriteBOF`. The [retained guard extraction](do-rototill-rng-guards-r1.json)
is reproducible with `python3 -m tools.rototill_rng_guard_audit` and pins
player race stat 14 to `[9,1]`, turn zero, no starting type-28 queue in
HST/M1/M2, ten single planet-target orders, four kind-2 wormholes, and the
sole post-AI queue word `0x00021c03` (three factories on planet 24). Against
the original raw procedures this disposes the following reachable callers:

| Original caller | Guard that prevents a draw in this turn |
|---|---|
| [`BreedColonistsInTransit`](../decompiler/export/raw/10b8_7e4e_BreedColonistsInTransit.c) | Its fleet loop requires a player with race stat 14 equal to 4; both retained players differ. |
| [`AutoTerraform`](../decompiler/export/raw/10b8_48f6_AutoTerraform.c) | Its planet loop requires a player with race stat 14 equal to 3; both retained players differ. |
| [`SpankTheCheaters`](../decompiler/export/raw/10f0_192a_SpankTheCheaters.c) | Its randomized body requires `game.turn > 9`; `FGenerateTurn` calls it before advancing this retained turn from zero. |
| [`FBuildObject`](../decompiler/export/raw/10b8_19b2_FBuildObject.c) | The only produced object is the AI-queued kind-7 factory. The original `switch(iItem)` handles factories in `case 7`; its four `Random` sites are confined to `case 0xd`. |
| [`MoveFleets`](../decompiler/export/raw/10b0_32ce_MoveFleets.c) | The retained players have dead flags zero. All ten input orders have task/warp zero; the sole X2 route mutation gives fleet 0 warp 5. Its `Random(4)` dead-player site, `Random(10)` warp-above-6 site, and `Random(10)` warp-10 site are therefore skipped. |
| [`FStargateJump`](../decompiler/export/raw/1080_0cfe_FStargateJump.c) | The original `MoveFleets` call is in its warp-11-or-higher branch. The highest order warp after X2 replay is 5. |
| [`FTravelThroughMineFields`](../decompiler/export/raw/10b0_4f60_FTravelThroughMineFields.c) | Its `Random(1000)` requires an intersecting kind-0 minefield. The retained input and output HST inventories each contain only four kind-2 wormholes; all input orders are stationary and the one AI order is a move, so no minefield-lay order precedes movement. |
| [`DoThingInteractions`](../decompiler/export/raw/1110_0b3a_DoThingInteractions.c) | Every `Random` site is inside the `fPostMove != 0` block's kind-3 trader interaction. The original HST contains only four kind-2 wormholes, and turn-zero [`MysteryTrader`](../decompiler/export/raw/10b8_5efa_MysteryTrader.c) returns before creating a trader. The output HST still contains only those four wormholes. |
| [`SetVisPFThings`](../decompiler/export/raw/1070_b9ee_SetVisPFThings.c) | Its only `Random(100)` site is in the minefield-visibility loop, which first requires the viewing player's race stat 14 to equal 5 and a kind-0 thing. The two retained race stats are 9 and 1, and every retained thing is kind 2. |
| [`DoBombing`](../decompiler/export/raw/10f0_aefa_DoBombing.c) | Called at the end of the sole post-movement [`DoBattles`](../decompiler/export/raw/10f0_3a26_DoBattles.c) pass. Its randomized body requires a fleet at a planet owned by another player. Every retained post-movement fleet is at its own planet or unowned planet 26. |
| [`FDoCoolBattle`](../decompiler/export/raw/10f0_8bcc_FDoCoolBattle.c) | [`LinkFleets`](../decompiler/export/raw/1038_1bb4_LinkFleets.c) links only fleets at the same coordinates. At the post-movement battle boundary, each coordinate group contains one owner, and no fleet is at a hostile planet. [`CplrBattle`](../decompiler/export/raw/10f0_2952_CplrBattle.c) therefore has no opposing participant and `DoBattles` does not enter this caller. |
| [`DxyMoveTokTo`](../decompiler/export/raw/10f0_5f18_DxyMoveTokTo.c), [`CTorpHit`](../decompiler/export/raw/10f0_6790_CTorpHit.c), and [`RandomizeTokOrder`](../decompiler/export/raw/10f0_44d4_RandomizeTokOrder.c) | Their generation paths descend from `FDoCoolBattle` through battle movement/attack or `InitializeBoard`; that battle is skipped on the retained post-movement state. |
| [`SpdOfShip`](../decompiler/export/raw/10f0_339c_SpdOfShip.c) | Its nonbattle `LComputePower` caller passes a null fleet and cannot enter the `Random(15)` block. Its live-fleet caller is `InitializeBoard`, which is reached only after `FDoCoolBattle` starts; this battle is skipped. |
| [`ValidateWaypoints`](../decompiler/export/raw/1038_68c6_ValidateWaypoints.c) | All four `Random` sites are inside object-kind-2 fleet-target repair. The input and sole X2 order change are single object-kind-1 planet targets. Original [`SatisfyOrders`](../decompiler/export/raw/10b0_6798_SatisfyOrders.c) bypasses its task-0 branch for a single order; the output retains all ten single kind-1 orders. The first and second validation passes therefore have no fleet-target repair. |
| [`DropColonists`](../decompiler/export/raw/10b8_34e2_DropColonists.c) | Its RNG body requires a queued colonist drop. `FGenerateTurn` initializes `cColDrop=0`; the exact X2 type-1 transfer is a positive planet-to-fleet colonist pickup, not a drop. Original `SatisfyOrders` only calls `FQueueColonistDrop` from task-1/2/5 branches; all retained orders are task 0. |
| [`DropSalvage`](../decompiler/export/raw/10f0_24dc_DropSalvage.c) | Its callers on the generation path are minefield damage, task-5 fleet retirement, and battle cargo/ship destruction. This turn has no kind-0 minefield, no task-5 order, and no battle; the site is unreachable. |
| [`ITechLearnATech`](../decompiler/export/raw/10f0_9918_ITechLearnATech.c) | Its generation callers are task-5 `SatisfyOrders`, `DropColonists` after a contested drop, and battle messages. None is entered on this turn. |

The retained [battle-boundary extraction](do-rototill-rng-guards-r1.json)
pins both turn states: player-0 fleets at `(1128,1300)` on planet 10;
player-1 fleets at `(1290,1258)` on planet 24, with one scout moving to
`(1313,1262)` on unowned planet 26. Original [`FGenerateTurn`](../decompiler/export/raw/10b0_0000_FGenerateTurn.c)
calls `DoOrders(0)` before movement and `DoOrders(1)` after it; only the
latter calls `DoBattles`, before any post-movement order fulfillment.
Production adds factories, not a fleet. The original output retains all ten
fleets at these positions and the same planet owners, matching that battle
boundary.

The original X2 [cargo-log analysis](satisfy-orders-e3-gate2-tracker.md#g2-01-cargo-log-generation-and-replay-2026-09-25)
identifies the retained seven-byte type-1 record as a positive
planet-to-fleet colonist transfer. The [guard extractor](do-rototill-rng-guards-r1.json)
now asserts the exact record body `01021800120819`, complete X2 record-kind
inventory, single task-0/planet-target fleet orders, and the two battle
boundaries.

**Zero named direct `Random` caller families remain undispositioned** in the
28-caller static inventory. That is not yet proof that the 38 source positions
are the entire original main stream: the inventory records one shortest path
per caller and may miss an indirect caller or a distinct call context for a
named function.

The reproducible [indirect-transfer inventory](do-rototill-rng-indirect-r1.json)
(`python3 -m tools.rototill_rng_indirect_audit`) traverses the exported direct
call graph from `FGenerateTurn`: 956 names and 35 indirect transfers are
statically reachable. Eighteen transfers have recovered target sets; none
includes original `Random` at `1040:16d2`, and the 65 exported address-taken
code-pointer candidates also contain no `Random` target. Nine transfers are
DOS/CRT service interrupts; eight remain unresolved: one `DrawPlanShip`
UI callback, two `_bsearch` and three `_qsort` comparator calls, a CRT stack
check continuation, and a math dispatch. This is a static overapproximation,
not a runtime claim about those paths.

The same inventory flags original [`Randomize2`](../decompiler/export/raw/1040_165a_Randomize2.c)
as directly reachable through `FLookupFleet -> AdvanceTutor -> EndTutor`.
[`EndTutor`](../decompiler/export/raw/10f8_0c02_EndTutor.c) gates its seed reset
on the tutorial bit. The [original design-name seed test](../../tests/test_rototill_rng_seed.py)
now excludes the entry tutorial branch: `FGenerateTurn` would call
`Randomize(0x499602d2)` before `FLoadGame`, producing seed words `(71,41)`
and first four `Random(13)` results `[9,11,12,1]`, whereas the retained
designs require `[12,6,12,2]` and uniquely recover `Randomize2` residue
6844. `PushRandom` and `PopRandom` are absent from direct static reachability.
The only original setter of the tutorial bit found in exported raw C is
`StartTutor`, called from UI command/title/new-game handlers, not from the
generation driver. A later UI-mediated `StartTutor`/`EndTutor` reentry is not
established by the retained files, so RT-E2.e remained open until that context
and the unresolved indirect transfers are closed or included in the stream.

## Indirect callsite resolution, 2026-09-27

The reproducible [binary resolution](do-rototill-rng-indirect-resolution-r1.json)
(`python3 -m tools.rototill_rng_indirect_resolution`) accounts for the eight
previously unresolved transfer sites. Original `LinkFleets` supplies
`1038:1fa2 ICompFleetPoint2` to `_bsearch` (two runtime callsites), and
`SortReportCache` supplies `1108:5bb8 ICompReport` to `_qsort` (three
callsites). The original NE data segment has 13 descriptors in the two
`DrawPlanShip` tables at offsets `0x7fc` and `0x90e`. Their callback
slots point only into code segments `1050` and `1058`; none
points to `1040:16d2 Random`. The last two sites are a CRT stack-overflow
continuation and a same-segment math dispatch. The audit checks the
original executable and raw callsites; its focused test passes.

One callback lies inside `DrawShipWayPtOrders`, whose static descendants
include `EstMineralsMined` and thus `Random`. The call requires UI order
dropdown selection 3, read by `SENDMESSAGE(0x407)`; the retained fleet
orders alone do not prove that dropdown state. The other callback entry
offsets also need path-level review. The direct call graph has no
`FGenerateTurn` path to `StartTutor`, though it
does contain the guarded `AdvanceTutor` → `EndTutor` → `Randomize2` path.
The original four design names exclude a tutorial-bit reset at generation
entry. This resolves the eight sites' immediate targets and makes a later
direct `StartTutor` call unlikely. It does **not** yet prove that a drawing
callback's descendants or synchronous UI reentry cannot alter the seed
during the retained turn. Those path-level exclusions, and any distinct
call contexts not represented by the shortest-path inventory, remained before
RT-E2.e could close. At that checkpoint the 38-position trace and 83-unit
project burndown were unchanged.

## Retained-turn execution-context closure, 2026-09-27

The [execution guard audit](do-rototill-rng-execution-guards-r1.json)
(`python3 -m tools.rototill_rng_execution_guard_audit`) uses the retained
original X2 and original call graph to cut seven impossible edges. All four
type-27 design records have a nonzero low nibble, so the raw
`FRunLogRecord` deletion path to `DestroyAllIshdef` is skipped. X2 has no
type-23 fleet-deletion/transfer record or type-37 merge record. All ten
retained fleet orders are single task-0 planet targets; the raw task-4
fleet-target `SatisfyOrders` merge path is skipped. After those guards,
`FDeleteFleet`, `ChangeMainObjSel`, and `DrawPlanShip` are unreachable from
`FGenerateTurn` in the direct graph. The possible mining-preview
`DrawShipWayPtOrders` → `EstMineralsMined` → `Random` path therefore does not
execute in this original batch turn, regardless of the unavailable UI
dropdown state.

The remaining direct routes to `InvalidateReport` are `FLookupFleet` and
`FLookupPlanet`; both pass `fReload=0`. Its raw `SortReportCache` call
requires a nonzero `fReload`, so `_qsort` and its `ICompReport` callback are
also unreachable. `_bsearch` uses the fixed `ICompFleetPoint2` comparator,
which has no RNG descendant. The CRT stack-overflow continuation and
same-segment math jump are compiler runtime paths, not calls to `Random`.
The original `-g` command enters `FGenerateTurn` synchronously through the
frame's batch message; the direct generation graph does not reach
`StartTutor`. The retained design names exclude the tutorial bit at entry,
so guarded `EndTutor` cannot call `Randomize2` on this turn. The seven-edge
cut confirms `Randomize2` is unreachable after the fixture guards.

The focused binary, execution-guard, and seed tests pass (3 tests, OK).
The complete source coordinator regression passes (1 test, 43.609 seconds)
with its pinned 33 callback draws, five writer draws, all 147 turn-1 records,
and complete M1/M2/HST byte comparison. Together with the 28 direct-caller
dispositions above, this closes **RT-E2.e** for the retained selector-3
turn. It does not establish original enemy/wormhole/fallback route outcomes
or live allocator/selected-pointer identity; RT-E2.b/c/d and parent RT-E2
remain open. The project now has **82 small units open, 9 closed of 91**.
