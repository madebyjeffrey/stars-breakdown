# DrawPopup original/UI integration checkpoint

The native `DrawPopup` checkpoint already covers all 51 recorded popup kinds,
default dispatch, client-derived arguments, GDI state restoration, and both
slot-dialog variants. The original Win98 executable was also exercised through
real popup consumers: mineral, population, report, and restored-frame states
are preserved without a Program Error dialog.

This establishes the original Win16 consumer boundary and advances
`DrawPopup` to Phase 2 step 5/6. It does not claim that packed `GlobalPD`
records, localized strings, hull filtering, formatting, or detailed GDI text
rendering are complete; those remain step-6 promotion gates.

Evidence:

- `tests/draw_popup_source_probe.c`
- `analysis/compat/draw-popup-native.md`
- `runs/vmctl-vc152drawpopup/POPSTA.TXT`
- `runs/vmctl-vc152drawpopup/POPCL.LOG`
- `runs/vmctl-vc152drawpopup/POPLINK.LOG`
- `runs/vmctl-vc152drawpopup/POPR.TXT`
- `runs/vmctl-vc152current/direct-popup-mineral.bmp`
- `runs/vmctl-vc152current/direct-popup-population.bmp`
- `runs/vmctl-vc152current/direct-popup-report-open.bmp`
- `runs/vmctl-vc152current/direct-popup-restored.bmp`
