# BrowserDlg reconstruction

`BrowserDlg` is a typed C reconstruction of the 2,462-byte NE
`001c:1ed8–2876` research-browser dialog procedure.  The source represents
the dialog's hull-group and component-item state explicitly and preserves the
original initialization geometry: metric-dependent dialog sizing, sticky
positioning, label-width measurement, control placement, browser-child
creation, compressed component-label population, and tutor notification.

The message paths cover paint and erase handling, button-face control-color
selection, cancel and destroy cleanup, component-combo selection changes, and
previous/next component navigation.  Navigation retains the original bounded
lookup retries, empty-combo group stepping, item wrapping, hidden-part
predicate, component invalidation, and close-menu state transitions.

`tests/browser_dlg_source_probe.c` covers these paths with 19 strict host C89
checks. A fresh VC++ 1.52c/Win98 checkpoint now compiles, links, and runs the
exact source with all 19 native checks passing. This advances the procedure to
Phase 2 step 4/6. The packed `vpartBrowser` record, exact component lookup
table, localized string storage, and USER control/message handles remain
callback-owned integration boundaries. The original Ship & Starbase Designer
Components view was also observed in Win98 with its group selector, icon/list
browser, scrollbar, and Done/Help controls; see
`analysis/compat/browser-dlg-integration.md`.

The packed item-word boundary was expanded to 20 strict C89/native cases. The
probe now starts with group bits in the high byte and verifies that component
selection and previous/next navigation modify only the low-byte item index
(`0x1201` → `0x1200` → `0x1201`). The fresh `C:\BRD2` VC++ 1.52c run passed
without a Program Error dialog. This closes the packed item-word preservation
and navigation-boundary concerns inside step 5/6; exact lookup tables,
localized storage, complete USER control/message integration, and promotion
remain callback-owned boundaries.

The exact component-group table is now recovered from the original code
segment at `CS:1EB5`: a sentinel followed by sixteen normal group values. The
host/native probe was expanded to 24 checks and verifies selection mapping plus
empty-combo next/previous wrapping across those entries while retaining the
packed item high byte. Fresh `C:\BRD3` VC++ 1.52c compile/link and execution
passed without a Program Error dialog. This closes the exact lookup-table
boundary inside step 5/6; localized storage, complete USER control/message
integration, packed `vpartBrowser` payload consumption, original differential
coverage, and promotion remain open.

The initialization resource sequence is now checked separately: a fresh
`C:\BRD4` native run verifies the exact compressed-string IDs `0x43F` through
`0x44F` in order (17 entries), with `BROWSER_DLG_CHECKS=25 FAILURES=0
RESULT=PASS` and no Program Error dialog. This narrows the localized-storage
boundary to the actual string payloads and USER control/message integration;
packed `vpartBrowser` payload consumption, original differential coverage, and
promotion remain open.

The typed `_compart` payload boundary is now consumed through fresh `C:\BRD6`
validation. The 33-check matrix materializes and reads the recovered 52-byte
record fields—ID, technology bytes, name, mass, resource cost, ore costs, and
bitmap ID—after BrowserDlg selects group 8 and group `0x10` entries. VC1.52c
compile/link and execution passed without a Program Error dialog. Original
payload values, localized string contents, complete USER control/message
integration, original differential coverage, and promotion remain open.

The packed `vpartBrowser` boundary is now exercised through fresh `C:\BRD5`
native validation. The 31-check matrix round-trips the packed group/item
header and recovered `FLookupPart` table offsets for group `0x0008` (base
`0x04E0`, stride `0x36`, 12 entries) and group `0x0010` (base `0x2450`,
stride `0x3C`, 24 entries) across initialization, navigation, and selection.
The report passed with no Program Error dialog. Actual component payloads,
localized string contents, complete USER control/message integration, original
differential coverage, and promotion remain open.

The original component payloads are now materialized from `target/stars.exe`
instead of synthetic values. The 35-check `C:\BRD7` matrix consumes the
recovered 12-entry group `0x0008` and 24-entry group `0x0010` tables and checks
actual `_compart` technology masks, names, mass/resource costs, ore costs, and
bitmap IDs through the BrowserDlg lookup boundary. VC++ 1.52c compile/link and
execution passed with no Program Error dialog. Localized string payloads,
complete USER control/message integration, original differential coverage, and
promotion remain open.

The localized payload boundary is now materialized as well. The 36-check
`C:\BRD8` matrix decodes and checks the actual text for all 17 original
compressed-string IDs `0x43F` through `0x44F`, from `All` through `Torpedoes`,
using the recovered decoder data boundary. VC++ 1.52c compile/link and
execution passed with no Program Error dialog. Complete USER control/message
integration, original differential coverage, and promotion remain open.

## Final Phase 2 acceptance

The real USER/message-loop adapter in fresh `C:\BRDMSG` passes
`BROWSER_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`, covering actual control
creation, all 17 component labels, paint/erase dispatch, Previous/Next
commands, invalidation, and Done teardown. A direct original caller-path run
then exercised `Commands > Ship Design... > Components`, selected `Quick Jump
5` and `Long Hump 6`, changed the group to Armor (`Tritanium`/`Crobmnium`),
selected `Tritanium`, and changed to Beam Weapons (`Laser`/`X-Ray Laser`).
The designer remained usable and no Program Error dialog appeared.

Together with the packed-header, original-payload, localized-string, and
`_compart` checks above, this closes the BrowserDlg acceptance boundary. The
review and coverage ledgers are synchronized and BrowserDlg is promoted to
Phase 2 step 6/6.
