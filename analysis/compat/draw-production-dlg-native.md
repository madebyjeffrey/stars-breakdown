# DrawProductionDlg native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dpd/`.

- `DPDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DPDCL.LOG`: VC++ 1.52c compiled `DPD.C` and `PROBE.C` without warnings
- `DPDLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `DPDR.TXT`: `DRAW_PRODUCTION_DLG_CHECKS=1 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`DPD.C` was transferred directly from the reviewed
`reconstructed/src/draw_production_dlg.c` file. The native probe exercises DC
acquisition/release, source and one-based planet-list selection normalization,
cost-row rendering, right-aligned mineral values, optional ETA dispatch, and
help-diamond placement.

This advances `DrawProductionDlg` from Phase 2 step 3/6 to step 4/6. Packed
production globals, exact formatting/resource tables, localized strings, and
full GDI integration remain open.

## Expanded formatting checkpoint

The current source was then synchronized into fresh `C:\DPD3`. The updated
host and native probes preserve the original decimal-cost and mineral-color
boundaries: values `1`, `20`, `300`, and `4000` are formatted through the
service boundary for both source and planet selections, with the four mineral
label colors and normal-text restoration observed.

- `runs/vmctl-vc152dpd3/DPDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `runs/vmctl-vc152dpd3/DPDCL.LOG`: VC++ 1.52c compiled `DPD.C` and `PROBE.C`
- `runs/vmctl-vc152dpd3/DPDLINK.LOG`: segmented link completed
- `runs/vmctl-vc152dpd3/DPDR.TXT`:
  `DRAW_PRODUCTION_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`
- `runs/vmctl-vc152dpd3/DPD-CONSOLE.TXT`: empty
- the post-run guest window list contained no Program Error dialog

This strengthens `DrawProductionDlg`'s Phase 2 step-5 evidence. Packed
production globals, complete resource-table comparison, localized strings,
and full GDI integration remain open, so the procedure is not promoted.

## Packed production-global checkpoint

The fresh `C:\DPD4` packet adds the recovered packed source-list and
planet-list handoff. The host/native probe checks both exact 32-bit words,
the planet list's one-based selection normalization, recorded-count bounds,
and preservation of the decoded-cost callback path.

- `runs/vmctl-vc152dpd4/DPDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `runs/vmctl-vc152dpd4/DPDCL.LOG`: VC++ 1.52c compiled `DPD.C` and `PROBE.C`
- `runs/vmctl-vc152dpd4/DPDLINK.LOG`: segmented link completed
- `runs/vmctl-vc152dpd4/DPDR.TXT`:
  `DRAW_PRODUCTION_DLG_CHECKS=17 FAILURES=0 RESULT=PASS`
- `runs/vmctl-vc152dpd4/DPD-CONSOLE.TXT`: empty
- the post-run window list contained no Program Error dialog

This closes the packed production-global E1 unit. Exact resource-table and
localized-string comparison plus full GDI/caller integration remain open, so
DrawProductionDlg remains at `1/3` Step-5 evidence units and is not promoted.
