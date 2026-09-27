# SimpleNewGameDlg reconstruction

`reconstructed/src/simple_new_game_dlg.c` is a source-level reconstruction of
`SimpleNewGameDlg` (`0010:76aa-7e91`, 2,023 bytes). It keeps the Win16 dialog
services behind typed callbacks while making the density/size radio groups,
turn-list setup, color handling, sticky positioning, accept/cancel decisions,
help dispatch, and race-wizard handoff ordinary C logic.

The focused probe exercises initialization, the original default density and
map-size radio selections including the raw no-selection sentinel fallback,
list population, accepted setup, custom-race
load/wizard/save flow, seven-entry list rotation and limit notification, focus
restoration, and cancellation. Host strict C89 compilation and the focused
probe pass. A fresh VC1.52c/Win98 checkpoint also compiles, links, and reports
five native checks without a runtime error dialog. The packed-player E1
follow-up copies the complete 192-byte temporary record into the selected
new-player slot and preserves all 15 neighboring slots; host and native
reports pass 3,074/3,074 with no Program Error dialog. The E2 resource
follow-up now checks all 16 controls in recovered resource 0xD1, including
exact rectangles and resource-text checksums; host and native reports pass
117/117 with no Program Error dialog. The E3 integration follow-up checks
accepted density/size/selection state at the EndDialog caller boundary,
custom-race load/wizard/save and list mutation, focus restoration, and cancel
preservation; host and native reports pass 12/12 with no Program Error dialog.
Evidence is in `analysis/compat/simple-new-game-dlg-integration-e3.md` and
`runs/vmctl-vc152sngdi/`.

The original Win98 executable has now been exercised through the custom-race
integration boundary: `Customize Race...` opened all six Custom Race Wizard
pages, `Finish` reached `Save As` with the `Stars! Race Files` type, and
canceling returned cleanly through the wizard and parent dialog. Evidence is
preserved in `analysis/compat/simple-new-game-integration.md` and
`runs/vmctl-vc152current/`. The packed-player evidence is preserved in
`analysis/compat/simple-new-game-dlg-packed-integration.md` and
`runs/vmctl-vc152sngdp/`. E1, E2, and E3 now close the complete Step-5
acceptance boundary; the procedure is promoted at Phase 2 step 6/6.
