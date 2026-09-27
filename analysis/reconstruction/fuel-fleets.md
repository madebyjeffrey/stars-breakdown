# FuelFleets reconstruction

`reconstructed/src/fuel_fleets.c` implements the recovered original fleet
fueling rules (NE 23:2EFA). Dead fleets are skipped. Own or allied starbases
with nonzero hull cargo capacity refill a fleet to its maximum fuel capacity.
The base owner's relation toward the fleet owner controls allied access.
Otherwise Anti-Matter Generators produce 50 fuel per component per ship and
hulls 25/26 produce 200 per ship, capped at fleet capacity.

The legacy API name `current_fuel` means maximum capacity; `base_fuel` is the
actual pre-stage fuel cargo. Comments and local names now make this explicit.
Capacity is queried only when refilling or generating fuel, matching original
branch eligibility. Eight strict-C89 source checks cover both transport hulls,
generator count, capacity saturation, dead fleets, friendly refill, and zero
production. They pass with `-pedantic -Wall -Wextra -Werror`.

## Gate 2 integration (2026-09-25)

The coordinator calls this service for every current surviving host fleet
(control 8, action 7). Current ship counts/design slots and original hull
records feed source `stars_get_fleet_stat` / `stars_wt_max_ship_design_stat`.
Hull fuel capacity is offset 54; offset 50 is the bitmap identifier. The first
integration run exposed and corrected an adapter offset mistake before closure.

Current planet ownership/starbase state and loaded player relations determine
refill permission. Fuel returns to movement state, mutable host fleet state,
and owner-1 raw fleet bytes. Existing output writers consume the new fuel.
No retained output value supplies a runtime fuel answer.

Both seeded replays retain original fleet records and exact RNG traces.
Separate copied-state tests verify relationship direction, a base without
cargo capacity, transport plus generator production, capacity saturation,
and fuel write-back for both owners. The input/selector suite passes 19 tests
with 1 existing skip (20 total), 15.276 seconds. The selector report includes
`FUELING=PASS`.

Historical VC1.52c reports `runs/vmctl-vc152ew/FUSTA.TXT` and `FURES.TXT`
predate this integration; no new native validation is claimed.
