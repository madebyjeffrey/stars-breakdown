# Fleet merge dialog recovery

`reconstructed/src/merge_fleets_dlg.c` is a genuine C reconstruction of
MergeFleetsDlg (NE 17:3376–36b5). It preserves the list-control population
order, fleet-name and special-fleet suffix operations, selected-fleet default,
tutor advancement, selected-row reads, merge confirmation veto, accept/cancel
results, select-all/select-none commands, background coloring, and help
dispatch. Win16 controls and fleet/game services are explicit typed callbacks;
the implementation does not replay emitted bytes.

The source probe covers initialization, control coloring, selection updates,
confirmation veto, accept/cancel, bulk selection, and help. VC1.52c under the
Win98 StarDev environment compiled and linked it; the native probe passed all 8
checks with zero failures
(`runs/vmctl-vc152ew/merge-fleets-dlg-r2/MFDRES.TXT`). This remains touched
semantic source rather than a formal ledger promotion until the real fleet
records and caller integration are reviewed.

`reconstructed/src/transfer_dlg.c` is a genuine C reconstruction of TransferDlg
(NE 11:5686–5a16). It preserves painting and erase-background behavior, the
mode-specific title and help context, the large-hull dialog expansion and
three-control repositioning calculations, transfer-button setup, tutor
advancement, accept/cancel handling, and mouse-coordinate forwarding. The
source probe covers these paths. VC1.52c under the Win98 StarDev environment
compiled and linked it; the native probe passed all 7 checks with zero failures
(`runs/vmctl-vc152ew/transfer-dlg/TRDRES.TXT`). This remains touched semantic
source, not a formal ledger promotion, until the live transfer state and
callers are integrated.

`reconstructed/src/draw_ship_waypoint_orders.c` is a genuine C reconstruction of
DrawShipWayPtOrders (NE 0B:0912-17B5). It preserves reset cleanup, tile geometry,
dropdown sizing and selection dispatch, report-editor visibility, reference
rectangles, mine-layer and minefield detail selection, starbase status messages,
and the tile-failure cleanup path. Window, localization, drawing, and game-state
services are explicit typed callbacks rather than byte or assembly replay. The
host probe covers the reset/report, minefield, mine-layer, and failed-tile paths.
VC1.52c under the Win98 StarDev environment compiled and linked it warning-free;
the native probe passed all 4 checks with zero failures
(`runs/vmctl-vc152ew/draw-ship-waypoint-orders-r2/WSORES.TXT`).
