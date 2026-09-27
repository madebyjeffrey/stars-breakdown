# New-game creation integration checkpoint

This checkpoint closes the complete creation-transition evidence unit for
`NewGameDlg` and `NewGameWizard`. The host probe accepts the reconstructed
settings dialog, verifies the packed game record, advances through wizard
pages 1–3, commits one packed player, accepts the game name, restores the
game-bit word, generates the world, and dispatches to the host callback.

The strict host C89 probe reports 11/11 checks. The fresh VC++ 1.52c/Win98
run in `runs/vmctl-vc152ngint/` reports compile and link success and
`NEW_GAME_CREATION_E3_CHECKS=11 FAILURES=0 RESULT=PASS`; the post-run window
inventory showed no Program Error dialog. The package and wrappers are
preserved under `compat/vc152/new-game-creation-integration/`.

The original executable replay used for the caller boundary is preserved in
`analysis/compat/new-game-flow-integration.md` and
`runs/vmctl-vc152current/`: New Game opened the settings dialog, Advanced
Game opened all three wizard pages, the player menu was reachable, Finish
reached Choose New Game Name, and the flow returned without a Program Error.

This closes E3 for `NewGameDlg` and `NewGameWizard`. `NewGameDlg` consequently
had no numbered Step-5 evidence units left and has now been promoted to 6/6.
`NewGameWizard` still has E2 open for localized resources and control geometry.
This is an integration acceptance packet, not a claim that
every callback-owned Win16 resource or full game-world implementation has
been reconstructed.
