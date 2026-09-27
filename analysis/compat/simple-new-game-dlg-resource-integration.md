# SimpleNewGameDlg resource and geometry checkpoint

This checkpoint closes E2 for `SimpleNewGameDlg`. The typed resource contract
matches recovered NE resource `0xD1`: a 240 by 190 dialog using an 8-point
font and 16 controls. It checks every control ID, rectangle, resource-text
length, and resource-text checksum, including the original off-screen
`Advanced Game...` placement that is repositioned by the dialog procedure.

The strict host C89 probe reports 117/117 checks. The fresh VC++ 1.52c/Win98
run in `runs/vmctl-vc152sngdr/` reports compile/link success and
`SIMPLE_NEW_GAME_DLG_RESOURCE_E2_CHECKS=117 FAILURES=0 RESULT=PASS`; the
post-run window inventory showed no Program Error dialog. The package and
CRLF wrappers are preserved under
`compat/vc152/simple-new-game-dlg-resource/`.

E1 packed-player storage is already closed. Only E3 creation mutation/caller
integration and final promotion remain.
