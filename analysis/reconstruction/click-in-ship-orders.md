# ClickInShipOrders

`ClickInShipOrders` is the ship-pane hit-test and drag dispatcher at NE
`000b:7cda–9165` (5,259 bytes). The semantic C boundary names the nineteen
reference regions, the selected fleet/order state, and the pane operations that
the original reaches through globals and Windows mouse/capture calls.

The reconstruction covers empty-selection rejection, transfer-to-planet and
transfer-to-orbit regions, order-kind and order-target gating, order and fuel
gauge drags with bounded values, mass-warp clamping, right-click order/zip
popups, popup selection, and the final refresh action. The callback boundary
keeps UI drawing, fleet lookup, and popup implementation outside the core
decision logic while preserving action order and state updates.

The strict host C89 probe passed 5 checks. The same source compiled and linked
with VC++ 1.52c in StarDev, and the native Win98 probe reported
`CLICK_SHIP_CHECKS=5 FAILURES=0 RESULT=PASS`.
