# MineClick E3 mine-window integration checkpoint

This packet closes the named Step-5 E3 boundary for `MineClick`. The
MineClick dispatcher is now wired to the real typed
`stars_popup_mineral_scan_choices` selector rather than a routing-only stub.
The integration probe checks the planet/fleet/thing menu markers, selected
fleet decoding, friendly-fleet redraw ordering, main-object selection, fleet
cargo dispatch, double-click suppression, and the remaining popup hit-code
families.

The strict host probe reports:

```text
MINE_CLICK_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS
```

A fresh VC++ 1.52c / Win98 package in `C:\MCLIE3` compiles the exact current
`MineClick` and `PopupMineralScanChoices` sources, links successfully, and
produces the same native report. The process exits 0, redirected console
output is empty, and the post-run window inventory contains no Program Error
dialog. The original Win98 mine-window selection-arrow observation remains
the caller-side behavior evidence: it advanced `Oop Be Gone` to `Armed Probe
#1` and redrew the dependent panes without an error dialog.

This closes MineClick E3. Together with E1 and E2, all three named Step-5
evidence units are complete; the procedure is ready for Step-6 ledger
promotion.

Evidence: `tests/mine_click_integration_probe.c`,
`reconstructed/src/mine_click.c`,
`reconstructed/src/popup_mineral_scan_choices.c`,
`compat/vc152/mine-click-integration-e3/`, and
`runs/vmctl-vc152mclint3/`.
