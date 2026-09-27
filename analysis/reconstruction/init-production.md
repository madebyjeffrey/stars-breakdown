# InitProduction reconstruction

`InitProduction` (`001b:015e-108f`, 3,889 bytes) builds the production
catalog for a selected planet and reconciles the existing queue against that
catalog. The typed C reconstruction covers ship designs, starbase capacity,
factories, mines, defenses, warp infrastructure, terraforming, research
items, quantity clamping, queue preservation, and the tutorial handoff.

The source uses named packed-item fields and production categories rather than
assembly or emitted-byte replay. The source probe checks catalog construction,
queue quantity preservation, and the tutorial callback. It passes strict host
C89 checks and the native VC++ 1.52c Win98 build/link/runtime gate. The
executable report is `CHECKS=1`, `FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152ak/init-production/IPRES.TXT`.


## SO-E3 audit correction (2026-09-25)

Queue reconciliation now preserves order, duplicates and packed progress while
consuming finite catalog capacity; 0x3ff availability is unlimited. The previous
implementation rebuilt in catalog order and lost duplicate queue entries.
`python3 -m unittest tests.test_ai_final_queue_source -q` passes the expanded
host regression. This change has not been rerun natively. The earlier native
report verifies the old narrow probe, not the expanded behavior.

Full catalog fidelity remains under audit for SO-E3: original planetary entries
use group 1 while the typed catalog builder currently uses group 2, and its
item enumeration needs reconciliation with original produce.c. The opening
coverage description must not be read as full original-behavior equivalence.


### Catalog correction (2026-09-25)

The group/enumeration mismatch identified above is now corrected: planetary
items use group 1, packets are 14..17, alchemy 11 is unconditional, scanner 27
requires scanner type 31 and non-AR race, and automatic items follow original
AR/CA exclusions. Starbase designs arrive as eligible group-2 design words;
part-14 eligibility requires lookup result exactly 1. The tutorial also requires
game flag 0x0800. Exact packed-entry/order checks pass in the expanded host
probe. Integration still must derive catalog eligibility from the runtime world;
no new native or complete-turn closure is claimed.
