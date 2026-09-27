# DrawPopup original consumer completion — E4

This checkpoint closes the final named Step-5 evidence unit for `DrawPopup`.
It combines the already authoritative native source/integration packets with
the preserved original Win98 consumer surfaces.  The source probes cover the
complete packed `GlobalPD` payload, localized content and hull filtering,
detailed GDI state/geometry, and the `ExecuteReportClick -> Popup ->
DrawPopup` caller path.  The original executable captures verify that those
surfaces are reached in the real Win16 frame and return cleanly.

The original consumer matrix is preserved under
`runs/vmctl-vc152current/`:

- `popup-probe-start.bmp` — fleet-summary popup surface;
- `popup-base.bmp` — starbase/base frame surface;
- `direct-popup-mineral.bmp` and `direct-popup-mineral2.bmp` — selected
  planet mineral/status surface;
- `direct-popup-population.bmp` and `direct-popup-population2.bmp` — planet
  population/report surface;
- `direct-popup-report-open.bmp` and `report-row-click.bmp` — Planet Summary
  Report consumer and selected-row boundary;
- `direct-popup-restored.bmp` and `popup-probe-restored.bmp` — restored frame
  after popup/report dismissal.

The captures retain the original `Stars!` frame, selected planet/fleet and
summary panels, report window, and popup-related transitions without a Program
Error dialog.  This is an original consumer-boundary comparison, not a claim
of pixel identity between the callback-based host renderer and the original
GDI rasterizer.

The final authoritative native packets remain:

- `runs/vmctl-vc152drawpopupe6/` — 38-check packed `GlobalPD` payload;
- `runs/vmctl-vc152drawpopupcontente2-r2/` — 21-check localized content;
- `runs/vmctl-vc152drawpopupgeometrye3r2/` — 25-check geometry/style;
- `runs/vmctl-vc152drawpopupstatee3/` — GDI state-order support;
- `runs/vmctl-vc152popintp/` — 26-check production caller replay.

Together these close E4.  Final review-ledger synchronization and promotion
are recorded in the `DrawPopup` review entry and TODO6.
