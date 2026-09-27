# FTrackXfer original Win98 integration checkpoint

This is the step-5 original-executable boundary for `FTrackXfer`. In the live
`StarDev` Win98 game, the selected fleet was `Stalwart Defender #5` and the
destination fleet was `Oop Be Gone`. The original `Cargo Transfer` dialog was
opened, an Ironium transfer control was activated while the dialog remained
open, and Cancel returned to the game without committing a transfer.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Dialog title: `Cargo Transfer [#32770]`.
- Screenshot: `runs/vmctl-vc152current/f-track-xfer.bmp`.
- Recovered boundary: `analysis/decompiler/export/raw/1050_5a16_FTrackXfer.c`.
- Native checkpoint: `runs/vmctl-vc152ftx/` and
  `analysis/compat/f-track-xfer-native.md`.

This advances `FTrackXfer` from 4/6 to 5/6. It does not close the remaining
packed transfer-record, exact cargo/supply mutation, or complete Win16 capture
integration gates required for promotion.
