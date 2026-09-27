# SatisfyOrders E3 original-instruction boundary audit

Current status: SO-E1 is closed with the verified original design cost and
fresh native 19/19 run. SO-E2 was later closed on 2026-09-24 with the exact
high-offer original inputs bound through the current-source task-5 path and
host/VC1.52c native 74/74 checks; see
[`satisfy-orders-task5-v4-current-source-native.md`](satisfy-orders-task5-v4-current-source-native.md).
The one full-turn differential check in SO-E3 remains open.
The later `satisfy-orders-task5-e2-e3-progress.md` adds a scoped four-pass
action-delta comparison and native task-5 service probe without closing either
remaining unit.

The retained original `DoOrders` export gives an exact orchestrator oracle at
the full-turn boundary. Its recovered sequence is:

- pre-movement: refresh inhabited-planet bits, `DoThingInteractions(0)`,
  `SatisfyOrders(1)`, `DropColonists`, `UpdateResearchStatus(0)`,
  `SatisfyOrders(2)`, and `TransferToOthers`;
- post-movement: refresh inhabited-planet bits, compute
  `idBattle=(game.turn&0xf)*0x100+1`, run `DoBattles(1)` and
  `DoThingInteractions(1)`, clear planet turn fields, then run
  `SatisfyOrders(3)`, `DropColonists`, `UpdateResearchStatus(0)`, and
  `SatisfyOrders(4)`.

The readable `stars_do_orders` source matches that sequence and the native
four-pass fixture records the same callback/event trace: the host and fresh
Win98 reports both pass all 18 checks, including battle-id creation, order
retirement, state mutation, message order, and the post-movement suppression
of `TransferToOthers`.

This is stronger original-instruction evidence for the orchestrator boundary,
but it is deliberately not recorded as the complete original executable
differential. A later isolated original-executable batch replay now supplies a
reproducible output fixture without submitting a turn in the user's live game:

- The host copied the verified `clean2400` XY/HST/M1/M2 files and the pinned
  `target/stars.exe` into fresh guest directory `C:\SYORIG1`. The Win98 agent
  ran `STARS.EXE -g C:\SYORIG1\CLEAN.HST` and returned process `EXIT 1`.
  The exit status alone is not treated as a pass.
- The original executable nonetheless wrote a 2,018-byte `CLEAN.HST`,
  1,700-byte `CLEAN.M1`, 807-byte `CLEAN.M2`, and 95-byte `CLEAN.H2`.
  Its `BACKUP/CLEAN.HST`, `.M1`, and `.M2` hashes exactly match the verified
  2400 inputs; the XY file remains byte-identical.
- Record framing parses to EOF: the new HST has 83 records and a BOF at turn
  1; M1 has 63 records with BOFs at turns 0 and 1. The new HST/M1 sizes and
  record-kind sequences match the earlier `clean-replay-a` original batch run.
  M1 record lengths match; HST and M2 each have three differing type-26 record
  lengths (74/43/72 versus 77/42/70). The M1 first 857 bytes match that
  replay exactly; output payloads
  are not globally byte-identical because ordinary replay has changing
  environmental/RNG inputs.

The retained reconstructed file-XOR routines now permit a plaintext
record-by-record comparison (`tools/stars_game_stream.py`; both source runs
are original-executable outputs). Against *each* earlier batch replay,
the new M1 has 63 records with the same kinds, lengths, and decoded bodies
except its second BOF and two bytes of the single new type-13 planet record.
The six type-16 fleet records, six type-20 order records, and sole new type-12
message record therefore agree exactly in plaintext. That message's first ID
is 63, the no-production-queue ID used by `reconstructed/src/produce.c`.

The stable turn-1 subset is now pinned in the machine-readable
[`satisfy-orders-original-m1-stable-e3.json`](satisfy-orders-original-m1-stable-e3.json):
the exact message body, each of six fleet bodies in order, and their six
interleaved order bodies. `tests/test_satisfy_orders_original_m1_oracle.py`
checks each of the three original file hashes, decrypts every file with the
reviewed stream XOR, and requires the entire 13-record plaintext sequence to
match. This prevents a changing original replay or an accidental record
reordering from silently becoming the future comparison baseline.

The scoped decoder in `tools/satisfy_orders_original_oracle.py` now applies
the original `_fleet`/`_order` CodeView fields and the reconstructed
`WriteFleet`/`WriteOrders` byte widths to this stable subset. In all three
original runs, turn 1 has owner 0's six fleets with IDs 0–5 at planet 10,
point `(1128,1300)`. Each has one ship in its corresponding design slot,
one compact order, and fifth cargo/fuel values `50, 300, 200, 450, 530,
210`. Each compact order targets planet 10 at `(1128,1300)` with task 0,
object kind 1, and valid-task bit set. Comparing those fleets with turn 0
shows no decoded ship, cargo, location, owner, or order-count change; the
saved flags change from `0x0907` to `0x2907` (the original `_fleet`
`fHereAllTurn` bit). The scoped decoder rejects truncation and an unexpected
record variant. This is logical *fleet/order* state from original output;
planet ownership/cargo interactions, full message arguments, and any
production-source result are not yet compared.

The same scoped decoder now reads the *prefix only* of the type-13 planet
record. In all three original runs, both turns retain planet ID 10, owner 0,
flags `0x2B87`, concentrations `68/87/68`, and environment `50/50/50`.
The mineral-level presence selector changes from `0x00` (three zeros) to
`0x15` (three present values `247/241/247`). This is a decoded original
planet-field change, not evidence that `SatisfyOrders` alone caused it;
the full turn includes other production and mining stages. Two bytes in the
later variable tail differ among original replays, so that tail remains
opaque and is not normalized into an invented logical value. The decoder
rejects truncated and invalid-prefix records.

A new strict-C89 host replay (`tests/satisfy_orders_clean_state_e3_probe.c`,
`tests/test_satisfy_orders_clean_state_e3.py`) initializes production
`stars_do_orders`/`stars_satisfy_orders` with the six decoded original fleet
identities, owners, planet IDs, fuel amounts, and compact-order task/target
fields. It executes pre- and post-movement `DoOrders`, observes passes
`1,2,3,4`, and compares the six surviving id/owner/planet/fuel/order-active
fields to the pinned original turn-1 records. The scoped comparison passes.
The production boundary retires no task-0 orders and emits no order messages;
the sole original turn-1 message has code 63 and originates in production,
not in `SatisfyOrders`. This is a partial field differential, not a complete
same-input game replay: the fixture omits many live global/service inputs,
and its six task-0 orders exercise no transfer, mining, routing, or retirement
branch. The saved `fHereAllTurn` flag change occurs elsewhere in the full
turn and is not attributed to this replay. An action-bearing original turn
is required to close SO-E3's state/message differential.

For that next run, `tools/stars_game_stream.py` now has a symmetric bounded
stream encoder. Tests require byte-exact encode/decode round trips for both
the verified turn-0 input and the two-BOF original output. The experimental
builder `tools/satisfy_orders_action_candidate.py` accepts only the pinned
turn-0 M1 SHA-256, changes the first compact type-20 order into one full
type-19 order (task 5, zeroed unverified task union), and re-encodes the
remainder with the correct XOR state. A decoded candidate differs in exactly
that record and is retained at `runs/syorig-action-candidate/CLEAN.M1`
(853 bytes, SHA-256
`f40e854a83eeb12fd34c17cd3f8b7467f6b7ebfde28d3afa03ef4aed7a7ebfdc`).
At this earlier staging point it was deliberately a *candidate*, not a
valid-order or original-behavior claim. Keep the original backup inputs intact.
The isolated five-file input package in `runs/syorig-action-bundle-1/` now
contains pinned `CLEAN.XY`, `CLEAN.HST`, `CLEAN.M2`, and `STARS.EXE` plus that
candidate `CLEAN.M1`. Its `MANIFEST.JSON` has sizes and SHA-256 hashes and
explicitly marks the bundle as input only. The packager refuses an existing
destination; its test decodes the bundled M1, finds exactly one task-5 order,
and verifies the overwrite guard. No live-game files should be touched.

## Isolated task-5 original-executable authority experiment

The Win98 agent ran three fresh, isolated copies of the pinned turn-0 files
with the original `STARS.EXE -g <copy>\CLEAN.HST`. Each process returned
`EXIT 1` as in the earlier clean batch run, but each wrote a turn-1 HST/M1/M2
and backup inputs. The backups, output M1 hashes, framing, fleet state, and
message body are pinned by
`tests/test_satisfy_orders_action_original_authority.py`. The sole edited
order is the first original type-20 task-0 order changed to type-19 task-5;
the unproven ten-byte union is zeroed. The stream encoder round-trips the
unmodified source byte-exactly and the package test asserts the one decoded
record mutation. The four cases are:

| Original run | Edited input | Turn-1 owner-0 fleets | Turn-1 message |
|---|---|---|---|
| `runs/vmctl-syorig1/` | none | IDs 0–5 | ID 63, body `3f000a000a` |
| `runs/vmctl-syact1/` | M1 only | IDs 0–5 | ID 63, same body |
| `runs/vmctl-syact2/` | HST and M1 | IDs 1–5 | ID 90, body `5a000a000014000a41010a000014000a3f000a000a` |
| `runs/vmctl-syact3/` | HST only | IDs 1–5 | ID 90, same body |

The five surviving fleets retain their original fuel/cargo fifth-channel
values `300,200,450,530,210` and one compact task-0 order each. The two
host-edited runs agree on those decoded outcomes despite differing M1 input;
the M1-only run agrees with the clean control. Thus the original turn engine
is responding to this order in HST, not merely preserving an edited M1
history record. Turn-1 HST also has one fewer fleet/order record in both
host-edited runs. This is action-bearing *original behavior evidence*, not
yet a production-source differential or proof that all other changing output
fields were caused by this task alone.

The original `SatisfyOrders` body at `10b0:~8207` enters its task-5 branch
on pass 1, performs salvage/resource/technology and message work, marks the
source fleet inactive, and can emit message `0x59` or `0x5a` according to a
planet flag. The observed `0x5a` is consistent with that branch. The current
production constant/callback named `BATTLE_PLAN` is a legacy partial-adapter
label for task 5; it does **not** reconstruct this original scrap/transfer
behavior. Production must bind the actual before-state and reproduce fleet
retirement, planet/resource mutations, message arguments/order, and all four
pass effects before SO-E3 closes. The experiment establishes a usable
action-bearing oracle and exposes that source gap, but does not close the
named evidence unit or Step-6 promotion.

## Projected production gap, pinned

`tests/test_satisfy_orders_task5_projected_gap.py` checks the HST-only run's
backed-up host input: its first order is the exact 18-byte task-5 record. It
then decodes the original turn-1 owner fleets (IDs 1–5) and compiles a
strict-C89 four-pass `DoOrders`/`SatisfyOrders` probe with the same six fleet
identities, fifth cargo values, planet ID, and task sequence. With no invented
scrap service callback, production currently leaves fleet 0's task-5 order
active and emits no order message. The original removed fleet 0 and emitted
message ID 90. This is a **known mismatch**, not a passing compatibility
test: the typed probe does not yet bind the original design costs, race
traits, planet resource table, or message service state. The source now calls
task 5 `SCRAP`; `BATTLE_PLAN` remains only as an alias for old fixtures and
the callback is explicitly marked incomplete. No E1/E2/E3 item closes from
this narrower comparison.

The HST comparison has ten differing records (BOF, two planets, three
type-26 ship definitions, and four type-43 records); the M2 comparison has
five (BOF, one planet, three ship definitions). These differences are retained
and not normalized away. Reproduce with:

    python3 tools/build_core.py
    python3 -m tools.stars_game_stream runs/vmctl-syorig1/CLEAN.M1 runs/step4/fixtures/clean-replay-a/CLEAN.M1

The earlier clean comparison is an original-versus-original stability study;
the task-5 experiment adds an action-bearing original oracle. Neither is yet
a production-source replay or a broad special-order case matrix.

This closes the missing *capture* prerequisite, not the named E3 differential.
Next, replay the now-retained action-bearing original input/output turn and
its verified order, fleet, planet, and relevant service inputs through
the production `DoOrders`/`SatisfyOrders` path. Compare order retirement,
cargo/ownership mutations, message IDs/order and arguments, and four-pass
effects at the same boundary. Until those
comparisons are retained, SatisfyOrders E3 and Step-6 promotion remain open.

## Fresh isolated selector-1 original pair (2026-09-24)

The [pair audit](../../runs/vmctl-sye3k-pair-r1/PAIR.JSON) and
[verification test](../../tests/test_satisfy_orders_fresh_ai_pair.py) record
a fresh control/action pair in empty guest directories `C:\SYE3K0` and
`C:\SYE3K1`. Each received exactly `CLEAN.HST`, `CLEAN.M1`, `CLEAN.M2`,
`CLEAN.XY`, and `STARS.EXE`; `CLEAN.X2` and `CLEAN.H2` were absent before the
run. The only decoded input change is HST record 45, from a type-20 task-0
order to a type-19 task-5 order. The other four input files are hash-identical.
Owner 1's HST/M2 player word is `0x2797` (AI selector 1), with the same four
fleet/order pairs in both inputs.

Both original processes returned `EXIT 1`; their reports and returned files
are retained, and process exit is not treated as a pass. The post-run window
lists contained no error dialog. Both runs generated their own 13-record
turn-0 backup `.X2` and 95-byte `.H2`. The owner-1 turn-1 fleet/order pairs
match across control/action and HST/M2: fleet 0 moves from planet 24 to 26 at
warp 5, while fleet 1 carries 25 colonists. The input HST has owner-1 fleet
IDs 0–3; each control/action output HST and player M2 has only IDs 0/1, so
fleets 2/3 are absent from the global turn output as well as M2. M1 message
IDs are `0x3F` in the control and `0x5A` in the action. Planet 24 changes from control minerals
`592/963/520/256`, auxiliary word 8249, to action `592/964/520/256`, 8248.
The decrypted non-BOF records of both returned H2 files are identical, but
they are post-run files and do not recover the absent starting H2 or random
draws.

The retained pre-run absence of `CLEAN.X2` also proves that the original AI
dispatch ran. `EnsureAis` opens player file type 1; `SetSzWorkFromDt` maps
type 1 to the `.X2` player log, and `EnsureAis` calls `DoAiTurn` when that
open fails. The input player word `0x2797` selects original personality 1,
whose `DoAiTurn` switch calls `DoTurinDroneAiTurn`. `DoAiTurn` loads the
player-specific `.M2` state. It contains four fleet records, all owned by
player 1, and only known planet 24. `DoTurinDroneAiTurn` builds its enemy
list from non-owner fleet records, leaving the `IdTargetScout` enemy list
empty for this input. It falls back to `IdNearestUnknownPlanet`, consistent
with the output to point 26, the nearest non-source map point. The post-call
marker table and outer random value are not serialized in the retained files;
the outer value is recovered from the initialized RNG stream in the audit
below.
A source/input regression now pins the fleet-0 scout subpath to one direct
`Random(100)` and proves the wormhole and fallback random sites are skipped
for this retained case. Exhaustive startup-seed recovery now gives this draw
value 99 in the control and 4 in the action, at main-stream position 5 in both
runs. The preceding `FinishProduction` calls are before the scout, but their
only random-bearing descendant reaches production scheduling through the
`FillPlanetProdLB` UI refresh, which is skipped while `fAi == 1`. The earlier
queue `Random(4)`/`Random(3)` sites also skip on retained technology and
slot-12 marker values, so their nested draws do not intervene. The later
turn-wide stream and complete production replay remain open.

The attached `C:\SOTC\CLEAN.X2` dialog is a separate archived rejection in
[`vmctl-sytech-control/MANIFEST.JSON`](../../runs/vmctl-sytech-control/MANIFEST.JSON).
The latest read-only [guest follow-up](../../runs/vmctl-e3-livecheck-20260925/READBACK.JSON)
confirms that the current guest `CLEAN.X2` is still 355 bytes and hash-matches
the archived return. The guest input manifest lists five files and omits X2,
while the current directory contains it; without a pre-run directory listing,
that cannot establish whether it pre-existed or was generated by the run. The
host decoder parses 13 record frames, but original Stars! reported the player
log as corrupt; framing is not application acceptance. The fresh desktop
capture shows the connected agent and Explorer, with no Stars! game window.
The user-supplied dialog image is retained as
[`stars-corrupt-player-log.png`](../../runs/diagnostic-soe3-x2-20260924/stars-corrupt-player-log.png)
(7,398 bytes, SHA-256
`254a4c6baf7cc560dde3976da7e16d6bb26396cae43a26c0749463b1ed4db780`); its
message matches the archived run manifest. It confirms the rejection only;
it does not identify which X2 field Stars! rejects. This case is not either
fresh isolated pair run.

