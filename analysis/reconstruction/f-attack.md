# FAttack reconstruction (partial)

## Complex-beam live hull damage E60

The E55 residual path now has a live-token case that proceeds past shield-only
damage into the hull layer. Its strict-C89 probe verifies shield absorption,
two hull casualties, live ship-count mutation, residual beam consumption, and
the FAttack hit/first-target record. Fresh Win98 VC++ 1.52c reports pass
15/15 raw damage, 24/24 packed part, and 17/17 torpedo dispatch checks; the
full host suite also passes. This is supporting evidence, not a named Step-5
closure. See `analysis/compat/f-attack-complex-beam-hull-e60.{md,json}` and
`runs/vmctl-fapraw-e60/`.

## FLookupPart original-segment record binding E47

`stars_lookup_part_record` now checks the current-CS marker, family
base/stride, selected offset, and full record extent before mapping the address
into a caller-provided segment byte span. The strict host-C89 probe obtains
original unrelocated NE segment-2 bytes from `target/stars.exe`, verifies all
16 family addresses, and matches range, damage, and initiative words for all
24 beam and 12 torpedo records. The 129-check probe and updated packed-FAttack
test pass. This binds the calculation to original static bytes on the host; it
does not establish the live game CS mapping or finish FAttack integration.
Fresh VC++ 1.52c/Win98 E50 builds and passes the same 129 checks; the returned
segment fixture is byte-identical to the exported original. No Step-5 unit is
closed. See
`analysis/compat/f-attack-original-segment-binding-e47.{md,json}`.

## Previous checkpoint — FLookupPart current-CS table pointer model E46

The raw assembly for all sixteen table families computes the record offset
from literal base/stride immediates and executes `MOV DX,CS` to store the far
pointer segment. `FLookupPart` is in NE `02:524E`, so these are CS-relative
tables in NE segment 2; the `0x10e0` selector is the Ghidra synthetic address
for NE29 and is not used for the table pointer. Same-offset DGROUP CodeView
names were overlays. The typed lookup now models the current-CS flag for all
groups and checks all family bases, byte strides, and item offsets. ZIP's
stride is `0x36`. The strict-C89 and fresh native VC++ 1.52c table probes pass
66 checks; the separate race/visibility probe remains green at 8 checks.
The actual caller CS/table-memory view, complete `TechStatus`/hidden/race
service binding, attack acceptance, original differential, and replay remain
open; this closes zero named Step-5 units. Evidence is in
`analysis/compat/f-attack-current-cs-tables-e46.{md,json}` and
`runs/vmctl-faptae48/`.

`reconstructed/src/f_attack.c` transfers the section-matching and
target/damage-dispatch layer of `FAttack` (NE `001f:69ac–7cdd`) into typed C.
It preserves attacker/target player-mask filtering, active-section and
initiative matching, including the attacker-base plus part-delta initiative
calculation and 63-point clamp, with raw unsigned wrap-before-cap behavior for
a negative part delta, the two packed target-mode masks (high nibble followed by
low nibble during the two selection passes), single- and dual-target-mode
masks, raw `DzFromBrcBrc` coordinate-range eligibility when the typed part
range is supplied, battle-rating fallback eligibility, beam-defense scaling for both non-torpedo
beam paths, cargo/fuel damage
adjustment, first-hit recording, and hit-count
return behavior.  The typed section distinguishes the raw simple beam path,
which broadcasts one damage event to every eligible token, from the complex
beam path, which selects one eligible best-damage target per effective pass.
Torpedo sections use the reconstructed `CTorpHit` percentage model and rank
eligible targets deterministically by raw resource value per expected
shield/hull torpedoes before consuming hit RNG for the selected target. They
preserve the raw two-pass default and expose the original shield-then-hull
damage passes through the typed damage callback. The shield
hit count is gated by the damage callback, while the raw hull dispatch advances
the hit count after the callback regardless of its return value.  When supplied, the source also updates the raw
six-byte battle-record view's hit-count field and first-hit token field while
leaving token, destination, round, and distance fields to the coordinator.

