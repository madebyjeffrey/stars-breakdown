# BattlePlansDlg reconstruction

`reconstructed/src/battle_plans_dlg.c` is a typed semantic reconstruction of the battle-plan editor. It keeps a working copy of the selected 36-byte plan, maps tactic and target selections into the original packed words, handles manual mode, and commits changes through the change logger when the selection or dialog closes. The list, creation, deletion, tutor, help, and network-player enable/disable paths are represented through callbacks.

Validation: the strict host C89 source probe passes (`tests/battle_plans_dlg_source_probe.c`), and the VC1.52c Win98 build and five-case native probe pass in `runs/vmctl-vc152e/BPSTA.TXT` and `runs/vmctl-vc152e/BPRES.TXT`.
