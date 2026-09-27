# DrawPlanetShipList production packed-record E1 native replay

The current production `DrawPlanetShipList` source and packed-record probe
were replayed under VC++ 1.52c in fresh Win98 directory `C:\DLP4`. The host
and native reports are:

```text
DRAW_PLANET_SHIP_LIST_PACKED_CHECKS=18 FAILURES=0 RESULT=PASS
```

The probe exercises the complete 226-byte selection view and 128-byte
transfer view: selection-object precedence, transfer identity/kind/owner,
fleet and thing modes, owner enablement, visibility/layout, and no-object
suppression. `DLP4STA-final.TXT` records `COMPILE=PASS` and `LINK=PASS`; the
process exited 0 and the final guest window inventory contained no Program
Error dialog.

This closes `DrawPlanetShipList` E1, the packed fleet/thing transfer-record
unit. E3 localized text/gauges/detail rendering, E4 complete drawing/transfer
integration, original differential coverage beyond the existing fleet-list
frame, and Step-6 promotion remain open.

Evidence is preserved in
`analysis/compat/draw-planet-ship-list-packed-production-e4-native.{md,json}`,
`compat/vc152/draw-planet-ship-list-packed-production-e4-native/`, and
`runs/vmctl-vc152dlp4/`.