The source probe passes normal filtering, dual target-mode selection, broadcast
and best-target beam paths, beam scaling, cargo adjustment, inactive-target
paths, coordinate-range gating, torpedo-hit calculation and two-pass
shield/hull dispatch under strict host C89. The E16 target probe additionally
checks deterministic torpedo ranking, selected-target-only RNG, and battle
record mutation. The source is now also exercised through the recovered packed
`_tok` (29-byte) and `_hul` (123-byte) views. The integration adapter decodes
the raw token identity, owner, active/target words, shield points, ship count,
and hull DP; it routes an attack through `FDamageTok`, writes changed shield
and count fields back to the raw token, and checks the six-byte battle
record's hit and first-target fields. A direct shield-only call checks the
residual shield layer while preserving the raw hull DP. Host and native
reports both return `F_ATTACK_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`,
and the fresh Win98 run compiled and linked without a runtime error dialog.

This advances the procedure to Phase 2 step 5/6. Complete packed part lookup,
raw torpedo shield/hull splitting, broader original damage differentials, and
full battle replay remain open before promotion.

The E3 battle-chain probe also exercises `FAttack` as the real attack service
inside `FDoCoolBattle`, rather than as an isolated callback. Host and fresh
VC++ 1.52c/Win98 runs both pass its 18 checks covering attacker/target
identity, damage amount, token mutation, event ordering, survivor-mask
termination, and report/cleanup lifecycle; the process exits normally with no
Program Error dialog. Native packed-part lookup, complete scaling, original
differential, full replay, and promotion remain open. Evidence is in
`analysis/compat/f-do-cool-battle-attack-integration-e3.{md,json}` and
`runs/vmctl-vc152cbatp/`.

The production battle-chain packet found one signed/unsigned compiler warning
at the section-index comparison. The readable source now makes that boundary
explicit with an unsigned 16-bit cast; the refreshed VC++ 1.52c build in
`runs/vmctl-vc152cbatp/COOLACL-final2.LOG` is clean and
`CBATRES-final2.TXT` still passes all 18 integration checks. This does not
close the remaining packed lookup, scaling, differential, replay, or
promotion units.

The source boundary also covers the raw initiative underflow case: `FAttack`
uses an unsigned 16-bit temporary for `initBase + part.dp` and caps only
values above 63. The host probe records the `0 + (-1) -> 0xffff -> 63`
boundary in `analysis/compat/f-attack-init-wrap-e4.{md,json}`. This is a
source-fidelity correction only; it does not close the remaining packed-part,
scaling, replay, or promotion evidence units.

The packed `_hs` component boundary is now explicit as a separate host
checkpoint. An optional resolver receives the raw group/item/count fields for
non-empty `0x10`/`0x20` slots before initiative matching, fills a local
resolved section, and preserves the caller's section. Its host adapter invokes
the existing reconstructed `stars_lookup_part` gate and checks the resolved
component identity. The strict probe passes
`F_ATTACK_PACKED_PART_CHECKS=8 FAILURES=0 RESULT=PASS` across successful
lookup/dispatch, unsupported-item rejection, and zero-count suppression. This
does not yet close the Step-5 part-lookup unit because native packed table
storage/service binding and replay remain open; see
`analysis/compat/f-attack-packed-part-e5.{md,json}`.

The next packed-slot checkpoint makes the storage boundary authoritative inside
the readable FAttack path. The optional `_hs` array uses the exact four-byte
layout (`grhst` word at `0`, item byte at `2`, count byte at `3`) and overrides
contradictory duplicated section fields. The strict probe now passes
`F_ATTACK_PACKED_PART_CHECKS=10 FAILURES=0 RESULT=PASS`, including raw-slot
forwarding, component identity `0x00100007`, dispatch, unsupported-item
rejection, zero-count suppression, and caller-section preservation. This is
host-only and closes **0** Step-5 units; native `FLookupPart` table/service
binding, complete battle-part scaling, broader original damage differentials,
native replay, and full battle replay remain open. Evidence is in
`analysis/compat/f-attack-packed-slot-e6.{md,json}`.

