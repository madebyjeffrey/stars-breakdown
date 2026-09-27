# DoAutomitronAiTurn AU-E1 — live planet-value services

The original first planet pass counts unowned, class-above-two planets
after calling PctPlanetOptValue. In the second pass it writes the AI
visit marker, calls PctPlanetDesirability for owned planets, and for
foreign planets writes the visibility status byte before calling
PctPlanetOptValue to decide the optimal-value marker.

The production source now has optional live callbacks for those two
services at those call sites. Its existing per-planet fields remain
fallback values only when a service is unbound. The strict-C89 host
probe passes 8/8: an unowned planet whose live optimal value overrides
its snapshot, an owned planet whose live negative desirability prevents
production and sets the bad-owned marker, two foreign optimal-value
outcomes, and the exact order/record state observed by each callback.

The services are not yet wired to original game memory, and the r12
VC1.52c packet has not run. Remaining resource/growth and fleet/mineral
branches plus an original-turn differential keep AU-E1 open.
