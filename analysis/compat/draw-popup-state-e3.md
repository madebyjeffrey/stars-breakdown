# DrawPopup drawing-state E3 validation

The state-order probe covers the detailed GDI boundary left after the packed
payload and popup-kind matrices:

- initial background, text, background-mode, and bold-font selection;
- the extra bold/opaque transition for slot-hull popup kinds 11 and 14;
- callback dispatch with the exact client and adjusted slot rectangles; and
- restoration of font, background mode, text color, and background color in
  the original reverse order.

Host strict C89 validation passes:

`DRAW_POPUP_STATE_CHECKS=7 FAILURES=0 RESULT=PASS`

The VC++ 1.52c packet in `compat/vc152/draw-popup-state-e3/` compiled and
linked in Win98. Its native report is:

`DRAW_POPUP_STATE_CHECKS=7 FAILURES=0 RESULT=PASS`

The process exited normally and the post-run window list contained no Program
Error dialog. This is native supporting evidence for E3; the named E3 unit
remains open until localized detailed GDI rendering and the caller differential
are also covered. Step-6 promotion remains open.

Evidence:

- `analysis/decompiler/export/raw/10c0_01c0_DrawPopup.c`
- `reconstructed/src/draw_popup.c`
- `tests/draw_popup_state_probe.c`
- `runs/vmctl-vc152drawpopupstatee3/DRINTSTA.TXT`
- `runs/vmctl-vc152drawpopupstatee3/DRINTCL.LOG`
- `runs/vmctl-vc152drawpopupstatee3/DRINTLINK.LOG`
- `runs/vmctl-vc152drawpopupstatee3/DRPS.TXT`
- `runs/vmctl-vc152drawpopupstatee3/DRPSRES.TXT`