The source-guard audit now pins the outer selector-1 action branch for fleet 0.
Its type-16 M2 input has one ship in slot 0, `iplan=0`, `cord=1`, and no ships in slots
1/2/3/8/9/13/14. Player-1 technology index 3 is 0 and design slot 0 uses hull
4, so the raw Turin hull-4/technology-over-5 early exit does not apply; the
raw `DoTurinDroneAiTurn` conditions reach `IdTargetScout`. The original EXE's
hull-4 definition has three slots and a weapon slot at index 2
(`grhst=0x193e`, item `0x0100`), so `FFleetMightHaveTeeth` is true and
`IdTargetScout`'s enemy search would be possible when enemy fleets are
present. In this retained M2 view, however, there are only four fleet records,
all owned by player 1, and only planet 24 is known. `DoTurinDroneAiTurn`
builds its enemy linked list from non-owner fleet records, so the list passed
to `IdTargetScout` is empty for this input and `fOnlyHumans` has no effect.
The scout path therefore falls back to `IdNearestUnknownPlanet`. Turin resets
`fMarkedPlanets` immediately before its fleet loop. The loaded player mode is 1,
so `IdNearestColonizablePlanet` initializes planet IDs absent from M2 to
`0x10`, then assigns status 1 to the sole known, player-owned planet 24. The
four retained fleet orders all target 24, but each fleet has `cord`/order count
1, so the original `cord > 1` marker overwrite does not run. The source-derived
32-byte status column therefore has `0x10` for the 31 absent planets and 1 at
planet 24; point 26 is the nearest candidate from fleet 0's point 24. The
outer `Random(100)` value is 99 in the control and 4 in the action; the HST
audit below establishes that no nearby wormhole can change the selected
planet even when the diversion gate fires.

The selector-routing test still uses a sentinel callback for its narrow
dispatch assertion. A separate host-only binding test,
`tests/test_satisfy_orders_turin_binding.py`, decodes the retained original
input `BACKUP/CLEAN.M2` and `CLEAN.XY` into a generated C fixture, then routes
selector 1 through the production Turin turn and concrete no-enemy scout
adapter. Its unknown-candidate mask comes from the retained mode word, known
planet record, and the original `fMarkedPlanets`/`IdNearestColonizablePlanet`
initialization path. Its expected fleet-0 point, packed order, and fuel are
decoded from the original turn-1 output `CLEAN.M2`. The route selects point 26,
writes a packed MOVE order at warp 5, and passes it to movement; the retained
result is `(1313,1262)` with fuel 298. Movement uses the reconstructed default
warp allowance. The 32 AI-planet status bytes are now generated from the
original initialization rules and retained M2 state; the outer RNG result
remains fixture-supplied. Fleet weapon capability is source-bound: the test maps
each retained M2 design ID through the original EXE hull table and calls
reconstructed `stars_fleet_might_have_teeth`/`stars_hull_has_teeth`. Design 0
hull 4 has a weapon slot; designs 1/hull 15 and 2/hull 20 do not. Ideal warp
now calls reconstructed `stars_find_ideal_warp` with design 0's retained engine
1 and its 12 fuel-use values from the original EXE engine table; the probe
checks one lookup of engine 1 and the resulting warp 5. This binds only the
retained scout case (`ignore_scoops=1`), not general engine/ore-cost behavior.
This same host harness now constructs runtime fleet records from the retained
M2 fields and calls production `stars_merge_two_fleets` when Turin dispatches
the slot-2/3 merge. It confirms two slot-2 ships and 420 fuel on destination
fleet 2, zeroed/deleted fleet 3, and the expected lookup, delete, and report
invalidation calls. Cargo balancing and those external services remain
explicit fixture callbacks. The route/merge/scrap strict-C89 probe now passes 69/69 checks for each of
100 injected outer `Random(100)` values. It sends both retained owner-1
survivors through `stars_move_fleets`: fleet 0 reaches point 26 with fuel 298,
and fleet 1 remains at point 24 with fuel 200, matching the retained original
turn-1 fleet record. Fleet 1's packed task-0 order is decoded from the retained
M2; fleet 0's order is produced by the Turin turn. In the same harness, production readers
load the retained type-26 SHDEF and type-13 planet, Turin writes task 5 after
the fleet-2/3 merge, and `stars_satisfy_orders_packed(pass=1)` retires fleet 2.
It reproduces planet-24 minerals `[532,956,500,250]` → `[597,956,519,250]`,
messages `0x5A/0x141`, and first-message mineral total 84. Technology marking
and learning call reconstructed production routines; the learn draw is scripted
to `Random(100)=0`. Cargo balance and merge lookup/delete/report services and
message serialization remain fixture callbacks. Neutral-planet and multi-order
AI-status branches, exact RNG, and other full-turn state/services remain open.
The probe loads the retained HST's four type-2 thing records into the production
wormhole finder and exhausts all 100 possible outer draws. All four wormholes
are outside the finder's `4 * 545` distance limit, so every value leaves this
fleet's target at 26; with no nearby candidate, the finder consumes no second
random draw. The original outer draw still affects the global RNG sequence.
This does not compare a complete turn or run natively.
SO-E3 remains at one named open item with two unresolved subissues: (1) compare
complete production/full-turn state and services to the pinned original pair
and (2) recover the complete turn-wide RNG draw accounting. The effective
startup residues and seed words, plus scout draw 5 (99/4), are now recovered;
later positions and values remain open. SO-E2 has zero remaining. The
source-dispatch and empty-enemy-list assertions remain in
`tests/test_satisfy_orders_fresh_ai_pair.py`.

The previous branch-audit note that fleet 0 had `cord=0` was incorrect. The
retained type-16 body ends in `iplan=0, cord=1`; `FReadFleet` reads that many
type-19/type-20 order records and `WriteFleet` writes the same count before
`WriteOrders`. All four input fleets have one compact order. The raw source
guard still reaches `IdTargetScout` for fleet 0 with its one task-0 order,
technology index 3 equal to 0, and hull-4 design slot.

### Fleet 1 colonist transfer service binding

The retained input M2 decodes planet 24's colonists as 250 and fleet 1's
colonist cargo as 0. Both isolated original output M2 files have the same
fleet-1 record with 25 colonists; planet 24 ends at 256 colonists in both, a
full-turn net increase of 6. Raw `DoTurinDroneAiTurn` contains the call
`XferAiSupply(1, fleet.idPlanet, 2, fleet.id, 3, 25)` on its owned-at-planet
fleet path.

The [binding test](../../tests/test_satisfy_orders_turin_binding.py) now
derives these values from both retained original outputs. Its strict-C89
[`satisfy_orders_turin_supply_e3_probe.c`](../../tests/satisfy_orders_turin_supply_e3_probe.c)
passes the exact starting cargo through production `stars_xfer_ai_supply` and
`stars_change_cargo`. Its strict-C89 selector-1 probe derives capacity from
retained M2 design 1, original EXE hull 15's 25-unit cargo maximum, and
reconstructed `stars_wt_max_ship_design_stat`, `stars_get_fleet_stat`, and
`stars_get_cargo_free`. The route fixture decodes current planet mineral values
and fleet cargo values from packed `_planet` and `_fleet` layouts.
Reconstructed `stars_turin_plan_local_transport` applies the raw owned-at-planet
branch: a fleet at its current owned planet with positive population selects
the local supply action. Neutral, empty-population, and off-planet fallback
branches are not generalized. The retained state selects transport;
the helper queries the planet, adds 25 to fleet 1, then debits the planet:
immediate states are 225/25. The same binding then passes retained
planet/player environment and growth inputs through production
`stars_planet_desirability`, `stars_calc_planet_max_pop`, and
`stars_change_population`; +31 with fraction 50 reaches the original 256
colonists. This matches the population service slice and source-binds the
retained transport decision, but does not establish the full surrounding call
order, so the full-turn differential remains open.

### Owner-1 planet 24 mining bound

The fresh pair's hash-pinned owner-1 planet/player inputs now run through the
current-source `stars_est_minerals_mined` service in strict C89. Raw
`CMinesOperating`/`CMaxOperableMines`/`CMaxMines` resolve 10 working mines
from 10 built mines, 250 colonists, mine-operating stat 9, and maximum planet
population 10,000. The player mining-efficiency stat is 10 tenths; mineral
concentrations are 68/87/68 and the three mining products are 680/870/680
hundredths. `EstMineralsMined` therefore yields floors 6/8/6, with random
rounding ranges 6–7/8–9/6–7. Two explicit draw witnesses produce 7/8/7 and
6/9/6; both reproduce mineral percentage levels 247/241/247 while retaining
concentrations 68/87/68. The strict-C89 replay is
[`test_owner1_mining_stage_is_replayed_from_retained_input`](../../tests/test_satisfy_orders_task5_four_pass_delta.py)
and uses the production implementation in
[`est_minerals_mined.c`](../../reconstructed/src/est_minerals_mined.c).

The current-source Turin merge/task-5 integration has planet stocks
597/956/519 before mining. Applying the bounded mining yield gives stock
ranges [603..604, 964..965, 525..526]. The original turn-1 stocks
are [592,963,520] in control and [592,964,520] in action. The ending queue
record's first design-0 entry carries 81 percent progress. The pinned original
EXE hull/part table rows store design-0 costs [35,16,2,8] in table order
(resources, ironium, boranium, germanium). Raw
[`GetProductionCosts`](../../analysis/decompiler/export/raw/10d0_3f20_GetProductionCosts.c)
reorders these as [16,2,8,35] for the production vector (ironium, boranium,
germanium, resources). Raw
[`Produce`](../../analysis/decompiler/export/raw/10b8_0000_Produce.c)
passes packed progress to
[`CBuildProdItem`](../../analysis/decompiler/export/raw/10b8_0c92_CBuildProdItem.c),
which prorates that vector. At 81 percent, its floor costs are [12,1,6,28],
so ore spend is [12,1,6]. The legal mining witnesses [7,8,7] for control and
[7,9,7] for action then give exact stock equations:

```
control: [597,956,519] + [7,8,7] - [12,1,6] = [592,963,520]
action:  [597,956,519] + [7,9,7] - [12,1,6] = [592,964,520]
```

This is a retained-data arithmetic witness for both mineral stock endpoints,
not a recovered random draw sequence or full-turn replay. The verified turn-0
HST has no type-28 queue. The retained 32-point map and raw Turin turn-zero
seed branch now give a source explanation for the two design-0 entries: with
an open production slot and no existing ship queue, the branch walks its
counter `32 → 2 → -28` and requests two one-hull entries. A strict-C89 current-
source probe feeds those generated entries directly into `Produce` and
matches the original ending queue and action mineral stocks. The retained
planet24 type-13 record has serialized flags `0x2b87`; `stars_read_planet`
produces runtime flags `0x0f07` and fourth population lane 250. Runtime bit 9
and the 200-population threshold pass the raw production-slot predicate.
`stars_planet_desirability` returns 100 from the retained planet environment
`[62,33,61]` and player environment `[62,33,61,32,6,26,92,60,96]`. Later Turin
queue branches are not all bound; this remains a scoped queue/production
replay. The original
`Randomize2(GetTickCount())` seed and full draw sequence remain unknown, as do
other surrounding turn services. The complete production/full-turn comparison
therefore remains open.

Both original turn-1 HST outputs place an 8-byte type-28 production record
immediately after planet 24 with identical payload `0100140501000400`. Its two
four-byte entries both decode as ship production, design 0, count 1; the
remaining packed words are `0x0514` and `0x0004`, encoding progress values 81
and 0 percent. The verified turn-0 HST has no type-28 record. Raw Turin's
turn-zero seed loop requests two design-0 entries for `cPlanMax=32`; the new
strict-C89 queue-to-`Produce` probe reconstructs that conditional branch and
matches the original ending queue. It derives the packed flag/population gate
from retained planet24 state, computes desirability 100 from retained player
and planet environments, and supplies the no-existing-ship-queue state; other
AI queue branches are not all bound from the original files. The retained
input's design-0 hull and
parts cost [35,16,2,8] in table order ([16,2,8,35] in runtime production
order); the 81-percent entry's prorated ore cost [12,1,6] plus the two legal
current-source mining witnesses exactly reconstruct both observed planet-24
stock triplets. `test_owner1_mining_stage_is_replayed_from_retained_input`
checks the packed queue fields, retained design costs, outputs, and source
mining witnesses. It does not recover the historical random draws or establish
the complete turn-service sequence.

The raw Turin source also contains a conditional `MergeAllShdefs(0xC)` pass
for design slots 2/3. The retained player-1 M2 binds its slot-2 SHDEF at record
5: flags `0x0907` leave the bit-9 gate clear, hull 20 is the original
two-slot `Midget Miner`, and its components are one item-1 group-1 part plus
two item-0 group-`0x80` `Robo-Midget Miner` parts. The hash-pinned executable's
group-`0x80` item-0 record has `dp=5`, so `CMineFromLpfl` returns 10 for each
one-ship fleet, below its 4000 sentinel. Fleets 2 and 3 have that design, same
owner, planet 24, and point `(1290,1258)`. M2 record order is fleet IDs 0, 1, 2,
3, so `MergeAllShdefs` takes fleet 2 as the destination and fleet 3 as the
deleted source. The remaining fleet still carries design slot 2. In the
turn-zero miner branch, `DoTurinDroneAiTurn` writes task 5 to that fleet; the
original `DoOrders` pre-movement `SatisfyOrders(1)` handles task 5 on pass 1 and
sets its dead bit. This source-derived chain explains why both IDs are absent
from the turn-1 HST and M2. It resolves the fleet-fate subquestion, but does not
close the production/full-turn differential or recover RNG. The HST/M2 ID assertions are retained in
[`test_satisfy_orders_fresh_ai_pair.py`](../../tests/test_satisfy_orders_fresh_ai_pair.py).

That audit now also feeds the retained fleet-2/3 identity, position, owner,
ship counts, mine value, and task-5 order into a strict-C89 integration probe.
It calls production `stars_merge_all_shdefs(0x000c)`, routes the merge callback
through production `stars_merge_two_fleets`, then sends the survivor through
packed `stars_satisfy_orders_packed` pass 1. The probe confirms fleet 2
survives with both slot-2 ships before task 5, fleet 3 is deleted, and task 5
marks fleet 2 inactive; the original turn-1 HST output retains only IDs 0/1.
The probe now loads the retained type-26 SHDEF and type-13 planet with the
production readers, and resolves hull and part records from the pinned original
EXE through the reconstructed lookup/repricing path. It runs the real packed
task-5 service after the production merge. The recovered design costs are
123/41/0/12; planet 24 changes from 532/956/500/250 to 597/956/519/250; and
the two messages are 0x5A and 0x141, with 84 as the first message mineral
total. Fleet 3 merges into fleet 2, then task 5 retires fleet 2, leaving output
fleet IDs 0/1. This corrects the earlier callback-only report.

The probe now calls production `stars_mark_techs_seen_shdef` using
technology bytes read from the pinned original EXE part rows. Its task-5
callback also calls production `stars_itech_learn_a_tech` with the retained
player technology levels and a scripted first `Random(100)=0`, which exercises
the no-learn result used for message `0x141`. That scripted value is not the
original run’s recovered draw; startup seeding and complete draw accounting
remain open. Message serialization stays a fixture callback, and cargo balance
still sums the retained cargo lanes instead of applying the full capacity
adapter. The 13-test fresh-pair audit passes. The integration remains host-only and
does not compare all full-turn services.

The attached `C:\SOTC\CLEAN.X2` dialog is a separate archived rejection in
[`vmctl-sytech-control/MANIFEST.JSON`](../../runs/vmctl-sytech-control/MANIFEST.JSON).
A read-only controller retrieval on 2026-09-24 first copied the 355-byte guest
file to [`runs/diagnostic-soe3-x2-20260924/CLEAN.X2`](../../runs/diagnostic-soe3-x2-20260924/CLEAN.X2).
A second live read on 2026-09-25 is retained in
[`runs/vmctl-e3-livecheck-20260925/`](../../runs/vmctl-e3-livecheck-20260925/);
its X2 hash matches the first retrieval, and the guest input manifest lists
only HST/M1/M2/XY/EXE. The current `C:\SOTC` listing contains X2, but it is not
a pre-run listing, so the file's starting presence remains unresolved. The
host decoder parses 13 record frames, but Stars! rejected the player log;
framing is not application acceptance. This screenshot is not either fresh
isolated pair run.

