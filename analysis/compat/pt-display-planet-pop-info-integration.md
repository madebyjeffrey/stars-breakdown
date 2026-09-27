# PtDisplayPlanetPopInfo original Win98 integration checkpoint

This is the step-5 original-executable boundary for
`PtDisplayPlanetPopInfo`. In the live `StarDev` Win98 game, the original
small window layout displayed the selected planet's population section in the
selection-summary child. For `Oop Be Gone`, it showed `Value: 100%`,
`Population: 25,000`, and the current-report status alongside the summary
header.

The output was produced in the real frame and remained stable without a
program-error dialog. It is the original text/GDI integration boundary for
the typed ownership, habitability, maximum-population, growth-warning, and
layout-point reconstruction.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Window layout: `View > Window Layout > Small Screen`.
- Original screenshot: `runs/vmctl-vc152current/layout-small.bmp`.
- Recovered boundary: `analysis/decompiler/export/raw/10c0_228e_PtDisplayPlanetPopInfo.c`.
- Native checkpoint: `runs/vmctl-vc152popinfo/` and
  `analysis/compat/pt-display-planet-pop-info-native.md`.

This advances `PtDisplayPlanetPopInfo` from 4/6 to 5/6. Packed planet
lookup, localized/fixed-point formatting, detailed GDI coordinates, and full
promotion evidence remain open.
