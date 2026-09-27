# DzMoveRangeToConsider reconstruction

`reconstructed/src/dz_move_range_to_consider.c` is a typed semantic reconstruction of movement-range evaluation, preserving distance calculation, target-class filtering, and range-to-consider updates. The host probe links the shared `battle.c` and `md_target.c` helpers.

Validation: `tests/dz_move_range_to_consider_source_probe.c` plus those helpers passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DZSTA.TXT` and `runs/vmctl-vc152ew/DZRES.TXT`.
