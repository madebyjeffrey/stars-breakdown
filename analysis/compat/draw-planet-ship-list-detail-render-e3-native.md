# DrawPlanetShipList detail-render E3 native replay

The current production `DrawPlanetShipList` source was replayed under VC++
1.52c in fresh Win98 directory `C:\DLD3` with a detail-render trace. The
host and native report is:

```text
DRAW_PLANET_SHIP_LIST_DETAIL_RENDER_CHECKS=9 FAILURES=0 RESULT=PASS
```

The trace verifies the fleet and thing detail callback boundary, the exact
detail rectangle `(4,2)-(116,10)`, the three-button order `1,2,0`, button
positions and dimensions, callback-before-button sequencing, unavailable
transfer suppression, and ship-mode suppression. `DLD3STA-final.TXT` records
`COMPILE=PASS` and `LINK=PASS`; the process exited 0 and the final guest
window inventory contained no Program Error dialog.

The original Win98 fleet-frame capture in
`runs/vmctl-vc152current/planet-click.bmp` supplies the consumer comparison:
the real frame displays the selected fleet's composition, fuel/cargo, and
waypoint controls. This closes E3, the localized/detail-render service
boundary. E4 complete drawing/transfer interaction, broader original
differential coverage, and Step-6 promotion remain open.

Evidence is preserved in
`analysis/compat/draw-planet-ship-list-detail-render-e3-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-details-e3-production-native/`, and
`runs/vmctl-vc152dld3p/`.
