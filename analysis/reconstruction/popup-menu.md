# PopupMenu

`PopupMenu` is represented as typed menu-building logic in
`reconstructed/src/popup_menu.c`.  It preserves direct and submenu label
construction, separator handling, ampersand escaping, checked-item flags,
ID-flagged name resolution, right-button tracking, menu cleanup, and the
command-to-selection conversion.

Win16 menu APIs, localized/name lookup, and the caller-owned ID/label records
remain callback-owned.  The host probe covers direct labels, separators,
checked items, escaped ampersands, flagged name sources, tracking, cleanup,
and selection decoding. A fresh VC++ 1.52c/Win98 harness now compiles and
links the exact source and probe and reports `POPUP_MENU_CHECKS=22
FAILURES=0 RESULT=PASS` without a Program Error dialog. Localized/name
lookup, full Win16 menu API behavior, caller-owned records, and
original-behavior evidence remain open, so this is still a partial semantic
source.

The original Win98 executable has now been exercised through the caller menu
boundary: the Reports menu displayed the six report choices, and the Planet
Name header opened the forward/reverse sort popup without a Program Error.
Evidence is preserved in `analysis/compat/report-popup-integration.md` and
`runs/vmctl-vc152current/`. This was the original caller-boundary checkpoint;
the later packed/Win16 and E2 runs below close the remaining concerns.

The source boundary was tightened at the submenu header: the raw routine
passes `rgsz[i + 1]` directly to `AppendMenu`, so the typed source now
preserves that caller-owned label instead of attempting a kind-0 name lookup.
A fresh packed/Win16 integration harness in `C:\\PMUINT` exercised the direct,
flagged, separator, checked, submenu, tracking, and selection paths through
the real Win16 menu APIs. The host report shows 30 checks and the native
wrapper adds a real `TrackPopupMenu` case: the first item was selected with
Enter and command `15000` decoded to selection `0`, for 31 native checks with
no failures. The expected second child-destroy result after parent ownership
is recorded as a Win16 cleanup effect. Because this Win16 API dispatches the
command directly rather than leaving it queued at the exact `PeekMessage`
point, the harness bridges the selected command into that queue boundary; the
reconstructed source is unchanged. Evidence is preserved in
`analysis/compat/popup-menu-integration.{md,json}` and
`runs/vmctl-vc152pmuint/`. This earlier checkpoint used a queue bridge; the
final E2 closure below removes that bridge and completes promotion.

The compressed-string lookup boundary is now exercised with the actual
original payloads for IDs `0x43F` through `0x44F`, not a placeholder label.
The expanded host/native integration probe checks all 17 labels in order and
reports `POPUP_MENU_INTEGRATION_CHECKS=32 FAILURES=0 RESULT=PASS` in fresh
`C:\PMUINT2`; the real popup was displayed and selected without a Program
Error dialog.

The final E2 closure uses fresh `C:\PMUINT3` and a real mouse selection of the
first item. The strict host and VC++ 1.52c/Win98 probes both report
`POPUP_MENU_E2_CHECKS=30 FAILURES=0 RESULT=PASS` with command `15000`, and the
native path captures and removes the actual owner `WM_COMMAND` using the exact
`PeekMessage` boundary without a synthetic `PostMessage` bridge. Packed fleet,
thing, planet, and deep-space caller records are checked, and the native
window inventory has no Program Error dialog. `PopupMenu` is therefore
promoted to Phase 2 step 6/6. Evidence is preserved in
`analysis/compat/popup-menu-integration.{md,json}` and
`runs/vmctl-vc152pmuint3/`.
