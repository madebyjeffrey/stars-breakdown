# FillBuildDD reconstruction

`reconstructed/src/fill_build_dd.c` is a source-level reconstruction of
`FillBuildDD` (`001a:5e80-63e7`, 1,383 bytes). It represents normal and
starbase designs, player design names, queued state, special-part names, and
localized labels with typed records and callbacks. The list reset, mode-based
population, button enablement, and height adjustment are ordinary C logic.

The source probe covers normal design population, special-part mode, starbase
label mode, list reset/add/end operations, delete/edit enablement, the 32-row
height clamp, and the original font-height rounding used for dropdown
resizing. Host strict C89 compilation and the 8-check
`FILL_BUILD_DD_CHECKS=8 ... PASS` probe pass. A fresh VC1.52c/Win98
checkpoint also compiles, links, and reports all 8 checks without a runtime
error dialog. The packed E1 packet additionally proves the 147-byte `_shdef`
stride, `fFree` visibility bit, name offset, and packed `cExist` edit-state
boundary against conflicting typed shadow records. The original Win98 Ship & Starbase Designer was also observed
with populated `Armed Probe` Existing Designs and `Small Freighter` Available
Hull Types selectors, component/capacity tiles, cost summaries, and no Program
Error dialog. The E2 packet additionally proves the exact normal (`0x52`
table, 13 IDs) and starbase (`0x7c` table, 8 IDs) compressed-string order for
both label modes. See `analysis/compat/fill-build-dd-integration.md` and
`runs/vmctl-vc152current/ship-design.bmp`. This is the step-5 integration
checkpoint with E1 packed design records, E2 localized labels/resources, and
E3 complete population/control integration closed. The E3 matrix covers
queued and built edit state, empty and hidden designs, special-part and enemy
branches, starbase limits, count clamping, resize arithmetic, and callback
ordering. Step-6 ledger promotion is also complete.
