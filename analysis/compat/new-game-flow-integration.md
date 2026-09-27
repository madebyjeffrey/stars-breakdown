# New-game flow step-5 integration checkpoint

This checkpoint covers the shared original-executable boundary for
`NewGameDlg`, `NewGameWizard`, and `NewGameDlg2`.

The original `C:\STARS!\STARS.EXE` was exercised in the connected `StarDev`
Win98 guest. `New Game` opened the settings dialog; `Advanced Game...` opened
the three wizard pages; a player row opened the race/computer/no-player menu;
and `Finish` reached `Choose New Game Name`. Canceling the existing-name
replacement returned to the wizard without a Program Error dialog. The
screenshots are preserved in `runs/vmctl-vc152current/` and the exact window
titles and sequence are recorded in
`analysis/compat/new-game-flow-integration.json`.

The instruction boundary is split across the raw exports for
`NewGameDlg`, `NewGameWizard`, and `NewGameDlg2`. The native checkpoints remain
the focused VC++ 1.52c/Win98 probes in `runs/vmctl-vc152ngd/`,
`runs/vmctl-vc152nwt/`, and `runs/vmctl-vc152nd2/`.

This advances each of the three procedures from Phase 2 step 4/6 to step 5/6
for the original dialog/wizard/menu/name-flow boundary. Packed player and
race records, localized resources, exact Win16 control messages and geometry,
and final promotion remain open.
