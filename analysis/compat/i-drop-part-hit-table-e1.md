# IDropPart packed slot hit table — E1

This checkpoint closes the packed slot-rectangle evidence unit for
`IDropPart`. The source now accepts the recovered slot rectangle table and
uses Win16 `PtInRect` semantics directly: left/top edges are included and
right/bottom edges are excluded. The callback hit tester remains the fallback
for callers that have not supplied the packed table.

The strict host C89 probe reports
`I_DROP_PART_HIT_TABLE_E1_CHECKS=12 FAILURES=0 RESULT=PASS`. It covers hits in
all three slots, same-slot classification, empty-side classification, every
rectangle edge boundary, and deliberate conflict with a callback that would
otherwise reject every hit.

The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152idphit/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same 12-check result. The process exited
with status 0 and the post-run window inventory showed no Program Error dialog.
Drag-state mutation, redraw/control integration, and complete original drop
behavior remain open.
