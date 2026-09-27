# BrowserDlg native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152brd/`.

- `BRDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `BRDCL.LOG`: VC++ 1.52c compiled `BRD.C` and `PROBE.C`; it emitted one
  C4769 near-pointer-to-long warning at the intentional combo string pointer
  transport in `BRD.C`, but no errors
- `BRDLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `BRDR.TXT`: `BROWSER_DLG_CHECKS=19 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`BRD.C` was transferred directly from the reviewed
`reconstructed/src/browser_dlg.c` file. The native probe exercises metric-based
initialization and layout, browser-child creation, localized combo population,
paint/erase/control-color handling, component selection, previous/next
navigation, lookup wrapping, invalidation, cancel/destroy cleanup, tutor
advancement, and default dispatch.

This advances `BrowserDlg` from Phase 2 step 3/6 to step 4/6. Packed
`vpartBrowser` records, exact component lookup tables, localized string
storage, and full USER control/message integration remain open.

## Expanded packed-item checkpoint

The maintained source probe was synchronized into a fresh `C:\BRD2` package.
The expanded matrix preserves the packed high byte while changing the
low-byte component index during selection and previous/next navigation:
`0x1201` becomes `0x1200` and returns to `0x1201` without losing the group
bits. VC++ 1.52c compile and link passed; the existing intentional C4769
near-pointer warning remains at the combo string transport. The Win98 report
is `BROWSER_DLG_CHECKS=20 FAILURES=0 RESULT=PASS`, the process exited 0, and
the post-run window list showed no Program Error dialog.

Evidence is preserved in `runs/vmctl-vc152brd2/`,
`compat/vc152/browser-dlg/BRD.C`,
`compat/vc152/browser-dlg/PROBE.C`, and
`tests/browser_dlg_source_probe.c`. This strengthens step 5/6 by closing the
packed item-word preservation and navigation-boundary cases; exact lookup
tables, localized storage, complete USER control/message integration, and
promotion remain open.

## Expanded group-table and resource-sequence checkpoints

The fresh `C:\BRD3` run verifies the recovered `CS:1EB5` component-group
table, including empty-combo wrap, with
`BROWSER_DLG_CHECKS=24 FAILURES=0 RESULT=PASS`. A second fresh `C:\BRD4` run
verifies the initialization resource IDs `0x43F` through `0x44F` in order and
reports `BROWSER_DLG_CHECKS=25 FAILURES=0 RESULT=PASS`. Both builds compile and
link successfully, retain only the intentional C4769 near-pointer warning,
exit normally, and show no Program Error dialog. These checkpoints narrow the
remaining scope to actual localized payloads, packed `vpartBrowser` data,
complete USER control/message behavior, original differential coverage, and
promotion.

## Packed `vpartBrowser` checkpoint

The fresh `C:\BRD5` run round-trips the packed group/item header through the
dialog path and checks the recovered `FLookupPart` table offsets for group
`0x0008` (`0x04E0 + item * 0x36`, 12 entries) and group `0x0010`
(`0x2450 + item * 0x3C`, 24 entries). The native report is
`BROWSER_DLG_CHECKS=31 FAILURES=0 RESULT=PASS`; compile/link passed, the
process exited normally, and no Program Error dialog appeared. Actual payload
records, localized string contents, complete USER control/message behavior,
original differential coverage, and promotion remain open.

## Packed component-payload checkpoint

The fresh `C:\BRD6` run consumes the recovered 52-byte `_compart` layout after
group 8 and group `0x10` selection: ID, technology bytes, name, mass, resource
cost, ore costs, and bitmap ID. The native report is
`BROWSER_DLG_CHECKS=33 FAILURES=0 RESULT=PASS`; compile/link passed, the
process exited normally, and no Program Error dialog appeared. Original
component payload values, localized strings, complete USER control/message
behavior, original differential coverage, and promotion remain open.

## Original component-payload checkpoint

The fresh `C:\BRD7` package uses the payload bytes recovered from the
preserved `target/stars.exe`, rather than synthetic component contents. It
checks the 52-byte `_compart` fields for representative records from both
original lookup tables: technology masks, names, mass, resource cost, ore
costs, and bitmap IDs. The native report is
`BROWSER_DLG_CHECKS=35 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited normally, and the post-run window list showed no Program Error
dialog. Localized string payloads, complete USER control/message behavior,
original differential coverage, and promotion remain open.

Evidence is preserved in `runs/vmctl-vc152brd7/` and the synchronized probe
sources under `tests/browser_dlg_source_probe.c` and
`compat/vc152/browser-dlg/PROBE.C`.

## Localized compressed-string checkpoint

The fresh `C:\BRD8` package extends the resource-sequence check from IDs to
actual decoded text. It checks the 17 original labels for IDs `0x43F` through
`0x44F`, including first/last text and a combined payload sum of 15229. The
native report is `BROWSER_DLG_CHECKS=36 FAILURES=0 RESULT=PASS`; compile and
link passed, and no Program Error dialog appeared. Complete USER
control/message integration, original differential coverage, and promotion
remain open.