### Wormhole diversion bound to the retained HST

The original input HST (`runs/vmctl-sye3k0/BACKUP/CLEAN.HST`, SHA-256
`74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6`)
contains a type-43 thing-count record of four, followed by four 18-byte type-2
wormhole records. Relative to fleet 0's source point `(1290,1258)`, their
squared distances are:

| Thing ID | Point | Distance squared |
|---:|---:|---:|
| 0 | `(1142,1049)` | 65,585 |
| 1 | `(1045,1197)` | 63,746 |
| 2 | `(1197,1024)` | 63,405 |
| 3 | `(1275,1132)` | 16,101 |

The nearest unknown planet is point 26 at squared distance 545. Raw
`IdNearestUnknownPlanet` calls `Random(100)` because fleet 0 is on planet 24;
only values below 5 call `LpthWormFind`. That routine considers things within
four times the supplied planet distance, or 2,180 square units here. None of
the four wormholes qualifies, so the production finder returns null without
its later candidate-selection random call. The integrated host test loads these
retained HST rows into `stars_lpth_worm_find` and exhausts all outer values 0 through 99;
every value yields the same target 26 and movement result. This establishes that the
unknown outer draw does not change fleet 0's selected target, while its effect
on subsequent global RNG state and turn services remains unaccounted.

### XY coordinate cross-check

The four retained `CLEAN.XY` copies used by the fresh selector-1 pair and the
older original replay share SHA-256
`580e4c3be92eee570c95c97faa2557e3b4fe92856387970cf548700e1bb3fd88`. The
original `FLoadGame` reader takes `game.cPlanMax` packed points after the
64-byte game record; `GenerateWorld` defines the delta-X and Y fields. Decoding
the 32 points gives planet-array entry 24 at `(1290,1258)`, matching fleet 0's
turn-0 position, and entry 26 at `(1313,1262)`, matching its original turn-1
destination. Entry 26 is the nearest non-source point from 24 (squared
distance 545; entry 27 is next at 1250). This supports the recorded movement
as a nearest-target result, but does not establish which candidates Turin
marked or which `IdTargetScout` sub-branch selected the target. The bounded
decode and coordinate assertions are retained in
`tests/test_satisfy_orders_fresh_ai_pair.py`.

### RNG initialization source trace

The fresh pair's launch path establishes that the `FGenerateTurn` tutorial
reseed gate is false. The original EXE's DGROUP starts with `gd.grBits` at
`1120:07ca` equal to `0x00000000` (segment 37, file-backed offset `0x7ca`,
verified against EXE SHA-256
`ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`).
WinMain's recorded `-g C:\\SYE3K0\\CLEAN.HST` / `-g C:\\SYE3K1\\CLEAN.HST`
launch path sets the turn-generation option and posts message `0x464`; that
handler runs `EnsureAis()` then `FGenerateTurn()` directly. The `-g` option
does not set `gd.fTutorial`; it is distinct from the `-b` batch-file option.
The startup `FCreateStuff` mask `0xfb7f` preserves bit `0x0800`, and the
source audit finds `StartTutor` as the explicit setter of that bit, outside
this `-g` path. Thus `FGenerateTurn` skips `Randomize(0x499602d2)` for this
pair. If the gate were true, that routine would select seed-table entries 18
and 11, values 71 and 41.

The process RNG state is still unknown. WinMain first calls
`Randomize2(GetTickCount())`; `Randomize2` derives its two seed-table indices
from the tick value as `(tick & 0x7f) ^ 0x35` and
`((tick >> 7) & 0x7f) ^ 0x5c` (incrementing the second modulo 128 if the
indices collide). The guest tick count at launch was not recorded, nor was the
complete draw sequence through startup/`EnsureAis` and both generated turns.
The pair's post-run H2 files do not expose the in-memory RNG state. The BOF
file salt is separate from the game's two seed words, but `WriteBOF` itself
calls `Random(2000)` before combining that value with `GetTickCount()`. Those
writer calls advance the same game RNG, so output serialization belongs in
the draw ledger even though the stored salt does not directly reveal the
seed pair. RNG accounting therefore remains one part of SO-E3's open full-turn
check.

### Static RNG call-site inventory

The address-based transitive call graph in
[`callgraph.json`](../decompiler/callgraph.json) gives an upper bound on
source sites that need checking. The retained Turin routine's call closure
contains ten functions with `Random` call edges, including Turin's
conditional queue/route branches and helpers such as
`IdNearestUnknownPlanet` and `SetAiFleetIdealSpeed`. The
`FGenerateTurn` closure contains 28 such functions, including
`EstMineralsMined`, `ValidateWaypoints`, `MoveThings`, `MoveFleets`, mine-field,
production, climate, bombing, and combat routines. These are statically
reachable sites, not proof that every site ran in this pair; their guards
depend on runtime state.

One AI prelude can be resolved for the retained input: `InitRandomPlanetList`
collects player-1's owned planets, and the retained state has only planet 24.
Its shuffle loop runs `vclpplAi - 1` times, so this list contributes zero
`Random` calls. This removes one possible source of variable pre-turn draws;
the later Turin branches still have unresolved conditional calls.

The retained M2 also resolves two direct Turin queue guards. Player technology
bytes are `[0,0,1,0,5,0]`, and the M2 contains SHDEF slots 0, 1, 2, and 16;
none of the four fleets has a design-12 ship. `FLoadGame` initializes all 16
per-player SHDEF entries with bit 9 set before reading saved SHDEF records, so
the absent slot 12 remains hidden. `EnsureTurinDroneShdefs` cannot create slot
12 because its two required technology bytes (indices 3 and 5) are both 0.
Thus `DoTurinDroneAiTurn` skips its slot-12 `Random(3)` guard and the nested
`Random(8)`/`Random(1)` sites. Its earlier `Random(4)` guard also fails:
technology byte 2 is 1, below the required value greater than 4. These input
and source checks are retained in
[`test_turin_rng_guard_inputs_are_pinned`](../../tests/test_satisfy_orders_fresh_ai_pair.py).

The new retained-input regression
[`test_retained_turin_fleet0_scout_path_has_one_known_random_call`](../../tests/test_satisfy_orders_fresh_ai_pair.py)
counts the direct draws on fleet 0's selector-1 scout subpath. The empty
non-owner enemy list skips `IdTargetScout`'s `Random(3)` tie-break, and nearest
unknown planet 26 avoids its `Random(game.cPlanMax)` sentinel fallback.
`IdNearestUnknownPlanet` consumes one `Random(100)` because the fleet is at
planet 24. If the value is below 5, all four retained HST wormholes remain
outside the squared-distance limit 2180 (the closest is 16101), so
`LpthWormFind` returns before its own `Random(100)` and the optional Turin
wormhole draw is also skipped. The branch therefore consumes exactly one
direct `Random(100)` for every possible outer result. Advancing the uniquely
recovered startup states through four zero-retry starbase-name draws gives
the observed scout values 99 (control) and 4 (action), at draw position 5 in
both runs. The route test separately exhausts all 100 outer values and
preserves target 26. The new source audit also checks that pre-scout
`FinishProduction` cannot draw through `FillPlanetProdLB`: the UI refresh is
guarded by `fAi == 0`, while `DoAiTurn` sets `fAi = 1` before Turin dispatch.
It checks the four lexically earlier Turin queue sites as well: `Random(4)`
is guarded by tech byte 2 > 4 (retained value 1); `Random(3)` is guarded by
slot-12 bit 9 being clear, but FLoadGame initializes absent slot 12 with that
bit set and the tech gate prevents its creation. Nested `Random(8)` and
`Random(iStack_9c * 2 + 1)` are skipped with the `Random(3)` branch. Other
random-bearing helpers and `FGenerateTurn` still need a full-turn draw trace.

The clean pair's retained `CLEAN.XY` decrypts to a 64-byte type-7 game record
with `game.cPlayer == 2` at offset 8; its hash is identical in control and
action. The direct `FGenerateTurn` shuffle therefore makes exactly two
structurally known calls on the retained non-tutorial path, `Random(2)` and
`Random(1)`, before the routine calls `Random(8)` and stores that result in the
game flags. The new regression checks the original game-record field, the
source loop, and the reconstructed RNG implementation. `stars_random` advances
both seeds before checking the requested range, so the final `Random(1)` call
still consumes a draw even though it returns zero. After the recorded
`Random(8)`, `FGenerateTurn` calls `FWriteDataFile` once for each player and
once for the aggregate output; successful `FCreateFile`/`FAppendFile` paths
write a BOF, and `WriteBOF` consumes one `Random(2000)` per BOF before using
the tick count in the file salt. The number and order of earlier conditional
draws through `EnsureAis`, log replay, and turn services are still not
recovered, so the call-site inventory does not close RNG accounting.

For the fresh pair, successful output artifacts bound this writer tail to five
`Random(2000)` calls: missing pre-run X2/H2 files take the retained owner-1
AI through `DoAiTurn`, which calls `FWriteLogFile` and `FWriteHistFile`; their
returned X2 and H2 streams each have one turn-0 BOF. `FGenerateTurn` writes
one data file per each of two players and one aggregate file; the returned HST,
M1, and M2 each contain a turn-1 BOF. M1's additional turn-0 BOF is the
preserved input record. Tests pin those BOF sequences for both control and
action and check the original `FCreateFile`/`FAppendFile` to `WriteBOF` call
path. This counts successful writer draws but does not recover earlier AI or
turn-service draws or their ordering relative to the shuffle.

The owner-planet inputs also pin six more `Random(100)` calls in the
`MineMinerals` stage. The HST has 32 planets, but only IDs 10 and 24 are owned;
both records show 250 colonists and ten built mines. Their owners have mining
efficiency 10 tenths and operating-mine limits of 10% and 9%, so the
population cap alone permits at least 25 and 22 mines and the ten built mines
are all operating. Both planets have concentrations 68/87/68, yielding
hundredths 680/870/680 with remainders 80/70/80. Raw `MineMinerals` visits all
32 records; unowned planets return before rounding, while each owned planet
has three nonzero remainders and therefore three `Random(100)` calls when the
original random-rounding flag is enabled. The new regression checks the full
input planet/player set, mine caps, products, remainders, and raw source guards.
It pins the count at six, not their values; turn-stage ordering and other
conditional draws remain unresolved.

Across the known runtime sites, the pair now has a lower bound of 15 RNG
calls: one Turin scout `Random(100)`, two AI output-writer `Random(2000)`
calls, two shuffle calls, six mining `Random(100)` calls, the observed
`Random(8)`, and three turn-output `Random(2000)` calls. Conditional calls from
movement, other production services, and Turin helpers can add draws between
these known sites. The value 15 is a lower bound, not the complete stream
length or startup seed.

One late pre-output draw is directly observable. `FGenerateTurn` calls
`Random(8)` and
stores its value in `game.overlap_16` bits 9–11; `WriteBOF` copies those bits
into the turn-file flags at bits 13–15. The turn-1 HST BOFs therefore record
the draw without decrypting or interpreting any other payload. The clean
`vmctl-syorig1` and `vmctl-sye3k0` runs have identical starting HST/M1/M2
backup hashes, yet their observed `Random(8)` results are 0 and 1. The fresh
task-5 action run `vmctl-sye3k1` records 4. This proves that identical clean
inputs across separate launches do not share the same observed random stream;
the task-0/task-5 output comparison cannot attribute all changed turn fields
to the order edit. The observation recovers only this one late pre-output
draw, not the startup seed or preceding calls. Each subsequent successful
output BOF consumes its own `Random(2000)` in `WriteBOF`; that call advances
the game RNG even though the stored file salt does not expose the two seed
words. The regression is in
[`test_satisfy_orders_fresh_ai_pair.py`](../../tests/test_satisfy_orders_fresh_ai_pair.py).

### Reconstructed turn-zero task-5 assignment

The original source path at `DoTurinDroneAiTurn`'s turn-zero miner branch
assigns task 5 to the surviving fleet after the SHDEF merge. The reconstructed
`stars_do_turin_drone_ai_turn` previously omitted this packed order mutation.
It now changes the first order's low nibble through
`stars_ai_set_first_order_kind`, preserving the other order flags and bytes.
The fleet view passed to the policy loop must reflect fleets removed by merge
callbacks; the selector-1 binding test models fleet 3's removal before checking
that only fleet 2 receives task 5.

The retained merge/scrap integration starts the first order at task 0, loads
and merges the original fleet-2/3 state, gives the merged survivor and its
packed PLORD to `stars_do_turin_drone_ai_turn`, and passes its generated order
into `stars_satisfy_orders_packed(pass=1)`. No task-5 order is written by the
fixture. The production chain still matches the original fleet IDs 0/1, cost
123/41/0/12, planet 24 state 597/956/519/250, and messages 0x5A/0x141 with
first-message total 84. The merge/scrap integration probe is host-only and keeps the production
tech-learn stub plus fixture technology marking, message serialization, and
cargo-capacity services. It binds this fleet fate, not the whole turn. A separate
fresh StarDev VC1.52c [task-write probe](../../runs/vmctl-turin-miner-task-e3-r1/MINER.TXT)
passes 24/24 and matches the [strict-C89 host report](../../runs/vmctl-turin-miner-task-e3-r1/HOST.TXT);
its [manifest](../../runs/vmctl-turin-miner-task-e3-r1/MANIFEST.JSON) pins source and returned artifact hashes.
That native check covers the turn-zero task-5 write only, not packed merge/scrap.

The same selector-1 binding now derives fleet 1's retained owned, inhabited
current-planet transport action through `stars_turin_plan_local_transport`,
then calls production `stars_xfer_ai_supply` and `stars_change_cargo`. It
verifies the original query/add/debit sequence and
immediate colonist state 250/0 → 225/25. It then supplies retained race and
planet environment values to production `stars_planet_desirability`,
`stars_calc_planet_max_pop`, and `stars_change_population`. The reconstructed
growth adds 31 with fraction 50, matching the original planet's final 256
colonists and fraction 50 after the transfer. The 25-unit capacity now comes
from retained M2 design 1, original EXE hull 15, and production fleet/free-cargo
helpers. The local transport decision is source-derived for this retained
state. Other planet/resource changes and the wider call sequence are still
unmatched.

### Current-source partial production arithmetic

The host strict-C89 test
[`test_c_build_prod_item_source.py`](../../tests/test_c_build_prod_item_source.py)
now exercises the reconstructed `CBuildProdItem` stage with the retained
design-0 runtime cost `[16,2,8,35]` and witnessed pre-build resources
`[604,965,526,28]` (ironium, boranium, germanium, resources). For the ordinary
ship queue item, raw `CBuildProdItem` writes 81-percent progress back into that
item, charging `[12,1,6,28]` and leaving `[592,964,520,0]`; it does not emit a
separate partial entry for this item class. A second call supplies the
synthetic arithmetic remainder `[4,1,2,7]`, completes one item, and clears its
progress.

The strict-C89 current-source [`Produce` probe](../../tests/test_produce_source.py)
starts with two supplied design-0 queue items at zero progress, then runs the
first through `stars_produce` and `stars_build_production_item`. The focused
SO-E3 [`Turin seed-to-Produce probe`](../../tests/test_satisfy_orders_turin_seed_produce.py)
now starts from the independently pinned post-task-5 planet-24 mineral stock
`[597,956,519]`, calls `stars_est_minerals_mined` with retained operating
mines/concentrations and legal `Random(100)` witnesses, and passes the mutated
stock plus the queue generated by `stars_do_turin_drone_ai_turn` to
`stars_produce`. The partial-build callback reads the retained EXE-backed
design-0 runtime cost vector `[16,2,8,35]`. The control witness `[0,99,0]`
produces `[7,8,7]` and the
action witness `[0,0,0]` produces `[7,9,7]`; after the 81-percent design-0
build, these match original planet-24 stocks `[592,963,520]` and
`[592,964,520]`. Both runs also match the original mineral levels/concentrations
and exact type-28 queue payload. The witness values are selected inside the
originally derived ranges and do not claim to recover the RNG stream. The
probe passes 12/12 strict-C89 checks per run. The separate seed/finish matrix
passes 24/24; adjacent schedule integration, differential, mutation, and
resource-queue probes pass 34/34 combined. The packed flag/population
production-slot predicate and desirability 100 come from retained planet24/
player state. This seed-only probe supplies task-5 scrap stock as an input; a
second probe now joins that stock to the same reconstructed turn.

