# DrawPopup production packed-selector bridge

The readable `DrawPopup` dispatcher now owns the first production decode of
the original 22-byte `_popupdata` view. `StarsDrawPopupInput` accepts an
optional `packed_popupdata` pointer and size; when the view is at least 22
bytes, the little-endian selector at offset 0 overrides the legacy typed
`popup_kind`. A short view falls back to the typed field instead of reading
past the supplied record.

The focused strict-C89 probe now exercises the production path directly. It
passes `DRAW_POPUP_PACKED_CHECKS=37 FAILURES=0 RESULT=PASS`, including raw
selector precedence for fleet, planet, slot-hull, every remaining dispatcher
kind, and the short-record fallback. The existing callback context still
retains the recovered 124-byte `_fleet` and 56-byte `_planet` views used to
check the consumer fields.

This is host-only source evidence. It closes no authoritative Step-5 unit:
fresh VC++ 1.52c/Win98 replay through the production bridge, complete
`GlobalPD` union payloads, localization, hull filtering/formatting, detailed
GDI rendering, original caller differential, and final promotion remain open.
The guest-agent reconnect is still required before a new native packet can be
claimed.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/draw_popup.c`
- `tests/draw_popup_packed_probe.c`
- `analysis/compat/draw-popup-packed-integration.{md,json}`
