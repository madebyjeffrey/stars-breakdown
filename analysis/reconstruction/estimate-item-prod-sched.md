# EstimateItemProdSched reconstruction

`reconstructed/src/estimate_item_prod_sched.c` is a source-level
reconstruction of `EstimateItemProdSched` (`001b:4f40-548f`, 1,359 bytes).
It makes the 100-turn simulation on a copied planet record, mineral/resource
refresh, research subtraction and negative-item query arithmetic, partial queue
item, special production item handling, build-phase results, first/last
completion turns, and population advance explicit through typed C records and
callbacks.

The source probe covers ordinary completion, caller-planet preservation, and
the negative-item resource/research query paths, including the raw special-item
bit discriminator and final `0x3fc` quantity sentinel. Host strict C89 compilation
and
`ESTIMATE_ITEM_PROD_SCHED_SOURCE=PASS` pass. A fresh VC++ 1.52c/Win98
checkpoint now compiles, links, and runs the exact source with four grouped
native assertions passing. A follow-up packed checkpoint binds the recovered
four-byte `PLPROD` header and `_prod` queue records, invokes the reconstructed
`CBuildProdItem` boundary, and round-trips queue quantities with two native
integration assertions passing. The original Production Queue surface also
shows the available items, empty queue, and required-mineral boundary without
a Program Error dialog. This advances the procedure to Phase 2 step 5/6;
broader original turn-by-turn differential and complete production-record/
resource mutation comparison remain open.

A broader host differential probe now passes
`ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0 RESULT=PASS`.
It covers phase-1 and phase-2 completion, the 100-turn fallback, final and
non-final special-item behavior, partial-item propagation, raw and
research-adjusted resource queries, the research-exempt flag, mineral refresh,
copied-planet isolation, and population timing. The packet at
`compat/vc152/estimate-item-prod-sched-diff/` was replayed in fresh `C:\EIPD`;
VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the native report is
`ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0 RESULT=PASS`.
The post-run window list had no Program Error dialog. This closes the native
replay sub-check, but not the authoritative E1 unit: complete packed
production-record/resource mutation and original per-turn queue/resource
replay remain open, as does promotion. Evidence is in
`runs/vmctl-vc152eipd1/`.

A second host boundary now passes
`ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS`.
It wires the scheduler through the recovered `GetResourcesAvailable` and
`GetProdQCost` services, checking copied-planet mineral handoff, research
exemption, callback order, two queued cost records across all four resource
lanes, completion turns, and caller-planet preservation. The matching native
packet at `compat/vc152/estimate-item-prod-sched-resource-queue/` was replayed
in fresh `C:\EIRQ`. After replacing a probe-only `_printf` summary with
`puts`, VC++ 1.52c reports `COMPILE=PASS` and `LINK=PASS`, and the native
report is
`ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS`.
The post-run Win98 window list had no Program Error dialog. This closes the
native resource/queue Step-5 unit; the broader original turn-by-turn
differential, complete production-record/resource mutation, and promotion
remain pending. Evidence is preserved in `runs/vmctl-vc152eirq2/`.

## Packed production-record mutation E2

The new packed mutation probe exercises the schedule through the current
`CBuildProdItem` boundary and writes the resulting quantity back to the raw
four-byte `_prod` records. Host strict C89 and a fresh VC++ 1.52c / Win98 run
in `C:\EIPM2` both report
`ESTIMATE_ITEM_PROD_SCHED_MUTATION_CHECKS=3 FAILURES=0 RESULT=PASS`. Complete,
partial, and failed-object cases check quantity preservation, packed
type/object-bit preservation, four-lane resource consumption or rollback, and
completion-message suppression on failure. Compile/link passed, the process
exited normally, and no Program Error dialog appeared. This closes E2, the
production-record/resource mutation unit. E1's independent original
per-turn/complete schedule differential and Step-6 promotion remain open.
Evidence is preserved in
`analysis/compat/estimate-item-prod-sched-mutation-e2.{md,json}`,
`compat/vc152/estimate-item-prod-sched-mutation-e2/`, and
`runs/vmctl-vc152eipm2/`.

## Original per-turn differential E1 closure

The remaining E1 boundary now passes a per-turn trace derived from the raw
x86 loop in fresh VC++ 1.52c / Win98 directory `C:\EIPOD`:
`ESTIMATE_ITEM_PROD_SCHED_ORIGINAL_DIFFERENTIAL_CHECKS=8 FAILURES=0 RESULT=PASS`.
The trace verifies queue order across a phase-5 turn break and resumed turn,
population advancement between turns, exact first/last completion timing,
mineral/resource refresh cadence, non-final special-item rejection, and the
100-turn fallback. Compile/link passed, the process exited 0, and no Program
Error dialog appeared. With E2 packed mutation and E3 resource/queue service
evidence already closed, E1 completes the Step-5 boundary and the procedure is
now promoted to `6/6`.

Evidence is preserved in
`analysis/compat/estimate-item-prod-sched-original-differential-e1-native.{md,json}`,
`compat/vc152/estimate-item-prod-sched-original-differential-native/`, and
`runs/vmctl-vc152eipod/`.

## Simulation correction (2026-09-25)

Original produce.c:2168-2380 requires private queue/planet simulation, mineral
stock accumulation and carry-over, a persistent partial queue entry, a mutable
four-lane build budget, and non-final alchemy skipping. The earlier helper and
small probe did not establish these behaviors. The corrected source and
expanded probe now cover them, including two simulated years and caller-state
preservation. StarsEstimateProdInput.build_item_mutable exposes the complete
budget and mutable queue entries; legacy build callbacks remain supported.
StarsEstimateProdMined returns annual new quantities. The scheduler adds these
to its private planet stock. Simulated mine/factory counts are now retained.

The SO-E3 AI queue-tail adapter is still unbound; this correction alone is not
integrated ETA or native compatibility evidence. See the Gate 2 tracker.
