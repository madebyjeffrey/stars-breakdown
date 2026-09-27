# SatisfyOrders reconstruction

## Current Phase-2 status — promoted 6/6 (2026-09-26)

SO-E1, SO-E2, and SO-E3 are closed for their documented acceptance boundaries.
The [Step-6 promotion audit](../compat/satisfy-orders-step6-promotion.md)
checks the six-part definition of done, records the retained native Win98
packets, and links the complete pinned control/action full-turn comparison.
All 274 turn-1 records and six HST/M1/M2 files match the corresponding
original outputs byte for byte in the host integration regression. That
comparison uses the two pinned inputs and recovered external clock residues;
it does not claim arbitrary-game or fresh native six-file coverage.

The older milestone sections below preserve their original checkpoint scope.
Statements there that SO-E2, SO-E3, or promotion remain open are historical.

## Current SO-E1 status — packed task-5 scrap accounting closed

The original HST task-5 order scraps fleet 0 on pass 1. The new opt-in packed
path reads the 124-byte fleet, 18-byte order, 147-byte SHDEF, and 60-byte
planet records; applies mineral return and friendly-colonist transfer; sends
the original 0x5A/0x141 message frames; and marks the fleet/order inactive.
The original clean/action output mineral values are
`476/902/383 → 485/907/388`, and the two serialized message frames match
byte for byte. Host and fresh Win98 VC1.52c probes pass 19/19. See
`analysis/compat/satisfy-orders-task5-e1.md` and `runs/vmctl-soe1f/`.

This closes SO-E1's packed accounting boundary. SO-E2 remains open for the
complete scrap service chain, deep-space salvage, race-specific resource and
technology effects, and broader special-order coverage. SO-E3 remains open
for the same-input four-pass original/production differential. Earlier E1/E2
closure language below is historical and was superseded by the task-5
original-executable correction.

## Raw transfer-failure ABI checkpoint

The production source now exposes `stars_satisfy_raw_failure_message` for the
remaining exact failure frames recovered in `turn3.c`. Set-amount and
set-colonists failures select `0x0121` and `0x0122` and preserve the complete
eight-word `FSendPlrMsg` frame. Deep-space colonist rejection selects `0x0165`
with `(fleet|0x8000, fleet, 0)`, while a futile mineral-packet transfer
selects `0x011e` with `(fleet|0x8000, fleet, thing_kind)`. The caller resolves
the packed action union and supplies the target/action fields through
`StarsSatisfyFailureFrame`; this avoids inventing state in the reduced typed
order record.

