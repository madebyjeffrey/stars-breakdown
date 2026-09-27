# EstimateItemProdSched original per-turn differential E1

The remaining `EstimateItemProdSched` E1 boundary was replayed through a
strict-C89 trace derived from the original x86 loop at
`analysis/decompiler/export/raw/10d0_4f40_EstimateItemProdSched.c`. The trace
keeps the original queue order and turn boundaries visible instead of only
checking final first/last values. It covers:

- preceding queue work that stops a turn at phase `> 4`, followed by the next
  turn's population advance and resumed queue order;
- target-item first/last completion on the exact turn and the per-turn mineral
  and resource refresh callbacks;
- the non-final special-item `-1/-1` early return with no build or population
  callback; and
- the 100-turn fallback when the target never reaches a terminal build phase.

The host and fresh VC++ 1.52c / Win98 packet both report:

```text
ESTIMATE_ITEM_PROD_SCHED_ORIGINAL_DIFFERENTIAL_CHECKS=8 FAILURES=0 RESULT=PASS
```

The native packet ran in `C:\EIPOD`; `EIPODSTA.TXT` records
`COMPILE=PASS` and `LINK=PASS`, the process exited 0, and the final guest
window inventory contained no Program Error dialog. This closes E1's original
per-turn differential. E2's packed production-record/resource mutation and
E3's resource/queue service boundary were already closed; together they make
the complete Step-5 acceptance boundary eligible for promotion.

Evidence is preserved in
`analysis/compat/estimate-item-prod-sched-original-differential-e1-native.{md,json}`,
`compat/vc152/estimate-item-prod-sched-original-differential-native/`, and
`runs/vmctl-vc152eipod/`.
