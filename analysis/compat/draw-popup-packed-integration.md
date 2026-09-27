# DrawPopup packed-record integration checkpoint

The packed-record adapter exercises the recovered `GlobalPD` popup selector
boundary through the existing typed `DrawPopup` dispatcher. It decodes the
22-byte `_popupdata` selector and representative values, retains the recovered
124-byte `_fleet` and 56-byte `_planet` storage views in the callback context,
and verifies the fleet, planet-state, slot-hull, and default dispatch paths.

The checked record sizes and offsets are the 22-byte `_popupdata` selector at
offset 0, planet/union fields at offsets 2, 4, 6, 8, and 10, the 124-byte
`_fleet` coordinates at offsets 8 and 10 with ship counts at 12 and 14, and
the 56-byte `_planet` flag word at offset 0. The dispatcher checks the raw
selector-derived kind, the callback-visible fleet and planet fields, the
slot-dialog rectangle, line-height adjustment, starbase mode, default path,
and GDI-state restoration.

Host strict C89 and native VC++ 1.52c reports are:

`DRAW_POPUP_PACKED_CHECKS=25 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152drint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog. Two earlier native reports are retained in the same run
directory as diagnostic history: both caught the probe's incorrect self-count
of 26 instead of its actual 25 checks.

This advances `DrawPopup` within Phase 2 step 5/6. It does not promote the
procedure: complete `GlobalPD` union coverage, localized strings, hull
filtering, formatting, detailed GDI text rendering, and final caller review
remain open.

Evidence:

- `tests/draw_popup_packed_probe.c`
- `compat/vc152/draw-popup-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/10c0_01c0_DrawPopup.c`
- `analysis/reconstruction/draw-popup.md`
- `runs/vmctl-vc152drint/DRINTSTA.TXT`
- `runs/vmctl-vc152drint/DRINTCL.LOG`
- `runs/vmctl-vc152drint/DRINTLINK.LOG`
- `runs/vmctl-vc152drint/DRINT.TXT`
- `runs/vmctl-vc152drint/DRINTDEBUG.TXT`
- `runs/vmctl-vc152drint/DRINTDEBUG2.TXT`

## Complete dispatcher-kind checkpoint

The maintained packed probe now exercises every remaining popup-kind callback,
including mineral, fleet-name, coordinate, order, population, factory/mine,
component, string, resource, and the kind-14 slot-hull alias. It checks the
shared GDI-state restoration after each dispatch and preserves the original
kind-11/kind-14 geometry distinction.

The fresh `C:\DRINT2` VC++ 1.52c / Win98 run reports
`DRAW_POPUP_PACKED_CHECKS=35 FAILURES=0 RESULT=PASS`; compile and link passed,
the process exited normally, and no Program Error dialog appeared. This
closes dispatcher-kind coverage inside step 5/6. Complete `GlobalPD` union
payload coverage beyond the dispatcher, localized strings, hull filtering and
formatting, detailed GDI text rendering, caller differential coverage, and
promotion remain open.

Evidence:

- `tests/draw_popup_packed_probe.c`
- `compat/vc152/draw-popup-packed/PROBE.C`
- `compat/vc152/draw-popup-packed/DRINTN.C`
- `compat/vc152/draw-popup-packed/RUNBLD2.BAT`
- `compat/vc152/draw-popup-packed/RUNPRB2.BAT`
- `runs/vmctl-vc152drint2/DRINTSTA2.TXT`
- `runs/vmctl-vc152drint2/DRINTCL2.LOG`
- `runs/vmctl-vc152drint2/DRINTLINK2.LOG`
- `runs/vmctl-vc152drint2/DRINT4.TXT`