Host strict-C89 and fresh VC++ 1.52c/Win98 `C:\SYOE22` validation both report
`SATISFY_ORDERS_RAW_FAILURE_CHECKS=44 FAILURES=0 RESULT=PASS`, with compile/link
pass, process exit 0, and no Program Error dialog. This is a supporting E2
transport milestone, not named E2 closure: the caller conditions, full-turn
original differential, E3 replay, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-failure-e2-native.{md,json}` and
`runs/vmctl-vc152syoe22/`.

## Phase-2 E2 closure

The previously separate supporting packets now close the named E2 boundary.
The production-source/native evidence covers the real technology-table/result
binding, `itech_learn_a_tech` and salvage services, created-player fleet and
design materialization, special-order conditions, packed special dispatch, and
all recovered ordinary/player-transfer/route/mine/merge/shortage/colonist/
technology/fuel/failure message layouts. The corrected raw sweep passes
141/141 affected-order checks; the fuel and failure packets pass 40 and 44
exact-payload checks. The closure record is
`analysis/compat/satisfy-orders-e2-closure.{md,json}`.

This does not close the complete four-pass `DoOrders`/`SatisfyOrders` turn or
the original executable full-turn differential. Those remain E3, followed by
Step-6 ledger promotion.

## E3 full-turn integration support

### Original-instruction boundary audit

The retained original `DoOrders` export now has an explicit machine-readable
orchestrator oracle. It records the exact pre-/post-movement callback order,
inhabited-planet flag update, battle-id formula, post-movement planet-turn
clear, and `TransferToOthers` gate. The readable `stars_do_orders` source and
the fresh native 18-check fixture agree with that sequence. This strengthens
the original-instruction boundary, but does not claim the missing original
executable full-turn state/message differential; the named E3 unit remains
open. See `analysis/compat/satisfy-orders-full-turn-e3-original-oracle.{md,json}`.

The existing sequencing harness was expanded into a four-pass pre-/post-
movement state and message matrix bound through the real `DoOrders` and
`SatisfyOrders` production sources. It checks the exact pass order,
battle/thing callbacks, planet and fleet mutation, order retirement,
mine/route/player-transfer dispatch, and generic/player message order. The
strict-C89 host probe reports
`SATISFY_ORDERS_FULL_TURN_E3_CHECKS=18 FAILURES=0 RESULT=PASS`.

The fresh VC++ 1.52c/Win98 `C:\SYFTE4` replay reports the same 18/18 pass,
with compile/link PASS, process exit 0, and no Program Error dialog. Evidence
is in `analysis/compat/satisfy-orders-full-turn-e3-native.{md,json}`,
`compat/vc152/satisfy-orders-full-turn-e3-r2/`, and
`runs/vmctl-vc152syfte4/`.

This closes only the supporting integration subcheck. The original
executable's complete full-turn state/message differential and Step-6
promotion remain open; SatisfyOrders stays at **2/3 named Step-5 units closed
and 5/6 overall**.

`reconstructed/src/satisfy_orders.c` provides the typed resource-transfer core
of `SatisfyOrders` (NE `0017:6798–9eb8`).  It names the five supply channels,
models the phase mask and active order state, clamps each transfer to the
source availability and destination capacity, accounts for partial transfers,
queues colonist drops, emits the
original transfer message families and defers shortage notifications to pass
four, and retires completed orders. Packed resource modes 5 and 6 now derive
their requested amount from a typed fleet-stat callback with the original
2,000,000 cap, overflow-avoiding percent arithmetic, current-resource delta,
and even-pass gate. It
models destination capacity as a typed target callback for planet, fleet,
point, and thing orders before the receive callback is invoked. It
preserves the raw destination-kind restrictions that exclude the fuel
channel from planet and thing transfers, expose only mineral channels to
thing destinations, and allow fleet and point fuel transfers. It
also models the enemy-colonist destination gate: unowned planets require the
raw eligibility flag, race-stat value 8 rejects the drop, and the planet
blocked flag emits the corresponding rejection message; same-owner drops stay
eligible for the queue callback. It
also gives the mine, ownership-gated merge, battle-plan, mine-laying, auto-route, and
player-transfer order kinds explicit pass-gated dispatch callbacks and named
result messages; mine dispatch distinguishes no fleet planet, no mine source,
hostile planet, and successful mining outcomes, with success retaining the raw
no-message path. Same-owner starbase auto-routing preserves the raw two-way
success-message selection, and pass-four automatic routing has a distinct
`AutoFleetOrder` callback boundary. Player transfer now exposes the five raw
result states and preserves the source/recipient message pairs for ship-design
creation and no-design outcomes. Battle-plan resource accounting remains at
its typed callback boundary. The typed fleet state also models the original pass-1 turn-bit reset
and inactive/already-serviced fleet gate.

The source probes pass the phase gate, mine-capability gate, race-stat mine-laying gate,
mineral and colonist transfer, fleet-transfer and destination-shortage message variants
including the pass-four shortage notification gate,
enemy-colonist eligibility and rejection messages, typed merge target
existence/ownership gates,
capacity/availability accounting, partial-transfer retention, destination-kind
fuel gating, merge and mine
dispatch, order completion callbacks, the same-owner starbase auto-route path,
and the pass-gated battle-plan,
mine-laying, auto-route, and player-transfer dispatches, including the named
player-transfer result/message cases and the packed stat-percent resource
modes, under host C89.  The
per-fleet turn-state reset and inactive/already-serviced fleet gate are covered
as well.  Missing transfer and special-order callbacks leave the corresponding
order and source state untouched rather than fabricating a completed operation.
Successful mine-laying also preserves the raw repeat-count behavior: zero retires
the order, values other than five decrement, and five remains the indefinite
repeat marker.  The
remaining packed-order branches—battle-plan resource accounting, tech
discovery, fleet splitting/ship-design copying, salvage creation, and their
complete message argument layouts—remain open. The source is now also bound
to recovered packed `_fleet` (124-byte) and `_order` (18-byte) views. The
adapter decodes fleet identity/flags, order target/id, task/object nibbles,
and the validity bit, routes a mineral transfer through the typed resource
core, writes the validity and pass-1 turn-flag changes back to the raw views,
and checks inactive-fleet gating. Host and native reports both return
`SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`; the fresh Win98
run compiled and linked without a runtime error dialog. This advances the
procedure to Phase 2 step 5/6; the remaining special-order branches and full
turn integration remain open.

The packed bridge is now part of `reconstructed/src/satisfy_orders.c` rather
than being duplicated only inside the integration test. It validates the
recovered record sizes, decodes the raw fleet/order identity and kind fields,
invokes the typed service, and writes the fleet turn flags and order validity
bit back to the caller's records. The integration probe now exercises that
production bridge directly and still reports
`SATISFY_ORDERS_INTEGRATION_CHECKS=3 FAILURES=0 RESULT=PASS`. This remains
host/native bridge evidence, not closure of battle-plan accounting, technology
discovery, ship/design copying, salvage, full message layouts, or the full
turn/original differential.

A turn-sequencing probe now binds `SatisfyOrders` through the real `DoOrders`
orchestrator. Host and fresh VC++ 1.52c/Win98 runs both report
`SATISFY_ORDERS_TURN_CHECKS=8 FAILURES=0 RESULT=PASS`, covering pre- and
post-movement pass ordering, battle-id creation, battle and thing callbacks,
planet flag/turn mutation, fleet cargo transfer, and order retirement. The
process exited normally and no Program Error dialog appeared. This closes the
native turn-sequencing replay subcheck but no Step-5 unit; packed battle-plan
resource accounting, special-order integration, technology discovery,
fleet/ship-design copying, salvage creation, complete message layouts,
original behavior, full-turn differential, and promotion remain open.
Evidence is in `analysis/compat/satisfy-orders-turn-e3.{md,json}` and
`runs/vmctl-vc152syturn/`.

The special-order dispatch packet is now explicit on the host as well. The
strict-C89 probe reports `SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0
RESULT=PASS`, with separate pass markers for mine capability/repeat behavior
and the callback gates. It covers battle-plan pass 1, mine-laying capability
and race-stat fallback, same-owner starbase auto-route and no-path messaging,
merge target/ownership/callback rejection, and player-transfer source/recipient
message pairs. This remains host-only and closes **0** authoritative Step-5
units; native packed battle-plan/resource accounting, technology discovery,
fleet/ship-design copying, salvage creation, complete message layouts,
full-turn replay, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-special-e4.{md,json}`.

