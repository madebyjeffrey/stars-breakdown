# DoBombing reconstruction

`reconstructed/src/do_bombing.c` is a typed semantic reconstruction of bombing resolution. It selects eligible enemy planets, applies attack and defense scaling, enforces the minimum damage rule, updates population, buildings, environment, minerals, owner/fleet reports, and completion/depletion state, while excluding starbases as in the original path.

Validation: `tests/do_bombing_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and two-case native probe pass in `runs/vmctl-vc152ew/DBMSTA.TXT` and `runs/vmctl-vc152ew/DBMRES.TXT`.
