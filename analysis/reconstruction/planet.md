# Planet ship-pane drawing recovery

`reconstructed/src/draw_plan_ship.c` is a genuine C reconstruction of
DrawPlanShip (NE 10:0d16–1086). It preserves the no-selection control hide
sequence, reference-rectangle sentinels, dead-planet diagonal message,
selected-object record masks, print/selection flag updates, per-record indirect
drawing callback, DC acquisition/release ownership, and restoration of the
font and colors. The unresolved original computed call is represented by an
explicit typed record callback rather than emitted bytes.

The source probe covers the no-selection, dead-planet, and selected-record
paths. VC1.52c under the Win98 StarDev environment compiled and linked the
source without warnings in the corrected run; the native probe passed all 3
checks with zero failures
(`runs/vmctl-vc152ew/draw-plan-ship-r2/DPSRES.TXT`). The formal ledger records
the typed callback boundary; final whole-game integration remains a separate
milestone.

## Planet mineral summary

`reconstructed/src/draw_planet_min_sum.c` is a genuine C reconstruction of
DrawPlanetMinSum (NE 0A:12B2–1715). It lays out the mineral-summary tile,
invalidates the six lower reference slots when the tile is clipped, paints
the three color-coded mineral rows, and emits the mine and factory summaries.
The special race branch uses operating counts; the normal branch derives the
packed capacity fields with named shifts. Fonts, colors, text formatting,
right-aligned values, brush restoration, and reference rectangles are typed
callbacks rather than selector arithmetic or emitted bytes.

The host C89 source probe covers tile layout, all three mineral rows, capacity
branches, formatting, reference regions, and state changes. VC1.52c under the
Win98 StarDev environment compiled and linked the source warning-free, and
the native probe passed its check with zero failures
(`runs/vmctl-vc152ew/draw-planet-min-sum/PMSURES2.TXT`).

## Main-object selection

`reconstructed/src/change_main_obj_sel.c` is a genuine C reconstruction of
ChangeMainObjSel (NE 10:3e7a–429f). It preserves the AI same-selection early
return, report invalidation order, planet and fleet lookup failure exits,
same-player fleet attachment and duplication, selection waypoint/fullness
state, fleet order selection, production/list refreshes, control hiding and
reference-rectangle sentinels, scan-flag clearing, redraw choice, title-bar
update, and tutorial advancement. Win16 globals and UI services are explicit
typed callbacks rather than pointer-offset or emitted-byte substitutes.

The source probe covers planet, fleet, same-kind redraw, AI no-op, and failed
lookup paths. VC1.52c under the Win98 StarDev environment compiled and linked
the source, and the native probe passed all 5 checks with zero failures
(`runs/vmctl-vc152ew/change-main-obj-sel-r2/CMRES.TXT`).

## Planet-pane click handling

`reconstructed/src/planet_click.c` is a genuine C reconstruction of
PlanetClick (NE 10:489e–515b). It preserves tile hit testing, the selected
object's order dispatch, button-strip expansion tracking, pointer capture and
fuzzy-border drag feedback, signed coordinate handling, cross-column moves,
row insertion, layout-flag updates, and reflow callbacks. The source uses
typed tile records and explicit UI-service callbacks instead of emitted bytes
or reconstructed far-pointer arithmetic.

The source probe covers drag/reorder, right-click ship-order dispatch,
expand/collapse, and edge rejection. VC1.52c under the Win98 StarDev
environment compiled and linked the source without warnings, and the native
probe passed all 4 checks with zero failures
(`runs/vmctl-vc152ew/planet-click-r2/PLRES.TXT`).

## Planet-pane order hit dispatch

`reconstructed/src/click_in_planet_orders.c` is a genuine C reconstruction of
ClickInPlanetOrders (NE 10:515C–587D). It preserves the reference-region
priority, cursor-query returns, population-row decoding, mining/factory
capacity selection, resource research adjustment, defense/scanner gating,
warp-machine and starbase popup preparation, toggle-button tracking, and ship
order forwarding. The global popup record, selected planet record, and Win16
services are expressed as typed state and callbacks rather than far-pointer
offset arithmetic.

The source probe covers population, mining, resources, scanner help and popup,
warp-button tracking, and ship-order dispatch. VC1.52c under the Win98 StarDev
environment compiled and linked the source warning-free, and the native probe
passed all 6 checks with zero failures
(`runs/vmctl-vc152ew/click-in-planet-orders-r1/CPRES.TXT`).

## Planet production-list filling

`reconstructed/src/fill_planet_prod_lb.c` is a genuine C reconstruction of
FillPlanetProdLB (NE 10:6692–6ACA). It preserves selected-planet fallback,
listbox clearing and insertion, direct-call first-line formatting, the empty
production string fallback, ETA status markers, packed production kind/item
decoding, and the renderer's item-status attribute bytes. Localized strings,
item naming, schedule estimation, line formatting, and Windows messages are
typed callbacks instead of hard-coded addresses or emitted bytes.

The source probe covers empty selected production, listbox population, direct
formatting, status markers, and the selected-item attributes. VC1.52c under
the Win98 StarDev environment compiled and linked the source, and the native
probe passed all 3 checks with zero failures
(`runs/vmctl-vc152ew/fill-planet-prod-lb-r2/FPRES.TXT`).