The same 16-case special-order matrix now also passes through the production
source in fresh VC++ 1.52c / Win98 directory `C:\SYOE4`:
`SATISFY_ORDERS_SPECIAL_CHECKS=16 FAILURES=0 RESULT=PASS`. Compile and link
passed, the process exited 0, and the final guest window inventory contained
no Program Error dialog. This upgrades battle-plan pass-1 dispatch, mine
capability/race gates and repeat counts, same-owner starbase auto-route,
merge ownership/callback gates, and player-transfer source/recipient message
pairs to native evidence. It remains a supporting milestone rather than a
named Step-5 closure because packed battle-plan/resource accounting,
technology discovery, fleet/ship-design copying, salvage creation, complete
message layouts, and the full-turn/original differential are still open.
Evidence is in
`analysis/compat/satisfy-orders-special-e4-native.{md,json}`,
`compat/vc152/satisfy-orders-special-native-e4/`, and
`runs/vmctl-vc152syoe4/`.

The production packed bridge was then extended with source-coordinate and
auto-route-message fields and exercised through raw 124-byte `_fleet` and
18-byte `_order` records in fresh `C:\SYOP5`. The native report is
`SATISFY_ORDERS_PACKED_SPECIAL_CHECKS=10 FAILURES=0 RESULT=PASS`; compile/link
passed, the process exited 0, and no Program Error dialog appeared. This
confirms packed battle-plan, mine, merge, pass-four auto-route, and
player-transfer creation/message dispatch. It remains supporting evidence,
not a named Step-5 closure, because technology discovery, fleet/ship-design
copying, salvage creation, complete message layouts, and the full-turn/
original differential remain open. Evidence is in
`analysis/compat/satisfy-orders-packed-special-e5-native.{md,json}`,
`compat/vc152/satisfy-orders-packed-special-e5/`, and
`runs/vmctl-vc152syop5/`.

## Technology-table/result binding E2 replay

