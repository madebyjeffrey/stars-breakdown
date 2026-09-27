# DrawPlanetShipList

`DrawPlanetShipList` is represented as typed control/layout logic in
`reconstructed/src/draw_planet_ship_list.c`.  It preserves tile-change hiding
and reference reset, title selection, failed-tile control hiding, combo
placement, transfer lookup gating, owner-based button enablement, details
rendering, three-button layout, and final action enablement.

Packed fleet/thing transfer records, localized text/gauges, and
drawing/reference geometry remain callback-owned at the service boundary, but
their production integration is now covered by combined host/native packets.
The exact Win16 control handles and button-layout ordering are covered by the
native E2 packet. The host probe covers tile reset, successful fleet detail,
transfer gating, and hidden thing-mode details. A fresh VC++ 1.52c/Win98
checkpoint also reports
`COMPILE=PASS`, `LINK=PASS`, and
`DRAW_PLANET_SHIP_LIST_CHECKS=19 FAILURES=0 RESULT=PASS` without a runtime
error dialog. Evidence is preserved in
`analysis/compat/draw-planet-ship-list-native.md` and
`runs/vmctl-vc152dpsl/`. The later E1–E4 packets below close the packed
fleet/thing, detail-render, and complete drawing/transfer boundaries, so this
procedure is now promoted semantic source.

The original Win98 game frame has now been exercised through the fleet-list
boundary: the selected fleet displayed fuel/cargo, composition, and waypoint
controls, and a map click refreshed the selection without a Program Error.
Evidence is preserved in `analysis/compat/planet-frame-integration.md` and
`runs/vmctl-vc152current/planet-click.bmp`. A packed-transfer checkpoint also
passed 18 host and native checks over the 226-byte `_sel` planet view, the
128-byte `_xfer` identity/kind/owner fields, fleet and thing-mode gating, and
the no-object visibility path. Evidence is preserved in
`analysis/compat/draw-planet-ship-list-packed-integration.{md,json}` and
`runs/vmctl-vc152dlistint/`. The later E4 packet below completes the remaining
integration boundary and promotion.

## Control-layout E2 validation

The typed interface now exposes the original button-window positioning order
separately from logical visibility. The host probe checks the populated-planet
title path, combo geometry, the `1, 2, 0` button-position permutation, logical
show order, transfer lookup, reference reset, detail fill, and enablement.
It passes `DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=7 FAILURES=0 RESULT=PASS`.

Fresh VC++ 1.52c/Win98 validation in `C:\DLIST` reports
`DRAW_PLANET_SHIP_LIST_CONTROL_CHECKS=7 FAILURES=0 RESULT=PASS`; compile/link
passed and no Program Error dialog appeared. This closes the E2 exact-control
layout unit. Evidence is preserved in
`runs/vmctl-vc152dlist-controls-e2/`; complete transfer/drawing behavior and
Step-6 promotion remain open.

The detail-dispatch boundary is now explicit. An optional typed
`draw_details` callback receives the detail rectangle, resolved fleet/thing
kind, and transfer index after lookup; the legacy `fill_details` callback is
retained as a fallback. The strict host probe passes
`DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=0 RESULT=PASS` for fleet
details, unavailable-transfer suppression, and ship-mode suppression. This
narrows the E3 text/gauge/detail branch but closes **0** authoritative Step-5
units until native fleet/thing rendering, exact controls, localized output,
original differential, and promotion are captured. Evidence is in
`analysis/compat/draw-planet-ship-list-details-e3.{md,json}`.

## Production packed-record bridge

The readable control/layout path now owns the recovered raw selection and
transfer boundary. Complete 226-byte selection views supply the object word at
offset 152; complete 128-byte transfer views supply the transfer ID, kind, and
fleet owner at offsets 0, 2, and 6. These fields take precedence over typed
state, while incomplete views retain the callback/typed fallback behavior.

