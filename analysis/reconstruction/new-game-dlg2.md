# NewGameDlg2

`NewGameDlg2` is represented as a typed dialog procedure in
`reconstructed/src/new_game_dlg2.c`. It preserves the second-page row hit
testing, read-only gates, race-button command validation, help/alert paths,
popup action decoding, create/open/edit/delete outcomes, encoded computer and
player selections, contiguous player-type propagation, invalidation, focus,
dialog termination, and the raw `WM_CONTROL_COLOR` (`0x19`) control-handle
recognition, button-face background-color, and brush-return boundary.

Win16 menu construction and tracking, localized labels, packed race/player
records, password checks, race-file handling, and window-coordinate/layout
details remain callback-owned. The host probe covers the normal and failure
paths. A fresh VC++ 1.52c/Win98 harness in `runs/vmctl-vc152nd2/` compiles
and links the current source and reports
`NEW_GAME_DLG2_CHECKS=29 FAILURES=0 RESULT=PASS` without a runtime error
dialog. Win16 menu/resource integration, packed race records, and exact
window layout remain open, so this remains a partial semantic source. The E2
resource follow-up now matches recovered resource `0x0187` (261x210, 8-point
font, five navigation controls) and passes 40/40 host and native checks
without a Program Error dialog. The E3 transition follow-up passes 9/9 host
and native checks for accepted command results, caller-stage dispatch, alert
gating, read-only behavior, and player-row race propagation. The E1 packed
follow-up now commits all bytes of the recovered 192-byte player record and
13-byte race-file slot for Create, Open, and password-accepted Edit paths,
rejects password failure without mutation, and preserves neighbors; host and
native reports pass 1,238/1,238 without a Program Error dialog. The procedure
is promoted to Phase 2 step 6/6.

The original Win98 executable has now been exercised through the player-row
menu boundary: Step 2 of the Advanced Game wizard opened the
`Predefined Race`, `Custom Race`, `Computer Player`, and `No Player` actions,
and the wizard proceeded to Step 3 and the name-finalization dialog without a
Program Error. Evidence is preserved in
`analysis/compat/new-game-flow-integration.md` and
`runs/vmctl-vc152current/`. The complete Step-5 boundary is now synchronized
and `NewGameDlg2` is promoted at Phase 2 step 6/6.