The strict-C89
[`merged task-5/scrap/mining/Produce probe`](../../tests/test_satisfy_orders_fresh_ai_pair.py)
reads retained M2 records and original EXE part rows, merges fleets 2/3,
assigns task 5 and queues two design-0 items through current-source Turin,
then runs reconstructed `DoOrders` passes 1–2 before mining/production and
passes 3–4 afterward. Packed task-5 scrap runs during pass 1; the same retired
fleet/order state is seen by the later passes. It then calls `Produce` and
`CBuildProdItem`. The control produces post-scrap stock `[597,956,519]`, mining
`[7,8,7]`, pre-build stock `[604,964,526]`, and endpoint `[592,963,520]`. The
action produces `[597,956,519]`, `[7,9,7]`, `[604,965,526]`, and
`[592,964,520]`. Both finish with two design-0 entries at 81% and 0% progress.
The three injected `Random(100)` values per run are legal witnesses inside the
retained output ranges, not recovered historical draws. The five-module
focused suite passes 26/26. Other turn services and original draw accounting
remain open.

**At this checkpoint**, SO-E2 had 0 closure checks remaining and SO-E3 had 1
named acceptance item with 2 open subissues: complete production/full-turn
comparison across fleet, planet, cargo, orders, resources, messages, and turn
services; and exact `Randomize2(GetTickCount())` startup seed plus complete
draw accounting. The new task assignment closed neither subissue.

Evidence: `analysis/compat/satisfy-orders-full-turn-e3-original-oracle.json`,
`analysis/decompiler/export/raw/10b0_179a_DoOrders.c`,
`analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`,
`tests/satisfy_orders_full_turn_e3_probe.c`, and the native artifacts in
`runs/vmctl-vc152syfte4/`. Original replay outputs and verified backup inputs
are in `runs/vmctl-syorig1/`; the fresh isolated pair returns are in
`runs/vmctl-sye3k0/` and `runs/vmctl-sye3k1/`.


## Source-bound owner-0 four-pass replay update (2026-09-24)

The strict-C89 owner-0 four-pass probe now creates its fixture header from
retained evidence. It decodes the six owner-0 fleet records and their
following type-19/type-20 orders from
`runs/vmctl-syact3/BACKUP/CLEAN.HST`, checks owner-0 IDs 0–5, and converts those
records to the runtime fleet/order layout. Planet 10's source fields come from
the retained original planet oracle; the tested hull ID and design costs come
from the EXE-backed design-cost oracle. The same `write_retained_probe_header`
helper feeds both action and control runs.

The combined focused suite
(`test_satisfy_orders_task5_four_pass_delta`,
`test_satisfy_orders_turin_binding`, and
`test_satisfy_orders_fresh_ai_pair`) passes 21/21. The four-pass action replay
matches the retained action's planet-10 minerals/population and the `0x5A`,
`0x141`, and `0x3F` message frames; the control/action test also checks the
retained control output. This is supporting evidence for one replay slice.

Fixture limits remain explicit: runtime fleet word `+118` is set to `0x0060`,
planet byte `+0x16` is set to `160`, the generated design record fills the
hull ID and cost words but not a complete live design, and the mining-rounding
witness is injected. The run is host-only and does not replay all original
turn services. Therefore neither SO-E3 component closes: full-turn state and
service comparison remains open, as does recovery of the startup seed and
complete RNG draw stream. SO-E3 remains one open acceptance item with two
unresolved components; SO-E2 remains complete.


## Startup RNG seed-domain update (2026-09-24)

`WinMain` passes `GetTickCount()` to `Randomize2`. The original `Randomize2`
formula uses `(tick & 0x7f)` and `((tick >> 7) & 0x7f)` to choose its two
seed-table indices; bits 14–31 do not affect the initialized pair. The new
regression `test_randomize2_tick_seed_search_uses_only_low_fourteen_bits`
checks all 16,384 possible low-14-bit tick values and verifies the current
initializer returns the same state after setting the upper tick bits. Thus the
startup search has 16,384 low-bit inputs. This does not identify the actual
input, and it does not account for calls after initialization. The 22-test
focused four-pass/Turin/fresh-pair suite passes with this check included. The
startup seed candidate and complete draw stream remain unresolved, so E3 still
has two open components.


## BOF tick/RNG mixed outputs (2026-09-24)

`WriteBOF` calls `Random(2000)`, then `GetTickCount()`, and stores
`(tick + random) * 32` alongside the low-five-bit player ID in a 16-bit word.
The upper eleven bits retain `(tick + Random(2000)) mod 2048`. The new retained
BOF test verifies the word against the source for both fresh runs. Values below
are exact decoded fields; turn-0 `CLEAN.M1` is inherited and is excluded.

| Run | X2 turn 0 | H2 turn 0 | M1 turn 1 | M2 turn 1 | HST turn 1 |
|---|---:|---:|---:|---:|---:|
| clean control `vmctl-sye3k0` | 752 | 946 | 1807 | 50 | 1509 |
| task-5 action `vmctl-sye3k1` | 1732 | 78 | 1018 | 330 | 901 |

Each value combines an unknown tick count with the `Random(2000)` result, so
this records five constraints per run without separating either value. The
full draw sequence and initial candidate are still open. The focused four-pass,
Turin-binding, and fresh-pair suites pass 24/24.

## Original mining draws bounded from the fresh pair (2026-09-24)

The new regression
(`tests/test_satisfy_orders_fresh_ai_pair.py::test_original_mining_outputs_bound_six_random100_draws`)
uses the fresh original turn-0 input `CLEAN.HST` (SHA-256
`74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6`) and
both retained turn-1 HST outputs. The full-map audit identifies only planets
10 and 24 as owned; each has ten operating mines, efficiency 10/10, and
concentrations 68/87/68. `MineMinerals` walks the planet array, while
`EstMineralsMined` evaluates the three resources in order and rounds each
nonzero remainder with `Random(100)` when the original random-rounding flag
is active.

The output mining amounts are isolated from the other stock changes using
the separately pinned owner-0 task-5 salvage `[9,6,5]`, owner-1 task-5
post-scrap stock `[597,956,519]`, and subsequent production spend `[12,1,6]`.
For a base yield `floor(10 * concentration / 100)` with remainder `r`, a
rounded-up output bounds the draw to `[0,r-1]`; an unrounded output bounds it
to `[r,99]`.

| Run | Planet | Ironium yield / draw range | Boranium yield / draw range | Germanium yield / draw range |
|---|---:|---:|---:|---:|
| Control `vmctl-sye3k0` | 10 | 7 / 0–79 | 9 / 0–69 | 7 / 0–79 |
| Control `vmctl-sye3k0` | 24 | 7 / 0–79 | 8 / 70–99 | 7 / 0–79 |
| Action `vmctl-sye3k1` | 10 | 6 / 80–99 | 9 / 0–69 | 7 / 0–79 |
| Action `vmctl-sye3k1` | 24 | 7 / 0–79 | 9 / 0–69 | 7 / 0–79 |

These ranges constrain all six mining-rounding draws in each original run.
They do not recover any exact `Random(100)` value or its position in the
global stream. The PAIR.JSON record keeps the output observations, stock
decomposition, and limitation together. The screenshot showing Stars! reject
`C:\SOTC\CLEAN.X2` remains a separate case: that file's pre-run presence is
unknown and it is excluded from this fresh pair. No SO-E3 component closes;
the full production-turn differential and exact startup seed/draw accounting
remain the two open components.

## Fresh owner-0 paired four-pass replay (2026-09-24)

The strict-C89 regression
`tests/test_satisfy_orders_task5_four_pass_delta.py::test_fresh_pair_owner0_action_runs_all_four_orders_passes`
now replays both sides of the isolated original pair. The retained turn-0
HST inputs differ only at record 45: control has task 0 and action has task 5
for fleet 0 at planet 10. Their `CLEAN.M1`, `CLEAN.M2`, `CLEAN.XY`, and
`STARS.EXE` inputs are identical. Planet 10 begins at `[469,893,376,250]`.

The probe executes production `DoOrders` passes 1 and 2, mining and `Produce`,
then passes 3 and 4. It matches both retained turn-1 planet records:

| Run | Planet-10 minerals and colonists | Task-5 fleet 0 | M1 messages |
|---|---|---|---|
| Control | `[476,902,383,287]` | remains | `0x3F`; raw record `3f000a000a` |
| Action | `[484,908,388,287]` | retired | `0x5A, 0x141, 0x3F`; raw record `5a000a000014000a41010a000014000a3f000a000a` |

The replay also matches the retained output fields and orders for surviving
owner-0 fleets 1–5. It injects bounded mining witnesses (`[0,0,0]` for control
and `[99,0,0]` for action); the original pair constrains these draws to ranges,
but does not reveal their exact values or stream positions. The five-module
focused host suite passes 27/27.

This owner-0 planet-10 replay and the separate owner-1 planet-24 merge,
Turin task-assignment, scrap, four-pass, mining, and production chain are
bounded current-source slices. Together they do not run one complete
`FGenerateTurn` comparison across all state and services. The original
`Randomize2(GetTickCount())` seed and complete random draw sequence also
remain unrecovered. SO-E3 therefore remains one open acceptance item with two
unresolved components; SO-E2 remains complete.

## Retained control/action output-record census (2026-09-24)

The fresh-pair regression now subtracts exact `(turn, kind, body)` record
multisets for the original control and action outputs, then records changed
records by file, turn, and kind in the machine-readable oracle. It pins these
control-only/action-only counts:

| File | Control-only records | Action-only records |
|---|---|---|
| `CLEAN.HST` | turn 1: kind 6 ×1, 8 ×1, 13 ×2, 16 ×1, 20 ×1, 26 ×5, 43 ×4 | turn 1: kind 6 ×1, 8 ×1, 13 ×2, 26 ×5, 43 ×4 |
| `CLEAN.M1` | turn 1: kind 6 ×1, 8 ×1, 12 ×1, 13 ×1, 16 ×1, 20 ×1, 26 ×1, 45 ×1 | turn 1: kind 6 ×1, 8 ×1, 12 ×1, 13 ×1, 26 ×1, 45 ×1 |
| `CLEAN.M2` | turn 1: kind 8 ×1, 13 ×1, 26 ×4 | turn 1: kind 8 ×1, 13 ×1, 26 ×4 |
| `CLEAN.H2` | turn 0: kind 8 ×1 | turn 0: kind 8 ×1 |

Kind 8 is the BOF; kind 12 is the owner-0 task-5 message; kinds 13, 16,
and 20 carry planet, fleet, and order records. The four kind-43 records per
run are retained moving things whose wormhole fields are separately replayed
by `tests/test_satisfy_orders_wormhole_move_e3.py`. The changed kind-26 and
kind-45 bodies, all planet fields, the complete message streams, and
global causal attribution are not covered by the owner-specific production
slices as one turn. The owner-0 kind-6 player record is now decoded: its only
changed byte is `_player.cFleet` at offset 4, changing from six to five in both
HST and M1, consistent with task-5 fleet retirement. The remaining 129 bytes
match. The type-26 records are now field-decoded against the recovered
`_rtshdef` layout. In HST and M1, the action-side design slot 0 differs only
at `_rtshdef.cExist` (byte offset 13), changing from 1 to 0; this is
consistent with the task-5 fleet retirement, but the combined current-source
turn replay does not yet serialize and compare that SHDEF mutation. In HST
and M2, AI design slots 17–20 retain identical 17-byte headers and component
pairs; only their variable compressed-name tails differ. The control/action
tail lengths are 5/5, 6/12, 11/11, and 12/13 bytes for slots 17–20. The
serializer writes that tail with `FCompressUserString`; the decoded name text
is now decoded with the retained `FDecompressUserString` nibble mapping. The
control names are `Cube`, `Monolith`, `Guardian Angel`, and `Hip to be Square`;
the action names are `Potato`, `Hip to be Square`, `Sphere of Doom`, and
`Rock of Gibraltar`. Re-encoding each name reproduces its compressed bytes
exactly. The AI names differ across separate launches despite matching
starting M2 inputs, consistent with launch-dependent RNG state. They do not
recover the draws or their global positions, and the current-source turn does
not yet generate and serialize these names in its full-turn comparison. The
kind-45 owner-0 `_scorex` record is decoded:
`_score.rgcsh[1]` changes from 2 to 1, while its other 23 bytes match. The
current-source four-pass probe does not generate or compare score-history
output. This census makes the original output surface explicit; it does not
claim that all changed bodies are caused by the task-5 edit or that
reconstructed production matches them. Regression:
`tests.test_satisfy_orders_fresh_ai_pair.SatisfyOrdersFreshAiPairTests.test_outputs_and_attached_corrupt_x2_case_are_separate`.

At this checkpoint no SO-E3 component closed from this inventory. SO-E2 had
0 closure checks remaining; SO-E3 had 1 named acceptance item with 2
unresolved components. The later complete RNG trace below resolves one of them.

## Retained post-production wormhole movement (2026-09-24)

The paired turn-1 HST outputs retain all four 18-byte wormhole records. The
previous `stars_move_things` adapter did not match the original `MoveThings(1)`
branch: when a wormhole's packed chance is zero, the original still advances
its packed movement field by four and chooses local coordinates as
`Random(25) + old_position - 12`. The reconstructed source now applies both
rules. The strict-C89 probe decodes the complete input and output HST records,
uses the retained XY planet table and fleet positions in production
`stars_validate_wormhole_position`, and reproduces every output record in both
runs.

| Run | Original positions → output positions | Packed word change | `Random(25)` coordinate witnesses |
|---|---|---|---|
| Control | `[1142,1049]→[1138,1054]`; `[1045,1197]→[1042,1185]`; `[1197,1024]→[1191,1030]`; `[1275,1132]→[1274,1133]` | `2002→2006`; `2000→2004`; `2002→2006`; `2001→2005` | `[8,17]`; `[9,0]`; `[6,18]`; `[11,13]` |
| Action | `[1142,1049]→[1145,1050]`; `[1045,1197]→[1038,1204]`; `[1197,1024]→[1201,1035]`; `[1275,1132]→[1277,1143]` | `2002→2006`; `2000→2004`; `2002→2006`; `2001→2005` | `[15,13]`; `[5,19]`; `[16,23]`; `[14,23]` |

All four packed chance fields decode to zero, so any `Random(100)` value selects
the local-jitter branch. Raw `FGenerateTurn` calls `MoveThings(1)` after
`Produce`; the four retained type-2 records therefore each consume one
`Random(100)` gate and enter the candidate loop. The loop always makes at least
one attempt, and the zero-chance branch consumes two `Random(25)` coordinates,
establishing a minimum of 12 calls per run. It can try up to 100 candidates per
wormhole. The output coordinates constrain the accepted candidate, but do not
reveal how many earlier candidates failed. The seven-module RNG accounting now
raises the retained full-run lower bound from 20 to 32 calls; exact draw values,
retry counts, and global stream positions remain unknown. The full
`FGenerateTurn` differential and startup seed/draw recovery remain the two open
SO-E3 components.

## Retained thing-table RNG gates (2026-09-25)

Both fresh turn-0 HST backups contain exactly four type-2 things (wormholes),
with no type-0 minefields or type-3 interaction objects. In the original turn
sequence, `MoveThings(0)` moves only type-3 things and its type-2 random gate
requires post-production mode, so neither random branch runs for these
records. `MoveFleets` reaches `FTravelThroughMineFields`, but that routine
builds route crossings only from type-0 things and returns before its sole
`Random(1000)` loop when the crossing count is zero. `DoOrders` calls
`DoThingInteractions`; all eleven random call sites in that procedure are
inside its type-3 thing block, which is empty for this pair. The existing
post-production `MoveThings(1)` wormhole path remains active and contributes
at least three calls for each of the four records.

The new retained-input regression checks the HST thing records, original call
sites, and early returns for those three skipped branches. It narrows the turn
sequence around movement; RNG calls from other AI, event, research, and build
services and the full state comparison remain open. Regression:
`tests/test_satisfy_orders_wormhole_move_e3.py::SatisfyOrdersWormholeMoveE3Tests.test_retained_thing_table_skips_minefield_random_paths`.

## Reconstructed player-log shuffle (2026-09-24)

The retained `CLEAN.XY` game record gives `cPlayer=2`. Raw
`FGenerateTurn` (`10b0_0000_FGenerateTurn.c`) initializes the player index
array, then performs a forward Fisher-Yates shuffle by calling
`Random(cPlayer-p)` at each index before loading player logs. The reconstructed
`stars_generate_turn` coordinator now implements this order and receives each
draw through its shared RNG callback; it stores the permutation and calls the
log loader with those player IDs. A positive player count now requires a valid
RNG callback result, rather than silently loading logs in numeric order. The
callback is invoked for `Random(1)` too, preserving the seed advance.

