# DrawMineSurvey original Win98 integration checkpoint

This is the step-5 original-executable boundary for `DrawMineSurvey`. In the
live `StarDev` Win98 game, the original small window layout exposed the
selection-summary child for `Oop Be Gone`. The pane showed the original
summary title, habitability value and population, gravity/temperature/
radiation bars, and Ironium/Boranium/Germanium rows with markers.

The panel was reached through the real game frame and remained stable with no
program-error dialog. The same child was redrawn after changing the selected
scan object through the summary arrow, confirming that the survey renderer is
integrated with the frame's current selection rather than being an isolated
dialog.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Window layout: `View > Window Layout > Small Screen`.
- Original screenshots: `runs/vmctl-vc152current/layout-small.bmp` and
  `runs/vmctl-vc152current/mine-summary-arrow.bmp`.
- Recovered boundary: `analysis/decompiler/export/raw/1028_065a_DrawMineSurvey.c`.
- Native checkpoint: `runs/vmctl-vc152dms/` and
  `analysis/compat/draw-mine-survey-native.md`.

This advances `DrawMineSurvey` from 4/6 to 5/6. Detailed GDI coordinates,
packed record/resource recovery, localized formatting, derived mining
arithmetic, and complete mine-survey promotion evidence remain open.
