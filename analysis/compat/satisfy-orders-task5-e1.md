# SatisfyOrders SO-E1: task-5 planet scrap accounting

SO-E1 is closed at the isolated packed task-5 accounting boundary after a
verified-input correction and fresh Win98 rerun. The branch now
consumes the raw 124-byte fleet, 18-byte order, 147-byte SHDEF, and 60-byte
PLANET records on pass 1, and applies mineral return, friendly colonist
transfer, message frames, and fleet/order retirement. The corrected
exact-input cost probe passes on host and in the guest; the older native
packet tested inferred costs. The complete special services and four-pass
original differential remain SO-E2 and SO-E3.

The pinned original HST-only action run in `runs/vmctl-syact3/` starts with one
full task-5 order on fleet 0. The original turn omits fleet 0 and its order,
and the owner message record begins with ID 0x5A, followed by ID 0x141 and the
ordinary ID 0x3F. The first two frames each encode object planet 10 and
parameters `[0,20,0,10]`: fleet code 0, scrap amount 20, high word 0, and
planet 10. Comparing its turn-1 planet-10 record with the clean original run
in `runs/vmctl-syorig1/` gives mineral stockpiles
`476/902/383 → 485/907/388`, differences of **+9/+5/+5**. The original
runs differ by the single HST order edit, but other turn services can change
their downstream planet stockpiles.

The original decompile at `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
enters task 5 on pass 1 at 10b0:8207. For an orbiting fleet it sums each
occupied design's three SHDEF mineral costs, quarters gift-design costs,
applies the ordinary planet scrap rate, and adds fleet mineral cargo to the
planet.
It sends 0x59 or 0x5A to the fleet owner according to the planet starbase bit,
then 0x140 or 0x141 to the planet owner; it transfers colonists to a friendly
planet and sets fleet `fDead` (raw flag 0x0400). Fleet 0 has one ship in
design 0 and zero mineral cargo. With the source's 4/5 starbase rate, the
original message value is 20. The later
[`satisfy-orders-design0-input-oracle.json`](satisfy-orders-design0-input-oracle.json)
decodes the HST design's hull and three components and reads their cost
records from the pinned original executable. The owner-0 technology/race
rule gives design costs **24 resources and 12/8/7 minerals**. At the starbase
rate, the isolated scrap return is **9/6/5** and the message amount is 20.
The original full-turn stockpile difference is 9/5/5, one boranium smaller.
That difference requires an E3 turn-service explanation and cannot be used
alone to infer the design costs.

`tests/satisfy_orders_task5_accounting_probe.c` now uses the verified owner
design costs and the exact retained
18-byte task-5 order, raw fleet/planet/design records, and checks pass gating,
the isolated 9/6/5 mineral return, friendly colonist transfer, both raw message
IDs and argument frames, the first two serialized original message frames,
fleet/order retirement, idempotence, missing-design
preflight, and the no-starbase 0x59/0x140 path. The corrected strict-C89 host
report passes 19/19. The fresh native packet at
`compat/vc152/satisfy-orders-task5-e1-r7/` compiled and linked under VC++
1.52c in `C:\SOE1G` and its [report](../../runs/vmctl-soe1g/SOE1RES.TXT)
passes 19/19; the [build status](../../runs/vmctl-soe1g/SYOSTA.TXT), logs,
map, object, and executable are retained. The earlier native 19/19 report
in `runs/vmctl-soe1f/` compiled the former illustrative 12/7/7 fixture and
does not validate this revision.

`tests/test_satisfy_orders_task5_projected_gap.py` also checks the original
9/5/5 full-turn planet difference, decodes the original message frames, and
runs the packed probe. Its unbound four-pass typed
case still demonstrates the known original/production divergence. The
existing 21/21 packed resource probe remains supporting transfer evidence.

SO-E2 still needs original/live service binding beyond the now-passing native
resource branch. SO-E3 requires the complete same-input four-pass
original/production comparison and explanation of the one-boranium full-turn
difference. That difference belongs to the surrounding turn boundary: the
isolated branch's design costs, 9/6/5 return, amount 20, and message frames
are now source-backed and native-verified.
