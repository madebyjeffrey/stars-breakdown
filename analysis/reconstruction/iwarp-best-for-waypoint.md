# IWarpBestForWaypoint reconstruction

`IWarpBestForWaypoint` selects the warp setting for a fleet waypoint at NE
`000c:7a18-8074` (1,628 bytes). The reviewed source is
[`reconstructed/src/iwarp_best_for_waypoint.c`](../../reconstructed/src/iwarp_best_for_waypoint.c).

The reconstruction retains the original decision stages: ideal and AI warp
lookup, special-order eligibility, enemy-colony fuel pressure, fleet-statistic
thresholds, cargo-hull handling, AI clamping, minimum-warp enforcement,
distance-derived rounding, stargate promotion to warp 11, and the final
9/11-bound clamp. Fleet tables, planet ownership, fuel/statistic calculations,
and technology policy are typed callbacks rather than raw far-pointer or
emitted-byte substitutions.

Validation:

- host C89 probe: `IWARP_BEST_CHECKS=3 FAILURES=0 RESULT=PASS`;
- native VC1.52c compile/link: `runs/vmctl-vc152ex/iwarp-best/IWSTA.TXT`;
- native Win98 execution: `runs/vmctl-vc152ex/iwarp-best/IWR.TXT`, with
  `IWARP_BEST_CHECKS=3 FAILURES=0 RESULT=PASS`.
