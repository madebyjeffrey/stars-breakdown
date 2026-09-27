# DrawPopup localization, hull filtering, and formatting — E2 host boundary

This checkpoint adds the readable source boundary for the content emitted by
`DrawPopup` popup kinds 1–4. The production bridge now exposes typed text
segments rather than burying resource selection and numeric formatting inside
the GDI callbacks. It covers localized string IDs, value-format kinds,
planet-name and fleet-name seams, coordinate labels, hull-name decoration,
hull visibility filtering, and count/damage formatting.

The packed fleet and hull-definition inputs are optional explicit raw views:
the fleet view is 124 bytes and the hull-definition view is 147 bytes. The
22-byte popup union continues to preserve far-pointer words without
dereferencing them in the dispatcher.

The strict C89 host probe reports:

```text
DRAW_POPUP_CONTENT_E2_CHECKS=21 FAILURES=0 RESULT=PASS
```

The matrix covers all four content cases, the original resource-ID choices,
full 32-bit fleet-pointer forwarding, hull 1 visibility rejection, tech-bit
extraction for hull decoration, visible hull count/damage formatting, and the
coordinate label/value sequence.

This was initially recorded as supporting host evidence only. The prepared
packet has since been replayed natively; the authoritative closure is recorded
in `analysis/compat/draw-popup-content-e2-native.{md,json}`. Detailed GDI
rendering, caller differential behavior, original completion, and Step-6
promotion remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/draw_popup.c`
- `tests/draw_popup_content_e2_probe.c`
- `compat/vc152/draw-popup-content-e2/`
- `analysis/reconstruction/current-handoff.md`
