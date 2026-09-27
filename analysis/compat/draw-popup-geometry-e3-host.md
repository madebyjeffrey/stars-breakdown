# DrawPopup geometry and style — E3 supporting boundary

The readable `DrawPopup` content path now carries the recovered text geometry
and style decisions instead of leaving every emitted segment at an implicit
origin. The typed text boundary records x/y, left/right/center anchor, normal
or bold font, and opaque background mode. An optional measurement callback
supplies the original label/title-width-dependent offsets used by popup kinds 1
and 4.

The strict C89 host probe reports:

```text
DRAW_POPUP_GEOMETRY_E3_CHECKS=25 FAILURES=0 RESULT=PASS
```

The probe covers centered titles and fleet text, measured label offsets,
row-height arithmetic, right-aligned counts and labels, the separator draw
rectangle and raster operation, the hull-row origin, bold-versus-normal font
selection, opaque text mode, the original text-color reset, and the short-client
fallback. This is supporting E3 evidence; the named E3 unit
still requires the complete detailed GDI/caller differential and original
completion boundary before closure.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/draw_popup.c`
- `tests/draw_popup_geometry_e3_probe.c`
- `analysis/decompiler/export/raw/10c0_01c0_DrawPopup.c`
- `compat/vc152/draw-popup-geometry-e3/`
