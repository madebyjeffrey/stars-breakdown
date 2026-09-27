# ShowTooltip reconstruction

`reconstructed/src/show_tooltip.c` is a typed semantic reconstruction of tooltip display, preserving hit-tested control selection, text lookup, placement, visibility, and hide/empty behavior.

Validation: `tests/show_tooltip_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/TSTA.TXT` and `runs/vmctl-vc152ew/TRES.TXT`.
