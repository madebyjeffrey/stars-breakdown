# IDropPart drag-state mutation — E2

This checkpoint closes the drag-state mutation evidence unit for `IDropPart`.
The focused matrix exercises capacity-limited merges, source exhaustion,
no-modify preservation, source-less merges, zero-capacity fallback, and
incompatible rejection. It also checks the callback order at the mutation
boundary.

The strict host C89 probe reports
`I_DROP_PART_MUTATION_E2_CHECKS=11 FAILURES=0 RESULT=PASS`. It verifies target
and source packed quantity updates, actual moved amounts, preservation when
modification is disabled, the source-index `-1` path, capacity fallback, and
the `USIA`/`SIA`/`B` service-call sequences.

The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152idpmut/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same 11-check result. The process exited
with status 0 and the post-run window inventory showed no Program Error dialog.
Redraw/control integration, complete original drop behavior, and Step-6
promotion remain open.
