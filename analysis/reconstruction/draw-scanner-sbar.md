# DrawScannerSBar reconstruction

`DrawScannerSBar` (`000c:62d8-6AFF`, 2,087 bytes) renders the scanner status
bar's two rows of lock-light boxes.  The reviewed source in
`reconstructed/src/draw_scanner_sbar.c` keeps the original paint intersection
and caller-owned DC lifetime, full-redraw background, planet/waypoint and
coordinate fields, object naming, distance/location text, and restoration of
the GDI state.

The original `_sbar`, scanner selection, and segmented resource pointers are
represented by `StarsDrawScannerBarState`.  The Win16 handles, string tables,
object-name lookup, distance formatting, and lock-light drawing are explicit
callbacks in `StarsDrawScannerBarOps`; this makes the source ordinary C while
preserving the service boundaries used by the executable.

Validation:

- Host C89 compilation passed with warnings treated as errors.
- The source probe exercised caller-owned DC acquisition/release, full redraw,
  lock-light boxes, planet naming, coordinates, and distance/location text.
- The same source and probe compiled and linked under VC++ 1.52c in StarDev;
  native evidence is `runs/vmctl-vc152ex/sbars/SSBARSTA.TXT` and
  `runs/vmctl-vc152ex/sbars/SSBARRES.TXT`.

This is semantic C-source evidence, not an assertion that this adapter's
instruction bytes match the original executable.
