# ZipProdDlg reconstruction

## Final Phase 2 status

`ZipProdDlg` is complete at Phase 2 step 6/6. The original Win98 dialog was
exercised through the Planet Summary Report with Add, Remove, Item Up, Item
Down, Clear, resource-total changes, and Cancel. A fresh native listbox USER
harness passes `FILL_ZIP_PROD_LB_USER_CHECKS=9 FAILURES=0 RESULT=PASS`; the
36 packed/layout checks and 14-control dialog USER loop also remain passing.
The combined evidence closes the last Step-5 mutation/resource boundary and
the review ledgers record promotion.

## Earlier reconstruction checkpoints (historical)

`ZipProdDlg` is a typed C reconstruction of the 2,400-byte NE
`001b:5490–5df0` production-queue dialog procedure.  It models four typed
five packed zip-production queues, queue names, the selected queue, the resource-tech
flag, the static list position, dirty state, and the original dialog message
paths.

Initialization preserves the title and control layout setup, radio selection,
button enablement, list refresh, default queue labels, sticky positioning, and
tutor transition.  Paint handling keeps the raised frame, localized title and
resource-tech label callbacks; erase and control-color handling retain the
button-face behavior.  Command handling covers queue selection, delete,
rename/create, help, accept/cancel close, focus, dirty-state updates, and
production-queue construction capped at the original twelve packed entries.

`tests/zip_prod_dlg_source_probe.c` covers these paths with 21 strict host C89
checks. A fresh VC++ 1.52c/Win98 checkpoint now compiles, links, and runs the
exact source with all 21 native checks passing. This advances the procedure to
Phase 2 step 4/6. The packed global queue/name records, production-record bit
layout, exact localized resources, rename dialog, and Win16 control handles
remain callback-owned integration boundaries. The original Win98
`Production Queue for Oop Be Gone` dialog was also observed with the available
item list, selected-queue list, queue controls, required-mineral totals, and
template/research options; see
`analysis/compat/zip-prod-dlg-integration.md`.

The packed queue boundary was expanded to 29 strict C89/native cases. The
additional cases reject negative and out-of-range production records, verify
the maximum packed kind/code word (`0xffc6`), and verify that construction is
capped at the original twelve entries. The fresh `C:\ZPD2` VC++ 1.52c run
passed with no Program Error dialog. This closes those production-word and
queue-cap concerns within step 5/6; packed global queue/name storage, exact
localized resources, rename-dialog behavior, and full Win16 control handles
remain callback-owned integration boundaries.

## Packed global storage correction

The recovered CodeView declarations identify `_zipprodq` as a 40-byte record,
not an 80-byte byte-array surrogate. Its exact fields are a 13-byte `name` at
offset 0, `valid` at 13, `no_research` at 14, `item_count` at 15, and twelve
16-bit packed words at offset 16. The global `vrgZipProd` storage contains five
records. The typed `StarsZipProductionQueue`, `StarsZipProdDlgState`, and
`FillZipProdLB` implementation now preserve those widths, offsets, and array
boundaries.

The host and native probes each pass 36 checks, including the recovered
layout, five-record storage, name mutation, packed words, invalid-record
rejection, and the twelve-entry cap. The fresh native report is
`runs/vmctl-vc152zpdlay/ZPDR.TXT`; compile and link pass and no Program Error
dialog appears. This closes the packed queue/name storage Step-5 unit. The
remaining Step-5 unit is the broader production-queue mutation/command and
exact resource boundary before promotion.

## Real USER/message-loop evidence

The exact reconstructed source was also compiled and linked into a fresh
`C:\ZPDMSG` Win98 executable with a real parent window and child controls.
The message loop dispatched initialization, paint, erase-background,
control-color, queue selection, delete, create, rename, help, and OK. The
native report `runs/vmctl-vc152zpdmsg/ZPDMSG3.TXT` passes all 14 checks and
records the selected queue, two packed items, dirty bit `0x20`, paint/erase
activity, both rename callbacks, and successful teardown. This closes the
Step-5 control/resource integration unit; packed global queue/name storage and
the broader production-queue mutation boundary remain open before promotion.
