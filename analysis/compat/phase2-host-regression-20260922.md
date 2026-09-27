# Phase 2 host regression — 2026-09-22

This checkpoint revalidates the current Phase 2 combat and order boundaries
while the Win98 guest agent is disconnected. It is host-only regression
evidence and does not substitute for the pending native replay.

Focused strict-C89 reports:

- `F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS`
- `SATISFY_ORDERS_FULL_TURN_E3_CHECKS=18 FAILURES=0 RESULT=PASS`

The full Python regression suite reports `Ran 519 tests in 38.438s`,
`OK (skipped=153)`. Header generation remains at 2,241 layout assertions and
846 function declarations. `python3 tools/baseline.py --check` and
`git diff --check` also pass.

No native evidence is claimed here: the live VMCTL1 listener has no guest
connection/banner. The prepared native packets remain queued for the next
manual `C:\VMCTL1\STARTAGT` startup, with FAttack E15 first in the queue.
The named Step-5 counters and promotion counts therefore remain unchanged.
