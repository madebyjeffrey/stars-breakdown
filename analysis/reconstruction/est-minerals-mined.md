# EstMineralsMined

`EstMineralsMined` is represented as typed mineral-estimation logic in
`reconstructed/src/est_minerals_mined.c`.  It preserves the operating-mine
sentinel, invalid-planet result, special-race efficiency, concentration floor,
fixed-point product/truncation/remainder rounding, per-mineral output, apply
path, and concentration-depletion loop boundary.

Packed planet/fleet storage and exact fixed-point arithmetic remain
callback-owned, but eligible fleet mine aggregation is now represented by
typed fleet-discovery and order-consumption callbacks around the same
recursive source routine. The host probe covers invalid estimation,
special-race flooring, rounding, apply, depletion, and recursive fleet
aggregation. A fresh VC++ 1.52c/Win98 checkpoint now compiles, links, and runs
the exact source with five grouped native assertions passing. An optional typed
`rgwtMin` view performs the original fixed-point concentration/percentage
depletion arithmetic directly; callback-only callers remain supported for
packed-storage integration.

A second fresh VC++ 1.52c/Win98 harness now round-trips the recovered packed
`_planet` mineral fields: player at offset 2, percentage levels at 6, mineral
concentrations at 9, and `rgwtMin[3]` at 28. It runs the reconstructed source
against that raw byte view and passes four native integration checks. The
original Win98 planet surface also showed the Minerals On Hand rows without a
program-error dialog. This advances the procedure to Phase 2 step 5/6; a
broader original mining-turn differential and complete fleet/planet mineral
record mutation remain open.

The packed integration harness was expanded to thirteen deterministic native
checks in fresh `C:\EMMINT3`. It now covers invalid mining context,
operating-mine lookup and efficiency, both random-rounding outcomes,
concentration-floor handling, special-race fleet recursion and consumption,
raw `rgwtMin` write-back, the null-operations guard, and packed fleet-order
eligibility. The new fleet case binds the recovered 124-byte `_fleet` player,
planet, flags, coordinate, and order-task fields and verifies the original
consumed-bit clear while the 56-byte planet record receives the mineral
write-back. VC1.52c compilation and linking passed; the Win98 report from the
final matrix was `EST_MINERALS_MINED_INTEGRATION_CHECKS=13 FAILURES=0
RESULT=PASS`, and the post-run window list contained no Program Error dialog.

The final `C:\EMMINT4` matrix closes the original-instruction populated-turn
differential: two ordinary-race efficiency/depletion vectors and one
special-race planet-plus-two-fleets vector agree on all three outputs,
percentage/concentration state, fixed-point weights, and consumed fleet
orders. It also corrected the original pre-efficiency depletion-product and
percentage-cap reset semantics in the typed source. The procedure's Phase 2
Step-5 evidence is closed; promotion is recorded in the review ledger.
