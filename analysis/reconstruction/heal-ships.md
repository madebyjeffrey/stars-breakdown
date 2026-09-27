# HealShips reconstruction

`HealShips` (`0018:444c-48f6`, 1,194 bytes) repairs damaged fleet hulls and
planetary starbases. The reconstruction uses the project’s existing packed
fleet and planet adapters: it skips ineligible fleets, derives repair rate
from movement/orbit state and race ability, repairs packed per-design hull
condition fields with saturation, and repairs eligible starbase damage with
the race bonus.

The source uses named status and repair constants and contains no assembly or
emitted-byte replay. The probe covers fleet condition repair, race-based
repair doubling, starbase repair, and saturation. It passes strict host C89
checks and the native VC++ 1.52c Win98 build/link gate. The executable report
is `CHECKS=1`, `FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152ai/heal-ships/HSRES.TXT`.

## Original-rule audit correction (2026-09-25)

The earlier probe covered too little to establish fidelity. Inspection of
`analysis/decompiler/export/raw/10b8_444c_HealShips.c` found missing or incorrect
rules in the reconstructed service. The corrected service now:

- skips dead and no-heal fleets (bits 10 and 14);
- uses the here-all-turn bit 13 to select moving (5), stationary deep-space
  (10), other/unowned orbit (15), own orbit (25), own repair-capable starbase
  without cargo capacity (40), or with cargo capacity (100);
- doubles that rate for race statistic 14 equal to 4, then adds a fleet-wide
  hull-25/hull-26 bonus of 25/50, without stacking;
- repairs every nonzero packed damage word, preserving low bits only while
  damage remains and clearing the entire word at saturation;
- repairs only present, unblocked starbases, at rate 50 or the race rate 75.

`StarsHealPlanet.id` now identifies orbit lookup independently of array order.
The expanded strict-C89 source probe checks 22 conditions, including disabled
healing, movement, ownership, both starbase classes, bonus order, low-bit
saturation, and starbase eligibility. Direct host compile/run passes with
`-std=c89 -pedantic -Wall -Wextra -Werror`.

The historical native report above predates these corrections and does not
validate the corrected source. Gate 2 package G2-08 is now bound to current surviving fleet/planet state.
Both integrated control/action replays and separate nonzero-damage adapter
checks pass; see `analysis/compat/satisfy-orders-e3-gate2-tracker.md`.
