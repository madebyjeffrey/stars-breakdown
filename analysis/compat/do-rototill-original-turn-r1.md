# DoRototillAiTurn original selector-3 turn witness

Status: **retained original output, not a closed RT-E1/E2/E3/E4 gate**.

The [candidate manifest](../../runs/rototill-original-turn-r1/input/MANIFEST.JSON)
pins the previously successful selector-1 turn-zero HST, M1, M2, XY, and
original EXE. The candidate changes only owner-1's player mode high byte
from `0x2797` (selector 1, Turin) to `0x6797` (selector 3, Rototill) in the
HST and M2 player records. Both decoded streams round-trip, every other
record/file is unchanged, and the input directory contains neither X2 nor
H2. `tests.test_prepare_rototill_original_turn` verifies those properties.

One original run used fresh guest `C:\RTORIG1`. The controller reported
`RUNSTART 4294776157`, then `EXIT 1`. In accordance with AGENTS.md, no
second original run followed. The [run manifest](../../runs/rototill-original-turn-r1/RUN.JSON)
records the file inventory and window list; no error dialog was listed.
The guest backup HST/M1/M2 bytes match the staged inputs exactly. A new
322-byte backup X2 and 95-byte H2 were returned. The output HST and M2
decode as turn 1, and M1 contains its earlier turn-0 section plus a turn-1
section. `EXIT 1` is not considered a process pass by itself.

The pinned [`test_rototill_original_turn_witness.py`](../../tests/test_rototill_original_turn_witness.py)
checks output hashes and decoded state. Original M2 starts with owner-1 fleet
IDs 0–3, each with one task-0 order to planet 24. Turn-1 M2 and HST retain
those four fleet/order pairs identically. Fleet 0 has a task-0 order to
planet 26 at warp 5, point `(1313,1262)`, and fuel 298. Fleet 1 stays at
planet 24 with 25 colonists. Planet 24 has minerals/colonists
`[539,965,498,256]`. Both turn-1 files contain the same four-byte type-28
production body `01 1c 92 05`; the input M2 had no type-28 record.
Under the recovered four-byte `_prod` layout, that word is one planet
factory item (`iItem=7`, `grobj=1`), quantity 1, at 89% progress. The
[turn oracle](do-rototill-turn-r1-oracle.json) decodes and pins these fields.
Planet 24's factory count rises from 10 to 12 in the same turn. This is an
end-of-turn queue observation; it does not identify the service that created
the item before production.

The [serialized turn oracle](do-rototill-turn-r1-oracle.json) now records every
decoded field of the four owner-1 fleet/order pairs, planet 24, the type-28
queue, and all 32 XY points on both sides of the retained turn. It pins six
source-file hashes and requires the turn-1 HST/M2 owner-1 fleet, planet, and
queue views to agree. The [materializer](../../tools/rototill_turn_oracle.py)
derives 26 changed leaves: fleet 0's route, point, fuel and order; fleet 1's
25 colonists; saved flag changes in fleets 1–3; planet 24's levels,
auxiliary word, minerals/colonists, population delta and factories; and the
new production body. [`test_rototill_turn_oracle.py`](../../tests/test_rototill_turn_oracle.py)
checks reproducibility and key deltas. This is the exact serialized-state
comparison target for a native replay. It does not reconstruct live PLORD,
AI-table, allocation, callback, or RNG state from compact files.

The same regression now pins the fleet-0 RT-E2 scout input: selector 3,
one design-0 ship, one order at owned planet 24, 300 fuel, and zero ships in
slots 1, 7, 8, 13, and 14. Design 0 uses hull 4, whose original hull
template has a weapon slot; installed design components alone do not settle
`FHullHasTeeth`. The 300 fuel exceeds the low-fuel fallback threshold.
The second fleet sweep therefore reaches `IdTargetScout`. The retained M2
player view contains only four player-1 fleets and planet 24. Its enemy list
is empty. Source `IdNearestColonizablePlanet`, called to initialize unknown
status, sets all 32 slots to `0x10` for selector 3 and marks the sole loaded
planet 24 as owned (`1`); no other fleet has a second order that changes a
status. Among the 31 unknown points, planet 26 is nearest to planet 24,
squared distance 545. The original output targets it at `(1313,1262)`.
The HST contains 32 planet records, but its full table is a different input
view from the M2 player view used for this scout decision.

The rebuilt current C library passes the same retained M2 owner view and XY
points through `stars_id_nearest_colonizable_planet` and
`stars_id_nearest_unknown_planet_strided`. It produces 31 status-`0x10`
slots, owned status 1 at 24, and target 26, matching the original compact
order's target. This is a host-side source-helper comparison with the
wormhole callback absent. The live AI status allocation is not serialized,
so its original runtime identity and before/after bytes are still
unobserved. Subsequent [selector-3 coordinator work](do-rototill-coordinator-e4-r1.md)
connects target preparation, the recovered [wormhole draw](do-rototill-rng-seed-r1.md),
ideal warp, and raw PLORD write on the host. No further original run was
made.

This gives a deterministic original-output target and input provenance for
RT-E1/E2/E4. The serialized game files carry compact orders, not the live
18-byte PLORD allocation or AI-table bytes, so they do not prove RT-E3's
internal before/after writes. The current native Rototill packet still uses
synthetic branch records; the selector-3 coordinator now replays the scout
route but has not replayed complete production, fleet, resource, and service
state. The next comparison must bind those services in the reconstructed
turn and compare resulting queue, fleet, order, planet, and resource state
against this retained witness. The process exit, full callback/RNG sequence,
and any nonserialized AI-table state require separate corroboration.