The resource-resolution technology branch now exposes the signed result from
the production `stars_itech_learn_a_tech` implementation through
`stars_bind_satisfy_technology`. Point destinations select the recovered
message pair `0x013c`/`0x013d`; planet destinations select
`0x013e`/`0x013f`. Negative results map to trader-part IDs, positive results
map to the next battle-technology ID, and zero preserves the no-learning
result.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE11`
both report
`SATISFY_ORDERS_TECHNOLOGY_BINDING_E2_CHECKS=12 FAILURES=0 RESULT=PASS`.
The native wrapper records `EXIT=0`, compile/link passed, and the final guest
window inventory contained no Program Error dialog. The native packet uses
the normal `/O` setting. Its response file includes `SYO11.DEF`; this was
required for the linker to apply the Win16 stack/heap/stub directives and
eliminate the earlier `not enough space for arguments` dialog.

This closes the signed technology-result/message-code boundary as supporting
E2 evidence, not the named E2 unit. Complete raw message layouts, the
original transfer differential, and E3 full-turn/original differential remain
open. Evidence is in
`analysis/compat/satisfy-orders-technology-binding-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-technology-binding-e2-native/`, and
`runs/vmctl-vc152syoe11/`.

## Packed resource E1 production replay

The production packed bridge now has a focused raw-record replay for the
first remaining Step-5 acceptance boundary. Fresh host and VC++ 1.52c/Win98
validation in `C:\SYOE6` both report
`SATISFY_ORDERS_PACKED_RESOURCE_E1_CHECKS=21 FAILURES=0 RESULT=PASS`.
The matrix covers the 124-byte `_fleet` and 18-byte `_order` views, pass-1
turn-flag clearing, active-bit write-back, source availability and
destination-capacity limiting, exact target-kind resource restrictions,
stat-percent mode, transfer and shortage message arguments, and pass-1
battle-plan dispatch.

Compile/link and process exit passed, and the final guest window inventory
contained no Program Error dialog. This closes SatisfyOrders E1 packed
battle-plan/resource accounting. E2 technology discovery, fleet/ship-design
copying, salvage and complete message layouts, E3 full-turn/original
differential, and promotion remain open. Evidence is preserved in
`analysis/compat/satisfy-orders-packed-resource-e1-native.{md,json}`,
`compat/vc152/satisfy-orders-packed-resource-e1-native/`, and
`runs/vmctl-vc152syoe6/`.

## Advanced E2 service-boundary replay

The production source now exposes explicit callbacks for the advanced effects
that the raw routine performs around resource resolution: technology
discovery, created-transfer ship-copy completion, point-drop salvage handoff,
and richer ordinary/player-transfer message-layout tracing. A strict-C89 host
probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE7` both pass
`SATISFY_ORDERS_ADVANCED_E2_CHECKS=20 FAILURES=0 RESULT=PASS`. The replay
covers typed and packed dispatch, transfer totals, technology amount, salvage
mineral vectors, created-transfer recipient/message pairs, no-design
suppression, and packed technology-flag propagation. Compile/link and process
exit passed, and no Program Error dialog appeared.

This is supporting E2 evidence rather than closure of the named E2 unit. The
original technology tables/results, actual fleet and ship-design record copy,
concrete salvage object creation/placement, complete raw message argument
layouts, and the E3 full-turn/original differential remain open. Evidence is
in `analysis/compat/satisfy-orders-advanced-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-advanced-e2-native/`, and
`runs/vmctl-vc152syoe7/`.

## Concrete production-service E2 replay

The next replay binds the SatisfyOrders technology and point-drop callbacks to
the existing production services. The typed path invokes
`stars_itech_learn_a_tech` and checks the resulting research, event, and
result/goto state; its point-drop path invokes `stars_drop_salvage` and checks
the actual two-object mineral split, values, and coordinates. The packed path
repeats the technology service through the raw 124-byte `_fleet` and 18-byte
`_order` bridge. Host and fresh VC++ 1.52c/Win98 validation in `C:\SYOE8` both
report `SATISFY_ORDERS_PRODUCTION_E2_CHECKS=18 TYPED=12 PACKED=6 FAILURES=0
RESULT=PASS`; the native wrapper also records `EXIT=0`. Compile/link passed
and the final guest window inventory contained no Program Error dialog.

