# ProductionDlg native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152pdg/`.

- `PDGSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `PDGCL.LOG`: VC++ 1.52c compiled `PDG.C` and `PROBE.C` without warnings
- `PDGLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `PDGR.TXT`: `PRODUCTION_DLG_CHECKS=9 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`PDG.C` was transferred directly from the reviewed
`reconstructed/src/production_dlg.c` file. The native probe exercises dialog
initialization and low-planet navigation disablement, paint and erase paths,
control-color handling, screen-cursor help hit testing, owner-draw list items,
measure-item height, command forwarding, and left/right popup dispatch.

This is the focused native portion of the Phase 2 step-5 evidence. The
original Ship & Starbase Designer observation in
`analysis/compat/production-dialog-integration.md` supplies the complementary
original-behavior checkpoint. Packed production globals, exact child geometry,
localized resources, and complete popup/command integration remain open, so
the procedure is not ready for step 6 promotion.

## Expanded popup-result checkpoint

The current source was synchronized into fresh `C:\PDG2`. The updated host and
native probes now preserve the right-click popup result: a valid saved queue
selection is forwarded through the typed selection boundary, while the final
editor entry opens the queue dialog boundary.

- `runs/vmctl-vc152pdg2/PDGSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `runs/vmctl-vc152pdg2/PDGCL.LOG`: VC++ 1.52c compiled `PDG.C` and `PROBE.C`
- `runs/vmctl-vc152pdg2/PDGLINK.LOG`: segmented link completed
- `runs/vmctl-vc152pdg2/PDGR.TXT`:
  `PRODUCTION_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`
- `runs/vmctl-vc152pdg2/PDG-CONSOLE.TXT`: empty
- the post-run guest window list contained no Program Error dialog

This strengthens the Phase 2 step-5 evidence. Packed production globals,
exact child geometry, localized popup resources, complete queue-control
integration, and promotion remain open.
