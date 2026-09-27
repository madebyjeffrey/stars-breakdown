# FDoCoolBattle reconstruction (partial)

`reconstructed/src/f_do_cool_battle.c` transfers the battle-lifecycle
orchestration of `FDoCoolBattle` (NE `001f:8bcc–9917`) into typed C.  It
preserves the fixed `0xFFC8`-byte/type-`0x0B` battle-log allocation and
initialization ordering, typed battle-header and semantic movement/attack event
emission (including the token coordinate and movement-state fields carried by
each battle record), with an optional little-endian C log path that stages a
battle in a temporary buffer, appends the 14-byte header and six-byte typed
records after existing log content, closes the per-battle header length,
reports that serialized per-battle length independently of any existing-log
prefix, and writes the raw end marker when the persistent log has no room,
token/round state, and the
InitializeBoard-derived initiative minimum/maximum used by descending
dispatch. It preserves the original round-zero shield-regeneration gate,
shield regeneration before movement and attack on later rounds, and
supports a typed token path for active-token participant-mask rebuilding,
and an optional direct `InitializeBoard` path that copies the reconstructed
board-token fields into the battle-token view,
shield and movement callbacks, initiative-range filtering, and per-token
attack dispatch, with token-capacity rejection before iteration,
and can feed the existing `DxyMoveTokTo` semantic implementation directly
when movement policy callbacks are supplied,
descending token-index dispatch and an active-player recheck before each
attack, with descending initiative dispatch using
the board-provided initiative-availability gate,
per-player attack-mask filtering, defending-starbase participation from the
primary fleet's planet, fleet-derived active-player participation
with ship-count and inactive-fleet gating,
post-attack active-token termination, surviving-player termination, battle message dispatch, and cleanup on both
initialization and reporting paths.

The source probe passes the normal two-player round, pre-attack elimination,
InitializeBoard-derived descending initiative dispatch, defending-starbase and inactive-fleet participation gating, and
cleanup paths under strict host C89, including the typed token callback path.
The token path now applies the attack-mask filter after shield regeneration and
movement, matching the raw routine's second surviving-player gate, and the
attack loop rechecks the filtered survivor mask before each candidate.
The packed-log boundary is now exercised end to end: the strict-C89 probe
checks the raw 14-byte battle header, six-byte movement/attack records,
append-at-existing-end behavior, per-battle serialized length, and the raw
`0xffff` overflow marker. The same probe ran natively in a fresh VC++
1.52c/Win98 `C:\CB` harness and returned
`F_DO_COOL_BATTLE_CHECKS=53 FAILURES=0 RESULT=PASS` without a runtime error
dialog. The original Win98 battle-report surface is preserved as the
consumer boundary. This advances the procedure to Phase 2 step 5/6; packed
token/global-state mapping, global log compaction, exact movement geometry,
exact `FAttack`/damage behavior, remaining record cases, and full battle
replay remain open. The report boundary also exposes the raw battle ID,
player and spectator masks, active-ship/player counts, round count, and
serialized record length passed to `SendBattleMessages`.
The staged-log boundary is now tightened by a fresh `C:\CB2` native case that
preserves a five-byte existing log prefix while copying the current 50-byte
battle record after it. The expanded 58-check host/native matrix passes with
no Program Error dialog. Packed token/global mapping, exact `FAttack`/damage
behavior, remaining record cases, full battle replay, and promotion remain
open.

A dependency probe now connects the actual reconstructed battle orchestrator
to the actual reconstructed `FAttack` procedure. Host and fresh VC++ 1.52c/
Win98 runs both pass
`F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS`,
covering descending-initiative dispatch, movement and attack event emission,
damage-driven target-token deactivation, survivor termination, battle summary,
and cleanup. The process exited normally and no Program Error dialog appeared.
This closes the native battle-chain replay subcheck but no Step-5 unit; packed
token/global mapping, exact movement and damage behavior, remaining records,
full replay, original differential, and promotion remain open. Evidence is in
`analysis/compat/f-do-cool-battle-attack-integration-e3.{md,json}` and
`runs/vmctl-vc152cbatp/`.

The final native replay also removes the only compiler warning found in the
first attempt: `FAttack` now explicitly compares the section index as an
unsigned 16-bit value before the packed-part bound check. The refreshed
`COOLATSTA-final2.TXT`/`COOLACL-final2.LOG` build is warning-free and its
`CBATRES-final2.TXT` report remains 18/18 with no Program Error dialog. This
is a source-quality correction, not an additional Step-5 closure.

