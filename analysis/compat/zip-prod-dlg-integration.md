# ZipProdDlg original Win98 integration checkpoint

## Final Step-5 closure and promotion

The original dialog was exercised through the real Planet Summary Report in
Win98. The retained screenshots show the queue being opened, populated with
two items, reordered in both directions, cleared, repopulated and removed,
with the required-mineral/resource totals changing after item selection and
Cancel returning to the report. No Program Error dialog appeared. See
`runs/vmctl-vc152zpd-e3/` for `queue-open.bmp`, `queue-add-one.bmp`,
`queue-add-two.bmp`, `queue-moved.bmp`, `queue-up.bmp`, `queue-clear.bmp`,
`queue-remove.bmp`, and `after-cancel.bmp`.

The companion fresh Win98 `FillZipProdLB` USER harness passes
`FILL_ZIP_PROD_LB_USER_CHECKS=9 FAILURES=0 RESULT=PASS` and covers real
listbox population, invalid/empty reset, full-list and add boundaries,
invalidation, paint, and message-loop delivery. This closes the remaining
Step-5 mutation/list/resource boundary. The procedure is now at Phase 2
step 6/6 pending no further work.

## Earlier Step-5 observations (historical)

This is the step-5 original-executable boundary for `ZipProdDlg`. In the live
`StarDev` Win98 game, the production command opened the original
`Production Queue for Oop Be Gone` dialog.

The dialog displayed the available production-item list, an empty selected
queue, Add, Remove, Item Up, Item Down, and Clear controls, required-mineral
totals, the production-template option, the research-contribution option, and
OK/Cancel navigation. No program-error dialog appeared.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Dialog title: `Production Queue for Oop Be Gone [#32770]`.
- Recovered boundary: `analysis/decompiler/export/raw/10d0_5490_ZipProdDlg.c`.
- Native checkpoint: `runs/vmctl-vc152zpd/` and
  `analysis/compat/zip-prod-dlg-native.md`.
- Original screenshot: `runs/vmctl-vc152current/production-queue-popup.bmp`.

This advances `ZipProdDlg` from 4/6 to 5/6. Packed queue/name records, exact
production bit layout, localized resources, rename behavior, and complete
Win16 control integration remain open before promotion.

## Native USER integration unit

The follow-up `C:\ZPDMSG` run exercises the exact source through a real Win16
parent window and message loop. It creates the queue radios, list box, queue
buttons, Help, and OK/Cancel controls, then dispatches initialization,
paint/erase/control-color, queue selection, delete, create, rename, help, and
accept. VC++ 1.52c compile and link pass, and
`runs/vmctl-vc152zpdmsg/ZPDMSG3.TXT` reports
`ZIP_PROD_DLG_USER_CHECKS=14 FAILURES=0 RESULT=PASS` with no Program Error
dialog. This closes the control/resource integration unit inside Step 5.

The remaining Step-5 units are now explicit: packed global queue/name storage
and the broader Add/Remove/Item Up/Item Down/Clear production-queue mutation
boundary. The separate Step-6 promotion item remains after those two units.

## Packed queue/name storage unit

The recovered CodeView and initialization evidence establish the global
storage used by the original dialog: five 40-byte `_zipprodq` records, with a
13-byte name, validity/no-research flags, an item count, and twelve 16-bit
packed queue words at the recorded offsets. The typed source was corrected to
match this layout. Host and native probes both pass 36 checks, including the
record offsets, name mutation, packed words, rejection cases, and the
twelve-entry cap. The native package is preserved in
`runs/vmctl-vc152zpdlay/`.

This closes the packed queue/name Step-5 unit. One broader Step-5 unit remains:
the complete production-queue mutation/command boundary and its exact
resource behavior, followed by Step-6 promotion.
