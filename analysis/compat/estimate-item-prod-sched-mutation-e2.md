# EstimateItemProdSched packed production-record mutation E2

The production scheduler was replayed through a fresh packed-queue adapter
using the current `EstimateItemProdSched` and `CBuildProdItem` reconstructions.
The adapter decodes the four-byte `_prod` records, calls the build service,
and writes the updated quantities back to the same raw records.

Host strict C89 and fresh VC++ 1.52c / Win98 validation both report:

`ESTIMATE_ITEM_PROD_SCHED_MUTATION_CHECKS=3 FAILURES=0 RESULT=PASS`

The three cases cover:

- complete production of two queue records, preserving packed type/object
  bits while consuming all four resource lanes;
- partial production, preserving the remaining raw quantity and consuming
  exactly the available resources; and
- failed object creation, preserving the original raw quantity and rolling
  all resource lanes back without sending a completion message.

The native packet in `C:\EIPM2` reports `COMPILE=PASS` and `LINK=PASS`. The
process exited 0 and the final guest window inventory contained no Program
Error dialog. This closes the named E2 production-record/resource mutation
unit for `EstimateItemProdSched`. The E1 original per-turn/complete schedule
differential remains open, as does final Step-6 promotion.

Evidence:

- `tests/estimate_item_prod_sched_mutation_probe.c`
- `compat/vc152/estimate-item-prod-sched-mutation-e2/`
- `runs/vmctl-vc152eipm2/EIPMSTA-final.TXT`
- `runs/vmctl-vc152eipm2/EIPMCL-final.LOG`
- `runs/vmctl-vc152eipm2/EIPMLINK-final.LOG`
- `runs/vmctl-vc152eipm2/EIPMR-final.TXT`