The packed-token boundary is now explicit rather than being represented by a
single broad “global mapping” checkbox. A strict-C89 round-trip probe drives
the production `StarsCoolBattleInput.packed_tokens` bridge to load two
29-byte little-endian `_tok` records after initialization, runs the real
coordinator, and stores the typed state after movement, attack, and reporting.
It passes `F_DO_COOL_BATTLE_PACKED_TOKEN_CHECKS=11 FAILURES=0 RESULT=PASS`:
the surviving token's id/owner/design/count are preserved, while the defeated
token's count becomes zero and its active bit is cleared. The exercised offsets
are id `0x00`, player/object/design/coordinate `0x02..0x05`, initiative
`0x06..0x08`, shield `0x11`, count `0x13`, target flags `0x17`, warp nibble
`0x1a`, and active/movement flags `0x1b`. This is host-source evidence only
and closes **0** authoritative Step-5 units. Native `_tok`/`vrgtok` binding,
complete field/global mapping, exact movement and damage behavior, remaining
record cases, full replay, and promotion remain open. Evidence is recorded in
`analysis/compat/f-do-cool-battle-packed-token-e6.{md,json}`.

A follow-up strict-C89 probe now checks every scalar field of the 29-byte
packed record through the production bridge, including all movement nibbles,
active/log flags, a 32-byte caller stride, and untouched trailing sentinels.
It passes `F_DO_COOL_BATTLE_PACKED_FIELD_CHECKS=24 FAILURES=0 RESULT=PASS`.
The probe also verifies that a typed speed change rewrites only the speed
nibble and that defeat rewrites only the packed count and active bit. This is
stronger host evidence but closes no Step-5 unit; native `_tok`/`vrgtok`
binding, global compaction, exact movement/damage behavior, full replay, and
promotion remain open. Evidence is recorded in
`analysis/compat/f-do-cool-battle-packed-fields-e7.{md,json}`.

The next host correction aligns the attack-mask/survivor ordering with the raw
loop. A focused strict-C89 probe passes
`F_DO_COOL_BATTLE_ATTACK_MASK_ORDER_CHECKS=3 FAILURES=0 RESULT=PASS`: both
tokens move before a disallowed player is filtered, and no attack is issued
once only one permitted player remains. Existing 58-check source, 18-check
FAttack-chain, 11-check packed-token, and 24-check packed-field probes remain
green. This closes no Step-5 unit; native movement/initiative, complete battle
records, full replay, and original differential remain open. Evidence is in
`analysis/compat/f-do-cool-battle-attack-mask-order-e8.{md,json}`.

The remaining Step-5 checklist is therefore: exact native global binding and
production callers; full 29-byte field and compaction coverage; exact movement
geometry and speed/initiative behavior; complete damage/shield/casualty/
salvage/bombing records; fresh VC++/Win98 execution with no Program Error;
and original full-battle replay before ledger promotion.

The E40 battle/attack callback also composes the packed 29-byte attacker-token
context with raw `_hs` hull-slot decoding. The integration packet passes the
decoded raw group/item/count through the FAttack resolver despite contradictory
typed section fields, then verifies damage, target deactivation, reporting,
and cleanup. Host and fresh VC++ 1.52c/Win98 reports pass 33/33 with no Program
Error dialog. This remains supporting evidence only: the callback still owns
design-to-hull selection, and native `_tok`/`vrgtok` global binding, complete
battle services, full replay, original differential, and promotion remain
open. See `analysis/compat/f-attack-cool-battle-hull-e40.{md,json}` and
`runs/vmctl-fcbate40/`.

E41 carries the design-selection boundary into the same callback. It derives
the original `_tok` player/design bytes and uses `stars_lpshdef_from_tok` to
select the normal/alternate per-player hull record. The adapter reads `_hul`
`rghs[16]` at offset 58 and `chs` at offset 122, then runs the existing raw-slot
FAttack path. The alternate player-1/design-`0x12` path passes host and native
38/38 integration checks. The exact game-global/per-player far-table callback,
full battle services and replay, original differential, and promotion remain
open. See `analysis/compat/f-attack-design-hull-resolution-e41.{md,json}` and
`runs/vmctl-fcbate41/`.

E42 adds the missing normal-table case alongside the integrated alternate-table
case. The adapter verifies player/design mapping, table index/selector,
selected hull slots, and zero active slots; the fresh native battle/attack
report passes 44/44. Exact game-global table binding, remaining combat
services, full replay, original differential, and promotion remain open. See
`analysis/compat/f-attack-design-hull-table-branches-e42.{md,json}` and
`runs/vmctl-fcbate42/`.

E43 replaces the callback-only table selector with raw DGROUP far-pointer
selection matching `rglpshdef`/`rglpshdefSB` at `0x00FE`/`0x014C`. The callback
receives the selected segment:offset after original design-stride arithmetic;
native integration covers both player/table branches and passes 44/44. The
final far-address resolver still stands in for the complete game's memory
mapping, so live-global linkage, full combat services/replay, original
differential, and promotion remain open. See
`analysis/compat/f-attack-dgroup-hull-far-tables-e43.{md,json}` and
`runs/vmctl-fcbate43/`.
