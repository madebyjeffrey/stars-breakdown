# PtDisplayPlanetStateInfo original Win98 integration checkpoint

This is the step-5 original-executable boundary for
`PtDisplayPlanetStateInfo`. In the live `StarDev` Win98 game, the original
small window layout displayed the selected planet's state section in the
selection-summary child. For `Oop Be Gone`, the frame showed the gravity,
temperature, and radiation rows as labeled colored gauges, followed by the
Ironium, Boranium, and Germanium rows with their plotted values.

The output was produced in the real frame and remained stable without a
program-error dialog. It is the original GDI/text integration boundary for
the typed state-row and layout-point reconstruction.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Window layout: `View > Window Layout > Small Screen`.
- Original screenshot: `runs/vmctl-vc152current/layout-small.bmp`.
- Recovered boundary: `analysis/decompiler/export/raw/10c0_1938_PtDisplayPlanetStateInfo.c`.
- Native checkpoint: `runs/vmctl-vc152stateinfo/` and
  `analysis/compat/pt-display-planet-state-info-native.md`.

This advances `PtDisplayPlanetStateInfo` from 4/6 to 5/6. Packed planet
lookup, localized/fixed-point formatting, detailed GDI coordinates, and full
promotion evidence remain open.
