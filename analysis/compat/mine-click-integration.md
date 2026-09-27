# MineClick original Win98 integration checkpoint

This is the step-5 original-executable boundary for `MineClick`. In the live
`StarDev` Win98 game, the selection arrow in the original `Oop Be Gone`
summary child was clicked while the small window layout was active. The game
advanced the selected scan object to `Armed Probe #1`, updated the fleet and
waypoint panes, and redrew the summary as `Armed Probe #1 Summary` with ship
count, fuel, cargo, mass, waypoint, task, and warp fields.

The interaction completed in the original frame with no program-error dialog.
This is the direct mine-window mouse path, not the toolbar magnifier's zoom
menu; it therefore provides original state-integration evidence for the
selection-advancement branch of `MineClick`.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Interaction: selection-summary arrow at the right of the survey title.
- Original screenshot: `runs/vmctl-vc152current/mine-summary-arrow.bmp`.
- Recovered boundary: `analysis/decompiler/export/raw/1028_3b4e_MineClick.c`.
- Native checkpoint: `runs/vmctl-vc152mcl/` and
  `analysis/compat/mine-click-native.md`.

This advances `MineClick` from 4/6 to 5/6. Packed `GlobalPD`, planet,
fleet, cargo, minefield, and part records; localized popup construction; and
complete popup/minefield integration remain open before promotion.
