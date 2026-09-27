# Production dialog step-5 integration checkpoint

The original generated game was continued in the connected `StarDev` Win98
guest. `Commands > Ship Design...` opened `Ship & Starbase Designer`. The
original dialog displayed Ships and Starbases modes; Existing Designs,
Available Hull Types, Enemy Hulls, and Components views; design/component
images; cost and statistics; and copy/delete/edit controls. No Program Error
dialog appeared. Screenshots are preserved in `runs/vmctl-vc152current/` and
the exact boundary is recorded in
`analysis/compat/production-dialog-integration.json`.

The raw boundaries are `ProductionDlg` and `DrawProductionDlg`; their focused
VC++ 1.52c/Win98 checkpoints remain `runs/vmctl-vc152pdg/PDGR.TXT` and
`runs/vmctl-vc152dpd/DPDR.TXT`.

This advances both procedures from Phase 2 step 4/6 to step 5/6 for the
original designer dialog/rendering boundary. Packed production globals, queue
and design records, exact formatting/resource tables, Win16 control geometry,
and promotion remain open.