The strict-C89 packed probe now calls production source directly and passes
`DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS` over raw
selection/transfer precedence, fleet and thing kinds, ownership enablement,
visibility, and layout. The source probe remains green at 19 checks. This is
host-only and closes no authoritative Step-5 unit; fresh native replay,
exact USER/GDI controls, localized text/gauges, complete detail rendering,
original differential coverage, and promotion remain open. Evidence is in
`analysis/compat/draw-planet-ship-list-packed-production-e4.{md,json}`.

## Native detail-dispatch E3 replay

The focused detail-dispatch probe now also passes in a fresh VC++ 1.52c / Win98
packet in `C:\DLISTDE3`:
`DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=0 RESULT=PASS`. Compile and
link passed, the process exited normally, and the final guest window list
contained no Program Error dialog. The replay covers resolved fleet detail,
unavailable-transfer suppression, and ship-mode suppression. This closes no
named Step-5 unit: localized text/gauges, complete transfer behavior, exact
caller/resource integration, original differential coverage, and promotion
remain open. Evidence is preserved in
`analysis/compat/draw-planet-ship-list-details-e3-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-details-e3-native/`, and
`runs/vmctl-vc152dlist-details-e3/`.

## Detail-render E3 production replay

The current production source now passes the expanded detail-render trace in
fresh VC++ 1.52c / Win98 directory `C:\DLD3`:
`DRAW_PLANET_SHIP_LIST_DETAIL_RENDER_CHECKS=9 FAILURES=0 RESULT=PASS`. The
host/native trace verifies fleet and thing detail dispatch, the exact detail
rectangle, three-button order and geometry, callback-before-button sequencing,
unavailable-transfer suppression, and ship-mode suppression. Compile and link
pass, the process exits 0, and no Program Error dialog appears. The original
`runs/vmctl-vc152current/planet-click.bmp` frame shows the corresponding fleet
composition, fuel/cargo, and waypoint controls. This closes E3; E4 complete
drawing/transfer integration, broader original differential coverage, and
promotion remain open. Evidence is in
`analysis/compat/draw-planet-ship-list-detail-render-e3-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-details-e3-production-native/`, and
`runs/vmctl-vc152dld3p/`.

## Production packed-record E1 native replay

The current production source and packed-record probe now also pass in fresh
VC++ 1.52c / Win98 directory `C:\DLP4`:
`DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS`. The replay
covers the 226-byte selection view, 128-byte transfer view, raw identity/kind/
owner precedence, fleet and thing modes, owner enablement, visibility/layout,
and no-object suppression. Compile and link pass, the process exits 0, and no
Program Error dialog appears. This closes E1, the packed fleet/thing
transfer-record unit. E3 localized text/gauges/detail rendering, E4 complete
drawing/transfer integration, original differential coverage, and promotion
remain open. Evidence is in
`analysis/compat/draw-planet-ship-list-packed-production-e4-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-packed-production-e4-native/`, and
`runs/vmctl-vc152dlp4/`.

## Complete integration E4 native replay and promotion

The current production source now passes a combined complete-integration trace
in fresh VC++ 1.52c / Win98 directory `C:\DLP4E`:
`DRAW_PLANET_SHIP_LIST_E4_CHECKS=12 FAILURES=0 RESULT=PASS`. The 12 checks
exercise the packed selection and transfer records, title/object decoding,
tile-change cleanup, exact combo geometry and cached-width flags, fleet detail
rectangle and callback order, the `1,2,0` button mapping, thing-mode gating,
unavailable-transfer suppression, and failed-tile cleanup. Compile and link
pass, the process exits 0, and the final guest inventory contains no Program
Error dialog. The post-run screenshot and the prior original fleet-frame
capture retain the caller-level comparison. E4 is closed; the review ledger
now records complete Step-5 evidence and Step-6 promotion.

Evidence is preserved in
`analysis/compat/draw-planet-ship-list-e4-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-e4-production-native/`, and
`runs/vmctl-vc152dlep4/`.
