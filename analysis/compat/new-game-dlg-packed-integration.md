# NewGameDlg packed-game checkpoint

The first NewGameDlg Step-5 evidence unit is now closed. The optional packed
64-byte `_game` adapter writes the recovered settings fields: map size at
offset 4, density at 6, start distance at 12, the seven option bits in the
word at 16, and the game name at 32. It preserves unrelated game-record
bytes, including the high option-word bits.

The host and fresh VC++ 1.52c/Win98 probe each report
`NEW_GAME_DLG_PACKED_E1_CHECKS=7 FAILURES=0 RESULT=PASS`. Compile and link
pass, the process exits 0, and the final Win98 window inventory has no Program
Error dialog. Exact resources/control mapping and complete creation replay
remain open.

Evidence:

- `tests/new_game_dlg_packed_probe.c`
- `compat/vc152/new-game-dlg-packed/`
- `runs/vmctl-vc152ngdp/NGDPSTA.TXT`
- `runs/vmctl-vc152ngdp/NGDPCL.LOG`
- `runs/vmctl-vc152ngdp/NGDPLINK.LOG`
- `runs/vmctl-vc152ngdp/NGDPE1.TXT`
- `analysis/decompiler/export/raw/1078_7e92_NewGameDlg.c`
- `reconstructed/src/new_game_dlg.c`
