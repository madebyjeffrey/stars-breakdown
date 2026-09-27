# SatisfyOrders SO-E2 / SO-E3 follow-up

Current status: **SO-E2 is complete; SO-E3 remains open**. SO-E2 closed on
2026-09-24 after the exact high-offer inputs passed current-source host and
VC1.52c native checks 74/74; see
[`satisfy-orders-task5-v4-current-source-native.md`](satisfy-orders-task5-v4-current-source-native.md).
The historical entries below record the incremental service work. The current
SO-E3 oracle now also bounds owner-1 planet-24 mining from retained inputs,
while the complete production/full-turn comparison and exact RNG seed/draw
accounting remain open; see
[`satisfy-orders-full-turn-e3-original-oracle.md`](satisfy-orders-full-turn-e3-original-oracle.md#owner-1-planet-24-mining-bound).

## SO-E2: task-5 service path

The packed task-5 path in `reconstructed/src/satisfy_orders.c` now handles
deep-space salvage as well as planet scrap. For deep space it applies the
original one-third mineral return plus fleet mineral cargo, calls the
salvage-creation service, sends the original `0x5B` two-argument message,
removes an existing `0x4E` fleet message, and retires the fleet/order. The
planet path now binds the optional `ITechLearnATech` result after the first
message, selecting `0x13C` for a negative result or `0x13D` for a positive
result and its corresponding result index. Both paths call the message-removal
service before setting the fleet's dead bit. The earlier `0x59/0x140` and
`0x5A/0x141` variants remain covered by the E1 probe.

A later source-order correction moved friendly-colonist transfer after
`FRemovePlayerMessage(0x4E)` and the fleet dead-bit store, matching the
decompiled branch. The expanded strict-C89 service probe passes **30/30**;
its removal callback observes unchanged planet colonists and a live fleet,
and the final state observes the colonist transfer. The earlier native 25/25
packet predates this correction and does not validate the current service
order. At that point no new guest run followed the original deep-space
failures; the later natural SMOKE case is documented below.

`tests/satisfy_orders_task5_services_probe.c` checks the source call order,
recipient and arguments, negative and positive technology variants, deep-space
mineral values, absent-service preflight, and retirement. The strict-C89 host
and fresh VC++ 1.52c Win98 runs pass 25/25. The native packet is
`compat/vc152/satisfy-orders-task5-e2-r1/`; the build status, compiler/linker
logs, map, object, executable, and report are in `runs/vmctl-soe2a/`. Native
`SYOSTA.TXT` says compile/link PASS and `SOE2RES.TXT` reports 25 checks,
zero failures. The process exited 0 and no Program Error window appeared.

The original decompiled branch at `10b0:8207-8e65` remains the service-order
oracle for the new deep-space and technology paths. The isolated original
planet action run in `runs/vmctl-syact3/` confirms only the ordinary
`0x5A/0x141` case. An experimental deep-space input was made by changing the
first fleet's planet ID in the verified HST action input from 10 to `-1`;
the single edited record is retained under
`runs/syorig-deepspace-bundle-1/`. The guest log records that the original
process timed out and was terminated. `CLEAN.M1` stayed at the 843-byte input
size; only partial `CLEAN.H2` and `CLEAN.X2` files were recovered in
`runs/vmctl-sydsp1-incomplete/`. This supplies **no original-behavior
acceptance**.

A second experiment, `runs/syorig-deepspace-bundle-2/`, moved fleet 0 to
deep space consistently in the HST and M1 records. It also failed: the
original process returned `4294967295` (`-1`), `CLEAN.M1` remained the
843-byte input, and only partial `.H2`/`.X2` files appeared. These and the
agent log are retained in `runs/vmctl-sydsp2-incomplete/`. Original
deep-space execution was suspended until a naturally occurring deep-space
fleet was found below; neither relocated experiment supports E2 acceptance.

The untouched original path also calls `MarkTechsSeen` for each occupied
design before accounting. With the **fleet** race's bit `0xC`, it copies each
SHDEF and calls `UpdateShdefCost` in that race's context. The **planet
owner** race's bit `5` selects a separate resource-conversion branch rather
than the ordinary mineral branch. That branch converts ship resource cost into
`vrgPlanResExtra`, uses `CResourcesAtPlanet` when available, and sends
`0x5C/0x5D` followed by `0x142/0x143` or signed technology-result
`0x13E/0x13F`. The ordinary path's source order is
mineral mutation or `DropSalvage`, first message, optional tech learning,
second message, `FRemovePlayerMessage(0x4E)`, fleet dead bit, then friendly
colonist transfer.

The packed implementation now has opt-in callbacks for `MarkTechsSeen`,
`UpdateShdefCost`, and `CResourcesAtPlanet`. Its resource-conversion branch
updates the supplied extra-resource word and sends the resource message pair.
The original race-bit-5 turn below exposed a source-order error: the branch
also credits planet minerals before resource conversion. The current
reconstruction now applies that credit. The strict-C89
`tests/satisfy_orders_task5_resource_probe.c` passes 62/62 checks over
repricing call order, quartered gift designs, 16-bit extra-resource mutation,
resource adjustment, both base and signed-technology message variants,
and retirement. The fresh VC++ 1.52c packet in
`compat/vc152/satisfy-orders-task5-e2-resource-r3/` compiled and linked in
`C:\SOE2C`; its [native report](../../runs/vmctl-soe2c/SOE2CRES.TXT)
passes 62/62. Build status, compiler/linker logs, map, object, and executable
are retained in `runs/vmctl-soe2c/`.

The current resource probe now binds its `scrap_mark_techs` callback to the
reconstructed `stars_mark_techs_seen` service through
`stars_mark_techs_seen_shdef`, which decodes the original 147-byte SHDEF's
hull ID, component groups/counts, and occupied slots. The probe supplies a
part-technology lookup and observes the resulting battle-tech and trader-tech
state after task-5 invokes the service, as well as the old call-order and
resource checks. Strict-C89 host source and packaged-file runs pass
**75/75**. The prior native 62/62 packet predates this binding and is stale
for the current resource probe. The fresh
`compat/vc152/satisfy-orders-task5-e2-resource-r5/` packet includes
`MARKTEC.C`. The earlier native run in `C:\SOE2F` compiled and linked with
VC1.52c and passed **75/75** checks; its [build status](../../runs/vmctl-soe2f/SYOSTA.TXT)
and [report](../../runs/vmctl-soe2f/SOE2FRES.TXT) are retained. The
earlier service-order packet in `C:\SOE2D` likewise compiled, linked, and passed
**30/30** checks; see its [build status](../../runs/vmctl-soe2d/SYOSTA.TXT)
and [report](../../runs/vmctl-soe2d/SOE2RES.TXT).

The [pinned race-bit-5 original oracle](satisfy-orders-racebit5-original-oracle.json)
uses the known successful player-0 task-5 HST order and changes only player
0's race-bit word from zero to `0x20`. The original executable completed a
full turn in `C:\SYRB5` without an error dialog. Compared with the retained
clean original turn, planet 10 gained **10/7/6 minerals**, exactly the
9/10 starbase return on verified design-0 ore costs **12/8/7**. Its M1
message stream contains **0x5D, 0x143, 0x3F** and omits fleet 0. This
demonstrates original resource-branch mineral credit and the two task-5
message IDs. The earlier packed implementation incorrectly skipped the
mineral credit for this branch; the corrected resource probe now expects
the mineral mutation. Fresh current-source Win98 [resource build and
75/75 report](../../runs/vmctl-soe2g/MANIFEST.JSON) and [service-order
build and 30/30 report](../../runs/vmctl-soe2h/MANIFEST.JSON) pass.

An additional isolated home-planet original turn changed only player 1
fleet 0's existing order at planet 24. Its [returned output](../../runs/vmctl-syhome2/MANIFEST.JSON)
retires that fleet and changes planet 24's stockpiles, but it does not
exercise deep-space salvage or a signed technology result. It is supporting
evidence only. The race-bit-5 [returned output](../../runs/vmctl-syrb5/MANIFEST.JSON)
is likewise a complete original resource case, not a complete SO-E2 closure.

At that point the remaining E2 gap was a complete original deep-space salvage
outcome and signed technology outcomes, plus original part lookup and live
`UpdateShdefCost` binding to verified design, planet, and race records.
The raw `MarkTechsSeen` adapter runs the reconstructed
service under a supplied lookup; it does not establish the guest's lookup
state or all other service effects. At that checkpoint, four closure checks
remained in `EVIDENCE.md` #2; see the current count below.

The next [verified-input resource probe](../../tests/satisfy_orders_task5_verified_resource_probe.c)
uses the retained owner-0 player attributes, planet-10 population/factories,
fleet/order, and design-0 costs. Its callback invokes production
`stars_cresources_at_planet` on the packed planet record. That service
returns **35** base resources; task 5 applies the verified isolated mineral
return **10/7/6**, increments the supplied extra-resource word by **24**,
and sends `0x5D/0x143` with adjusted resource amount **14**. The strict-C89
host probe passes **22/22** and the fresh [VC1.52c build and report](../../runs/vmctl-soe2l/MANIFEST.JSON)
passes **22/22** with identical output after CRLF normalization. Its input
scalars are pinned against the original HST and design-cost oracle in
`tests/test_satisfy_orders_task5_verified_resource.py`. This binds the
resource and `CResourcesAtPlanet` slice; the later part and salvage bindings
are documented below, while technology services remain unbound.

The next [SHDEF repricing probe](../../tests/satisfy_orders_shdef_reprice_probe.c)
uses the four exact hull/component records recovered from the original
executable's NE segment 2 for design 0. A new production adapter resolves
the packed SHDEF's hull and three occupied slots through reconstructed
`FLookupPart`, reads each original part's technology and four cost lanes,
and applies reconstructed `GetTruePartCost` for owner 0. It returns the
verified **24/12/8/7** design costs. Malformed component counts, missing
parts, and truncated tables fail before writing the destination. The host
probe first passed **24/24**; its [VC1.52c native report](../../runs/vmctl-soe2m/MANIFEST.JSON)
also passed **24/24**. The integrated probe calls this adapter from packed
task 5 four times (three mineral lanes and resources) while using the
retained owner-0, planet-10, fleet/order, and design-0 values. It passes
**67/67** on the host and in a fresh [VC1.52c native build](../../runs/vmctl-soe2n/MANIFEST.JSON):
the isolated planet minerals become **478/899/381**, the message IDs are
`0x5A/0x141`, and the fleet retires. The source fixture is regenerated and
checked against the original EXE/HST hashes by
`tools/satisfy_orders_part_fixture.py`. This is a source-backed repricing
binding for the retained design; the probe enables the fleet-race repricing
branch explicitly. The original turn has not yet exercised that race bit,
and the guest's live part table has not been captured. At that checkpoint,
four SO-E2 closure checks remained; see the current count below.

A retained [SMOKE turn-1 input](../../runs/step4/fixtures/replay-a/manifest.json)
has a genuine owner-1 fleet 0 in deep space at **(1276,1380)**, with one
design-0 ship and 298 fuel. Its HST and matching M2 first order were changed
only from valid point task 0 to task 5 in the
[candidate bundle](../../runs/syorig-smoke-deepspace-bundle-1/MANIFEST.JSON).
The original Stars! executable completed both that candidate and an
unaltered [control](../../runs/vmctl-sysmk0/MANIFEST.JSON), with no error
dialog. The [candidate output](../../runs/vmctl-sysmk1/MANIFEST.JSON) removes
the selected fleet and adds one owner-1 salvage thing at (1276,1380), with
**5/0/2 minerals**. The original EXE's part records and owner-1 technology
give design-0 cost **35 resources, 16/2/8 minerals**, so the saved salvage
is the expected one-third return. The [pinned original oracle](satisfy-orders-smoke-deepspace-original-oracle.json)
checks the paired HST/M2 input and control/action output hashes, fleet
retirement in both output streams, original design cost, and the raw salvage
record. The previous two relocated-fleet attempts remain incomplete
diagnostics; this natural input supplied the first complete original
deep-space case.

The same fleet, order, and design costs now drive production packed task 5,
`DropSalvage`, and subsequent `ThingDecay` in
`tests/satisfy_orders_smoke_deepspace_probe.c`. The fresh
[VC1.52c native report](../../runs/vmctl-soe2q/MANIFEST.JSON) passes
**21/21**. It matches the original 18-byte saved thing
`0022fc046405ff8305000000020002000000`, 5/0/2 minerals, fleet retirement,
and the `0x5B` callback frame with `-6`, salvage full ID `0x2200`, and fleet
word 0. The comparison exposed and corrected two source mistakes: the new
salvage thing initializes its packed flags with `0x3FF` and value zero, and
the first `ThingDecay` pass clears bit 14 before mineral decay.

The AI owner's original M2 suppresses this message. A paired
[human-message control/action run](satisfy-orders-smoke-human-message-original-oracle.json)
clears only the owner's AI suppression bit in matching HST/M2 inputs. Both
turns complete; the action's saved kind-12 entry is `5b02faff002200`,
decoding to `0x5B`, goto `-6`, salvage ID `0x2200`, and fleet word 0. The
paired input and returned output hashes are pinned by the oracle. This
closes the deep-space salvage check; **three SO-E2 checks remain**.

## Corrected original signed-technology candidate (2026-09-23)

The first trader-tech candidate builder selected the wrong ship definition.
In the retained HST, player records report six normal ship designs for player
0 and three for player 1. The type-26 records 35–40 are player 0 designs;
records 41–43 are player 1 designs. The ship-definition slot is bits 10–14
of the flags word. Player-1 fleet 0 starts with slot 0 set, so its definition
is record 41 (Scout hull 4). Record 39 is player 0 slot 4 (Destroyer hull 6).
The earlier builder's bits-8/12 check confused slot bits with an owner value.
Its results, including any apparent signed-tech result, are retained as
diagnostics and do not establish the trader path.

The replacement
[slot-verified candidate builder](../../tools/prepare_satisfy_orders_trader_technology_original_candidate_v2.py)
checks player record counts, owner and slot for each target SHDEF, each hull
slot's original group/item/count, the packed fleet bitmap/counts, and a full
stream round-trip. It adds one ship each from player-1 design slots 0–2 to the
tasking fleet, then replaces only hull-valid parts with trader parts: three
Enigma Pulsars (tech 9), two Multi-Cargo Pods (tech 0), and two Alien Miners
(tech 4). Control and action inputs differ only in the compact order's task
flag. Their manifests are in
[`bundle-v2-control`](../../runs/syorig-tech-trader-bundle-v2-control/MANIFEST.JSON)
and [`bundle-v2-action`](../../runs/syorig-tech-trader-bundle-v2-action/MANIFEST.JSON).

The fresh-input original pair loaded without an error dialog. Task 0 keeps
owner-1 fleet 0 and player-0 tech `[3,3,3,3,3,3]`; task 5 retires that fleet,
emits original message `0x13D`, and raises tech index 2 from 3 to 4. The
decoded observations, command, input/output hashes, and screenshots are in
the [control run](../../runs/vmctl-sytech-correct-v2-control/RUN.JSON) and
[action run](../../runs/vmctl-sytech-correct-v2-action/RUN.JSON). The matched
negative `0x13C` result is recorded below.

Five further paired runs used exact shared H2 inputs and loaded without an
error dialog. In seeds 1 and 2 both controls retain the target fleet and both
task-5 actions retire it; neither pair changes technology or emits
`0x13C/0x13D`. Their seed provenance and return hashes are recorded in the
[seed-1 control run](../../runs/vmctl-sytech-correct-v2-seed1-control/RUN.JSON),
[seed-1 action run](../../runs/vmctl-sytech-correct-v2-seed1-action/RUN.JSON),
[seed-2 control run](../../runs/vmctl-sytech-correct-v2-seed2-control/RUN.JSON),
and [seed-2 action run](../../runs/vmctl-sytech-correct-v2-seed2-action/RUN.JSON).
Seed 3 also retains the target fleet in control and retires it in task 5; the
action emits `0x13D` and raises player-0 tech index 0 from 3 to 4. The paired
[seed-3 control run](../../runs/vmctl-sytech-correct-v2-seed3-control/RUN.JSON)
and [seed-3 action run](../../runs/vmctl-sytech-correct-v2-seed3-action/RUN.JSON)
record the shared input H2 and return hashes. Seed 4 has the same fleet
control/action result; its task-5 action emits `0x13D` and raises player-0
tech index 4 from 3 to 4. See the [seed-4 control run](../../runs/vmctl-sytech-correct-v2-seed4-control/RUN.JSON)
and [seed-4 action run](../../runs/vmctl-sytech-correct-v2-seed4-action/RUN.JSON).
Seed 5 likewise retains the fleet in control and retires it in task 5; the
action emits `0x13D` and raises player-0 tech index 2 from 3 to 4. See the
[seed-5 control run](../../runs/vmctl-sytech-correct-v2-seed5-control/RUN.JSON)
and [seed-5 action run](../../runs/vmctl-sytech-correct-v2-seed5-action/RUN.JSON).
The user's screenshot of a corrupt `CLEAN.X2` dialog belongs to an earlier
invalid candidate and remains excluded; none of these corrected runs created
a `CLEAN.X2` file or showed an error window. **Two SO-E2 closure checks remain;
SO-E3 still has one.**

## Matched negative signed-technology result (2026-09-24)

A corrected v3 candidate increased the trader offer count by using a
slot-verified player-1 slot-0 Battleship design with 11 hull-valid components.
The control and action use the same H2 input (SHA-256
`69dcea347053bfc511760f793895f98447eff68e17ad02fd018c3f559112bb89`); their
candidate HST inputs differ only in the task flag at record 57. The candidate
builder and input manifests are retained in
[the v3 builder](../../tools/prepare_satisfy_orders_trader_technology_original_candidate_v3.py),
[the v3 control bundle](../../runs/syorig-tech-trader-v3-seed1-control/MANIFEST.JSON),
and [the v3 action bundle](../../runs/syorig-tech-trader-v3-seed1-action/MANIFEST.JSON).

Both corrected original-executable runs loaded without an error dialog. The
[task-0 control](../../runs/vmctl-sytech-correct-v3-seed1-control/RUN.JSON)
kept owner-1 fleet 0, emitted no point-technology message, and retained player-0
tech levels `[3,3,3,3,3,3]`. The [task-5 action](../../runs/vmctl-sytech-correct-v3-seed1-action/RUN.JSON)
retired owner-1 fleet 0 and emitted negative result `0x13C`. Its frame is
`header_word=0x073C`, `goto=-15866`, args `[512,1147,0,10]`, with encoded
argument bytes `0002 7b04 00 0a`. Player-0 tech levels stayed unchanged, while
`grbitTrader=0x0004` and `wFlags=0x0008` (`fLearned=true`) record the learned
trader-technology state. The returned HST/M1/M2 files and hashes are retained
for both sides in each run directory. The target design is player-1 SHDEF
record 41, slot 0, Battleship hull 9; earlier incorrect record-39 experiments
remain excluded diagnostics.

The paired positive `0x13D` result remains in the corrected v2 action run and
raises player-0 tech index 2 from 3 to 4. Together these runs complete the
matched ordinary-branch signed-result evidence required here. The distinct
race-bit-5 planet-resource branch can emit `0x13E/0x13F`; that branch remains a
separate extension to the current acceptance check. The supplied corrupt-file
screenshot is evidence only of the earlier invalid candidate and does not
contradict these clean v2/v3 runs.

## Race-bit-C high-offer original pair (2026-09-24)

The reproducible [v4 oracle](satisfy-orders-reprice-v4-bitc-original-oracle.json)
verifies a matched task-0/task-5 pair with shared input H2 SHA-256
`69dcea347053bfc511760f793895f98447eff68e17ad02fd018c3f559112bb89`. The
candidate HST inputs differ only at full-order record 57. Both carry the
slot-0 player-1 fleet and the same 11-component, hull-9 SHDEF at record 41;
player 1 has technology `[0,0,1,0,5,0]`, race stat 1, and race bits
`0x80003045` with bit C set. The original executable and both input bundles
are hash-pinned.

Original Stars! completed the control in `C:\SVC92C` and action in
`C:\SVC92A` with process exit 0. Neither run showed an error dialog or
created `CLEAN.X2`. The [control return](../../runs/vmctl-sytech-reprice-v4-bitc-control/RUN.JSON)
retains player-1 fleet 0, leaves player-0 technologies at `[3,3,3,3,3,3]`,
and has no point-technology result. The [action return](../../runs/vmctl-sytech-reprice-v4-bitc-action/RUN.JSON)
retires owner-1 fleet 0, stores the raw `0x13D` message frame, and raises
player-0 technology 0 from 3 to 4. The resulting H2 output files differ; they
are not treated as the original random seed. The attached `C:\SOTC\CLEAN.X2`
dialog remains excluded as an earlier invalid candidate, separate from these
isolated `C:\SVC92*` runs.

The original pair now binds the live race-bit-C condition and high-offer design
to an observed original outcome. The [field-level differential](satisfy-orders-task5-field-differential.md)
closes the callback-order/message comparison against current-source native
cases. The final current-source binding now passes host and VC1.52c native
74/74 checks using the exact high-offer SHDEF, player-1 race/technology, planet,
and part inputs. See the [v4 closure note](satisfy-orders-task5-v4-current-source-native.md),
[native report](../../runs/vmctl-soe2s/SOE2SRES.TXT), and [run manifest](../../runs/vmctl-soe2s/MANIFEST.JSON).
The decompiled original sets and clears `gd.grBits2` bit `0x0008` around
task-5 `UpdateShdefCost`; the reconstruction's previous `0x0800` test was
incorrect. `stars_get_true_part_cost` now checks bit `0x0008`, and the packed
task-5 path sets it during repricing and clears it on completion or failure.
The host/native result reports repriced costs 1930/494/681/261, an isolated
395/544/208 mineral return from planet input 469/893/376, original callback
order, `0x5A`/`0x13D`, player-0 technology advancement, and fleet retirement.
The original full-turn action/control delta is 396/543/209 and remains in the
separate SO-E3 surrounding mining/RNG comparison. **SO-E2 has zero closure
checks remaining; SO-E3 remains at one.**

## SO-E3: scoped four-pass input replay

`tests/satisfy_orders_task5_four_pass_probe.c` runs production `DoOrders`
pre- and post-movement and calls the packed task-5 path on passes 1–4. It
retains the six verified original fleet identities, positions, owners,
ship counts, five cargo fields, and orders in packed records. The source
fleet's task-5 order retires on pass 1;
the other five task-0 orders stay active. The planet's minerals and colonists
start from the verified original **turn-0 HST input** (469/893/376/250).
The verified original input
design costs 24 resources and 12/8/7 minerals for owner 0, as decoded in
the [design-cost oracle](satisfy-orders-design0-input-oracle.json). The
isolated starbase scrap returns 9/6/5 minerals, totaling 20 for the first
message. The original full-turn clean/action stockpile differences are
9/5/5. The one-boranium discrepancy is consistent with random mining
rounding; the retained control below shows one-unit variation without an
accepted scrap action.

`tests/test_satisfy_orders_task5_four_pass_delta.py` pins the original HST
input hash, decodes the action run's turn-1 owner fleets/orders and planet
stockpiles, and compares the production output. It passes for the four pass
numbers, fleet 0 retirement, five surviving fleet ID/owner/planet/position,
16 ship counts, five cargo fields, and compact order fields, plus the two
task-5 message IDs/arguments in original order. The
replay now invokes reconstructed `stars_produce` between `DoOrders(0)` and
`DoOrders(1)` for the verified owned planet with a null production queue.
That stage emits the original trailing `0x3F` queue-empty message with
arguments 10/10, so the host replay's complete message sequence is
**0x5A, 0x141, 0x3F**. Its mining callback now invokes reconstructed
`stars_est_minerals_mined` with the verified input's 10 operating mines,
10 mine efficiency, 68/87/68 concentrations, and zero initial mineral
levels. An explicit random-draw witness **0/99/0** makes the three
probabilistic rounding decisions **7/8/7**. The replay then matches the
original action output minerals **485/907/388** and levels
**247/241/247**, with concentrations unchanged. The random draws are
chosen to witness compatibility; the original process seed and draws have
not been recovered. The retained player-0 record has 15% ideal growth and
perfect 50/50/50 environment on planet 10. The original `raNone` capacity
bonus gives maximum population 12,000, so reconstructed
`stars_change_population` now grows **250→287** with fractional carry
**0→50**, matching the saved action record. The helper's previous growth
update applied hundredths as whole units and has been corrected and moved
to a standalone source file for native probing. `stars_produce` also now
implements the original null-queue branch for an owned planet; the prior
reconstruction omitted this message.
No failure branch is exercised by this input.

### Expanded original output audit (2026-09-24)

`tests/test_satisfy_orders_task5_four_pass_delta.py` now pins the player-1
portion of the same original task-0/task-5 control/action pair in addition to
the player-0 task-5 output. It checks the four turn-0 owner-1 fleets in the
backed-up input and both surviving turn-1 owner-1 fleet/order records in HST
and M2: IDs 0/1, all 16 ship-count slots, all five cargo channels, owner,
planet, point, saved flags, order count, and compact task/warp/target fields.
Planet 24's owner, mineral levels/concentrations, and environment are pinned
in both streams. The action and control agree on these owner-1 fields; M1's
full turn-1 message is pinned as `0x5A` for the action and `0x3F` for control.
The focused test passes 2/2.

This is an expanded original-output oracle only. The current-source four-pass
probe still models the player-0 fleet set and planet 10; it does not reproduce
the owner-1 movement/merge or all retained HST/M1/M2 fields. It also has no
representative failure-argument case. The mining draws remain an output
witness rather than a recovered original RNG stream. **SO-E3 remains one open
check.**

SO-E3 still needs original planet resource and race/service state
bound into the production replay, the full same-input
planet and ownership comparison through the surrounding turn services,
and representative failure-argument cases. The earlier native
four-pass packet in `compat/vc152/satisfy-orders-task5-e3-r2/` compiled and linked in
`C:\SOE3B`. Its [native report](../../runs/vmctl-soe3b/SOE3RES.TXT) matches
the earlier scoped host replay: passes 1–4, fleet 0 retired, five other fleets
active, first two message frames, and planet 485/908/388 from a clean
turn-1 output baseline. Build status,
compiler/linker logs, map, object, and executable are retained in
`runs/vmctl-soe3b/`. The current same-input replay has now compiled and linked
in `C:\SOE3G` and its [native report](../../runs/vmctl-soe3g/SOE3RES.TXT)
matches the strict-C89 host report exactly after CRLF normalization. The
[build status](../../runs/vmctl-soe3g/SYOSTA.TXT), compiler/linker logs,
map, object, executable, agent log, and guest screenshot are retained in
`runs/vmctl-soe3g/`. VC1.52c reported two integral-conversion warnings in
`ESTMIN.C` but no compile or link error. This is a scoped replay rather than
a complete original full-turn comparison.

The [same-input planet oracle](satisfy-orders-planet10-same-input-oracle.json)
now decodes the actual original HST planet-10 input through the recovered
variable-width `FReadPlanet` layout. Clean, M1-only, and HST-action runs have
the same planet record: minerals **469/893/376** and colonists **250**. The
clean turn-1 output is **476/902/383/287** and the action output is
**485/907/388/287** (minerals/colonists). Thus their input-relative deltas
are **7/9/7/37** and **16/14/12/37**. The isolated source-derived scrap
return **9/6/5/0** leaves a full-turn residual of **0/−1/0/0**. Both
outputs have identical mineral levels, while an auxiliary planet word is
8259 in clean and 8256 in action. This localizes the discrepancy without
assigning it to a specific surrounding service. The decoder test passes
2/2 and no original executable was run for this analysis.

The same packed records encode **10 mines, 10 factories, and 10 defenses**
for planet 10 in the input and both outputs. The original owner-0 player
record encodes mine efficiency **10** and mine operating limit **10**; with
250 colonists, all 10 mines can operate. The original mining arithmetic gives
floors **6/8/6** with probabilistic remainders **80/70/80**. The clean
full-turn gains are **7/9/7**, and the action gains after subtracting the
isolated scrap return are **7/8/7**. Both vectors fit the original rounding
rule, with boranium alone taking the lower branch in the action run.

The original turn pipeline runs
task-5 in `DoOrders(0)` before `Produce()`, whose first step is `MineMinerals()`.
`ITechLearnATech` can consume `Random(100)` even when it returns no technology;
`EstMineralsMined` independently uses `Random(100)` to round a nonzero mineral
remainder. An altered random stream is therefore a concrete candidate for
the one-boranium full-turn difference. This is **not yet a demonstrated
cause**: the actual pre-task-5 `fLearned` state and random seeds/draws have
not been replayed from this input. A retained M1-only run has the identical
clean HST input and **does not execute task-5 scrap**; its turn-1 planet
minerals are **475/902/383**, one ironium below the clean run. This is an
original-executable control demonstrating one-unit mineral variation without
accepted scrap action. It prevents treating the action-run boranium gap as
proof of a missing task-5 mutation. The decoded
auxiliary word is `uGuesses` in `FReadPlanet`; its difference is not a mineral
stockpile difference.

### Current-source control/action replay and failure frame (2026-09-24)

`tests/test_satisfy_orders_task5_four_pass_delta.py` now runs the current-source
production path from the same verified starting state twice: task-0 control and
task-5 action. It compares owner-0 task state, retirement, planet-10 minerals
and colonists, plus message IDs and arguments against the retained original
control/action outputs. The current-source control result is
**476/902/383/287**; action is **485/907/388/287**. Their distinct mining draw
vectors are explicit output witnesses, not recovered original seeds.

Both host reports now include a representative `SET_AMOUNT` failure frame
through `stars_satisfy_raw_failure_message`: recipient 2, fleet 11, resource 3,
requested amount 27, target word `0x0123`, and target coordinate 14. The saved
message is `0x0121` with argument words
`800b,000b,0003,001b,0000,000e,0123,0000`. Fresh VC1.52c packet
`compat/vc152/satisfy-orders-task5-e3-r10/` built in `C:\SOE3J`; returned
[`SOE3RES.TXT`](../../runs/vmctl-soe3j/SOE3RES.TXT) matches a strict-C89 host
build byte-for-byte after CRLF normalization. Its
[`MANIFEST.JSON`](../../runs/vmctl-soe3j/MANIFEST.JSON) records all nine returned
artifacts, hashes the input manifest, and records the two known C4135
integral-conversion warnings from `ESTMIN.C`.

The retained owner-1 original-output audit is now compared directly with a
current-source diagnostic. The control/action `CLEAN.M2` input backups are
byte-identical (SHA-256
`588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b`), and all
four owner-1 fleet/order pairs in M2 match the turn-0 HST snapshots. Original
task-0/task-5 outputs contain only owner-1 fleets 0 and 1: fleet 0 reaches
planet 26 and spends two fuel; fleet 1 carries 25 colonists. Planet 24's
complete type-13 body decodes identically in HST and M2. Control has minerals
**592/963/520/256** and `uGuesses=8257`; action has **591/964/520/256** and
`uGuesses=8255`, with matching owner, flags, mineral levels/concentrations,
environment, growth, and improvements. The action/control deltas are therefore
−1 ironium, +1 boranium, and −2 `uGuesses`; they are not assigned to the task-5
branch without the RNG state. The current-source task-order-only path leaves
owner-1 fleets 0–3 at planet 24 with unchanged cargo. This rules out a different
owner-1 M2 order log as the explanation, but does not attribute the remaining
full-turn changes to one service. This is a retained mismatch, not a full-turn
pass. Original RNG state/draws are still not recovered, and Stars! was not
launched during this native probe. **SO-E2 has zero closure checks left; SO-E3
remains one open check.**

The same input audit now pins owner 1's type-6 player record. Its word at
offset 6 is `0x2797` in control/action HST and M2 inputs. The recovered
`_player` bitfields decode this as `fAi=1`, `lvlAi=1`, and `idAi=1`. The
original `DoAiTurn` export at
`analysis/decompiler/export/raw/1088_0000_DoAiTurn.c` maps selector 1 to
`DoTurinDroneAiTurn`; reconstructed `stars_do_ai_turn` exposes this through
`personalities[selector]`. The retained records show owner-1 fleet 0's output
order targets planet 26 at warp 5, while its input order targets planet 24 at
warp 0. Binding the Turin stage before movement is now the next targeted step.
The current evidence does not prove that this AI call caused the order change.

The new [selector-routing probe](../../tests/test_do_ai_turn_source.py) passes
with player word `0x2797`: production `stars_do_ai_turn` sends player 1 to
selector-1's callback. That callback is a sentinel, not the Turin implementation;
the probe confirms routing but does not reproduce an AI order or movement.

The remaining production seam is now explicit. `stars_do_turin_drone_ai_turn`
delegates fleet planning through an optional `ops.plan_fleet_action`; no
concrete Turin fleet-planner adapter is present in `reconstructed/src/`. The
raw Turin export contains an `IdTargetScout` path followed by `FMoveAiFleet`,
but the current evidence does not establish that the retained owner-1 fleet 0
took that exact branch. The next source binding must connect selector 1 to the
typed Turin turn with packed fleet planning/order writes and movement, then
compare it against the verified original pair.

### Owner-1 AI file provenance audit (2026-09-24)

The attached screenshot shows the Stars! dialog `Player log file
C:\SOTC\CLEAN.X2 appears to be corrupt, unable to load file.` It is direct
evidence that this separate `C:\SOTC` candidate had an invalid player log; it
does not identify the state of the retained `C:\SYORIG1` or `C:\SYACT3`
runs. The associated [SOTC run manifest](../../runs/vmctl-sytech-control/MANIFEST.JSON)
records a 355-byte `CLEAN.X2` return at SHA-256
`17cf6d4b2dd559360247ff66c01430c0fb2801d879a1230866cf13919a7905e0`. The
guest pre-run listing was not retained, so the starting presence of that exact
returned file is unresolved. The host stream decoder can frame/decrypt its 13
records, but Stars!' rejection is the acceptance result; framing alone does
not certify a usable player log.

The raw `EnsureAis` path sets `fFileErrSilent=1` while checking each `.X<n>`
log, and raw `FileError` displays an alert only when that flag is zero. This
indicates the screenshot did not come from that silent probe; the screenshot
alone does not identify which operation displayed it.

The original source path makes `.X2` and `.H2` state material to the E3 AI
comparison. Raw `EnsureAis` calls `FOpenFile(1, player, 0x20)` and dispatches
`DoAiTurn` only when that open fails. `SetSzWorkFromDt` maps file type 1 to
`.X<n>` and type 4 to `.H<n>`. `DoAiTurn` loads `player+1` through
`FLoadGame`, then writes the player log and history file. The candidate bundle
manifest omits both `.X2` and `.H2`. The retained `AGENT.LOG` records `LIST`
commands before upload, after upload, and after the run, but does not preserve
the directory entries returned by those commands.

The return tree contains `BACKUP/CLEAN.X2` in both the control and action
folders (356 bytes, SHA-256
`fb56eb05a6d4a44b0c812242cbebc6b8a8826c1cdc07129dd1bdfa438664fd31`; and 361
bytes, SHA-256
`fc751635acf7fe9659e460bf7b5789c547335fb12d04a6fecfd8773915e43526`). Both
parse as 13 turn-0 records. Their timing and role as initial files or returned
backups are not established. The root `CLEAN.H2` files returned by these runs
are 95 bytes and differ as well: control SHA-256
`fdab9ff37560d5de45b310d33a20ca8bce18854210a00839a5a8fb92cf92afc8`, action
SHA-256 `bef799175a50f49fadcff1a352bfa9e6644cf2ffbe27862b47cb254cee3f7891`.
They are post-run returns and do not establish their starting state.
Therefore these original outputs remain supporting evidence; they do not yet
prove that the selector-1 AI ran from the same pinned `.X2`/`.H2` state. The
focused audit test pins this gap. SO-E3 stays at one remaining check;
**SO-E2 has zero.**

### Fresh isolated selector-1 original pair (2026-09-24)

The new [pair audit](../../runs/vmctl-sye3k-pair-r1/PAIR.JSON) and its
[verification test](../../tests/test_satisfy_orders_fresh_ai_pair.py) fix the
starting-file provenance for a clean original control/action pair. The two
guest directories (`C:\SYE3K0` and `C:\SYE3K1`) were empty before upload. Each
received exactly `CLEAN.HST`, `CLEAN.M1`, `CLEAN.M2`, `CLEAN.XY`, and
`STARS.EXE`; neither `.X2` nor `.H2` was present at launch. Their shared M1,
M2, XY, and executable hashes match. The only decoded HST input change is
record 45: a compact task-0 order (type 20) becomes the task-5 order (type 19).
Owner 1's player mode word is `0x2797` (`fAi=1`, `lvlAi=1`, `idAi=1`), and the
same four owner-1 fleet/order pairs occur in turn-0 HST and M2 inputs on both
sides.

Both original runs returned files with process result `EXIT 1`; outputs were
collected, and exit status is not used as a pass criterion. The post-run window
lists did not contain an error dialog. Each run generated a turn-0, 13-record
`BACKUP/CLEAN.X2` and a 95-byte `CLEAN.H2`, now hash-pinned in `PAIR.JSON`.
Original output has the same owner-1 fleet/order pairs in HST and M2 for the
control and action: fleet 0 moves from planet 24 to planet 26 at warp 5, and
fleet 1 carries 25 colonists. The turn-1 M1 message is `0x3F` in control and
`0x5A` in action. Planet 24 changes from control minerals
`[592,963,520,256]`/auxiliary word `8249` to action
`[592,964,520,256]`/`8248`. These outputs strengthen the original behavior
evidence but do not show that the reconstructed production path ran Turin AI.
The returned control/action `.H2` files have identical decrypted non-BOF
records, but they are post-run files and do not reveal the absent starting H2
or original random draws.

The attached `C:\SOTC` dialog belongs to the separate rejected-control bundle,
not these two clean runs. The host decoder parses its returned `CLEAN.X2` into
13 records, while original Stars! reports that file as corrupt. The fresh pair
records the exact pre-run absence and generated returns; the rejected SOTC log
is retained as a separate failure case. Original RNG state/draws and the
production Turin binding remain open, so **SO-E3 remains at one** and
**SO-E2 remains at zero**.

## Current native packets and returns

`tools/package_satisfy_orders_task5_current.py` staged fresh DOS 8.3,
CRLF, overwrite-guarded packets from the current `core.h`,
`satisfy_orders.c`, `do_orders.c`, and probes:

| Boundary | Host-validated packet | Guest directory and report |
|---|---|---|
| SO-E2 service order | `compat/vc152/satisfy-orders-task5-e2-r3/` | `C:\SOE2H\SOE2RES.TXT` |
| SO-E2 corrected resource path | `compat/vc152/satisfy-orders-task5-e2-resource-r6/` | `C:\SOE2G\SOE2GRES.TXT` |
| SO-E2 verified HST resource and `CResourcesAtPlanet` binding | `compat/vc152/satisfy-orders-task5-verified-resource-r1/` | `C:\SOE2L\SOE2LRES.TXT` |
| SO-E2 exact v4 high-offer live repricing | `compat/vc152/satisfy-orders-reprice-v4-e2-native-r1/` | `C:\SOE2S\SOE2SRES.TXT` |
| SO-E3 four passes, packed surviving fleets, mining, population, and production message | `compat/vc152/satisfy-orders-task5-e3-r7/` | `C:\SOE3G\SOE3RES.TXT` |
| SO-E3 current-source owner-0 control/action replay and raw failure frame | `compat/vc152/satisfy-orders-task5-e3-r10/` | `C:\SOE3J\SOE3RES.TXT` |

Each input packet has a SHA-256 manifest. Strict-C89 host compilation passes:
30/30, 75/75, and the thirteen-line scoped four-pass report (minerals
485/907/388, levels 247/241/247, colonists 287, fraction 50, three messages
in original order, explicit RNG witness). The user started STARSAGT in
StarDev; the host controller then reported `AGENTINFO STARSAGT 0.4.2`, `PONG`,
and guest windows. All three packets were transferred and built under
VC1.52c, with `COMPILE=PASS`, `LINK=PASS`, process exit 0, and the expected
native reports. Returned [SO-E2 service](../../runs/vmctl-soe2h/MANIFEST.JSON),
[SO-E2 resource](../../runs/vmctl-soe2g/MANIFEST.JSON), and
[SO-E3 four-pass](../../runs/vmctl-soe3g/MANIFEST.JSON) manifests pin the
logs, maps, objects, executables, and reports. The final guest window list
showed no Program Error dialog. The original Stars! executable was not run.
The additional [SO-E2 v4 native report](../../runs/vmctl-soe2s/SOE2SRES.TXT)
matches the strict-C89 host report at 74/74. Its [manifest](../../runs/vmctl-soe2s/MANIFEST.JSON)
pins the exact current-source packet and retained native artifacts; only the
standalone report executable ran in this step, not Stars!.
