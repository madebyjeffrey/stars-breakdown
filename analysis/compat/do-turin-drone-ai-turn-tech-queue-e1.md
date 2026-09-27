# DoTurinDroneAiTurn E1 group-8..9 production milestone

Original `CheckAiShdefStatus` returns the sum of eligible group designs'
existing counts, saturated at 32,000 (`1090_9b10_CheckAiShdefStatus.c:27-62`).
Turin stores that return for designs 8–9 at raw line 140. Its later
production branch at raw lines 280–294 computes the target as the greater
of twice the AI-data word at `vlpbAiData+2` and the player's planet count
divided by ten. When player technology byte 2 exceeds four and an 8–9
design is selected, a total below the target queues one selected design.
A total below `target*10/7` instead consumes `Random(4)` and queues only
when that call returns zero. At or above the expanded threshold, no random
call or queue write occurs.

The Turin source now accepts these typed table/service results and a random
callback, preserving that short-circuit order before later production loops.
It refuses a missing queue callback when this policy is enabled and a
missing RNG callback when the expanded-threshold branch needs one; it does
not silently claim to have completed the original action. The strict-C89
matrix covers direct, RNG-accepted and RNG-rejected queues, threshold,
technology and selection bypasses, the max-of-two target, and callback
failures. Host and fresh VC++ 1.52c/Win98 `C:\TURTQ2` both report
`TURIN_TECH_QUEUE_CHECKS=21 FAILURES=0 RESULT=PASS`. The native compile,
link, and process exit pass, and the post-run window list shows no Program
Error. Package: `compat/vc152/turin-tech-queue-e1-r2/`; logs, map, object,
executable, report, and agent log: `runs/vmctl-turtq2/`. The earlier
`C:\TURTQ1` 17-check packet is retained as historical evidence before the
missing-callback contract was tightened.

One full host-suite run passed 536 tests, with 153 expected skips, 2,241
layout assertions, and 846 declarations. Two other broad-suite attempts
terminated early with exit 138; their cause is not established, so this is
not claimed as a consistently stable full-suite gate. The focused Turin
probe and native replay pass.

This is supporting E1 evidence, not closure. The design-1 four-item branch,
minelayer RNG/fleet scan, support-fleet choices, live packed table binding,
actual production-service side effects, and original-turn differential
remain. In particular, the current queue callback records only design and
quantity, not the full four-argument `AddItemToQueue` request needed by the
minelayer path.
