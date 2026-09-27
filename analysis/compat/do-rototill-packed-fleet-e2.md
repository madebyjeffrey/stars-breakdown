# DoRototillAiTurn RT-E2 — packed fleet entry and flag sweep

The original NE 20:~2300 fleet pass first reads each owned 124-byte `_fleet`
and clears bit 15 of its offset-4 status word (`fMark`) before transport and
route decisions. The reconstructed optional packed view now reads ID and
owner at 0/2, planet ID at 6, sixteen signed 16-bit `rgcsh` counts at 12,
and `cord` at 98. It clears `raw[5]` bit 7 for owned fleets before invoking
the typed/legacy fleet callbacks. This also removes the previous byte-wide
ship-count limitation in the typed view. The caller still supplies the
current order kind: the far `lpplord` route content is not yet bound.

The strict-C89 probe passes 5/5. It supplies deliberately contradictory
typed snapshots, including a 300-ship count that cannot fit in a byte;
checks the callback view and ordering of the flag clear; confirms a foreign
fleet remains untouched; and rejects a short or ID-mismatched record before
any fleet mutation. The prior typed-fleet probe still passes 3/3.

This is a narrow RT-E2/RT-E3 supporting subcheck, not exact routing,
target selection, or packed `PLORD` acceptance. The fresh r3 Win16 packet
at `compat/vc152/rototill-production-e1-r3/` combines this 5-check probe
with the 20-check production probe for `C:\RTE1N3`; it has not run.

At 2026-09-23 05:48 MDT, the full host suite passed 565 tests with 153
expected skips and zero failures; baseline inventory and diff checks pass.
The running StarDev manager exposes no separate guest control window, and
the STARSAGT listener received no connection. These checks do not establish
VC1.52c or original-game behavior.

## Unowned-destination marker continuation

The original branch following the owned-fleet `fMark` clear tests ship
slots 7/8 and `cord`. When they lead to the upper branch, a current
`idPlanet` is marked only if `LpplFromId` says it is unowned; if the
current planet is `-1` and `cord >= 2`, the second packed order's ID at
PLORD allocation offset 26 supplies the destination. The optional
`fleet_unowned_marker_bound` path now applies `| 0x80` to AI-record byte 1
at that target. It requires a complete planet list and live ID-indexed
AI table. The raw PLORD header/count/length, target index, and fleet
records are preflighted before any planet or fleet mutation.

The expanded strict-C89 packed-fleet probe passes 12/12: current-planet
and second-order markers, short PLORD and out-of-range
target rejection before writes, and the occupied-planet no-marker case,
in addition to the earlier fleet-field/flag cases. The occupied-planet
route-reset path and the broader transport/attack/target search are still
unimplemented; this does not close RT-E2 or RT-E3. The fresh r4 packet
at `compat/vc152/rototill-production-e1-r4/` targets `C:\RTE1N4` and
supersedes unrun r3.

Verification at 2026-09-23 05:54 MDT: full host suite 565 tests,
153 expected skips, zero failures; baseline inventory and diff checks pass.
The STARSAGT listener again received no guest connection, so r4 native
and original-turn evidence remain missing.

## Occupied-current-planet repair continuation

The original upper fleet branch sends a slot-7/8 fleet with a current
occupied `idPlanet` to `ChangeMainObjSel(2, fleet)`, sets selected
`cord = 1` and `PLORD.iordMac = 1`, calls `FLookupFleet(-1, sel.fl)`,
then `ClearAiCurrentTask(fleet, 0)`. An optional packed binding now
recognizes this current-planet case, requires a mutable 124-byte fleet,
mutable PLORD header, and all three service callbacks before any write,
then makes those two packed writes in that order between select and
refresh and follows with task clear. It skips the generic fleet visits
after this original early-exit branch.

The strict-C89 packed-fleet probe is now 17/17. It checks the full
select/write/refresh/clear sequence and arguments, preflight rejection
for a missing callback or PLORD, and select failure before order mutation.
The binding assumes the supplied packed pointers identify the selected
fleet and its order allocation; that assumption still needs a real Win16
caller check. The broader transport/attack route predicates, possible
reroute of an occupied target, target search, and original-turn comparison
still keep RT-E2/E3 open. Fresh native packet r5 is prepared at
`compat/vc152/rototill-production-e1-r5/` for `C:\RTE1N5`, superseding
unrun r4.

Verification at 2026-09-23 05:59 MDT: full host suite 565 tests,
153 expected skips, zero failures; baseline inventory and diff checks pass.
The StarDev manager reports the VM running, but STARSAGT did not connect
to the local listener. r5 has no VC1.52c or original-turn return.
