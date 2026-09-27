# NewGameDlg resource and geometry checkpoint

The second NewGameDlg Step-5 evidence unit is now closed. The readable source
exposes the recovered resource-0x0186 contract: a 261 by 210 dialog using
8-point text and all 27 controls, including the game-name edit, three radio
groups, seven option controls, and Help/Cancel/Back/Next/Finish buttons. Every
control ID and rectangle is checked against the retained original NE resource
layout.

The host and fresh VC++ 1.52c/Win98 probe each report
`NEW_GAME_DLG_RESOURCE_E2_CHECKS=30 FAILURES=0 RESULT=PASS`. Compile and link
pass, the process exits 0, and the final Win98 window inventory has no Program
Error dialog. Only the complete wizard mutation and original creation replay
remain for this procedure before promotion.

Evidence:

- `tests/new_game_dlg_resource_probe.c`
- `compat/vc152/new-game-dlg-resource/`
- `runs/vmctl-vc152ngdr/NGDRSTA.TXT`
- `runs/vmctl-vc152ngdr/NGDRCL.LOG`
- `runs/vmctl-vc152ngdr/NGDRLINK.LOG`
- `runs/vmctl-vc152ngdr/NGDPE2.TXT`
- `analysis/ne/layout.json`
- `analysis/decompiler/export/raw/1078_7e92_NewGameDlg.c`
- `reconstructed/src/new_game_dlg.c`
