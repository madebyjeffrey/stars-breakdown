# EstimateItemProdSched resource/queue service boundary (host E3)

## Result

The strict host C89 probe passes:

```text
ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS
```

## Scope

The probe runs `EstimateItemProdSched` through the recovered resource and
production-queue services. It checks the copied-planet mineral handoff into
`GetResourcesAvailable`, the research-exempt resource result, the resource
and mining callback order, the queue count and two per-item cost lookups from
`GetProdQCost`, all four accumulated queued-cost lanes, first/last completion
turns, and caller-planet preservation.

The corresponding native packet at
`compat/vc152/estimate-item-prod-sched-resource-queue/` was replayed in fresh
guest directory `C:\EIRQ`. The first attempt exposed only a harness-link issue
(`_printf` is absent from this `/NOD` Win16 packet); after changing the
probe-local summary to `puts`, the corrected run recorded:

```text
EIRQSTA.TXT: COMPILE=PASS
EIRQSTA.TXT: LINK=PASS
EIRQR.TXT: ESTIMATE_ITEM_PROD_SCHED_RESOURCE_QUEUE_CHECKS=8 FAILURES=0 RESULT=PASS
```

The post-run window list contained no Program Error dialog. This closes the
native replay unit for the resource/queue boundary. Broader original
turn-by-turn comparison, complete production-record/resource mutation, and
final promotion remain pending.

The Step-5 work is intentionally split into three acceptance units:

1. **E1 turn schedule and production-record differential** — first/last turn,
   population advancement, 100-turn limit, special-item/research-exempt
   branches, and complete caller-record mutation; open.
2. **E2 packed production/resource mutation** — the existing packed production
   integration checks and the remaining complete resource write-back/original
   caller comparison; open.
3. **E3 resource/queue service boundary** — host callback/order probe and the
   fresh native VC1.52c/Win98 replay above; closed.
