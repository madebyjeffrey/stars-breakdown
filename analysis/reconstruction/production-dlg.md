# ProductionDlg reconstruction

`reconstructed/src/production_dlg.c` is a source-level reconstruction of
`ProductionDlg` (`001b:1204-1994`, 1,936 bytes). It keeps the dialog
dispatcher in ordinary C and exposes Win16 drawing, child-layout, list-item,
cursor, popup, initialization, and command services through typed callbacks.

The implementation covers paint/erase, control color, help-cursor hit testing
using the actual screen cursor mapped into client coordinates,
owner-draw focus/list-item handling, measure-item height, initialization and
window layout, command forwarding, and left/right popup dispatch. The source
probe exercises each of those message families and the low-player navigation
disable path.

Host validation: strict C89 compilation and `PRODUCTION_DLG_SOURCE=PASS`.
A fresh VC++ 1.52c/Win98 checkpoint now compiles, links, and runs the exact
source with nine grouped native message-family assertions passing. Together
with the original Ship & Starbase Designer observation below, this supplies
the current Phase 2 step-5 evidence; packed production globals, exact child
geometry, and complete popup/command integration remain open.

The right-click path was then completed through a typed popup-result boundary:
valid saved-queue selections are forwarded to the command layer and the final
editor entry is forwarded to the queue-dialog boundary. A fresh `C:\PDG2`
VC++ 1.52c run reports
`PRODUCTION_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`, with empty redirected
output and no Program Error dialog. This strengthens, but does not close,
step 5/6; packed production globals, exact child geometry, localized popup
resources, complete queue-control integration, and promotion remain open.

The original Win98 executable has now been exercised through the production
dialog boundary: `Commands > Ship Design...` opened Ship & Starbase Designer
with Ships/Starbases modes, design/hull/component views, images, cost and
statistics, and copy/delete/edit controls without a Program Error. Evidence is
preserved in `analysis/compat/production-dialog-integration.md` and
`runs/vmctl-vc152current/`. This advances `ProductionDlg` to Phase 2 step 5/6
for the original designer boundary; packed production globals, exact child
geometry, complete popup/command integration, and promotion remain open.

The exact child-geometry sub-boundary inside the E2 geometry/resources unit is
now checked in fresh `C:\PDGE2`. The host and
native probes each pass `PRODUCTION_DLG_LAYOUT_CHECKS=23 FAILURES=0
RESULT=PASS`, covering full-width and compact-width initialization, research
and list/control positions, dimensions and flags, navigation enablement, and
initialization ordering. Compile, link, process exit, empty redirected
output, and the no-Program-Error window check also pass. Evidence is recorded
in `analysis/compat/production-dlg-layout-e2.{md,json}` and
`runs/vmctl-vc152pdge2/`. The full E2 unit remains open because localized
resource comparison is not yet covered; packed production globals, localized
popup resources, complete queue-control integration, and final promotion
remain.

The exact localized-resource lookup sub-boundary is now covered in fresh
`C:\PDGRS2`. The source exposes the recovered `0x00d3` copy-string call before
the left-click popup and `0x04c0` compressed-string call before the right-click
queue menu. Host and native probes each pass
`PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS`; compile, link,
process exit, empty redirected output, and the no-Program-Error window check
also pass. Evidence is recorded in
`analysis/compat/production-dlg-resource-e2.{md,json}` and
`runs/vmctl-vc152pdgrs2/`. The complete resource-table/string comparison,
packed production globals, queue-control integration, and final promotion
remain open.

The complementary accepted-editor outcome is now covered in fresh `C:\PDGQ4`.
The typed editor boundary preserves accepted queue mutations and restores the
pre-dialog queue table on cancel, while retaining the sparse-slot menu mapping
and localized editor entry checks. Host and native probes each pass
`PRODUCTION_DLG_QUEUE_CHECKS=9 FAILURES=0 RESULT=PASS`; compile, link, process
exit, empty redirected output, and the no-Program-Error window check also pass.
Evidence is recorded in
`analysis/compat/production-dlg-queue-e4.{md,json}` and
`runs/vmctl-vc152pdgq4/`. Live USER/message-loop behavior, original queue
differential evidence, packed production globals, complete resource comparison,
and final promotion remain open.

