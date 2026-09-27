# DrawNewGame2 and DrawNewGame3 original Win98 integration checkpoint

This is the step-5 original-executable boundary for the two wizard-page
renderers. In the connected `StarDev` Win98 guest, the original Advanced New
Game Wizard reached both relevant pages without a program-error dialog.

Step 2 showed the player table rendered by `DrawNewGame2`: sixteen numbered
player rows, race diamonds, human/computer/no-player labels, and the wizard
navigation controls. Step 3 showed the criteria table rendered by
`DrawNewGame3`: the nine victory-criterion rows, paired checkboxes/spinners,
the winner-count control, and the minimum-years control.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Recovered boundaries:
  `analysis/decompiler/export/raw/1078_9556_DrawNewGame2.c` and
  `analysis/decompiler/export/raw/1078_9d5e_DrawNewGame3.c`.
- Native checkpoints: `runs/vmctl-vc152dng2/` and
  `runs/vmctl-vc152dng3/`.
- Original screenshots:
  `runs/vmctl-vc152current/original-advanced-page2.bmp` and
  `runs/vmctl-vc152current/original-advanced-page3.bmp`.

This advances both procedures from 4/6 to 5/6. Exact packed player/type and
race-creation tables, complete localization, packed rectangles, and full GDI
integration remain open before promotion.
