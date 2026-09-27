# SweepForMines reconstruction

`reconstructed/src/sweep_for_mines.c` is a typed semantic reconstruction of mine-sweeping resolution, preserving sweep eligibility, range/state checks, mine removal, and resulting fleet/game updates.

Validation: `tests/sweep_for_mines_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SWSTA.TXT` and `runs/vmctl-vc152ew/SWRES.TXT`.