`tests.test_generate_turn_source` compiles a strict-C89 probe and checks the
retained pair's two-player limits `[2,1]` and three-player limits `[3,2,1]`,
the injected-draw permutation/log order, the state-advancing final draw, and
rejection of absent or out-of-range RNG results. It now also asserts the exact
coordinator callback order from load/initialize and player shuffle through
both `DoOrders` modes, movement and production stages, score/backup update,
player and aggregate writes, and cleanup. Its failed-log case verifies the
early-return order and that no output writes occur. The seven-module focused
suite passes 32/32 after the retained waypoint, transit-breeding, and
auto-terraform RNG-gate audits. These assertions validate the reconstructed
coordinator ordering; they do not bind its callbacks to all
original turn services or compare complete output state. The actual original
shuffle values remain unknown, and the complete turn/service differential is
still open.

## Retained waypoint-validation RNG gate (2026-09-24)

The original `ValidateWaypoints` contains four `Random` sites, all inside the
branch that handles fleet-target (`object_kind=2`) waypoints. The new
retained-input regression inspects every fleet/order pair in both clean runs:
each control/action turn-0 HST backup has ten pairs, while the turn-1 HST
returns have eight control pairs and seven action pairs. Every retained order
has `object_kind=1` (planet target), including the action's expanded type-19
task-5 order. Thus the saved fleet orders do not enter the random fleet-target
repair branch in these retained input/output states.

This narrows the RNG audit for the serialized pair state; it does not prove
that log replay leaves the same transient in-memory order types at either
`ValidateWaypoints` call, and it does not account for random draws in other
turn services. The exact startup seed and complete draw sequence remain open.
Regression: `tests/test_satisfy_orders_fresh_ai_pair.py::SatisfyOrdersFreshAiPairTests.test_validate_waypoints_draws_are_skipped_for_retained_fleet_orders`.

## Retained transit-breeding RNG gate (2026-09-24)

`BreedColonistsInTransit` has one random site, `Random(3)`, reached only after
the procedure finds at least one player whose `_player.rgAttr[0x0e]` equals 4.
The recovered player layout places the one-byte `rgAttr` array at offset 62,
so stat 14 is byte 76; the original `GetRaceStat` returns that indexed
attribute. Both fresh input HSTs retain identical player records with stat-14
values `[9,1]`. Neither player enables the transit-breeding loop, so its
`Random(3)` site is skipped for this pair regardless of fleet colonist cargo.

The regression checks the two retained player records, `_player` layout and
`GetRaceStat` accessor, plus the original procedure's guard around the sole
draw. It closes this service's direct draw for the retained players only;
other RNG-bearing stages, the complete stream, and full-turn state comparison
remain open. Regression: `tests/test_satisfy_orders_fresh_ai_pair.py::SatisfyOrdersFreshAiPairTests.test_transit_breeding_random_gate_is_skipped_for_retained_players`.

## Retained auto-terraform RNG gate (2026-09-24)

The original `AutoTerraform` scans planets only if a player's stat 14 equals
3. Its three possible draws—`Random(3)`, `Random(10)`, and `Random(1000)`—are
inside that eligible-player branch. The fresh control/action player records
both have stat-14 values `[9,1]`, so the procedure skips its planet scan and
all three random sites in this pair.

The regression checks both input player records and the original branch and
draw sites. This bounds only `AutoTerraform` for the retained player state;
the remaining turn RNG sites, startup seed, and full-turn state comparison
remain open. Regression: `tests/test_satisfy_orders_fresh_ai_pair.py::SatisfyOrdersFreshAiPairTests.test_auto_terraform_random_gate_is_skipped_for_retained_players`.

## Retained `UpdateGuesses` draw bounds (2026-09-24)

The fresh turn-0 HST input has 32 planet records, but only planets 10 and 24
carry the full-mineral-detail flag. Both are owned, and their retained
population inputs are 250. Original `FReadPlanet` clears each new 56-byte
`_planet` and reads the four `rgwtMin` values only under the serialized
`0x2000` detail flag. The recovered layout puts `rgwtMin` at byte 28 and
`uGuesses` at byte 18. The same two planets are the only full-detail records
in both turn-1 outputs; their final populations are 287 and 256.

Original `UpdateGuesses` has one `Random` site. It skips a planet whose
population `rgwtMin[3]` is zero, and for a nonzero population it skips the draw
only when the owner's race stat 14 equals 8. Both retained owners have stat-14
values `[9,1]`. The routine runs after movement, production, and
`AutoTerraform`, immediately before the output-writing phase in
`FGenerateTurn`. The low 12 bits of the output `uGuesses` word therefore
constrain this site's result after each final population is established.

| Run | Planet | Final population | `Random` limit | Output low guess | Implied draw range |
|---|---:|---:|---:|---:|---:|
| Control `vmctl-sye3k0` | 10 | 287 | 71 | 78 | 60–63 |
| Control `vmctl-sye3k0` | 24 | 256 | 64 | 57 | 4–7 |
| Action `vmctl-sye3k1` | 10 | 287 | 71 | 77 | 56–59 |
| Action `vmctl-sye3k1` | 24 | 256 | 64 | 56 | 0–3 |

The bounds invert the original/reconstructed formula
`(population - (population >> 3) + Random(population >> 2)) >> 2`; each
four-value interval is consistent with the retained low guess bits. The
regression checks the loader and routine guards, full map counts, owners,
player attributes, final output records, and all four intervals. It narrows
the pair-specific `UpdateGuesses` evidence but does not recover exact draw
values, their global stream positions, or the startup seed. The full-turn
state comparison and complete RNG sequence remain the two open SO-E3
components. Regression:
`tests/test_satisfy_orders_fresh_ai_pair.py::SatisfyOrdersFreshAiPairTests.test_update_guesses_draws_are_bounded_by_retained_planet_outputs`.

## Retained `SpankTheCheaters` RNG gate (2026-09-24)

All seven `Random` sites in the original `SpankTheCheaters` routine are inside
its outer `game.turn > 9` block. The fresh control and action backup HSTs each
retain one turn-0 BOF. In original `FGenerateTurn`, the service runs before
`UpdateGuesses`, and `game.turn` is incremented only after both calls. The
turn gate is therefore false for both retained runs, so this service consumes
no random draws regardless of its per-player cheat flags.

The regression ties the seven raw random call sites to that outer guard,
checks the original turn-service/increment order, and verifies the turn-0 BOF
in both pair inputs. This closes one more early turn-service gate for the
retained pair; it does not recover any exact RNG value or the complete stream.
Regression:
`tests/test_satisfy_orders_fresh_ai_pair.py::SatisfyOrdersFreshAiPairTests.test_spank_cheaters_random_sites_are_skipped_before_turn_ten`.

## Retained turn-zero random-event entry (2026-09-24)

`Produce` calls `RandomEvents` when `_game.fNoRandom` (bit 7 at offset 16) is
clear. Both retained XY game records have `fNoRandom=0`, turn 0, and
`mdSize=0`, so the branch is active in both runs. `RandomEvents` invokes
`MeteorStrike`, `PlanetaryClimateChange`, `DiscoverNewMinerals`, and
`MysteryTrader` in that order. The first three each call one random gate
unconditionally when entered: `Random(20)`, `Random(20)`, and
`Random(15)` respectively. At turn 0, meteor and mineral discovery can each
add one planet-selection draw; their event effects are gated until after turn
9. Climate change can make up to five follow-on draws: one planet selection
and four guarded climate draws. `MysteryTrader` skips its entire body until
after turn 39. The source guards therefore bound this dispatcher to **3–10
random calls per retained run**: meteor 1–2, climate 1–6, mineral discovery
1–2, and trader 0. The regression checks those per-event bounds along with
the retained turn-zero inputs. Exact branch rolls, draw values, and stream
positions remain unknown. Regression:
`tests/test_satisfy_orders_fresh_ai_pair.py::SatisfyOrdersFreshAiPairTests.test_random_events_enter_and_take_three_first_draws_at_turn_zero`.

## Retained Turin AI starbase-name draws (2026-09-25)

Both original turn-0 M2 inputs have no type-26 designs in slots 17–20; both
turn-1 M2 outputs contain all four. The names are decoded in the output census
above and each compressed tail re-encodes byte-for-byte. The two runs start
from the same M2 SHA-256, `588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b`,
but choose different names for three of the four slots.

The four names are starbase designs created inside Turin's AI turn. The source
path is `EnsureAis` → `DoAiTurn` selector 1 → `DoTurinDroneAiTurn` →
`IroEnsureAi` → `EnsureAiStarbaseDesigns` → `FCreateAiStarbase` →
`PickANameAndBmp`. `EnsureAiStarbaseDesigns` creates local designs in call
order 2, 4, 1, 3; `FCreateAiStarbase` adds `0x10` to each slot, so the saved
order is 18, 20, 17, 19. Each name comes from the executable's string pool
starting at ID `0x432` (1,074), with 13 candidates, and the picker calls
`Random(13)` for each candidate.

The retained names expose the accepted pool offsets in creation order:

| Run | Slots in creation order | Accepted `Random(13)` values |
|---|---|---|
| control | 18, 20, 17, 19 | 4, 9, 7, 0 |
| action | 18, 20, 17, 19 | 9, 5, 6, 10 |

The first and third accepted values in each run are direct first candidates:
their same-hull name sets are empty or contain no pool candidate. The second
and fourth accepted values may have duplicate retries against the preceding
same-hull starbase name. Fewer than four same-hull designs exist before each
creation, so the picker's optional `Random(4)` bitmap-selection branch cannot
run. The four required calls raise the retained whole-run source/output-backed
lower bound from 32 to **37 calls per run**, including the scout at position 5
and the starbase-upgrade roll at position 6. Exhaustive search over all 16,384
effective low-14-bit `Randomize2` tick residues uniquely matches each accepted
`Random(13)` sequence while allowing the source's same-hull duplicate retries.
The recovered residues are control 13,747 and action 9,953, yielding initial
seed words `[19, 279]` and `[443, 67]`; both runs have zero retries. The first
four stream positions are therefore recovered as draws 1–4. This identifies the
effective low bits of `GetTickCount()`, not its complete 32-bit value.

The search is source-guarded: `WinMain` seeds before `FCreateStuff`, whose
callgraph has no `Random`; `InitInstance` reaches only the serial parser's
random calls, which are bracketed by `PushRandom`/`PopRandom`; star-field UI
drawing is bracketed the same way. The pre-Turin AI prelude runs
`ComputeShdefPowers`, `MarkPlanetsUnderAttack`, `IncreaseAIMinefieldSizes`, and
`InitRandomPlanetList` in order. For this retained pair the owner has one
planet, so the shuffle loop consumes no calls; `SpdOfShip`'s `Random(15)` is
inside its non-null-fleet branch, while `ComputeShdefPowers` passes a null
fleet. Turin calls `IroEnsureAi` first, before its later random sites.

The complete later turn-wide draw sequence, its remaining global positions
and values, and the production full-turn state/service comparison remain
open. The regressions check accepted name values and source order, exhaustively
recover the effective residues and seed words, and pin the zero-draw prelude
and first scout draw:
`tests.test_satisfy_orders_fresh_ai_pair.py::SatisfyOrdersFreshAiPairTests.test_recover_startup_tick_and_first_turin_scout_draw`.
At this checkpoint, this added RNG evidence but closed neither SO-E3 component.
SO-E2 remained at zero checks; SO-E3 remained one named item with two
components open.

## Retained Turin `KeepFleetsMoving` RNG gate (2026-09-25)

`DoTurinDroneAiTurn` finishes by calling `HandleBasicAiTasks`, whose first
step is `KeepFleetsMoving`. Both retained turn-0 HST inputs contain exactly
four things, all type 2 wormholes. `KeepFleetsMoving` excludes type 2 from its
candidate list, so it passes zero minefield candidates to
`SetAiFleetIdealSpeed`. That helper initializes `bVar2` false and places both
`Random(10)` calls inside the `if (bVar2)` branch; the flag becomes true only
when a listed minefield intersects the fleet. This retained path therefore
adds no fleet-speed RNG calls in either run.

The regression combines the retained type table with the raw source guards:
`tests/test_satisfy_orders_wormhole_move_e3.py::SatisfyOrdersWormholeMoveE3Tests.test_retained_thing_table_skips_minefield_random_paths`.
This closes only the `KeepFleetsMoving` slice. At this checkpoint, later AI
planet-task draws, turn services, and the complete RNG stream remained
unresolved, so SO-E3 had one named open item with two components open. The
complete retained-pair RNG stream is recorded below; the full reconstructed
production differential remains the one current open component.

## Retained Turin starbase-upgrade RNG gate (2026-09-25)

After the four starbase-name calls (draws 1–4) and fleet-0 scout fallback
(`Random(100)` at draw 5, values 99/4), `HandleBasicAiTasks` reaches
`FUpgradeAiStarbase`. `IroEnsureAi` supplies `IshdefAiSBLatest=0`: absent
SHDEF slot 5 retains the loader's bit-9 marker. Planet 24's retained starbase
type is 0, so it skips the first out-of-range branch. The alternate gate sees
packed starbase value 0, threshold 4, and present slot 2 with its marker clear,
then consumes `Random(100) < 6` at draw 6. Replaying the recovered seed words
gives control 7 and action 75; both miss the gate, so neither queues an
upgrade.

The other direct random branches in this `HandleBasicAiTasks` slice are
bounded by retained inputs: AI difficulty is 1 in mode word `0x2797`, so
`FAIFling`'s difficulty-greater-than-1 sites are skipped; scanner, defense,
terraform, blocked-mines, and production-queue random paths are also absent for
the retained markers and design slots. The source and retained-input checks
are pinned by
`tests.test_satisfy_orders_fresh_ai_pair.SatisfyOrdersFreshAiPairTests.test_recover_startup_tick_and_first_turin_scout_draw`.
This recovers the first six stream positions and raises the source/output-backed
whole-run lower bound to 37 calls per run. Later conditional draws and the
complete state/service differential remain unresolved; SO-E2 has 0 checks
remaining and SO-E3 remains 1 named item with 2 unresolved components.

## Retained owner-1 AI-output writer RNG draws (2026-09-25)

The source phase order places the owner-1 log and history files immediately
after the Turin task and `FillProductionQueue` work. `EnsureAis` clears its
temporary `gd.grBits` bit-1 file-probe flag before calling `DoAiTurn`, and
`FGenerateTurn` sets that bit only after `EnsureAis` returns. Since
`GetResourcesAvailable` reaches `EstMineralsMined`, whose `Random(100)` rounding
site requires bit 1, the four pre-scout resource queries and later AI resource
queries add no mineral draws. `DoAiTurn` also keeps `fAi=1` through both output
writes, suppressing the `FinishProduction` and `ChangeMainObjSel` UI refresh
path through `FillPlanetProdLB`.

After the draw-6 starbase upgrade roll misses, the retained difficulty-1
player skips the random branches in `FAIFling`; the source callgraph leaves
only the two `WriteBOF` calls in the successful `FWriteLogFile` then
`FWriteHistFile` writer closures. These are the retained owner-1 X2 and H2
returns (`BACKUP/CLEAN.X2` and `CLEAN.H2`).
Advancing each recovered seed through draws 1–6 yields `Random(2000)` values
1240/1434 at draws 7/8 in the control run and 1922/267 in the action run.

The regression pins the `EnsureAis`/`FGenerateTurn` gate order, the
`GetResourcesAvailable` and `FinishProduction` guards, callgraph random
descendants for the production queue and both output writers, writer order,
and the replayed values:
`tests.test_satisfy_orders_fresh_ai_pair.SatisfyOrdersFreshAiPairTests.test_recover_startup_tick_and_first_turin_scout_draw`.
This recovers the first eight RNG positions but closes neither SO-E3 component;
the later full-turn draw stream and complete production state/service
differential remain open.

## Retained FGenerateTurn player-shuffle RNG draws (2026-09-25)

`HostTimerProc` calls `EnsureAis` before `FGenerateTurn`. After the owner-1
log/history writes, `EnsureAis`'s cleanup calls `DestroyCurGame` and
`FLoadGame`. Their broad callgraph closures include conditional random paths:
`FLoadGame` can reach `EstMineralsMined`, whose rounding draw needs `gd.grBits`
bit 1, and `DestroyCurGame` can reach `WriteBOF` through `PromptSaveGame` only
when the current game is dirty. The bit stays clear through the AI cleanup,
and the successful `FWriteLogFile` call clears `game.fDirty` before that
cleanup, so neither conditional path executes here. The retained `-g`
input has `fTutorial=false`, so `FGenerateTurn` skips its conditional reseed.
Its first main-stream calls are therefore the player shuffle, with
`cPlayer=2`: `Random(2)` at draw 9 and `Random(1)` at draw 10. The latter
returns zero while still advancing both seeds.

