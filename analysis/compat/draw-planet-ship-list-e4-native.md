# DrawPlanetShipList E4 complete integration native replay

The current production `DrawPlanetShipList` source was replayed in fresh
VC++ 1.52c / Win98 directory `C:\DLP4E` using a complete callback trace. The
host and native report is:

```text
DRAW_PLANET_SHIP_LIST_E4_CHECKS=12 FAILURES=0 RESULT=PASS
```

The 12 checks combine the recovered 226-byte selection view and 128-byte
transfer view with the typed control/drawing boundary. They cover title and
object decoding, transfer identity/kind/owner propagation, tile-change
hide/reset cleanup, exact combo geometry and cached-width flags, fleet detail
dispatch and rectangle, the `1,2,0` button-position mapping, callback order,
thing-mode owner/action gating, unavailable-transfer suppression, and the
failed-tile cleanup path.

`DLP4ESTA.TXT` records `COMPILE=PASS` and `LINK=PASS`; the process exited 0.
The final window inventory returned to the original Stars frame and contained
no Program Error dialog. The post-run screenshot is preserved as
`runs/vmctl-vc152dlep4/guest-after-e4.bmp`; the earlier original fleet-frame
capture supplies the visible composition, fuel/cargo, and waypoint comparison.

This closes E4, the complete drawing/transfer integration boundary. Together
with E1 packed records, E2 exact controls, E3 detail rendering, and the
original frame evidence, the procedure is eligible for Step-6 promotion.

Evidence is preserved in
`analysis/compat/draw-planet-ship-list-e4-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-e4-production-native/`, and
`runs/vmctl-vc152dlep4/`.
