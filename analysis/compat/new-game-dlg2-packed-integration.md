# NewGameDlg2 packed race and password checkpoint

This checkpoint closes E1 for `NewGameDlg2`. Successful Create, Open, and
Edit race actions now commit the recovered 0xC0-byte temporary player record
and 13-byte DOS race-file entry into the selected slot, using explicit 16-bit
strides and preserving neighboring slots. Edit actions pass through the
password-check callback before mutation; a rejected password leaves the
selected record unchanged.

The strict host C89 probe reports
`NEW_GAME_DLG2_PACKED_E1_CHECKS=1238 FAILURES=0 RESULT=PASS`. The fresh VC++
1.52c/Win98 run in `runs/vmctl-vc152nd2pack/` reports compile/link success
and the same 1,238-check result; the process exited with status 0 and the
post-run window inventory showed no Program Error dialog.

Together with E2 resource/control geometry, E3 transition/caller evidence,
and the original three-page player-menu replay, this closes the complete
Step-5 boundary. The review record and derived ledgers promote
`NewGameDlg2` to Phase 2 step 6/6.