This is stronger supporting E2 evidence, not named E2 closure. Original
technology-table/result binding, actual fleet/ship-design copying, complete
raw message argument layouts, the original destruction-to-salvage caller
comparison, and E3 full-turn/original differential remain open. Evidence is
in `analysis/compat/satisfy-orders-production-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-production-e2-native/`, and
`runs/vmctl-vc152syoe8/`.

## Concrete fleet/design copy E2 replay

The created player-transfer branch now invokes a separate fleet-copy service
boundary. The fresh host and VC++ 1.52c/Win98 packet in `C:\SYOE9` call the
production `stars_find_similar_design` helper for matching and no-match
selection, then call `stars_duplicate_fleet` from `common.c` and verify the
complete fixed 124-byte fleet record plus the allocated variable tail of
18-byte order records. Both report
`SATISFY_ORDERS_FLEET_COPY_E2_CHECKS=10 FAILURES=0 RESULT=PASS`; the native
wrapper records `EXIT=0`, compile/link passed, and no Program Error dialog
appeared.

This closes the design-selection helper and a concrete fleet-state-copy
boundary but remains supporting E2 evidence, not named E2 closure.
Player-specific ship-design record materialization/copying, original
technology-table/result binding, complete raw message layouts, original
transfer differential, and E3 full-turn/original differential remain open.
Evidence is in
`analysis/compat/satisfy-orders-fleet-copy-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-fleet-copy-e2-native/`, and
`runs/vmctl-vc152syoe9/`.

## Player-design materialization E2 replay

The created player-transfer branch now invokes a separate production
`materialize_designs` boundary. Its helper reconstructs the original
`IshFindSimilarDesign`/record-copy sequence: equivalent active designs are
reused, unmatched designs take the first hidden destination slot, the complete
0x93-byte record is copied, the destination identity flags are rewritten, the
two accumulated design totals are cleared, and the source fleet's per-design
count/status plus both 32-bit totals are applied. The source record remains
unchanged.

The host and fresh VC++ 1.52c/Win98 packet in `C:\SYOE10` report
`SATISFY_ORDERS_DESIGN_MATERIALIZATION_E2_CHECKS=15 FAILURES=0 RESULT=PASS`;
the native wrapper records `EXIT=0`, compile/link passed, and no Program Error
dialog appeared. The native packet uses the production `common.c` and
`satisfy_orders.c`; `/O` is used only to keep the combined packet within the
VC++ 1.52c code-segment limit.

This closes the player-specific design-record materialization/copying boundary
as supporting E2 evidence, not named E2 closure. Original technology-table/
result binding, complete raw message layouts, original transfer differential,
and E3 full-turn/original differential remain open. Evidence is in
`analysis/compat/satisfy-orders-design-materialization-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-design-materialization-e2-native/`, and
`runs/vmctl-vc152syoe10/`.

## Raw player-transfer message E2 replay

The production source now exposes `raw_message2` for the original
three-payload `FSendPlrMsg2` ABI. The caller supplies the resolved `WFromLpfl`
words and, for a newly created destination, the new fleet identity. The host
and fresh VC++ 1.52c/Win98 `C:\SYOE12` replay both pass
`SATISFY_ORDERS_RAW_TRANSFER_CHECKS=21 FAILURES=0 RESULT=PASS`.

The matrix checks exact recipient, code, and payload triples for created
source/recipient (`0x014d`/`0x014e`), no-design source/recipient
(`0x014a`/`0x014b`), no-ship (`0x0149`), blocked (`0x014c`), and invalid-target
(`0x0148`) outcomes. The native build and link pass, the process exits 0, and
the final guest window inventory contains no Program Error dialog.

This is a supporting E2 boundary rather than closure of the named E2 unit.
Ordinary eight-payload `FSendPlrMsg` layouts, the remaining mine/shortage/
route/merge/colonist/technology message families, the original transfer
differential, and the E3 full-turn/original differential remain open. Evidence
is in `analysis/compat/satisfy-orders-raw-transfer-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-transfer-e2-native/`, and
`runs/vmctl-vc152syoe12/`.

## Raw auto-route message E2 replay

The production source now exposes `raw_message` for the original eight-payload
`FSendPlrMsg` auto-route messages. The typed fleet view carries raw `idPlanet`
and the order view preserves the starbase word used by the original
`(lStarbase & 0x3ff) - 1` payload calculation. Host and fresh VC++ 1.52c/Win98
validation in `C:\SYOE13` both pass
`SATISFY_ORDERS_RAW_ROUTE_CHECKS=20 FAILURES=0 RESULT=PASS`.

