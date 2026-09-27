# Simple new-game step-5 integration checkpoint

The original `C:\STARS!\STARS.EXE` was exercised in the connected `StarDev`
Win98 guest. From the `New Game` dialog, `Customize Race...` opened the
six-page `Custom Race Wizard`. Completing the pages reached the original
`Save As` dialog with the file type `Stars! Race Files`; canceling returned
through the wizard and parent dialog without a Program Error. Screenshots and
the exact sequence are preserved in `runs/vmctl-vc152current/` and
`analysis/compat/simple-new-game-integration.json`.

The raw acceptance boundary is `SimpleNewGameDlg` at
`analysis/decompiler/export/raw/1078_76aa_SimpleNewGameDlg.c`, including the
`RaceCreationWizard` dispatch, list reset, focus restoration, and save/cancel
path. The focused native checkpoint remains
`runs/vmctl-vc152sngd/SNGDR.TXT`.

This advances `SimpleNewGameDlg` from Phase 2 step 4/6 to step 5/6 for the
original custom-race and save-dialog boundary. Packed player records,
localized resources, exact geometry/control integration, and promotion remain
open.
