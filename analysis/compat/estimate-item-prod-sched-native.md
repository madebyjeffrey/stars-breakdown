# EstimateItemProdSched native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152eips/`.

- `EIPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `EIPCL.LOG`: VC++ 1.52c compiled `EIP.C` and `PROBE.C` without warnings
- `EIPLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `EIPR.TXT`: `ESTIMATE_ITEM_PROD_SCHED_CHECKS=4 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`EIP.C` was transferred directly from the reviewed
`reconstructed/src/estimate_item_prod_sched.c` file. The native probe
exercises ordinary queue completion and planet preservation, negative-item
resource and research queries, and the corrected special-item bit discriminator
with the final `0x3fc` quantity sentinel observed by the build callback.

The first local compact-probe attempt was rejected before guest execution
because its trace structure was not initialized; the corrected probe was then
recompiled and transferred. This advances `EstimateItemProdSched` from Phase 2
step 3/6 to step 4/6. Exact packed production records, resource/population
mutation, CBuildProdItem integration, and original differential cases remain
open.