Replaying the recovered seeds through the two owner-1 `WriteBOF` draws yields
control shuffle values 0/0 and action values 1/0. The regression checks the
host-timer phase order, the guarded cleanup descendants, skipped tutorial reseed,
and recovered values alongside the original player-count source evidence:
`tests.test_satisfy_orders_fresh_ai_pair.SatisfyOrdersFreshAiPairTests.test_recover_startup_tick_and_first_turin_scout_draw`.
At this checkpoint the first ten positions were recovered, while conditional
turn services, later draws, the complete output stream, and production
state/service differential remained open. SO-E2 had 0 checks remaining; SO-E3
had 1 named item with 2 unresolved components.

## Complete retained pair RNG trace (2026-09-25)

The recovered effective `Randomize2` seed words now reproduce the main RNG
stream from the first Turin starbase name through the last turn-1 output BOF.
The 32-bit `GetTickCount()` values were not retained; the high 18 bits are not
used by `Randomize2`, so this does not leave a seed ambiguity for these runs.

| Phase | Control positions and values | Task-5 action positions and values |
|---|---|---|
| Turin names (`Random(13)`) | 1–4: `4,9,7,0` | 1–4: `9,5,6,10` |
| Scout, upgrade (`Random(100)`) | 5–6: `99,7` | 5–6: `4,75` |
| AI X2/H2 BOF writers (`Random(2000)`) | 7–8: `1240,1434` | 7–8: `1922,267` |
| Player shuffle (`Random(2), Random(1)`) | 9–10: `0,0` | 9–10: `1,0` |
| Task-5 technology learning | 11–30: gate `74`, 13 `Random(13)` values, then six `Random(6)` values | 11–12: `43,12`; both `Random(100)` gates fail before nested calls |
| Mineral rounding | 31–36: `67,11,28,32,99,3` | 13–18: `95,17,59,38,26,18` |
| Random-event entry gates | 37–39: `19,18,10` | 19–21: `11,7,2` |
| Four wormhole moves | 40–51: `29,8,17,21,9,0,28,6,18,77,11,13` | 22–33: `62,15,13,20,5,19,78,16,23,69,14,23` |
| `UpdateGuesses` | 52–53: `63,6` | 34–35: `59,1` |
| Saved turn flag (`Random(8)`) | 54: `1` | 36: `4` |
| Turn-1 M1/M2/HST writers (`Random(2000)`) | 55–57: `153,442,1900` | 37–39: `1114,424,993` |

This is 57 main-stream calls in the control and 39 in the action. Mining
values land inside all six independently derived stock/remainder ranges. All
three event gates are nonzero, so their conditional follow-up draws are
skipped, and `MysteryTrader` is skipped at turn zero. Each wormhole has zero
movement chance; its first `Random(25)` pair equals the retained coordinate
delta and passes `stars_validate_wormhole_position`, so all four retry counts
are zero. The exact `UpdateGuesses` values fit each retained output range, and
the `Random(8)` values match turn-1 HST BOF flags.

The five observed BOF tick/random mixtures provide a further cross-check.
Subtracting each recovered random value modulo 2048 yields these low-11-bit
tick residues in write order:

| Run | X2 | H2 | M1 turn 1 | M2 turn 1 | HST turn 1 |
|---|---:|---:|---:|---:|---:|
| Control | 1560 | 1560 | 1654 | 1656 | 1657 |
| Action | 1858 | 1859 | 1952 | 1954 | 1956 |

The new regression
`tests.test_satisfy_orders_fresh_ai_pair.SatisfyOrdersFreshAiPairTests.test_fresh_pair_seeded_turn_rng_trace_through_output_writers`
checks all limits, values, positions, original output bounds, wormhole
coordinates, saved flags, and BOF tick residues. The fresh-pair and wormhole
modules pass 28/28. This resolves the RNG component for the isolated pair; it
does not complete the current-source full-turn state/service differential.
SO-E2 has 0 closure checks. SO-E3 remains one open named item with one
component: the complete reconstructed-production comparison against both
pinned original outputs.


## Recovered task-5 RNG bound into the source replay (2026-09-25)

The retained action's first task-5 technology-learning gate is main-stream
draw 11: `Random(100)=43`. The packed task-5 current-source replay now passes
that recovered value to `stars_itech_learn_a_tech`, checks that exactly one
random call takes its no-learn return, and reports `TECH_DRAW=43`. It also runs
the action planet-24 mining slice with its recovered draws at positions 16–18:
`38, 26, 18`, yielding `7, 9, 7`. The action endpoint remains
`592, 964, 520`; the control-labeled mining values in this probe remain
output-bound witnesses. The targeted replay passes 1/1. This binds another
retained stage to the recovered stream, but it does not run the shared RNG
through a complete production turn. **SO-E2: 0 checks left. SO-E3: 1 component
left**, the full reconstructed/original production-turn state and service
differential.


## Fleet-merge state effects use production services (2026-09-25)

The task-5 merge/scrap replay no longer adds the two raw cargo arrays in a
fixture. The separate selector-1 route probe still has its own cargo-balance
fixture callback. In the task-5 replay, `stars_merge_two_fleets` now calls production
`stars_fleet_transfer_cargo_balance`, whose weight callback runs production
`stars_wt_max_ship_design_stat` over the retained slot-2 components and original
EXE hull row. Its lookup callback resolves both pre-merge retained fleets. The
probe verifies all five cargo lanes are conserved, the deleted fleet has zero
cargo in every lane, and destination fleet 2 holds the original 420 fuel.

The same task-5 probe now commits both merged records through production
`stars_lookup_fleet` into a four-slot service table containing retained M2
records at IDs 2/3 and inert slots 0/1. It removes fleet 3 through production
`stars_delete_fleet` (including the `0x0400` dead bit and sorted-table
compaction), and invalidates its current fleet report through
`stars_invalidate_report` (cache cleared and view flag `0x0200` set). The
strict-C89 replay passes as part of the 28/28 fresh-pair and
wormhole suite; the two focused global-table lookup/deletion regressions also
pass. These production bindings apply to the task-5 merge/scrap probe. The
separate selector-1 route probe still uses fixture cargo and fleet/report
callbacks. Raw message serialization and an end-to-end shared-stream adapter
remain fixture boundaries; the whole-turn state/service comparison remains
open. **SO-E2: 0 checks left. SO-E3: 1 component left.**

## Combined selector-1 route replay uses production merge services (2026-09-25)

The broader selector-1 route/merge/scrap probe now uses the same production
`stars_fleet_transfer_cargo_balance`, `stars_lookup_fleet`,
`stars_delete_fleet`, and `stars_invalidate_report` services as the focused
task-5 replay. Its cargo weight callback joins the retained slot-2 SHDEF data
to the original EXE hull record. It checks that all five cargo lanes are
conserved, fleet 3 is empty and marked dead, the four-slot service table is
compacted, and the fleet report cache/view flag are updated.

The expanded strict-C89 report passes **81/81 checks for each of 100 injected
outer `Random(100)` values**. The selector-1 route, retained transport,
two-fleet merge, task-5 scrap, and movement now share this probe. It still
uses a scripted no-learn witness for task-5 learning and fixture message
serialization; it does not run the complete `FGenerateTurn` stage sequence
or compare the complete HST/M1/M2 outputs. **SO-E2: 0 checks left. SO-E3: 1
full-turn differential component left.**

## Retained Turin starbase startup RNG is source-bound (2026-09-25)

`stars_f_create_ai_starbase` now runs the reconstructed
`stars_pick_ai_name_and_bitmap` after applying the original design-class flags
and before handing the completed work record to the change/publish service.
The work record carries the generated hull, flags, bitmap, and name through
that callback boundary.

The new strict-C89
`tests/satisfy_orders_turin_starbase_rng_probe.c` calls production
`stars_do_turin_drone_ai_turn` with the retained owner-1 research bytes. The
callback enters production `stars_iro_ensure_ai`, reads the 31-byte
`vrgAiTurinDroneResOrder` table from the pinned EXE symbol, and calls
`stars_ensure_ai_starbase_designs`, `stars_f_create_ai_starbase`, and
`stars_pick_ai_name_and_bitmap` in source creation order. This reproduces the
owner-1 turn-1 HST current-technology byte `0x32` and research percentage 15;
the six technology-level bytes remain unchanged, matching the original
turn-1 HST record.

Starting from the recovered control/action `Randomize2` seeds, the four
`Random(13)` values are control `[4,9,7,0]` and action `[9,5,6,10]`; they select
the same string-pool IDs decoded from the retained M2 names. Without resetting
the RNG, the probe then calls the production Turin scout planner: draw 5 is
`Random(100)=99` for control and `4` for action, both retaining planet 26 as
the target. The original-output regression
`test_new_turin_starbase_names_pin_accepted_random13_values` independently
maps those IDs through the embedded executable string table and verifies the
retained compressed name bytes.

This is a scoped production binding: the `DoTurinDroneAiTurn` invocation has
no planet/fleet list, and the subsequent scout-planner call uses retained route
fixtures. Starbase capability bits, latest-design index, hull/work fields,
and the name/bitmap occupancy state remain fixtures. The four SHDEF records
are not materialized or serialized to M2, and the complete turn is not yet
integrated. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component
left.**

## Seeded selector-1 startup-to-route integration (2026-09-25)

The combined strict-C89 probe in
[`tests/satisfy_orders_turin_binding.py`](../../tests/test_satisfy_orders_turin_binding.py)
now accepts seeded `control` and `action` modes in addition to its 100
scripted outer `Random(100)` witnesses. Those modes initialize the recovered
effective `Randomize2` state, then pass it through the production outer
`stars_do_ai_turn` and `stars_do_turin_drone_ai_turn` calls. The latter enters
`stars_iro_ensure_ai` using the retained owner-1 HST research bytes and the
31-byte `vrgAiTurinDroneResOrder` table read from the pinned EXE. It invokes the
production starbase-design selector and name picker before the production
selector-1 scout planner consumes the next RNG value.

The probe checks the four original name-pool string IDs, per-hull bitmap
selection, and the same seed's draw 5: control names `[4,9,7,0]`, then scout
`Random(100)=99`; action names `[9,5,6,10]`, then scout `Random(100)=4`. Both
retain target planet 26. The Iro slice changes current technology from `0x60`
to `0x32`, retains 15 percent research, and leaves all six technology-level
bytes unchanged, matching the HST turn-1 record. The probe also continues the
selector-1 route through the retained movement, merge, transport, and task-5
checks in the same harness. The 100 injected-value route cases and both seeded
cases pass in the eight-module focused suite (**41/41**).

The combined run still has fixture boundaries: starbase capability bits,
latest-design index, hull/work construction, and SHDEF publication storage are
provided by the test. It does not serialize the four SHDEFs into M2 or replay
the complete `FGenerateTurn`/production pipeline against both original
outputs. Those remaining state and service comparisons are the single open
SO-E3 component. **SO-E2: 0 checks; SO-E3: 1 component; TODO7: 39 open items
(29 evidence, 10 promotions).**

## Seeded selector-1 startup-to-post-production wormhole RNG pipeline (2026-09-25)

The same seeded modes now continue through the next recovered random sites in
the combined strict-C89 probe, without resetting `StarsSeeds`. During the
retained Turin call, production `stars_f_upgrade_ai_starbase` consumes draw
6. After `stars_do_ai_turn`, two production `stars_write_bof` calls (player 1,
X2 then H2) consume draws 7–8. Production `stars_generate_turn` performs the
two-player shuffle at draws 9–10. Finally, the production
`stars_itech_learn_a_tech` path consumes the owner-1 task-5 gate and, for the
control, its nested candidate/battle draws; the action's second gate is a
separate direct production call. The probe then
calls production `stars_mine_minerals` over the two owned planets in the
retained HST input; its adapter supplies retained player/planet mining values
to production `stars_est_minerals_mined`, consuming draws 31–36 / 13–18.
After mining, the probe calls the production `stars_random_events`
dispatcher.
Its retained-turn adapters consume only the Meteor, Climate, and
New-Minerals entry gates; their nonzero values skip nested event draws, and
the turn-zero Mystery-Trader path exits before drawing.

The original recovered values and the probe's production assertions agree:

| Run | Upgrade, draw 6 | BOFs, draws 7–8 | Shuffle, draws 9–10 | Task-5 technology draws | Mining draws/yields (planets 10, 24) | Event gates | Wormhole movement draws | `UpdateGuesses` | Saved flag | Output BOFs | Last integrated draw |
|---|---:|---:|---:|---|---|---|---|---|---|---|---:|
| Control | `Random(100)=7` | `Random(2000)=[1240,1434]` | `Random(2), Random(1)=[0,0]` | 20 draws, values `[74,8,1,9,2,3,4,0,8,0,10,11,3,2,0,5,2,4,4,4]` | `[67,11,28,32,99,3]` → `[7,9,7]`, `[7,8,7]` | `Random(20), Random(20), Random(15)=[19,18,10]` | `[29,8,17,21,9,0,28,6,18,77,11,13]` at draws 40–51 | draws 52–53 `[63,6]` → guesses `0x204e,0x2039` | draw 54 `Random(8)=1` | draws 55–57 `Random(2000)=[153,442,1900]` | 57 |
| Action | `Random(100)=75` | `Random(2000)=[1922,267]` | `Random(2), Random(1)=[1,0]` | owner-1 task-5 gate `43`, followed by second gate `12` | `[95,17,59,38,26,18]` → `[6,9,7]`, `[7,9,7]` | `Random(20), Random(20), Random(15)=[11,7,2]` | `[62,15,13,20,5,19,78,16,23,69,14,23]` at draws 22–33 | draws 34–35 `[59,1]` → guesses `0x204d,0x2038` | draw 36 `Random(8)=4` | draws 37–39 `Random(2000)=[1114,424,993]` | 39 |

The probe also checks the earlier X2/H2 BOF record headers and body flags,
extracts their random values with a zero-tick test callback, checks generated
player order, compares every task-5 random limit/value to the retained phase
trace, verifies both mining yields from production arithmetic, and checks the
random-event dispatcher order and gate values. It then calls production
`stars_move_things(post_production=1)` with the same seed state, checks all 12
draw limits and values in each run, and compares all four complete 18-byte
wormhole output records to the corresponding original turn-1 HST. The probe
then calls production `stars_update_guesses` on the two retained output planets,
using production-derived populations, retained defense/guess fields, and the
recovered `Random(71), Random(64)` values. The same derived populations feed
production `stars_cmax_operable_defenses`. It reproduces both full guess words
in each run. Its survival result is now calculated from retained source fields:
production `stars_get_best_defense_part` and `stars_lookup_part` select armor
item 9 for owners 0 and 1 from the pinned EXE table (damage 10). Production
`stars_cmax_operable_defenses` returns 12 and 11 from the respective planet
environment, population, and player fields; the ten installed defenses cap
both counts at 10. `stars_calc_pct_survive` yields 90 percent for each planet,
matching the high nibble in the original guess words. The saved-flag draw
follows, then three production `stars_write_bof` calls emit
M1/M2/HST header records. For each writer, the fixture supplies the
low-11-bit `GetTickCount` residue derived from the original BOF salt and the
recovered `Random(2000)` value. Starting from the input HST game ID and turn-1
state, the probe compares each complete 18-byte output BOF record with the raw
record in the corresponding original file. The movement chance and candidate validation
use production `stars_pct_wormhole_moves` and
`stars_validate_wormhole_position`; each of the four first jitter candidates
is accepted. The waypoint callback and event effects remain test adapters.
The eight-module host suite passed **41/41**; after adding raw BOF equality
and tick-residue cross-checks, the selector-1 probe and RNG-trace regression
passed **2/2**. The same test still covers 100 injected outer `Random(100)` route
cases. Evidence is also recorded in the machine-readable
[`combined_seeded_selector1_startup_to_route`](satisfy-orders-full-turn-e3-original-oracle.json)
object and in [EVIDENCE.md](../../EVIDENCE.md).

