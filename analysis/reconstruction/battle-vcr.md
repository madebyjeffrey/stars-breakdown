# BattleVCR reconstruction

`reconstructed/src/battle_vcr.c` is a typed semantic reconstruction of battle-playback setup, preserving compact/expanded board selection, battle-record lookup, token and damage workspace setup, step initialization, tutor/dialog dispatch, missing-record alerts, and cleanup.

Validation: `tests/battle_vcr_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/BVRSTA.TXT` and `runs/vmctl-vc152ew/BVRRES.TXT`.
