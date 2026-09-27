# EstimateItemProdSched schedule differential boundary

The readable `EstimateItemProdSched` source now has a broader strict-C89
differential probe covering the remaining schedule semantics at the host
callback boundary. The probe exercises ordinary phase-1 completion, phase-2
completion and its preceding-turn result, the 100-turn fallback, final
special-item `0x3fc` quantity substitution, non-final special-item rejection,
partial-item propagation, raw and research-adjusted resource queries,
research-exempt flags, per-turn mineral refresh, copied-planet isolation, and
population advancement only after a complete queue pass.

The host report is:

`ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0 RESULT=PASS`

The VC++ 1.52c packet in `compat/vc152/estimate-item-prod-sched-diff/` was
replayed in fresh guest directory `C:\EIPD`. The native status and report are:

```text
EIPDSTA.TXT: COMPILE=PASS
EIPDSTA.TXT: LINK=PASS
EIPDR.TXT: ESTIMATE_ITEM_PROD_SCHED_DIFFERENTIAL_CHECKS=21 FAILURES=0 RESULT=PASS
```

The post-run Win98 window list contained no Program Error dialog. This closes
the native replay sub-check for the schedule differential. The authoritative
E1 acceptance unit remains open because complete packed production-record and
resource mutation, original per-turn queue/resource replay, and final
promotion are separate requirements.

Evidence is preserved in `runs/vmctl-vc152eipd1/`.
