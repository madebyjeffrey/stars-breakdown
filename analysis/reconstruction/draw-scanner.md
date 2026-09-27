# DrawScanner reconstruction

`DrawScanner` (`000c:108a-47D1`, 14,151 bytes) is the main scanner viewport
renderer from `scan.c`.  The reviewed source in
`reconstructed/src/draw_scanner.c` replaces the packed segmented tables and
direct GDI sprite-sheet accesses with typed planet, fleet, thing, selection,
and waypoint records plus explicit drawing services.

The source retains the important control flow: scrollbar exclusion and
viewport expansion, inverted galaxy-coordinate conversion, fleet linking,
radar and near-radar collection, radar-percent scaling, visibility and
selection gates, planet/fleet/thing marker dispatch, fleet counts, waypoint
paths, caller-owned DC acquisition/release, and clip restoration.  Resource
handles, bitmap sheets, packed relation tables, and the original GDI marker
primitives remain service callbacks so the C source does not encode emitted
bytes or assembly fragments.

Validation:

- Host C89 compilation passed with warnings treated as errors.
- The source probe exercised DC ownership, viewport clipping, background fill,
  radar collection, all three object families, fleet counts, and paths.
- The same source and probe compiled and linked under VC++ 1.52c in StarDev;
  the final clean native build is recorded in
  `runs/vmctl-vc152ex/scanner/SCANRSTA3.TXT`, with the probe report in
  `runs/vmctl-vc152ex/scanner/SCANRRES2.TXT`.

This is semantic C-source evidence, not a claim that the adapter's generated
instruction bytes match the original executable.
