# Rototill second-sweep scout dispatch

Status: **source caller and low-fuel fallback verified; RT-E2 open**.

The original `DoRototillAiTurn` at NE 20:~28D0 tests ownership, slots 7/8,
order count, slot 1, `FIsAiTransport`, slots 13/14, then slot 0. For a scout,
it selects the fleet and either reaches `IdTargetScout` or, when the first
hull-slot byte equals 1 and fuel is below 2, selects again and changes the
first PLORD order kind to 5. The reconstruction now owns these gates and
selection order in `rototill_scout_dispatch`. Its `target_scout` service is
the still-open target-search and `FMoveAiFleet` boundary; it returns a
boolean service result, rather than the original helper's status code.

The strict-C89 source probe checks normal callback sequence `IST`, transport
skip `I`, low-fuel sequence `ISSR` with the first order changing from kind 2
to 5, fMark clearing, and malformed packed-order preflight. It passes
10/10 host checks. The fresh [Win16 r15 return](../../runs/rototill-production-e1-r15/)
pins the source/package hashes and returned artifacts in `MANIFEST.JSON`.
`BLDSTAT.TXT` says `COMPILE=PASS`, `LINK=PASS`; the VC++ 1.52c driver report
has `SCOUT_CHECKS=10 RESULT=PASS` and 103/103 combined checks. Build and
probe processes both exited 0. Compiler logs, map, objects, executable,
report, and agent log are retained. No original Stars! run was made.

The [retained selector-3 turn](do-rototill-original-turn-r1.md) supplies the
fleet-0 input and original planet-26 output. Current shared C status and
nearest-unknown helpers also select 26 from that input on the host. The
Rototill `target_scout` service is not yet bound to those helpers, live
AI-table identity, wormhole/RNG, ideal warp, and packed order mutation.
Other second-sweep route families and a complete native/original turn
differential remain. This packet does not close RT-E2 or RT-E4.
