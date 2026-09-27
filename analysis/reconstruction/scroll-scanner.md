# ScrollScanner reconstruction

`reconstructed/src/scroll_scanner.c` is a typed semantic reconstruction of scanner scrolling, preserving range bounds, scroll positions, selection visibility, and redraw callbacks.

Validation: `tests/scroll_scanner_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ev/SSSTA.TXT` and `runs/vmctl-vc152ev/SSRES.TXT`.
