# GetVCRStats reconstruction

`reconstructed/src/get_vcr_stats.c` is a typed semantic reconstruction of battle-playback statistics extraction, preserving token counts, damage totals, player grouping, and empty-record behavior.

Validation: `tests/get_vcr_stats_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/GVSSTA.TXT` and `runs/vmctl-vc152ew/GVSRES.TXT`.