The following table-address checkpoint exposes the fixed native-facing metadata
from `FLookupPart`. For packed group `0x0010`, item `7`, the host path now
reports segment `0x10e0`, base `0x2450`, stride `0x003c`, and item offset
`0x25f4` while preserving the raw-slot gate. The combined packed probe passes
`F_ATTACK_PACKED_PART_CHECKS=13 FAILURES=0 RESULT=PASS`, and the existing lookup
source probe remains green at `LOOKUP_PART_CHECKS=8`. This remains host-only and
closes **0** Step-5 units: native segment/address binding, resource-relative
families, complete battle-part scaling, broader damage differentials, and
replay remain open. Evidence is in
`analysis/compat/f-attack-part-table-e7.{md,json}`.

The fixed-table-family checkpoint broadens that contract across all eleven
known DGROUP/far-table families: hull, engine, scanner, weapon, shield, bomb,
mine, mech, fuel, battle, and component. The strict-C89 probe checks each
family's segment, base, stride, item offset, and component identity, and uses
special group `0x0100` as an explicit resource-relative control whose metadata
remains zero. It passes
`LOOKUP_PART_TABLE_CHECKS=34 FAILURES=0 RESULT=PASS`. This is still host-only
and closes **0** authoritative Step-5 units; native table/service binding,
resource-relative families, battle-part scaling, broader damage differentials,
and full battle replay remain open. Evidence is in
`analysis/compat/f-attack-part-table-families-e8.{md,json}`.

The native production packet closes the native lookup/resolver subcheck and
passes the current 34-check typed scaling matrix in VC++ 1.52c. It does not
yet close E1: the raw complex-beam path still computes target hull/resource,
shield, jam, and capacity formulas that are not represented by the current
typed fields. Complete raw battle-part scaling, broader original damage
differentials, full native battle replay, and promotion remain open.
Evidence is in `analysis/compat/f-attack-packed-native-e9.{md,json}` and
`runs/vmctl-vc152fapne9/`.

The E10 checkpoint adds a first typed source boundary for the raw complex-beam
target-score branch. `stars_f_attack_complex_target_score` covers resource plus
ironium valuation, ship-count multiplication, the 100x/saturation boundary,
beam-defense reduction, shield/hull capacity, minimum hull handling, and the
weapon-flag shield-only path. The strict host probe and fresh VC++ 1.52c
packet both pass all 8 checks; the native report records the corrected
minimum-hull result of `18181`, and the post-run Win98 inventory contains no
Program Error dialog. This is supporting evidence only and closes 0 named
Step-5 units: the raw post-selection dispatch, remaining hull/resource/
shield/jam/capacity interactions, broader original differential, full replay,
and promotion remain. Evidence is in
`analysis/compat/f-attack-complex-score-e10.{md,json}` and
`runs/vmctl-vc152fapne10/`.

The E11 checkpoint extends that boundary with the raw packed `_tok.dv` word.
The readable token view now preserves its low-seven-bit `pctSh` and upper
nine-bit `pctDp` components, and the target-score helper applies the original
truncating hull-reduction sequence before the minimum-hull clamp. The strict
host probe and fresh `C:\FAPNE11` VC++ 1.52c packet pass 9 checks with clean
compile/link status and no Program Error dialog. This closes 0 named Step-5
units: complete complex target-selection integration, weapon-part
scaling/capacity/jam branches, post-selection dispatch, original differential,
full replay, and promotion remain. Evidence is in
`analysis/compat/f-attack-defense-modifier-e11.{md,json}` and
`runs/vmctl-vc152fapne11/`.

The E12 checkpoint connects the raw target-score helper to the readable
`stars_f_attack` complex-beam candidate loop. An explicit nonzero
`raw_scaling` section field selects score-based ranking, while the selected
candidate still reaches the damage callback with ordinary beam damage. Host
and fresh `C:\FAPNE12` VC++ 1.52c probes pass 3 checks with clean compile/link
status and no Program Error dialog. This closes 0 named Step-5 units because
part-dependent scaling, capacity/jam behavior, distance and shield/hull
dispatch, original differential, replay, and promotion remain. Evidence is
in `analysis/compat/f-attack-complex-selection-e12.{md,json}` and
`runs/vmctl-vc152fapne12/`.