This joins the current-source host harness to the recovered main stream through
draws 57/39, and the three generated turn-1 BOF records match their original
18-byte records exactly. The harness now derives both planet populations
through production services and feeds them to `stars_update_guesses` and its
operable-defense calculation. Owner 0's planet 10 grows from 250
colonists/fraction 0 to 287/fraction 50 using desirability 100, maximum
population 12,000, and ideal growth 15. Owner 1's planet 24 reaches 256 after
the production transfer and +31 growth already described above. Turn-1 guess
fields and per-writer tick-residue inputs remain retained; the harness does
not construct complete M1/M2/HST files in one production turn. Starbase-work
and planet adapter inputs, a minimal
`FGenerateTurn` state, and fixture event, waypoint, message, and serialization
boundaries remain. Nested random-event effects and the complete output-file
comparison remain open. The second action technology gate at draw 12 is now
bound to the retained owner-0 planet-10 task-5 order: action pass 1 replays
that fleet and order through `stars_satisfy_orders_packed`, with a production
player-0 `stars_itech_learn_a_tech` call in its source callback after the
owner-1 draw-11 task-5 replay. On 2026-09-25, `python3 -m unittest
tests.test_satisfy_orders_turin_binding -v` passed (1 test, 8.1 seconds),
recompiling and rerunning the retained selector-1 route and seeded
control/action cases. The latest selector-1 rerun passes in 11.35 seconds,
and the eight-module SO-E3 regression passes 41/41 in 104.93 seconds. The
newly attached corruption screenshot has SHA-256
`254a4c6baf7cc560dde3976da7e16d6bb26396cae43a26c0749463b1ed4db780`, exactly
matching the archived SOTC screenshot; it adds no new guest-state evidence.
The rejected X2 was also compared with the paired control/action `BACKUP/CLEAN.X2`
copies. All three decode to the same 13 record kinds and share BOF game ID,
version, turn, player, and flags; their salts differ, and the rejected file's
BOF/type-9/type-27 bodies are not byte-identical to either paired copy. This
does not identify the rejected field or prove that either reference was
accepted by Stars!; the comparison is recorded in the machine-readable oracle.
The complete HST/M1/M2 production differential remains open. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound type-45 score-history selection (2026-09-25)

Original `FWriteDataFile` writes type `0x2d` only when saving a player file and
the score-history array exists. It emits the 24-byte `_scorex` body for each
player who is visible, is the current player, is dead, or is included by the
victory-history game flag after turn 19. Game HST output does not receive this
section.

`reconstructed/src/file.c::stars_write_score_records` now implements this
selection and passes the original body bytes unchanged to the game-record
writer. The strict-C89 regression covers current-player, visible, dead,
victory-history, game-file exclusion, invalid player, record kind/size, and
body preservation; it passes 6/6. The focused selector replay passes 1/1 in
11.263 seconds and the writer-order regression passes 1/1.

The expanded ten-module SO-E3 suite passes 49/49 in 114.801 seconds.
`tools/build_core.py`, `tools/baseline.py --check`, oracle JSON parsing,
Python syntax compilation, and `git diff --check` pass.

The selector's data-file callback now invokes `stars_write_score_records` for
each output. It emits one retained 24-byte score body for M1 and one for M2
in both control and action, and emits no type-45 record for HST. The combined
selector/pair/score-writer regression passes 28/28 after this integration.

This binds the serializer within the current coordinator, but the score bodies
are still retained output fixtures. `turn_update_scores` remains a no-op, so
score values are not derived from the complete current turn state. Other
record sections and state inputs also remain incomplete, and HST/M1/M2 are not
assembled or compared as full files. SO-E3 remains open; SO-E2 remains at 0
checks left.


## CalcPlayerScore starbase-bonus correction (2026-09-25)

The original `CalcPlayerScore` body scores each owned planet by the capped
fourth `rgwtMin` value, sums `CResourcesAtPlanet`, and adds three points for
each planet whose starbase is present and whose starbase hull has nonzero
cargo capacity. The previous typed helper treated its final planet field as a
generic developed flag and added three times that planet's mineral score.
That representation could not be used to build current type-45 bodies.

`StarsScorePlanet.score_starbase` now represents the complete qualifying
starbase predicate. `StarsScoreSummary` also exposes the starbase count and
the six race technology levels accumulated into `_score.cStarbase` and
`_score.cTechLevels`. The retained formula regression now checks score 56,
two owned planets, 90 resources, one qualifying starbase, technology sum 25,
and fleet power classes `[1,0,2]`.

Verification: `tests.test_common_source` 221/221; selector-1 binding 1/1;
score-record and writer-order source tests 2/2; core build and baseline check
pass. This repairs one calculation used by the missing score path. It does
not yet derive the full current player/planet/fleet/design inputs, invoke
`stars_update_player_scores` during the seeded turn, or emit turn-1 `_scorex`
records. SO-E3 remains open.

## Seeded turn coordinator sequence bound to retained control/action stream (2026-09-25)

The seeded control/action path in `tests/satisfy_orders_turin_binding_e3_probe.c`
now calls production `stars_generate_turn` for the complete coordinator sequence.
Previously the harness invoked that function only to check its two-player shuffle,
then ran later helpers manually. Before the pre-movement order phase,
production `stars_unmark_mine_fields` and pre-production
`stars_move_things(post_production=0)` run on all four retained type-2
wormholes. Their identity marker keeps every byte and coordinate unchanged;
the move stage consumes no RNG. The `ThingDecay` callback runs production
`stars_thing_decay` on the same retained type-2 records and the retained fleet
flag subset; it frees no things and preserves the original stream. The coordinator callback invokes production
`stars_do_orders` before and after movement and records passes 1–4. Pass 1 loads
the retained scrap inputs, checks their pre-scrap bytes, and runs packed task 5;
the action also performs its retained technology-learning call.

The coordinator's fleet stage calls production `stars_move_fleets`. Its produce
stage generates planet 24's two design-0 seed items by calling production
`stars_do_turin_drone_ai_turn` with the retained 32-point map, open production
slot, and desirability 100. It then invokes production `stars_produce`: the
mining callback runs the retained two-planet mining replay and copies the
resulting stocks into the production planet records. Production
`stars_build_production_item` uses the retained EXE-backed cost vector
`[16,2,8,35]` and resource budget 28; it reaches 81-percent progress on the
first queue item, leaves the second at zero, and reproduces planet 24's output
stocks in both runs. The population callback runs the production-derived
growth calculation before production dispatches random events. Planet 10 has
the observed null queue, so the production message callback emits exactly one
queue-empty message. Its player/code/object fields produce the original
player-0 M1 body `3f000a000a`. The random-event callback enters production
`stars_random_events`; retained turn-zero gates skip nested event effects. The
post-production movement stage calls `stars_move_things`; the guess
stage derives both populations and calls `stars_update_guesses`; mark-backup
consumes `Random(8)`; and the three data-file callbacks write the original M1,
M2, and HST BOF records. Assertions cover the two player-log loads, both
waypoint validation callbacks, pre/post order phases and pass values, battle ID,
stage counts, planet flags/turns, final coordinator status, retained draw totals,
and all five complete BOF records. The seeded control/action streams remain
57/39 draws and the selector-1 regression recompiles and passes **1/1**.

This is not yet a complete production turn. The load callback creates a minimal
two-player state rather than reading the retained full files. Several source
stages remain no-op callbacks, including colonist breeding/fuel, battles,
planet effects, score updates, and player-file writes. The mine/event services,
waypoint validation, message bytes, and data-file body serialization still have
fixture boundaries; random-event nested effects are skipped by retained gates.
The three BOF records match, but full HST/M1/M2 output generation and the
field-by-field original-pair comparison are still required. The repeated SOTC
corruption screenshot has the archived hash and only records the rejected
`CLEAN.X2` file; it does not change the clean-pair comparison.

**No acceptance item closes. SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**

## Planet-10 task 5 and complete type-13 records (2026-09-25)

The same seeded coordinator now runs the retained owner-0 action task-5 fleet
and order through `stars_satisfy_orders_packed` before mining. Planet 10 has
stocks `478/899/381` after scrap and before mining; the production mining replay
then produces the retained action stocks `484/908/388`. The control path skips
this owner-0 action and produces `476/902/383`. The owner-1 planet-24 task-5
replay remains active in both runs and leaves its pre-mining stocks
`597/956/519`.

Both planets now serialize through production `stars_write_planet`; each
37-byte type-13 record compares byte-for-byte against its retained control and
action record. The control values are planet 10 `476/902/383/287` and planet
24 `592/963/520/256`; action values are planet 10 `484/908/388/287` and
planet 24 `592/964/520/256`. Mineral levels, populations, guess words, flags,
and every other byte match as well. The strict-C89 selector-1 regression passes
1/1 after rebuilding the coordinator against current `produce.c`,
`c_build_prod_item.c`, and `file.c`.

The coordinator's planet writer now covers the full 32-record HST planet
table. Planets 10 and 24 are emitted from their production-updated states; the
other 30 are decoded from the retained turn-0 HST through `stars_read_planet`
and re-encoded with `stars_write_planet`. All 32 record bodies match the
corresponding original control and action turn-1 HSTs. This exposed an
absent-tail `ReadPlanet` default bug: the unowned-planet sentinel was stored in
the wrong runtime word, so `WritePlanet` emitted an extra eight bytes. The
default now matches the original sentinel in `opaque_20[1]`. The strict-C89
selector-1 regression passes 1/1 and reports `PLANET_RECORDS=32/32` in both
seeded runs. The current eight-module SO-E3 regression passes 41/41 in
104.79 seconds, including this full-table serialization check.

This closes the HST planet-record slice only. The coordinator still does not write
the type-28 queue or complete HST/M1/M2 files, and several turn services and
message outputs remain fixture boundaries. SO-E3 stays at **1 full-turn
differential component left**; SO-E2 stays at **0 checks left**. TODO7 remains
**39 open** (29 evidence, 10 promotions).

## Production data-file coordinator in seeded turn (2026-09-25)

The seeded `stars_generate_turn` callback now invokes production
`stars_write_data_file` for player 0, player 1, and the game output, in that
order. The file-create callback calls `stars_write_bof` and retains the exact
18-byte M1/M2/HST BOFs already recovered. The game-file planet callback writes
the 32 type-13 HST planet records through production `stars_read_planet` and
`stars_write_planet`. The coordinator's final turn callback serializes the
current `state.turn` as the two-byte type-0 body `01 00`; the callback and
close stage run once for each file.

The fresh control/action outputs each have one final turn-1 type-0 marker in
M1, M2, and HST, all with body `01 00`. The test verifies all six retained
markers and the seeded production writer's three markers in each run. The
eight-module focused suite passes 41/41 in 105.289 seconds.

This exercises the production save coordinator, but does not assemble complete
streams. Only its create/BOF, game-file planet, turn-marker, and close callbacks
have source-bound output. Battle/player records, game salt, messages,
M2 planet 26, SHDEFs, fleets/orders, and battle plans are not written. The
full retained load state and multiple turn-stage callbacks also remain
incomplete. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component
left. TODO7: 39 open (29 evidence, 10 promotions).**

## Source-bound player-log planet records (2026-09-25)

The `write_planets` callback in the seeded production `stars_write_data_file`
path now writes all observed M1/M2 planet records through production readers,
visibility logic, and writers. For M1, planet 7 and planet 12 type-14 records
are read from retained M1 input by `stars_read_planet` and re-emitted; planet
10 is written from its production-updated type-13 state. For M2, planet 24 is
written from its production-updated type-13 state.

The remaining M2 record is planet 26, type 14, body
`1af80301005016542d0b2d`. Before writing M2, the visibility callback reads the
32 turn-0 HST planets through `stars_read_planet` and passes them with fleet 0's
arrival state to production `stars_set_vis_pf_init`. Production
`stars_move_fleets` leaves player-1 fleet 0 stationary at planet 26's retained
XY coordinates `(1313,1262)`, with `moving=0` and `target_id=26`. The
visibility initializer marks planet 26 at detail 3; the adapter applies the
production detail mask and yields runtime flags `0x0103`. `stars_write_planet`
then emits the exact retained type-14 body in both control and action runs.

All **5/5** player-log planet records now compare byte-for-byte: M1 **3/3** and
M2 **2/2**. The strict-C89 selector regression passes 1/1 and prints
`PLAYER_PLANETS=5/5`; the eight-module focused suite passes 41/41 in 105.715
seconds. Complete stream generation and the field-level full-turn differential
remain open. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component
left. TODO7: 39 open (29 evidence, 10 promotions).**

## Source-bound M2 fleet and compact-order records (2026-09-25)

The retained selector-1 coordinator now sets `fleet_count=2` for the player-1
M2 output and passes both fleets through production `stars_write_fleet`, which
in turn writes each current order with production `stars_write_orders`. The
fleet IDs, player bits, design plan, unchanged ship counts, and `cord` are
resolved from the retained M2 input. Fleet 0's moved destination/fuel and its
second current PLORD entry come from the production Turin/movement path. Fleet
1's stationary position/fuel comes from production `stars_move_fleets`; its 25
colonists come from the production supply transfer, and its current compact
order is emitted. For this retained case, the output `fHereAllTurn` bit follows
the production movement status: fleet 0 moved, fleet 1 stayed at its planet.

The adapter reconstructs the type-16 record prefix around the payload emitted
by `stars_write_fleet`, then compares complete plaintext record bodies and
following compact orders against both original files:

| Fleet | Type-16 bytes | Type-20 order bytes |
|---:|---|---|
| 0 | `0002010007091a002105ee0401000100022a0100000001` | `2105ee041a005011` |
| 1 | `01020100072918000a05ea04020001400119c800000001` | `0a05ea0418000011` |

Each type-16 record is 23 bytes; each type-20 compact order is 8 bytes. Both
original M2 outputs have the same fleet/order bodies. The targeted strict-C89
selector test passes 1/1 in 11.388 seconds. The eight-module SO-E3 regression
passes 41/41 in 104.809 seconds. The attached `CLEAN.X2` dialog is the same
archived screenshot hash; it documents that separate file's rejection and
does not identify the cause or add fresh guest-state evidence.

This closes only a current-source M2 fleet/order serializer slice. The
coordinator still has a minimal synthetic load, no-op turn services, and
fixture message/other section callbacks; it does not emit all HST/M1/M2
records or perform the complete record-by-record comparison. **SO-E2: 0 checks
left. SO-E3: 1 full-turn differential component left. TODO7: 39 open (29
evidence, 10 promotions).**

## Source-bound HST owner-1 fleet/order records (2026-09-25)

The game-file visit now asks the production `stars_write_data_file`
coordinator to serialize owner-1 fleet IDs 0 and 1 using the same production
fleet/order serializer inputs as the M2 player-log visit. In the retained
control and action outputs, these HST records are byte-identical to the M2
owner-1 records. Production output matches all four original plaintext bodies:
two 23-byte type-16 fleet records and their two 8-byte type-20 compact orders.
The targeted selector test passes 1/1; the full eight-module regression passes
41/41 in 105.636 seconds.

This binds the owner-1 HST fleet/order slice only. Owner-0 HST fleets and the
remaining file sections are not serialized here, and full current-source
HST/M1/M2 generation and comparison remain open. **SO-E2: 0 checks left.
SO-E3: 1 full-turn differential component left. TODO7: 39 open (29 evidence,
10 promotions).**

## Source-bound complete HST fleet/order records (2026-09-25)

The production HST fleet writer now covers both owners. For owner 0, the probe
loads the six retained turn-0 fleet/order states, runs their unchanged
same-planet movement through `stars_move_fleets`, and feeds the resulting
point/fuel/flags plus current compact orders through `stars_write_fleet` and
`stars_write_orders`. In the action run, the existing production task-5 scrap
path retires owner-0 fleet 0, so only IDs 1–5 are written. Owner-1 IDs 0/1 use
the current Turin route, movement, and supply state already described above.

The emitted records compare byte-for-byte against both retained original HST
outputs: control has owner-0 IDs 0–5 and owner-1 IDs 0/1 (8 fleet records and 8
compact orders); action has owner-0 IDs 1–5 and owner-1 IDs 0/1 (7 fleet records
and 7 compact orders). The machine-readable oracle stores every decoded owner,
ID, raw type-16 body, and following type-20 body.

The targeted strict-C89 selector test passes 1/1 in 11.355 seconds; the
eight-module SO-E3 suite passes 41/41 in 105.209 seconds. M2's two owner-1
fleet/order pairs still match in the same production run. M1 fleet output,
other save sections, full retained-state loading, and complete file comparison
remain open. The attached corrupt-X2 image has the archived hash and does not
identify the rejected field. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound M1 fleet/order records (2026-09-25)