The matrix checks success `0x0127` and no-path/fuel `0x0128`, the recipient,
all eight payloads, identity markers, source planet, derived starbase index,
and trailing zero fields. Compile/link and process exit pass, and the final
guest window inventory contains no Program Error dialog.

This is supporting E2 evidence only. Ordinary resource-transfer layouts, the
remaining special message families, the original transfer differential, and
the E3 full-turn/original differential remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-route-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-route-e2-native/`, and
`runs/vmctl-vc152syoe13/`.

## Raw shortage message E2 replay

The production source now binds pass-four resource shortages to the original
three-payload `FSendPlrMsg2` ABI. Planet, fleet, and point destinations emit
`0x011f`, `0x0120`, and `0x0123` respectively. Every case uses the source
fleet owner as recipient and carries `fleet.id | 0x8000`, the raw fleet id,
and the resource index. The host and fresh VC++ 1.52c/Win98 `C:\SYOE16` replay
both pass `SATISFY_ORDERS_RAW_SHORTAGE_CHECKS=12 FAILURES=0 RESULT=PASS`;
compile/link pass, the process exits 0, and no Program Error dialog remains.
This is supporting E2 evidence; successful transfer layouts, the remaining
message families, original differential, E3 comparison, and promotion remain
open. Evidence is in
`analysis/compat/satisfy-orders-raw-shortage-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-shortage-e2-native/`, and
`runs/vmctl-vc152syoe16/`.

## Raw colonist rejection message E2 replay

The production source now binds the unowned, race-specific, and
planet-blocked colonist-drop rejections to the original three-payload
`FSendPlrMsg2` ABI: `0x0055`, `0x0056`, and `0x0135`. Each uses the source fleet
owner as recipient and carries `fleet.id | 0x8000`, the raw fleet id, and the
target planet id. The host and fresh VC++ 1.52c/Win98 `C:\SYOE17` replay both
pass `SATISFY_ORDERS_RAW_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/
link pass, the process exits 0, and no Program Error dialog remains. This is
supporting E2 evidence; successful transfer layouts, technology messages,
original differential, E3 comparison, and promotion remain open. Evidence is
in `analysis/compat/satisfy-orders-raw-colonist-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-colonist-e2-native/`, and
`runs/vmctl-vc152syoe17/`.

## Raw resource-transfer message E2 replay

The production source now preserves the standard successful `FSendPlrMsg`
frames for planet mineral/colonist (`0x002d`/`0x002e`) and same-owner fleet
mineral/colonist (`0x002b`/`0x002c`) transfers. Caller-resolved target word and
x coordinate fields retain the full eight-word frame: source marker, fleet id,
accepted amount, packed target, resource, x, raw target id, and zero. The host
and fresh VC++ 1.52c/Win98 `C:\SYOE19` replay both pass
`SATISFY_ORDERS_RAW_RESOURCE_CHECKS=24 FAILURES=0 RESULT=PASS`; compile/link
pass, the process exits 0, and no Program Error dialog remains. One non-fatal
C4018 warning is preserved in the native compile log. Foreign-fleet colonist,
fuel/optimal-fuel, permission/failure families, original differential, E3
comparison, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-resource-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-resource-e2-native/`, and
`runs/vmctl-vc152syoe19/`.

## Raw technology frame E2 replay

The production source now preserves the caller-resolved recipient and
eight-word `FSendPlrMsg` frame through signed technology-result binding and the
raw message callback. The host and fresh VC++ 1.52c/Win98 `C:\SYOE18` replay
both pass `SATISFY_ORDERS_RAW_TECHNOLOGY_CHECKS=16 FAILURES=0 RESULT=PASS`;
compile/link pass, the process exits 0, and no Program Error dialog remains.
The matrix checks negative point code `0x013c`, positive planet code `0x013f`,
recipient, all eight forwarded words, result id, amount, and order completion.
This is a raw transport boundary backed by the existing production technology
binding; complete derivation of every original payload field, successful
ordinary-transfer layouts, original differential, E3 comparison, and
promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-technology-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-technology-e2-native/`, and
`runs/vmctl-vc152syoe18/`.

### Raw technology duplicate-frame correction

