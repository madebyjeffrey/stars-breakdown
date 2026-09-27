# FTravelThroughMineFields reconstruction

`FTravelThroughMineFields` (`0017:4f60-6797`, 6,199 bytes) is the fleet
minefield collision and damage service used by movement. The reconstruction
uses typed fleet, minefield, and engine records. It selects the earliest
foreign minefield intersecting the current travel segment, truncates travel
at the collision, computes shield-adjusted damage, applies the fleet-side
damage callback, updates messages and cargo, and removes an exhausted
minefield. A caller may also supply a known hit for the direct-collision
path.

Geometry, shield formulas, damage application, cargo balancing, and message
delivery remain explicit callbacks into the surrounding game subsystems. The
source is ordinary readable C and contains no assembly or emitted-byte
replay. The probe covers collision and no-collision returns, travel
truncation, damage/removal, and message counts.

The source passes strict host C89 checks and the native VC++ 1.52c Win98
build/link gate. The executable report is `CHECKS=1`, `FAILURES=0`,
`RESULT=PASS` in
`runs/vmctl-vc152ae/travel-mine-fields/MINRES.TXT`.
