# ZipOrderDlg packed-record integration checkpoint

## Current Step-5 status

The packed-record evidence in this checkpoint closes E1, the stored-order
layout and mutation boundary. The four 24-byte `_ziporder` records, five
order-word copy, name/ampersand handling, validity cleanup, rename
persistence, and Cancel closure all pass in both host and fresh VC++ 1.52c
runs. The remaining Step-5 units are E2 localized resource/command agreement
and E3 complete live Win16/original differential integration.

The packed adapter decodes four recovered 24-byte `_ziporder` records: five
order words at offsets 0–9, the stored name at offset 10, and the validity byte
at offset 23. It also provides a packed order source with the five words at
offset 12 of a recovered order record, matching the original Create copy
boundary.

The checkpoint verifies raw record sizing and decoding, initialization from
packed validity/name/word state, selection, Create copying all five order
words, name synchronization through the dialog text boundary, ampersand
handling, Delete validity cleanup, Rename persistence, and Cancel closure.

Host strict C89 and native VC++ 1.52c reports are:

`ZIP_ORDER_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152zodint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This adds packed order-storage and create/delete/rename evidence within Phase
2 step 5/6. Localized resources, exact original dialog/resource behavior,
complete live Win16 integration, original differential coverage, and final
promotion remain open.

Evidence:

- `tests/zip_order_dlg_packed_probe.c`
- `compat/vc152/zip-order-dlg-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1080_0000_ZipOrderDlg.c`
- `analysis/reconstruction/zip-order-dlg.md`
- `runs/vmctl-vc152zodint/ZODISTA.TXT`
- `runs/vmctl-vc152zodint/ZODICL.LOG`
- `runs/vmctl-vc152zodint/ZODILINK.LOG`
- `runs/vmctl-vc152zodint/ZODINT.TXT`
- `runs/vmctl-vc152zodint/native-screen.bmp`