The ordinary successful-resource raw callback is now guarded by
`technology_flag`. This preserves the existing typed transfer notification
while preventing a duplicate raw `FSendPlrMsg` frame when the technology
branch emits its dedicated signed-result frame. The host regression suite
passes all nine raw-order probes (141/141 checks), and a fresh Win98 rebuild
of `C:\SYOE18` passes the 16-check technology probe with compile/link pass,
exit 0, and no Program Error dialog. Evidence is in
`runs/vmctl-vc152syoe18-r2/`.

## Raw foreign-fleet colonist rejection E2 replay

The production source now preserves the original foreign-fleet colonist
permission branch. A foreign target fleet emits `0x0155` through the exact
three-payload `FSendPlrMsg2` boundary with the source fleet marker, source
fleet id, and source current-planet id; it performs no cargo transfer and
cancels the order. Host and fresh Win98 `C:\SYOE20` validation both pass the
12-check probe with compile/link pass, exit 0, and no Program Error dialog.
This is supporting E2 evidence; fuel/optimal-fuel and remaining permission or
failure families, original differential, E3 comparison, and promotion remain
open. Evidence is in
`analysis/compat/satisfy-orders-raw-foreign-colonist-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-foreign-colonist-e2-native/`, and
`runs/vmctl-vc152syoe20/`.

The first packet used an incorrect message-enum transcription (`0x0119`).
The recovered enum identifies `idmAllowedTransferColonistsAnotherPlayer` as
`0x0155`; `0x0119` is `idmHasStolen`. The production constant and native
packet were corrected, and the fresh `runs/vmctl-vc152syoe20-r2/` replay passes
the same 12-check matrix with clean compile/link, exit 0, and no dialog. The
original run remains preserved as superseded evidence.

## Raw fuel and optimal-fuel E2 replay

The production source now exposes the caller-resolved fuel ABI boundary for
the route engine. Signed optimal-fuel adjustments emit the original loaded or
unloaded eight-word frame, while failed-load `0x0126`, capacity `0x003d`, and
insufficient-fuel `0x003c` preserve their distinct payload layouts. Host and
fresh Win98 `C:\SYOE21` validation both pass the 40-check matrix with
compile/link pass, exit 0, and no Program Error dialog. This is supporting E2
evidence; upstream route-engine fuel calculation/pass gating, original
differential, E3 comparison, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-fuel-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-fuel-e2-native/`, and
`runs/vmctl-vc152syoe21/`.

## Raw merge message E2 replay

The production source now binds merge outcomes to the original three-payload
`FSendPlrMsg2` ABI. A same-owner success emits `0x00f7` with the destination
fleet marker, the source `WFromLpfl` word, and the destination id. An enemy
destination emits `0x00f6`; an absent or inactive destination emits `0x00f5`.
The two rejection forms carry the source fleet marker, source id, and zero, and
all cases use the source fleet owner as recipient.

The host and fresh VC++ 1.52c/Win98 `C:\SYOE15` replay both pass
`SATISFY_ORDERS_RAW_MERGE_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link pass,
the process exits 0, and no Program Error dialog remains. The exact layouts
agree with the recovered `SatisfyOrders` decompilation and the reviewed
`turn3.c` source. This remains supporting E2 evidence; ordinary transfer
layouts, the remaining message families, the original transfer differential,
E3 comparison, and promotion remain open. Evidence is in
`analysis/compat/satisfy-orders-raw-merge-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-merge-e2-native/`, and
`runs/vmctl-vc152syoe15/`.

## Raw mining message E2 replay

The production source now binds mining failure outcomes to the original
three-payload `FSendPlrMsg2` ABI: no-planet `0x0077`, empty-source `0x0075`,
and hostile-planet `0x0076`. The exact `0x8000` fleet marker, fleet id, and
optional target planet id are preserved. Host and fresh VC++ 1.52c/Win98
validation in `C:\SYOE14` both pass
`SATISFY_ORDERS_RAW_MINE_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link pass,
the process exits 0, and no Program Error dialog remains.

This is supporting E2 evidence only. Ordinary resource-transfer layouts, the
remaining shortage/merge/colonist/technology families, the original transfer
differential, and the E3 full-turn/original differential remain open. Evidence
is in `analysis/compat/satisfy-orders-raw-mine-e2-native.{md,json}`,
`compat/vc152/satisfy-orders-raw-mine-e2-native/`, and
`runs/vmctl-vc152syoe14/`.
