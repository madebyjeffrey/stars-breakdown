# FDeleteBattlePlan reconstruction

`reconstructed/src/delete_battle_plan.c` is a typed semantic reconstruction of battle-plan deletion, preserving confirmation, index shifting, list storage, and current-plan state updates.

Validation: `tests/delete_battle_plan_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DBPSTA.TXT` and `runs/vmctl-vc152ew/DBPRES.TXT`.
