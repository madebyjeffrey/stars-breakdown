# ProductionDlg complete direct-resource E2 checkpoint

The original `ProductionDlg` body has an exhaustive direct resource call set of
two localized entries: `CchGetString(0x00d3, ...)` for the left-click help
popup and `PszGetCompressedString(0x04c0)` for the right-click queue menu. The
raw procedure contains no other string/resource lookup. The reconstructed
source preserves both IDs, the lookup-before-dispatch order, and suppression
outside the help diamond. The live original Win98 dialog supplies an
additional boundary check: the right-click menu contains `<Default>` and
`<Customize>`, with `<Customize>` occupying the editor entry. The host/native
probe therefore treats the `0x00d3` text as callback data rather than claiming
an unverified localized spelling, while the `0x04c0` fixture is `<Customize>`.

The strict host C89 probe passes:

`PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS`

The corrected packet was rerun in the resumed Win98 `StarDev` guest under the
fresh `C:\PDGRS4` directory. The native report is identical; `PDGRSSTA.TXT` reports
`COMPILE=PASS` and `LINK=PASS`, the process exited 0, redirected console output
is empty, and the post-run window inventory contains no Program Error dialog.

This closes the direct resource-ID/callback-order contract for `ProductionDlg`
(E2). It does not claim that the entire localized resource table has been
recovered. `InitializeProductionDlg`'s separate title resource `0x033e` is
already covered by its own reviewed and natively validated ledger entry; it is
not an additional direct lookup in `ProductionDlg`. Original queue differential
and final promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/10d0_1204_ProductionDlg.c`
- `tests/production_dlg_resource_probe.c`
- `reconstructed/src/production_dlg.c`
- `compat/vc152/production-dlg-resource-e2/`
- `runs/vmctl-vc152pdgrs3/`
- `runs/vmctl-vc152pdgrs3/original-production-right-real.png`
- `runs/vmctl-vc152pdgrs3/original-production-left-real.png`
- `runs/vmctl-vc152pdgrs3/RINFO-left.txt`
- `runs/vmctl-vc152pdgrs4/`
