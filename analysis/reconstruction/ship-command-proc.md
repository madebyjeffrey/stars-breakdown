# ShipCommandProc

`ShipCommandProc` is the ship-pane command router at NE `000b:2640–3d32`
(5,874 bytes). The raw procedure decodes the low word of `lParam` as the
originating control and the high word as its notification code, then mutates
the selected fleet, order record, report flag, and pane windows through a
large set of globals and Windows callbacks.

The semantic C version names the control IDs, notification classes, and domain
actions. It covers fleet navigation and rename/transfer/split/merge commands,
ship/fleet/battle dropdown changes, report-toggle state, order-kind/target/
value updates, and the order-edit numeric limits. Application-specific pane
refreshes and fleet mutations are emitted through `StarsShipCommandAction`,
while selection and edit controls are supplied by callbacks.

Validation covers button routing, ship-list and dropdown notifications,
order-field updates, edit-value clamping, and the report toggle. The strict
host C89 probe passed 5 checks. The same source compiled and linked with VC++
1.52c in StarDev, and the native Win98 probe reported
`SHIP_COMMAND_CHECKS=5 FAILURES=0 RESULT=PASS`.
