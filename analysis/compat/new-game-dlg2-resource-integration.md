# NewGameDlg2 resource and geometry checkpoint

This checkpoint closes E2 for `NewGameDlg2`. The typed resource contract
matches recovered NE resource `0x0187`, the second Advanced New Game page: a
261 by 210 dialog using an 8-point font with the five navigation controls
Finish, Help, Cancel, Back, and Next. Every control ID, rectangle, text
length, and text checksum is checked independently from the source contract.

The strict host C89 probe reports
`NEW_GAME_DLG2_RESOURCE_E2_CHECKS=40 FAILURES=0 RESULT=PASS`. The fresh VC++
1.52c/Win98 run in `runs/vmctl-vc152nd2res2/` reports compile/link success
and the same 40-check result; the process exited with status 0 and the
post-run window inventory showed no Program Error dialog. The failed first
package compile is preserved separately in `runs/vmctl-vc152nd2res/`; it was
caused by a stale header that predated the new contract declaration and was
not a source failure.

The original player-row menu and three-page wizard observation remains in
`analysis/compat/new-game-flow-integration.md`. E1 packed race/password
storage and E3 complete wizard-path replay remain open.
