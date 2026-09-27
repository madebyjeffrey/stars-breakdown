# NewGameWizard packed-player checkpoint

The first NewGameWizard Step-5 evidence unit is now closed. The optional
packed-player adapter writes only the fields owned by the reconstructed wizard
and preserves the copied race/design bytes: the flags word at `_player+6`,
the all-ones/randomized salt at `_player+12`, the dead marker at `_player+84`,
and the normal/display names at `_player+128` and `_player+160`.

The host probe and fresh VC++ 1.52c/Win98 package each report
`NEW_GAME_WIZARD_PACKED_E1_CHECKS=11 FAILURES=0 RESULT=PASS`. The native build
and link both pass, the process exits 0, and the final Win98 window inventory
has no Program Error dialog. This closes E1, but does not promote the
procedure: localized resources/control geometry and the complete creation
transition remain open.

Evidence:

- `tests/new_game_wizard_packed_probe.c`
- `compat/vc152/new-game-wizard-packed/`
- `runs/vmctl-vc152nwp/NWPSTA2.TXT`
- `runs/vmctl-vc152nwp/NWPCL2.LOG`
- `runs/vmctl-vc152nwp/NWPLINK2.LOG`
- `runs/vmctl-vc152nwp/NWPE1.TXT`
- `analysis/decompiler/export/raw/1078_6022_NewGameWizard.c`
- `reconstructed/src/new_game_wizard.c`