The E13 checkpoint extends the integrated raw complex-beam path through the
first post-selection damage scaling boundary. For `raw_scaling` sections, the
typed source applies attacker capacity, target beam defense, and truncated
distance/part-DP attenuation in the raw order before invoking the damage
callback. The host and fresh `C:\FAPNE13` VC++ 1.52c probes pass 3 checks with
clean compile/link status and no Program Error dialog. This closes 0 named
Step-5 units: remaining part-dependent scaling, capacity/jam branches,
shield/hull follow-up dispatch, complex torpedo selection, original
differential, replay, and promotion remain. Evidence is in
`analysis/compat/f-attack-beam-dispatch-e13.{md,json}` and
`runs/vmctl-vc152fapne13/`.

The E14 checkpoint adds an optional raw damage callback to the readable
FAttack operations boundary. Raw beam dispatch now passes its selected amount
by reference to a stateful callback when supplied, allowing the existing
`FDamageTok` reconstruction to consume shield points, apply residual hull
damage, mutate ship count, and preserve the two-record order. The typed
callback remains the fallback for non-raw sections. The strict host probe and
fresh `C:\FAPNE14` VC++ 1.52c packet pass 4 checks with clean compile/link
status and no Program Error dialog. This closes 0 named Step-5 units: full
raw part scaling, capacity/jam formulas, complex torpedo selection, original
differential, replay, and promotion remain. Evidence is in
`analysis/compat/f-attack-raw-damage-e14.{md,json}` and
`runs/vmctl-vc152fapne14/`.

The E15 host checkpoint composes that stateful callback with the packed
attacker-part boundary. The strict C89 probe reports
`F_ATTACK_RAW_DAMAGE_CHECKS=7 FAILURES=0 RESULT=PASS`, covering exact `_hs`
group/item/count forwarding, local resolved-section ownership, raw capacity /
beam-defense / distance attenuation, `FDamageTok` shield-then-hull records,
casualty mutation, and the FAttack hit/battle-record fields. A fresh native
replay is prepared in `compat/vc152/f-attack-complex-native-e15/` for
`C:\FAPNE15`; it is pending the Win98 agent reconnect. This remains supporting
evidence only: the broader original damage differential, complete replay, and
promotion remain open. Evidence is in
`analysis/compat/f-attack-packed-raw-damage-e15.{md,json}`.

The E16 checkpoint corrects raw torpedo target selection and RNG order. The
original candidate path at `analysis/decompiler/export/raw/10f0_69ac_FAttack.c`
computes resource value and effective hull/shield capacity, ranks targets by
value per expected torpedoes, and calls `CTorpHit` only after choosing a
target. The typed code now preserves those truncating score calculations and
consumes random draws only for the selected candidate. Its strict-C89 probe
passes 5 checks; the existing FAttack source, score, selection, packed-part,
and raw-damage probes pass 34/9/3/13/7 checks. The full suite passes 519 tests
with 153 expected skips. This is host support only and closes 0 named Step-5
units. A fresh 42-file native packet for `C:\FAPNE16` is prepared with all 41
text files ASCII/CRLF; native execution is pending agent reconnection. Raw
torpedo shield/hull splitting, residual-torpedo behavior, broader original
differentials, full battle replay, and promotion remain open. Evidence is in
`analysis/compat/f-attack-torpedo-target-e16.{md,json}`.

The E17 callback bridge now forwards the raw `FDamageTok` torpedo-call
parameters (mutable beam value, torpedo damage, `grfWeapon`, shields-only
gate, and packet count) for shield and hull calls. This is not yet full call
fidelity: the packed-part resolver does not derive `grfWeapon` from weapon/
target state, and the split/residual loop remains open. See
`analysis/compat/f-attack-raw-torpedo-dispatch-e17.md`.

E18 derives the torpedo `grfWeapon` class from the packed weapon item: class 4
normally, class 12 for item indices 8–11, plus `0x80` for shield-only calls.
Items 7 and 8 are covered by the strict-C89 host probe. Beam class derivation,
the damage partition, and residual-packet loop remain open; see
`analysis/compat/f-attack-torpedo-damage-class-e18.md`.

E19 now passes the launched torpedo budget, rather than the successful-hit
count, through `pcTorp`; a two-hit/three-packet case verifies the distinction.
The raw split terms and post-damage same-target residual loop are still not
modeled. See `analysis/compat/f-attack-torpedo-packet-budget-e19.md`.

