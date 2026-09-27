# ProductionDlg real USER-loop checkpoint

The reconstructed dispatcher was exercised through a real Win98 USER message
loop in fresh `C:\PDGU5`. The harness creates an overlapped window and the
dialog's child controls, then queues initialization, `WM_COMMAND`, left-click,
and two right-click messages. The window procedure routes real `WM_PAINT` and
`WM_ERASEBKGND` messages as well as the queued command/mouse traffic through
`stars_production_dlg`.

The native report is:

`PRODUCTION_DLG_USER_CHECKS=12 FAILURES=0 RESULT=PASS`

It records one initialization, one command dispatch, one help popup, two queue
menus, sparse-slot selection of queue 2, accepted editor mutation, two
compressed-string lookups, and 74 messages through the USER loop. VC++ 1.52c
reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0, the redirected
console file is empty, and the post-run window inventory had no Program Error
dialog.

This closes the native USER/message-loop sub-boundary of E3. It does not close
the original-executable queue differential, packed production globals,
complete localized resource comparison, or final ledger promotion.

Evidence:

- `reconstructed/src/production_dlg.c`
- `compat/vc152/production-dlg-user-e5/`
- `runs/vmctl-vc152pdgu5/`
