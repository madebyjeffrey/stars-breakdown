# SpdOfShip reconstruction

`reconstructed/src/spd_of_ship.c` is a typed semantic reconstruction of ship-speed calculation, preserving engine/fuel component effects, fleet weight, racial bonuses, cargo/token effects, and the no-engine path.

Validation: `tests/spd_of_ship_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SPSTA.TXT` and `runs/vmctl-vc152ew/SPRES.TXT`.

## Gate 2 design-power dependency audit (2026-09-25)

Original `SpdOfShip` distinguishes the selected engine's count from maneuver
bonuses supplied by engine 8 and maneuver jets. The old reconstruction used
one counter for both, causing ordinary engines to return zero speed and using
the wrong weight divisor. It also processed zero-count slots. These rules
are corrected: nonempty engine slots set the engine count; maneuver points
contribute separately; the original first hull slot's count divides the
weight penalty. The design-only call has no fleet race/cargo/random effects.

The strict-C89 source probe now supplies a real engine count in its existing
live-fleet case and checks ordinary one/two-engine designs, empty engines,
and the separate engine-8 bonus. The cargo-dumping expectation is corrected
from 8 to 7 for the original first-slot weight divisor. The host probe passes.
Historical native reports above predate these corrections.

SO-E3's score preparation now calls this service with loaded design weight,
current component slots, and engine fuel rows from the original part table,
then calls `stars_compute_design_power`. Both retained runs match original
score-history counts in all three ship power classes; full scores remain open.
