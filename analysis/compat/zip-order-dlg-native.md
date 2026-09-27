# ZipOrderDlg native checkpoint

## Current Step-5 status

The packed order/name storage unit is now closed as E1. The retained
`C:\ZODINT` host/native matrix passes 11 checks over four 24-byte records,
five-word copying, names and ampersands, validity cleanup, Rename persistence,
and Cancel closure. E2 localized resources and E3 complete live Win16 and
original differential integration remain open.

The E2 resource checkpoint is now closed in a fresh `C:\ZOD` run. The probe
uses the original resource IDs and text: compressed title `0x231` / `Customize
Zip Orders`, caption `0x4c1` / `Custom Orders`, empty-slot `0x4c8` / `Empty
Custom Slot`, and action IDs `0x6d`–`0x71` for `(no action)`, `Load All
Available`, `Unload All`, `Load Exactly...`, and `Unload Exactly...`.
`runs/vmctl-vc152zod/ZODSTA-RES.TXT` reports compile/link pass and
`ZODR-RES.TXT` reports `ZIP_ORDER_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`.
The redirected console is empty and the final window list contains no Program
Error dialog. Only the broader live Win16/original differential E3 unit
remains.

The E3 integration checkpoint is now closed in a fresh `C:\ZOD` USER run
extended with an actual Win16 parent window and child controls. The native
report `runs/vmctl-vc152zodmsg/ZODMSG.TXT` reports
`ZIP_ORDER_DLG_USER_CHECKS=12 FAILURES=0 RESULT=PASS`; compile/link passed,
the real loop dispatched paint, radio selection, Create, Rename, Delete,
control-color, Help, and OK, and the final window list contained no Program
Error dialog. The original `Rename Fleet` caller observation remains
preserved in `runs/vmctl-vc152current/fleet-rename.bmp`. Together these close
Step 5 and promote `ZipOrderDlg` to 6/6.

## Earlier native checkpoint (historical)

The final fresh Win98 guest run is preserved in `runs/vmctl-vc152zod/`.

- `ZODSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `ZODCL.LOG`: VC++ 1.52c compiled `ZIP.C` and `PROBE.C` without warnings
- `ZODLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `ZODR.TXT` and `ZODR2.TXT`: `ZIP_ORDER_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`
- `ZOD-CONSOLE.TXT`: zero bytes after redirected execution

`ZIP.C` was transferred directly from the reviewed
`reconstructed/src/zip_order_dlg.c` file. The native probe exercises
initialization, radio-frame geometry, erase/control-color handling, empty
rename fallback, paint, selection, create, delete, help, and OK close/tutor
paths through the typed callback boundary.

This advances `ZipOrderDlg` from Phase 2 step 3/6 to step 4/6. Packed order
storage, localized resources, original rename-dialog behavior, and complete
Win16 integration remain open.
