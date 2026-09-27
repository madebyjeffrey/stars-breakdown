# DrawRadarCircle reconstruction

`DrawRadarCircle` is the scanner radar-disc accumulator at NE
`000c:4e7c-540b` (1,423 bytes). The reviewed source is
[`reconstructed/src/draw_radar_circle.c`](../../reconstructed/src/draw_radar_circle.c).

The source keeps the original clip-intersection gate, fixed-width midpoint and
radius calculation, four-corner clip coverage test, bounded circle table,
contained-circle replacement/rejection, hollow XOR-style drawing transitions,
flush behavior, and immediate drawing when the table is full. GDI operations
and `INTERSECTRECT` are typed callbacks; the circle ownership and redundancy
decisions are ordinary C state transitions.

Validation:

- host C89 probe: `RADAR_CIRCLE_CHECKS=3 FAILURES=0 RESULT=PASS`;
- native VC1.52c compile/link: `runs/vmctl-vc152ex/radar-circle/RDSTA.TXT`;
- native Win98 execution: `runs/vmctl-vc152ex/radar-circle/RDR.TXT`, with
  `RADAR_CIRCLE_CHECKS=3 FAILURES=0 RESULT=PASS`.
