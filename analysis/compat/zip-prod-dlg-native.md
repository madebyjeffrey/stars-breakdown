# ZipProdDlg native checkpoint

## Current Step-5 closure

The final remaining integration unit is now closed. A fresh `C:\ZPDLAY`
Win98 harness compiled the maintained `FillZipProdLB` source into a real
listbox/message loop and reports
`FILL_ZIP_PROD_LB_USER_CHECKS=9 FAILURES=0 RESULT=PASS` in
`runs/vmctl-vc152zpd-e3/FZPMSG2.TXT`. It covers full and empty/invalid queue
population, list reset/add boundaries, invalidation, paint, and actual USER
message delivery. The build and link reports are `FZPMSGSTA3.TXT`,
`FZPMSGCL3.LOG`, and `FZPMSGLINK3.LOG` in the same run directory; the final
window list contained no Program Error dialog.

The original Win98 dialog was also exercised from the real Planet Summary
Report. The preserved sequence shows Add, a second Add with changed required
minerals/resource totals, Item Down, Item Up, Clear, Add then Remove, and
Cancel back to the report without an error dialog. The screenshots are in
`runs/vmctl-vc152zpd-e3/`. Along with the 36 packed/layout checks and the
14-control USER loop already recorded below, this closes the complete
Step-5 acceptance boundary. `ZipProdDlg` is promoted to Phase 2 step 6/6.

## Earlier checkpoints (historical)

The fresh Win98 guest run is preserved in `runs/vmctl-vc152zpd/`.

- `ZPDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `ZPDCL.LOG`: VC++ 1.52c compiled `ZPD.C` and `PROBE.C` without warnings
- `ZPDLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `ZPDR.TXT`: `ZIP_PROD_DLG_CHECKS=21 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`ZPD.C` was transferred directly from the reviewed
`reconstructed/src/zip_prod_dlg.c` file. The native probe exercises dialog
initialization, queue-label creation, paint and erase/control-color handling,
radio selection, queue deletion, production-item packing, rename/create,
help, dirty-state tracking, focus, tutor advancement, and accept/close.

This advances `ZipProdDlg` from Phase 2 step 3/6 to step 4/6. Packed global
queue/name records, production-record bit layout, exact localized resources,
rename-dialog behavior, and full Win16 control integration remain open.

## Expanded packed-queue checkpoint

The maintained source probe was synchronized into a fresh `C:\ZPD2` package.
The expanded matrix adds rejected production records, the maximum packed
kind/code word (`0xffc6`), and the original twelve-item queue cap. VC++ 1.52c
compile and link passed, and the Win98 report is
`ZIP_PROD_DLG_CHECKS=29 FAILURES=0 RESULT=PASS`. The process exited 0, the
post-run window list showed no Program Error dialog, and no redirected console
file was produced.

Evidence is preserved in `runs/vmctl-vc152zpd2/`,
`compat/vc152/zip-prod-dlg/ZPD.C`,
`compat/vc152/zip-prod-dlg/PROBE.C`, and
`tests/zip_prod_dlg_source_probe.c`. This strengthens step 5/6 by closing the
production-word rejection, maximum-word, and queue-cap cases; packed global
queue/name storage, exact localized resources, rename-dialog behavior, and
full Win16 control integration remain open.

## Recovered packed-storage checkpoint

The CodeView type records and the original initialization path identify the
global storage boundary precisely: `_zipprodq` is 40 bytes, `vrgZipProd` is
an array of five records, and each record contains a 13-byte name at offset
0, `fValid` at 13, `fNoResearch` at 14, `cpq` at 15, and twelve packed 16-bit
queue words beginning at offset 16. The typed source and `FillZipProdLB` were
corrected to use that layout rather than an 80-byte byte-array surrogate.

The strict host probe now reports `ZIP_PROD_DLG_CHECKS=36 FAILURES=0
RESULT=PASS`, including size, offset, five-record storage, name mutation,
packed-word, rejection, and twelve-entry-cap checks. A fresh `C:\ZPDLAY`
VC++ 1.52c package independently reports the same
`ZIP_PROD_DLG_CHECKS=36 FAILURES=0 RESULT=PASS` in
`runs/vmctl-vc152zpdlay/ZPDR.TXT`; compile and link both pass, and the final
window list contains no Program Error dialog. This closes the packed
queue/name storage unit inside Step 5. The broader production-queue mutation
and exact command/resource boundary remains open.

## Real USER/message-loop checkpoint

The maintained source was then run through an actual Win16 USER window and
message loop in a fresh `C:\ZPDMSG` package. The harness created the parent
window, four queue radio buttons, the queue list, Create/Rename/Delete/Help,
and OK/Cancel controls, then dispatched the reconstructed procedure for
initialization, paint, erase-background, control-color, radio selection,
delete, create, rename, help, and accept.

- `runs/vmctl-vc152zpdmsg/ZPDMSGSTA5.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `runs/vmctl-vc152zpdmsg/ZPDMSGCL5.LOG`: VC++ 1.52c compile log; only the
  expected near-pointer-to-long warnings are present
- `runs/vmctl-vc152zpdmsg/ZPDMSGLINK5.LOG`: segmented link completed
- `runs/vmctl-vc152zpdmsg/ZPDMSG3.TXT`: `ZIP_PROD_DLG_USER_CHECKS=14`,
  `FAILURES=0`, `RESULT=PASS`
- `runs/vmctl-vc152zpdmsg/native-screen-final.bmp`: post-run screen capture;
  the window list contained no Program Error dialog

The report records seven dispatched loop messages, two paint passes, three
erase passes, the selected queue, two packed production items, dirty bit
`0x20`, one help action, two rename callbacks (create and rename), and an OK
result. This closes the real USER/control/resource evidence unit. It does not
yet close the packed global queue/name storage or the full production-queue
mutation boundary, so the procedure remains at Phase 2 step 5/6.
