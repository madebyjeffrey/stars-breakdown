# DrawProductionDlg resource-table E2 validation

The E2 host packet now reconciles the resource and table boundaries visible in
the original `DrawProductionDlg` instruction stream:

- `0x33f` supplies `Required Minerals:`;
- `0x2fe` supplies the `kT` separator used after the first three mineral
  quantities;
- `0x340` supplies `%d%% Done,   Completion ` for the planet-list ETA path;
- `0x0d2` supplies `Apply or define a production template` for the help
  diamond; and
- DGROUP `rgszMinerals` at offset `0x04cc` supplies the six-entry pointer
  table, of which the dialog uses entries 0, 1, 2, and 5:
  `Ironium`, `Boranium`, `Germanium`, and `Resources`.

The readable source now exposes the mineral pointer table directly and makes
the ETA format lookup observable before the ETA service call. The strict host
C89 probe passes:

`DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=0 RESULT=PASS`

The fresh VC++ 1.52c/Win98 packet in
`compat/vc152/draw-production-dlg-resource-e2/` compiled and linked in
`C:\DPDRE2`. Its native report is:

`DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=0 RESULT=PASS`

The process exited normally, the redirected console was empty, and the
post-run Win98 window list contained no Program Error dialog. This closes the
named E2 resource/formatting evidence unit. E3 full GDI/caller integration and
Step-6 promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/10d0_35dc_DrawProductionDlg.c`
- `analysis/reconstruction/game-strings.md`
- `reconstructed/src/draw_production_dlg.c`
- `reconstructed/include/core.h`
- `tests/draw_production_dlg_resource_probe.c`
- `compat/vc152/draw-production-dlg-resource-e2/`
- `runs/vmctl-vc152dpdre2/DPDRESTA.TXT`
- `runs/vmctl-vc152dpdre2/DPDRECL.LOG`
- `runs/vmctl-vc152dpdre2/DPDRELINK.LOG`
- `runs/vmctl-vc152dpdre2/DPDRE.TXT`
- `runs/vmctl-vc152dpdre2/DPDRE-CONSOLE.TXT`
