# NewGameDlg reconstruction

`NewGameDlg` is a typed C reconstruction of the 2,367-byte NE
`0010:7e92–87d1` new-game settings dialog procedure.  Its state names the
map-size, density, start-distance, game-name, read-only mode, and seven option
bits that the original stored in the packed game field.

The source preserves the three framed radio groups and localized labels during
paint, button-face erase/control-color handling, initialization of radio and
option controls, name length messaging, read-only disabling, and sticky dialog
positioning.  Command handling preserves help, race-button result selection,
radio scanning including the raw no-selection sentinel fallback, option-bit
packing, name retrieval/default fallback, cancel and accept end-dialog
results, and sticky cleanup.

`tests/new_game_dlg_source_probe.c` covers these paths with 22 strict host C89
checks. An optional packed 64-byte `_game` record now receives the recovered
settings fields at offsets 4, 6, 12, 16, and 32 while unrelated bytes remain
untouched. The focused host/native E1 probe reports 7/7 checks with compile,
link, and process success and no runtime error dialog. A second host/native
probe checks all 27 controls in the recovered 0x186 dialog resource and its
261 by 210 geometry. Only complete wizard mutation and original creation
replay remain open.

The original Win98 executable has now been exercised through this dialog's
acceptance boundary: the `New Game` dialog displayed its settings controls and
dispatched `Advanced Game...` into the three-page wizard, which reached the
name-finalization dialog without a Program Error. The shared creation probe
also closes the complete wizard-mutation/caller-transition unit with 11/11
host and native checks. Evidence is preserved in
`analysis/compat/new-game-creation-integration.md` and
`runs/vmctl-vc152ngint/`, alongside the original replay in
`analysis/compat/new-game-flow-integration.md`. `NewGameDlg` is now at `0 + P`:
all numbered Phase 2 step-5 units are closed and only final ledger promotion
remains.
