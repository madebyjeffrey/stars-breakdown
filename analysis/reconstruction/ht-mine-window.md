# HtMineWindow reconstruction

`reconstructed/src/ht_mine_window.c` is a typed semantic reconstruction of mine-window hit testing and placement, preserving coordinate transforms, mine selection, visibility, and dialog callbacks.

Validation: `tests/ht_mine_window_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/HMWSTA.TXT` and `runs/vmctl-vc152ew/HMWRES.TXT`.
