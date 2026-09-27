# DrawPopup native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152drawpopup/`.

- `POPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `POPCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `POP.C` and `PROBE.C` source
- `POPLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `POPR.TXT`: `DRAW_POPUP_CHECKS=51 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `POP.C` is byte-identical to
`reconstructed/src/draw_popup.c`, and `PROBE.C` is byte-identical to
`tests/draw_popup_source_probe.c`. The probe covers every recorded popup kind,
default dispatch, GDI state restoration, client-derived arguments, and both
slot-dialog variants.

This advances `DrawPopup` from Phase 2 step 3/6 to step 4/6. Packed GlobalPD
records, localized strings, hull filtering, formatting, detailed GDI drawing,
and original-behavior evidence remain open.
