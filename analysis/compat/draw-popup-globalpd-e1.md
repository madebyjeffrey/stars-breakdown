# DrawPopup complete `_popupdata` / `GlobalPD` payload E1

The production `DrawPopup` bridge now materializes the complete recovered
22-byte `_popupdata` union before dispatch. `StarsDrawPopupPayload` preserves
the raw bytes and every recorded overlay: the five `rgi` long values, planet
and player value views, production counters, near/far pointer words, damage
and display flags, the eight-byte `_part` view, and token summary fields.
Pointer values are intentionally retained as raw Win16 words; the dispatcher
does not dereference guest addresses.

The strict-C89 host probe reports:

```text
DRAW_POPUP_PACKED_CHECKS=38 FAILURES=0 RESULT=PASS
```

The probe checks the payload through the production callback for all named
overlays, while retaining the existing selector precedence, all popup kinds,
kind-14 alias, geometry, state restoration, and short-record fallback checks.

A fresh VC++ 1.52c/Win98 packet in
`compat/vc152/draw-popup-packed-e6-payload/` was replayed in `C:\DPE6`.
Compile and link passed, the process exited 0, and the final guest window list
contained no Program Error dialog. The authoritative native report is:

```text
DRAW_POPUP_PACKED_CHECKS=38 FAILURES=0 RESULT=PASS
```

The first diagnostic replay is retained in the same run directory; it exposed
an incorrect test oracle rather than a source failure and was corrected before
the final pass. This closes E1, complete packed `GlobalPD` union payload
coverage. Localization/hull filtering/formatting, detailed GDI and caller
differential coverage, and final promotion remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/draw_popup.c`
- `tests/draw_popup_packed_probe.c`
- `compat/vc152/draw-popup-packed-e6-payload/`
- `runs/vmctl-vc152drawpopupe6/DRINT-final.TXT`
- `runs/vmctl-vc152drawpopupe6/DRINTSTA.TXT`
- `runs/vmctl-vc152drawpopupe6/DRINTCL.LOG`
- `runs/vmctl-vc152drawpopupe6/DRINTLINK.LOG`
- `runs/vmctl-vc152drawpopupe6/DRINT-e6-diagnostic.TXT`