The native USER/message-loop sub-boundary is now covered in fresh `C:\PDGU5`.
An actual Win98 window and child controls queue initialization, command, left
click, two right clicks, paint, and erase messages through the reconstructed
dispatcher. The run passes
`PRODUCTION_DLG_USER_CHECKS=12 FAILURES=0 RESULT=PASS`, including sparse queue
selection, accepted editor mutation, two queue menus, and 74 USER-loop
dispatches. Compile, link, process exit, empty redirected output, and the
no-Program-Error window check also pass. Evidence is recorded in
`analysis/compat/production-dlg-user-e5.{md,json}` and
`runs/vmctl-vc152pdgu5/`. Original-executable queue differential, packed
production globals, complete resource comparison, and final promotion remain
open.

The queue boundary now also keeps the packed queue capacity unsigned until the
recovered four-entry cap is applied. A fresh `C:\PDGQ5` packet exercises an
oversized `0xffff` capacity against the four-record table; host C89 and native
VC++ 1.52c/Win98 validation each pass
`PRODUCTION_DLG_QUEUE_CHECKS=10 FAILURES=0 RESULT=PASS`. Compile, link,
process exit, empty redirected output, and the no-Program-Error window check
also pass. Evidence is recorded in
`analysis/compat/production-dlg-queue-e5.{md,json}` and
`runs/vmctl-vc152pdgq5/`. Original queue differential behavior, packed
production globals, complete resource comparison, and final promotion remain
open.

The saved-queue menu and editor-cancel rollback sub-boundary is now covered in
fresh `C:\PDGQ3`. When queue records are supplied, the source filters valid
entries from the four queue slots, preserves sparse original indices, passes
the localized editor entry after the queue names, and snapshots/restores the
queue table when the editor reports cancel. Host and native probes each pass
`PRODUCTION_DLG_QUEUE_CHECKS=7 FAILURES=0 RESULT=PASS`; compile, link, process
exit, empty redirected output, and the no-Program-Error window check also pass.
Evidence is recorded in
`analysis/compat/production-dlg-queue-e3.{md,json}` and
`runs/vmctl-vc152pdgq3/`. Full queue-control integration, accepted editor
mutation, live USER/message-loop behavior, original differential evidence,
packed production globals, complete resource comparison, and final promotion
remain open.

## E1 packed production globals

The readable source now owns the raw `vrgZipProd` caller boundary through
`stars_production_dlg_decode_zip_queues`. The adapter preserves the recovered
40-byte `_zipprodq` record: name at offset 0, `valid` at 13, `no_research` at
14, item count at 15, and twelve little-endian 16-bit words from offset 16.
It caps decoded storage at five records, while the dialog's popup preserves the
original four-entry visible/menu cap and sparse slot indices.

Host and native validation each pass
`PRODUCTION_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS`; compile/link,
process exit, empty redirected output, and the no-Program-Error window check
also pass. Evidence is recorded in
`analysis/compat/production-dlg-packed-e1.{md,json}` and
`runs/vmctl-vc152pdgp/`. This closes E1. E2 complete localized resource
comparison, E3 original queue differential, and final promotion remain open.

## E2 complete direct localized-resource contract

The raw `ProductionDlg` body has only two direct localized lookups: `0x00d3`
through `CchGetString` for the left-click help popup and `0x04c0` through
`PszGetCompressedString` for the right-click queue-menu label. The strict host
and fresh resumed-VM VC++ 1.52c probes each pass
`PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS`, including the exact
callback values/order and suppression outside the diamond. The current native
run is `C:\PDGRS3`; compile, link, process exit, empty redirected output, and
the no-Program-Error window check all pass. `InitializeProductionDlg`'s
separate `0x033e` title lookup is already covered by its own reviewed entry.
This closes E2 for `ProductionDlg`; only E3 original queue differential and
Step-6 promotion remain. Evidence is recorded in
`analysis/compat/production-dlg-resource-e2-complete.{md,json}` and
`runs/vmctl-vc152pdgrs3/`.
