# ProductionDlg original queue-boundary E3 checkpoint

The resumed Win98 original executable was returned to the actual
`Production Queue for Oop Be Gone` dialog and explicitly refocused through the
game window before the probe. A real right-button event on the drawn template
diamond opened the production-template menu with `<Default>` and
`<Customize>`. A second real mouse event selected `<Customize>` and opened the
original `Customize Production Templates` dialog. Pressing Escape canceled
that editor and returned to the original ProductionDlg. The retained
screenshots show the parent, menu, editor, and post-cancel states; the window
inventory contained no Program Error dialog.

This closes ProductionDlg's original queue differential boundary when combined
with the already-promoted ZipProdDlg original sequence: queue-menu entry
construction, editor transition, cancel return, and the Add/Remove/Item
Up/Item Down/Clear/resource-total behavior are now covered at the original
executable boundary. The typed host/native queue packets cover the matching
sparse-slot mapping, accepted/canceled mutation, USER dispatch, and unsigned
capacity clamp.

Evidence:

- `analysis/decompiler/export/raw/10d0_1204_ProductionDlg.c`
- `analysis/compat/zip-prod-dlg-integration.md`
- `runs/vmctl-vc152zpd-e3/`
- `runs/vmctl-vc152pdgrs3/production-refocus2.png`
- `runs/vmctl-vc152pdgrs3/original-production-right-real.png`
- `runs/vmctl-vc152pdgrs3/original-production-customize-real2.png`
- `runs/vmctl-vc152pdgrs3/original-production-after-customize-cancel.png`

ProductionDlg is promoted to Phase 2 step 6/6. Its separate direct resource
contract remains scoped to IDs and callback order; no unverified `0x00d3`
localized spelling is being claimed.
