# SimpleNewGameDlg creation and caller-boundary checkpoint

This checkpoint closes E3 for `SimpleNewGameDlg`. The focused integration
probe exercises the acceptance boundary with selected density, map size, and
race-list entry; verifies that those values are written to dialog state before
the `EndDialog(0x430)` caller boundary; and verifies the custom-race path's
load/wizard/save, list-delete/add/limit, and focus-restoration sequence. It
also checks that Cancel returns `0xD4` without changing the accepted state.

The strict host C89 probe reports
`SIMPLE_NEW_GAME_DLG_INTEGRATION_E3_CHECKS=12 FAILURES=0 RESULT=PASS`. The
fresh VC++ 1.52c/Win98 package in
`compat/vc152/simple-new-game-dlg-integration/` reports the same result in
`runs/vmctl-vc152sngdi/SNGDIE3.TXT`; compile and link both passed, the process
exited with status 0, and the post-run window inventory showed no Program
Error dialog.

Together with E1 packed-player storage, E2 resource/control geometry, and the
original six-page custom-race Save As/cancel observation, this closes the
procedure's Step-5 evidence boundary. The review record and derived ledgers
then promote `SimpleNewGameDlg` to Phase 2 step 6/6.