The selector-1 probe now drives the production `stars_write_data_file`
coordinator for M1's owner-0 records. The retained turn-0 HST fleets and
compact orders feed production `stars_move_fleets`; current movement state and
current orders then pass through `stars_write_fleet` and `stars_write_orders`.
The original M1 owner-0 bodies match the corresponding original HST records.

Control serializes IDs 0–5: all six type-16 fleet bodies and following
type-20 compact orders match the original M1 output. In the action run,
production task 5 retires fleet 0 and IDs 1–5 match. Together with other slices
in the same probe, current-source output matches M2's 2/2 owner-1 pairs and the
full HST's 8/8 control and 7/7 action fleet/order pairs. The oracle JSON stores
the M1 raw records alongside the HST owner-0 records.

The targeted strict-C89 selector test passes 1/1 in 11.307 seconds; the
eight-module regression passes 41/41 in 105.462 seconds. Remaining M1/M2/HST
record sections, complete source-state loading, and a complete decoded file
differential are still open. The attached SOTC dialog is an already archived
rejection image and does not identify the rejected field. **SO-E2: 0 checks
left. SO-E3: 1 full-turn differential component left. TODO7: 39 open (29
evidence, 10 promotions).**


## Source-bound planet-24 type-28 production queue (2026-09-25)

The production data-file callback now writes planet 24's two post-`Produce`
queue entries in M2 and HST, immediately after the planet's type-13 record.
The new `stars_write_production_queue_record` service packs ordinary queue
items using the recovered four-byte `_prod` layout: quantity in bits 0–9, item
ID in bits 10–16, object kind in bits 17–19, and percent complete in bits
20–26.

Current Turin turn-zero queue generation produces two design-0 ship entries,
then current `stars_produce` leaves them at 81% and 0% progress. The emitted
eight-byte type-28 body is `0100140501000400`. It matches all four retained
M2/HST control/action records byte-for-byte and remains adjacent to planet 24
in each stream. The focused selector test passes 1/1 in 11.330 seconds; the
eight-module SO-E3 regression passes 41/41 in 105.606 seconds.

This adds one production queue record slice. The planet-24 resource budget is
still supplied from the retained EXE-backed witness, other queue branches and
message state remain unbound, and the source does not yet assemble complete
HST/M1/M2 files. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential
component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound HST type-43 wormhole records (2026-09-25)

The recovered `FWriteDataFile` source places score records after the second
ship-design bank, then counts visible things, emits one type-43 count record,
and writes each selected thing as an 18-byte type-43 record. The reconstructed
`stars_write_data_file` coordinator now follows this ordering around the
design/fleet sections. The separate `stars_write_thing_records` serializer
emits the count as a two-byte little-endian body and then each raw 18-byte
thing body.

In the retained selector-1 control and action replay, four current things are
the four moved HST wormholes. Their current raw bytes are compared after
production `stars_move_things` and match all four retained original HST
type-43 bodies exactly. The resulting HST slice is a type-43 count body
`0400` followed by four type-43 bodies of length 18. M1 and M2 have no type-43
records in this pair.

The strict-C89 writer-order source probe passes 4 checks, and the focused
selector test passes 1/1 in 11.265 seconds. The combined eight-module SO-E3
regression plus the new writer-order module passes 42/42 in 107.747 seconds.
The baseline, JSON, Python syntax, and whitespace checks pass.

At that earlier checkpoint, the score-writing callback was a no-op. The
later score-history subsection records its coordinator integration. Other
stream records remain unbound, current turn state loading still uses retained
fields and service callbacks, and the full HST/M1/M2 files are not yet
assembled or compared. The supplied image remains
evidence of a Stars! rejection dialog for `C:\SOTC\CLEAN.X2`; it does not
establish why the file was rejected. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Uncapped fleet history in type-45 score summaries (2026-09-25)

The retained turn-1 M1 type-45 records contain fleet-history words `[4,2,0]`
for control and `[4,1,0]` for the action, while both score summaries report
score 25, resources 38, one planet, one starbase, and technology total 18.
`stars_calc_player_score` now preserves the active fleet-class counts in
`StarsScoreSummary.fleet_counts` and caps a separate copy only when computing
the category score bonus. This keeps the score at 25 while matching the
uncapped history words in both retained records.

The formula regression passes for both count vectors, and the retained-pair
audit passes 26/26. The focused common score tests pass 2/2, and the core
library builds successfully. The raw decompiled `CalcPlayerScore` listing
appears to cap counters before packing, so that source-level discrepancy still
needs reconciliation against the retained executable output.

This fixes one serialized field behavior; it does not close SO-E3. The type-45
writer callback still receives no current-source turn-1 bodies, full score
inputs and HST/M1/M2 assembly remain open, and no complete-file differential
has been established. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential
component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Production packing for type-45 score history (2026-09-25)

`stars_pack_score_history_record` now packs a `StarsScoreSummary` into the
original 24-byte `_scorex` body. It preserves the caller-maintained flags and
rank words, writes the score/resource/planet/starbase/technology fields at
their retained offsets, and uses `stars_pack_long` for the three fleet-history
lanes. A retained-shaped control summary packs to
`200001001900000026000000010001000400020000001200`; the action count vector
packs to `200001001900000026000000010001000400010000001200`. Both match the
corresponding retained turn-1 M1 type-45 bodies byte-for-byte.

The combined focused score, retained-pair, and writer run passes 29/29. This
binds the score summary's record layout, but `turn_update_scores` still lacks
complete current player/planet/design/fleet inputs and the data-file score
callback still emits no source-generated type-45 bodies. Full HST/M1/M2
assembly and comparison remain open. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound turn-zero SpankTheCheaters stage (2026-09-25)

The selector-1 turn coordinator now invokes production
`stars_spank_the_cheaters` at the retained turn-zero position. Its `turn <= 9`
gate returns before fleet or planet penalties; the integration callback
checks that no random draw, seed change, or penalty message occurs. The
strict-C89 selector-1 probe, including 100 injected outer `Random(100)` values,
passes.

This replaces one no-op stage for the retained turn. Other service callbacks
and state loading remain incomplete, and complete HST/M1/M2 files are not
assembled or compared. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound turn-zero race-gated services (2026-09-25)

The turn coordinator now calls production `stars_breed_colonists_in_transit`
and `stars_auto_terraform`. The callbacks use race stat `0x0e` values `[9,1]`
decoded from the pinned turn-0 HST player records. Neither value enables
transit breeding (trait value 4) or auto-terraforming (trait value 3), so both
production services return before fleet/planet processing. The callbacks
confirm unchanged RNG seeds and call counts. The strict-C89 selector test and
the retained-pair suite pass 27/27, including 100 injected outer
`Random(100)` values.

This replaces two more no-op stages for the retained pair. Other service
callbacks and state loading remain incomplete, and full HST/M1/M2 files are
not assembled or compared. **SO-E2: 0 checks left. SO-E3: 1 full-turn
differential component left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound mine sweeping on the retained pair (2026-09-25)

The selector-1 coordinator now calls production
`stars_sweep_for_mines` after post-production wormhole movement. Its input
tables contain the current 8 control / 7 action active fleets, all 32 planets,
and the four current type-2 things. Planet ownership comes from turn-0 HST
records and is checked unchanged against each retained turn-1 HST; coordinates
come from the pinned `CLEAN.XY` map. The action has one fewer fleet because
the task-5 order retired fleet 0.

The retained type-43 table contains only wormholes. Positive fleet and planet
sweep-power witnesses drive the production loops, but the routine skips each
thing at its `kind != 0` check. The `can_attack`, message, and free callbacks
are never reached, and every thing stays active with its owner mask unchanged.
This establishes the service's no-minefield state effect for these pinned
inputs; the exact game fleet/planet sweep-power queries remain fixture
boundaries.

The focused selector-1 test passes in both seeded runs and across 100 injected
outer `Random(100)` values. The combined selector/pair suite passes 27/27;
the core build, baseline, oracle JSON, and whitespace checks pass. This removes
one turn-stage placeholder for the retained pair. Fueling, ship healing,
remote terraforming, score calculation/writing, other service callbacks,
complete HST/M1/M2 generation, and the record-by-record differential remain
open. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component
left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound M2 input fleet reading (2026-09-25)

The selector-1 probe now reads all four owner-1 turn-0 type-16 fleet bodies
and their following compact type-20 orders through production
`stars_read_fleet`. The retained body supplies the sparse ship counts, cargo,
planet and coordinates, design plan, and order; the reader expands each
eight-byte compact order into the 18-byte runtime order stride. The probe
checks the decoded fleet bytes and compact order fields against the retained
input records. It performs this load before Turin AI runs, so the subsequent
AI mutations are preserved through the full-turn coordinator.

The strict-C89 selector test passes for both seeded original runs and all 100
injected outer `Random(100)` values. The combined selector/pair suite passes
27/27; the core build, baseline, oracle JSON, and whitespace checks pass. This
binds M2 fleet/order inputs only. Owner-0 M1 input loading, other record
sections, shared RNG and service boundaries, complete HST/M1/M2 assembly, and
the full record-by-record differential remain open. **SO-E2: 0 checks left.
SO-E3: 1 full-turn differential component left. TODO7: 39 open (29 evidence,
10 promotions).**


## Source-bound owner-0 HST fleet reading (2026-09-25)

The retained movement pass now reads all six owner-0 turn-0 type-16 fleet
bodies and their following compact type-20 orders through production
`stars_read_fleet`. The movement adapter takes each fleet's position and fuel
from the decoded record, and its order callback reads the expanded 18-byte
runtime order generated from the original eight-byte compact record. The
probe validates the decoded ID/player bits, planet, coordinates, ship counts,
cargo, design plan, and order bytes against the pinned HST input.

The selector regression passes for both seeded original runs and all 100
injected outer `Random(100)` values; it asserts 6/6 owner-0 records and 4/4
owner-1 M2 records were read. The combined selector/pair suite passes 27/27;
the core build, baseline, oracle JSON, and whitespace checks pass. This adds
source-bound movement inputs for the six owner-0 fleets. Fleet lifecycle,
other M1/HST record sections, shared RNG and remaining service boundaries,
complete file assembly, and the full record-by-record differential remain
open. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component left.
TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound loading of all HST planet inputs (2026-09-25)

The selector-1 `startup_turn_load` callback now reads all 32 turn-0 HST
type-13 planet records through production `stars_read_planet`. It checks each
record's retained length and decoded planet ID, then keeps the decoded records
in one current-state array indexed by planet ID. This includes planets 10 and
24, whose current state continues through the already-bound order, mining,
growth, and production callbacks.

The HST writer now serializes all 32 planet records from that current-state
array. It no longer reparses turn-0 inputs inside the writer for unowned
planets. The M2 visibility initialization copies the loaded array before it
sets player-only visibility on planet 26, keeping those flags out of the HST
state. Both seeded control/action HST outputs match all 32 retained turn-1
planet bodies byte-for-byte. The probe reports `HST_INPUT_PLANETS=32/32` and
`PLANET_RECORDS=32/32`.

The focused strict-C89 selector-1 regression passes for both pinned runs and
100 injected outer `Random(100)` values. The combined selector/pair suite
passes 27/27; `build_core.py`, `baseline.py --check`, JSON parsing, Python
syntax, and whitespace checks pass.

This closes only current HST planet input loading and planet-record output for
this retained pair. Other input records, player-log planet state, service
callbacks, full HST/M1/M2 assembly, and the complete decoded differential
remain open. **SO-E2: 0 checks left. SO-E3: 1 full-turn differential component
left. TODO7: 39 open (29 evidence, 10 promotions).**


## Source-bound M1 and M2 planet inputs (2026-09-25)

The production `load_player_log` callback now decodes the retained owner-0 M1
planet records and owner-1 M2 planet records through `stars_read_planet` before
turn processing. It loads M1 planets 7, 10, and 12, plus M2 planet 24, and
checks each decoded ID. The shared M1 planet 10 and M2 planet 24 records are
also checked against their corresponding current HST state at load time. The
M2 log load also examines all four retained turn-0 type-26 SHDEF records in
stream order. Designs 0, 1, and 2 decode through production
`stars_read_shdef`, and the design-2 cost fields are checked. Design 16 names
hull 34, which the retained type-0x4000 hull lookup cannot resolve: that table
accepts item IDs only through 31. The probe records this one unresolved input
instead of fabricating a hull record.

The M1 writer now serializes its loaded type-14 planet 7 and planet 12 states
instead of rereading those input bodies at output time. Planet 10 continues to
use the current production-updated type-13 state. M2 planet 24 is decoded and
validated as player-log input; its output uses the same current planet-24 state
after the production changes. The test continues to match all 5/5 M1/M2
planet records byte-for-byte and asserts `M1_INPUT_PLANETS=3/3` and
`M2_INPUT_PLANETS=1/1`. At task-5 order pass 1, the production scrap adapter
reuses loaded M2 planet 24 and loaded design 2 instead of reparsing those
retained inputs mid-turn. The isolated non-turn scrap test loads the same three
resolvable SHDEF records through its direct input path.

The strict-C89 selector test passes in control/action and for 100 injected
outer `Random(100)` values. Its marker reports `M2_SHDEFS=3/4 INVALID=1`.
The combined selector/pair/score-writer run passes 28/28.

This binds the four player-log planet inputs and three resolvable M2 design
records. The design-16/hull-34 record, other player-log input sections,
remaining service and RNG boundaries, complete HST/M1/M2 assembly, and the
full decoded differential remain open. **SO-E2: 0 checks left. SO-E3: 1
full-turn differential component left. TODO7: 39 open (29 evidence, 10
promotions).**


## Full-turn acceptance audit (2026-09-25)

The retained turn-1 streams contain 32/30 M1 records, 24/24 M2 records, and
83/81 HST records for control/action. The probe checks selected writer slices;
it does not assemble or compare those complete streams.

| Record kind | M1 control/action | M2 control/action | HST control/action | Probe status |
|---|---:|---:|---:|---|
| 0 turn marker | 1/1 | 1/1 | 1/1 | Written and checked |
| 6 player record | 1/1 | 1/1 | 2/2 | Unbound; player-file callback is a no-op |
| 8 | 1/1 | 1/1 | 1/1 | Unbound |
| 12 | 1/1 | — | — | Unbound |
| 13/14 planets | 1+2 / 1+2 | 1+1 / 1+1 | 32/32 | Compared with retained output bodies |
| 16/20 fleets/orders | 6/5 each | 2/2 each | 8/7 each | Compared with retained output bodies |
| 26 ship designs | 7/7 | 8/8 | 15/15 | No output writer; only 3/4 M2 inputs decode |
| 28 production queue | — | 1/1 | 1/1 | One planet-24 queue case; 32 base resources are now source-derived and 15% research leaves 28 |
| 30 | 5/5 | 5/5 | 10/10 | Unbound |
| 43 things | — | — | 5/5 | HST count and four thing bodies checked |
| 45 score history | 1/1 | 1/1 | — | Writer integrated; bodies remain fixtures |

This is why the formal remaining count has not moved: record kinds 6, 8, 12,
26, and 30 are not written or compared, score values are fixture-backed, and
the full streams are not assembled. Design 16/hull 34 is unresolved as an
input. **Remaining: SO-E2: 0 checks. SO-E3: 1 full-turn differential gate.**

## Source-bound planet-24 production resources (2026-09-25)

The selector production probe no longer seeds planet 24's queue with the
retained 28-resource output. It builds the runtime resource inputs from the
retained planet/player records, applies the current-source Turin supply
transfer (planet colonists 250→225), and calls `stars_cresources_at_planet`.
The decoded retained player has environment stat 10, factory stat 10, and
`pctResearch=15`; planet 24 has 10 factories and its research-exemption bit 23
is clear. At the pre-production population of 225, the source calculator
returns 32. Reconstructed `stars_produce` then reserves `32*15/100 = 4` for
research and gives 28 to the queue, reproducing the retained 81% progress and
mineral result. The retained player cheater bit is clear for this case.

`StarsProducePlanet` now includes the research percentage and exemption flag.
The production routine applies research after the existing fCheater scale and
accounts for research plus unused resources in its last-year resource value.
The independent fCheater 35→28 regression remains. The production, seed queue,
four-pass, and selector tests pass 10/10 in 70.95 seconds, including 100
outer random draws and both retained seed cases.

This resolves the injected production-budget subissue only. It does not close
any of the four SO-E3 grouped gates: full state/service inputs, the complete
current-source turn, complete HST/M1/M2 serialization, and the full decoded
control/action differential remain open. **SO-E2: 0 checks left. SO-E3: 1
acceptance item / 4 grouped closure gates. TODO7: 39 open (29 evidence, 10
promotions).**
