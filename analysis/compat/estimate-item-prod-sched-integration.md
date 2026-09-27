# EstimateItemProdSched packed/integration checkpoint

The reconstructed schedule procedure was exercised through a raw packed
production queue in a fresh VC1.52c/Win98 harness. The adapter binds the
recovered `PLPROD` header (`cAlloc`, `iprodMax`, and `iprodMac`) and its
four-byte `_prod` records, decodes the packed quantity/type/object fields,
and writes the updated queue quantities back to the same raw records.

The build callback invokes the reconstructed `CBuildProdItem` source, so the
checkpoint covers the schedule-to-build boundary, resource consumption,
object creation callbacks, first/last completion turns, and preservation of
the caller's packed planet mineral words.

Host and native reports are:

`ESTIMATE_ITEM_PROD_SCHED_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`

The original Win98 Production Queue dialog showed the available production
items, an empty selected queue, required-mineral totals, and the queue command
controls without a Program Error dialog. That is the consumer/UI boundary;
it is not claimed as a populated schedule differential. A broader original
turn-by-turn comparison and complete production-record/resource mutation
remain open, so this advances `EstimateItemProdSched` to Phase 2 step 5/6,
not promotion.

Evidence:

- `tests/estimate_item_prod_sched_integration_probe.c`
- `compat/vc152/estimate-item-prod-sched-integration/`
- `runs/vmctl-vc152eipint/EIPISTA.TXT`
- `runs/vmctl-vc152eipint/EIPICL.LOG`
- `runs/vmctl-vc152eipint/EIPILINK.LOG`
- `runs/vmctl-vc152eipint/EIPIR.TXT`
- `runs/vmctl-vc152eipint/native-screen.bmp`
- `runs/vmctl-vc152current/production-queue-popup.bmp`
