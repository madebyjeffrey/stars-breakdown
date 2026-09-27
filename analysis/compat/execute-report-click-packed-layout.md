# ExecuteReportClick native packed-layout checkpoint

The smaller native adapter checks the recovered record sizes and representative
little-endian offsets without the larger callback surface. It covers the
124-byte `_fleet`, 56-byte `_planet`, 14-byte `_btldata`, and 22-byte
`_popupdata` records, including fleet identity/coordinates, planet flags and
mineral values, battle coordinates, and installation/mineral popup fields.

Both strict host C89 and VC++ 1.52c/Win98 report:

`EXECUTE_REPORT_CLICK_PACKED_LAYOUT_CHECKS=16 FAILURES=0 RESULT=PASS`

The fresh native run was `C:\EXLAY3`; compilation and linking passed, the
process exited 0, and no Program Error dialog appeared. This is a supporting
layout checkpoint for `ExecuteReportClick`, not final promotion evidence.
