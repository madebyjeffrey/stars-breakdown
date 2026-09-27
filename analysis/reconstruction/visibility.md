# Visibility initialization

`SetVisPFInit` is represented by `reconstructed/src/set_vis_pf_init.c`.
The host-facing input names the portions of the original DGROUP records that
this pass actually reads or changes: player detail flags and counters, the
sixteen normal and ten starbase design entries, planet visibility flags,
fleet ship counts and scanner results, and thing destination/ownership masks.

The routine first resets the per-player counters and establishes the initial
knowledge/detail state. It then marks planets and eligible fleets, accumulates
observed ship and starbase counts, and delegates scanner range and mine
strength to callbacks corresponding to the original far calls. The final
thing pass preserves the distinct behavior for freighters, packets, mine
records, and owner-targeted objects. The source uses named bit constants and
typed records; it does not copy or emit the original instruction bytes.

The local C89 probe and the native VC1.52c Win98 probe both pass. Full
integration with the original DGROUP heaps, object arrays, and the remaining
visibility passes is still required before formal review promotion.

`SetVisPFThings` is represented by `reconstructed/src/set_vis_pf_things.c`.
The race-stat 6 path treats owned freighter things as visibility sources,
uses the original squared radius and cloak attenuation for nearby fleets, and
updates the distinct freighter, packet, mine-record, and owner-target masks.
It then marks nearby planets with detail 2 or 3 according to the source
planet's starbase knowledge threshold. The race-stat 5 path uses the owned
object's stored squared range and the original random cloak comparison for
unowned fleets. Both paths expose marking and randomness as typed callbacks.
The local C89 probe and native VC1.52c Win98 probe pass; full integration with
the original DGROUP object arrays remains open.

`SetVisPFPlanets` is represented by `reconstructed/src/set_vis_pf_planets.c`.
The typed source keeps the original four passes separate: ordinary
planet-centered scanning, race-stat-7 stargate scanning, special-thing mask
updates, and cloak-attenuated fleet detection. It names the scanner and deep
radii, starbase-dependent detail-2/detail-3 decisions, the viewer mask, and
the four progress-gauge callbacks. The local C89 probe and native VC1.52c
Win98 probe pass; full integration with the original DGROUP records remains
open.
