# NewGameDlg2 transition and caller-boundary checkpoint

This checkpoint closes E3 for `NewGameDlg2`. The integration probe verifies
that accepted second-page commands reach the caller through `EndDialog` with
the expected page result, that the caller-stage mapping is preserved, that a
missing first player is rejected with the original alert, that read-only
acceptance bypasses the editable-only alert gate, and that a player-row popup
can create and propagate a race type before acceptance.

The strict host C89 probe reports
`NEW_GAME_DLG2_INTEGRATION_E3_CHECKS=9 FAILURES=0 RESULT=PASS`. The fresh
VC++ 1.52c/Win98 run in `runs/vmctl-vc152nd2int/` reports compile/link
success and the same 9-check result; the process exited with status 0 and the
post-run window inventory showed no Program Error dialog. The original
three-page Advanced New Game replay and player-row menu observation remain
preserved in `analysis/compat/new-game-flow-integration.md` and
`runs/vmctl-vc152current/`.

E1 packed race/password storage remains the only named Step-5 unit before
promotion.
