# TransferStuff original Win98 integration checkpoint

This is the step-5 original-executable boundary for `TransferStuff`. The live
`StarDev` guest was left in the original Stars! game with the selected fleet
`Stalwart Defender #5` and nearby fleet `Oop Be Gone`. Opening `Xfer` displayed
the original `Cargo Transfer` dialog with fuel, cargo hold, Ironium, Boranium,
Germanium, and Colonists rows plus per-row transfer controls. Cancel closed the
dialog without committing a game-state change.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Dialog title: `Cargo Transfer [#32770]`.
- Screenshot: `runs/vmctl-vc152current/fleet-transfer.bmp`.
- Recovered boundary: `analysis/decompiler/export/raw/1050_4faa_TransferStuff.c`.
- Native checkpoint: `runs/vmctl-vc152trn/` and
  `analysis/compat/transfer-stuff-native.md`.

This advances `TransferStuff` from 4/6 to the Step-5 evidence gate. The later
E2 packet closes exact cargo mutation through the reconstructed balance service;
the remaining E3 gates are packed fleet/planet/thing lookup, the exact Win16
transfer-dialog path, and complete lookup/refresh integration required for
promotion.