The E20 source map traces the raw allocation scan, its `/8` shield and `/2`
hull damage terms, the mutable `FDamageTok` calls, and both residual-loop
conditions. The readable implementation still lacks this allocation and
same-target state feedback; `lVar19`/`uStack_26` remains unresolved rather than
being assigned a speculative typed meaning. See
`analysis/compat/f-attack-torpedo-split-analysis-e20.md`.

E21 implements the split scan and its integer rounding, with nine strict-C89
dispatch cases across fallback, capacity-scan, packed item classes, and the
special no-shield power rule. The full suite passes 519 tests with 153
expected skips. The residual loop and actual `wtFuelMax` part resolution
remain open. See
`analysis/compat/f-attack-torpedo-allocation-e21.md`.

E39 adds the production raw `_hs` slot decoder. It explicitly reads the
little-endian group word plus item/count bytes from each four-byte packed
record, validates source stride and destination capacity, and binds converted
slots into `StarsAttackInput` without type-punning. The strict-C89 packed-part
probe now begins with raw bytes, proves those bytes override contradictory
typed section metadata, and passes invalid-item, empty-slot, short-stride, and
undersized-destination cases (16/16). Fresh VC++ 1.52c/Win98 packed-part and
surrounding attack probes pass. This closes no named Step-5 unit: the complete
native part lookup/race/visibility services, design-to-hull/target bridge,
raw damage-service acceptance, original differential, and full battle replay
remain. Evidence is in
`analysis/compat/f-attack-raw-hs-slot-bridge-e39.{md,json}` and
`runs/vmctl-vc152fapne39/`.

E40 composes the battle-token context adapter with the raw `_hs` decoder in a
single FAttack callback boundary. The FDoCoolBattle integration probe proves
that group/item/count values decoded from the callback-provided hull bytes
override conflicting typed section values, reach part lookup, and retain the
existing target-damage and battle-lifecycle behavior. Host and fresh Win98
reports pass 33/33. This does not resolve which design/hull supplies those
bytes; native `LpshdefFromTok`/global binding, full `FLookupPart` services,
complete beam/torpedo damage acceptance, original differential, and full
battle replay remain open. Evidence is in
`analysis/compat/f-attack-cool-battle-hull-e40.{md,json}` and
`runs/vmctl-fcbate40/`.

E41 removes the preselected-hull fixture from that callback: the adapter copies
the battle token's player/design bytes into the original `_tok` offsets and
calls `stars_lpshdef_from_tok`, then reads the selected 123-byte `_hul` record's
16 inline `_hs` entries at offset 58 and count byte `chs` at offset 122. The
strict host and native integration selects the alternate table for player 1,
design `0x12` (index 2), and carries its raw component through FAttack lookup
and damage. The per-player table callback still supplies the selected table
view, so exact native globals/far-table binding, full part services, damage
acceptance, original differential, and full battle replay remain open. See
`analysis/compat/f-attack-design-hull-resolution-e41.{md,json}` and
`runs/vmctl-fcbate41/`.

E42 adds both normal and alternate table cases to the native attack callback
chain. Player 1/design `0x12` selects alternate index 2 and runs one raw hull
slot through lookup/damage; player 3/design 5 selects normal index 5 and the
empty-slot path. The fresh VC++ 1.52c/Win98 report passes 44/44. Native global
table pointers, complete part services, damage acceptance, original
differential, and full battle replay remain open. Evidence is in
`analysis/compat/f-attack-design-hull-table-branches-e42.{md,json}` and
`runs/vmctl-fcbate42/`.

E43 adds the raw DGROUP-table path for `LpshdefFromTok`. The recovered global
symbols are `rglpshdef` at offset `0x00FE` and `rglpshdefSB` at `0x014C`; the
adapter reads the player's four-byte far pointer, applies the correct design
index and `0x93` stride, and supplies the exact segmented address to the
resolver. Both branches pass native integration (`1234:2126` alternate and
`2345:12DF` normal), and the selected alternate hull flows through FAttack
lookup/damage. The resolver still must bind actual live game hull memory.
Complete `FLookupPart`, damage acceptance, original differential, and battle
replay remain open. Evidence is in
`analysis/compat/f-attack-dgroup-hull-far-tables-e43.{md,json}` and
`runs/vmctl-fcbate43/`.
