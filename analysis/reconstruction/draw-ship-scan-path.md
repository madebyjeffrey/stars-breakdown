# DrawShipScanPath

`DrawShipScanPath` is the scanner path renderer at NE `000c:540c–62d8`
(3,788 bytes). The semantic C version separates scanner state from drawing:
it gates redraws on selection eligibility, visibility changes, and the scanner
busy flag; emits the selected fleet/thing vector and arrow marks; draws
planet and fleet order routes; suppresses duplicate route segments; and
acquires/releases the scanner DC when the caller does not provide one.

The named drawing callbacks preserve pen/mode transitions and primitive order
without embedding GDI assembly or emitted bytes. The strict host C89 probe
passed 5 checks covering vector geometry, route output, DC ownership, mode
changes, and redraw suppression. The same source compiled and linked with
VC++ 1.52c in StarDev, and the native Win98 probe reported
`SCAN_PATH_CHECKS=5 FAILURES=0 RESULT=PASS`.
