# MineClick

`MineClick` is represented as a typed minefield/scan dispatcher in
`reconstructed/src/mine_click.c`. The source preserves hit-code routing,
double-click restrictions, mineral/terraform/object/fleet popup selection,
the mineral-graph threshold menu, unconditional mine-window invalidation and
conditional scanner invalidation boundary, mineral-scan
choice handling, scan-selection advancement, and fleet-cargo popup routing.

The original packed `GlobalPD`, planet, fleet, cargo, minefield, and part
records remain callback-owned, as do localized popup construction and the
derived mining/terraform arithmetic. The host probe covers every hit-code
family and the threshold/double-click paths; it also checks that the threshold
popup receives the selected-item index rather than the raw threshold value.
A fresh VC++ 1.52c/Win98 checkpoint now compiles, links, and runs the exact
source with the current 25 native assertions passing. The original Win98 mine-window
selection arrow was then observed advancing from `Oop Be Gone` to `Armed Probe
#1` and redrawing the dependent summary and fleet panes without a
program-error dialog; see `analysis/compat/mine-click-integration.md`. This
advances the procedure to Phase 2 step 5/6; packed record integration,
localized popup construction, and complete minefield behavior remain open.

The reviewed source was synchronized into a repaired fresh `C:\MCL3` package
with a directory-first VC1.52c wrapper. The native report was
`MINE_CLICK_CHECKS=25 FAILURES=0 RESULT=PASS`; redirected console output was
empty and the post-run window list contained no Program Error dialog. This
strengthens, but does not close, Phase 2 step 5/6; packed GlobalPD and
minefield records, localized popup construction, derived mining arithmetic,
and complete minefield integration remain open.

The packed-record E1 boundary is now closed. `MineClick` accepts optional raw
56-byte `_planet`, 124-byte `_fleet`, and 18-byte `_thing` records and decodes
their identity, owner, detail, environment bounds, fleet ship count, and
object point/kind fields into a typed GlobalPD-facing popup payload. The
callback-only path remains unchanged when no raw records are supplied. Strict
host C89 and a fresh VC++ 1.52c/Win98 `C:\\MCLE2` run both report
`MINE_CLICK_CHECKS=30 FAILURES=0 RESULT=PASS`; compile/link and process exit
pass, with no Program Error dialog. MineClick is now **1/3** at Step 5; E2
localized popup/derived arithmetic and E3 complete minefield integration
remain. Evidence is recorded in
`analysis/compat/mine-click-packed-e1.{md,json}` and
`runs/vmctl-vc152mcle2/`.

The localized/derived E2 boundary is now closed. The source maps the hit
families to the recovered popup kinds, obtains the localized resource ID, and
passes the selected planet variable, player bounds, terraform bounds, and
three mineral outputs through typed resolver callbacks before the final popup
callback. Strict host C89 and a fresh VC++ 1.52c/Win98 `C:\MCLE3` run both
report `MINE_CLICK_CHECKS=36 FAILURES=0 RESULT=PASS`; compile/link and process
exit pass, with no Program Error dialog. MineClick is now **2/3** at Step 5;
E3 complete minefield caller integration and final promotion remain. Evidence
is recorded in `analysis/compat/mine-click-e2.{md,json}` and
`runs/vmctl-vc152mcle3/`.

The complete mine-window caller boundary is now closed. The integration probe
wires `MineClick` case 9 to the real typed `PopupMineralScanChoices` selector
and checks planet/fleet/thing markers, selected-fleet decoding, friendly-fleet
redraw/main-selection ordering, fleet-cargo dispatch, double-click suppression,
and every remaining popup hit family. Strict host C89 and a fresh VC++ 1.52c/
Win98 `C:\MCLIE3` run report
`MINE_CLICK_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link and
process exit pass, with no Program Error dialog. The original Win98 selection
arrow evidence remains in `analysis/compat/mine-click-integration.md`. All
three named Step-5 evidence units are now closed; MineClick is ready for
Step-6 promotion. Evidence is recorded in
`analysis/compat/mine-click-integration-e3.{md,json}` and
`runs/vmctl-vc152mclint3/`.
