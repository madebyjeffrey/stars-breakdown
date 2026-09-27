# DoMacintiAiTurn MA-E2: attack-design buddy/miner followup

Status: **supporting evidence only; MA-E2 remains open**.

For an owned one-order attack-design fleet whose first PLORD kind is
already 3, the original 10a0:1ded–1f15 path tries
FFindBuddyAndJoinUp(fleet,14,15,72,108) unconditionally when the
earlier attack-design pressure counter exceeds 58, or on a nonzero
Random(3) result when that counter is 49–58. This local counter begins
with the owned attack-design fleet census and can increase when earlier
production queues an item; it is **not game.turn**. A successful join
advances to the next fleet. Otherwise, an owned current planet supplies
three unsigned concentration bytes at packed planet offsets 9–11.
A sum below 30 calls FRetargetMiner and advances; a sum from 30 to 59
does so on Random(3)==0. All other cases draw Random(10), retargeting
on zero before continuing into the later order gate.

The bound source now offers this branch when find_buddy is installed.
When CheckAiShdefStatus is exposed, it directly counts the original
owned-fleet, design-available attack-design census from packed records
before production. It checks that a supplied post-production pressure
is not below that census. The later increment can now come from a marked
plan_production queue action when the caller opts into complete
pre-fleet attack-plan reporting; otherwise the caller must supply the
final pressure explicitly. Random and live-retarget services are also
required before actions begin. The packed fleet and planet views,
including owner and
order kind, determine the gates; buddy and retarget callbacks receive
the bound fleet, not stale typed fields. Without these live services,
the older typed action adapter remains transitional. Subsequent
colonizer/order decisions are not claimed complete.

The strict-C89 tests/do_macinti_buddy_miner_e2_probe.c passes **16/16**:
pressure-59 join, pressure-49 successful/unsuccessful buddy draws, the
pressure-48 below-30 gate, sum-30 and sum-59 Random(3) behavior, sum-60 fallback,
foreign-owner fallback, pressure-58 boundary, missing-service and
missing-pressure preflight, and invalid random result rejection. It checks
the 14/15/72/108 buddy arguments, random bound/callback order,
design-available owned-fleet census, below-census rejection, and
unavailable-design/foreign-fleet exclusions. Earlier Macinti probes
pass 31/31, 12/12,
10/10, 8/8, and 14/14.
The full host suite passes **574 tests**, 153 expected skips, zero
failures; baseline and diff checks are clean.

The fresh DOS-8.3/CRLF VC1.52c packet at
compat/vc152/macinti-planet-table-e1-r16/ targets C:\MAE1N16 and
contains all seven probes (31/13/12/10/16/8/14). It is staged, not compiled
or run in Win98. Full attack/colonizer/transport route sequencing,
the exact attack-production choice predicate, native return, and
original-executable comparison remain; no named
Step-5 unit or procedure promotion closes from the host result alone.
