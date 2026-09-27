# GenerateWorld step-5 integration checkpoint

This checkpoint covers the creation/file/UI acceptance boundary that remained
open after the focused native probe.

The original `C:\STARS!\STARS.EXE` was run in the connected `StarDev` Win98
guest. The title screen opened, `New Game` opened, the default game name was
accepted, and the original executable reached the generated game frame titled
`Stars! -- A Barefoot JayWalk -- Humanoids -- game.m1`. No Program Error dialog
appeared. The screenshots and downloaded original artifacts are preserved in
`runs/vmctl-vc152current/`; their sizes and hashes are recorded in
`analysis/compat/generate-world-integration.json`.

The original instruction export gives the acceptance sequence at
`analysis/decompiler/export/raw/1078_0136_GenerateWorld.c`:

1. `NE 16:1366-1377` calls `FCreateFile(0,-1,0)` and writes the type-7,
   0x40-byte game record.
2. `NE 16:1379-1392` packs each planet point from the y coordinate, assigned
   planet ID, and delta-x from the previous point, then calls `RgToStream`.
3. `NE 16:1397` writes the type-0, two-byte player-count record and closes the
   main stream.
4. `NE 16:1400-1402` calls `FWriteDataFile` for player `-1` and each player
   index through `game.cPlayer - 1`.
5. `NE 16:1404-1440` performs the local/hosted post-write UI transition.

The typed `stars_generate_world_pipeline` probe binds those stages in one
pipeline, checks the packed point and record values, checks the data-file
iteration, and checks the local/hosted finalization branches. Its focused
VC++ 1.52c/Win98 checkpoint remains `GENERATE_WORLD_CHECKS=31 FAILURES=0
RESULT=PASS` in `runs/vmctl-vc152gwt/GWR.TXT`.

This advances `GenerateWorld` from Phase 2 step 4/6 to step 5/6 for the
creation/file/UI boundary. At that earlier checkpoint, full packed game-state
integration, broader resource/UI coverage, and promotion remained open; the
E1 packed-state closure is recorded below.

## E1 packed game-state and RNG closure

The reconstructed source now exposes the recovered 64-byte `_game` boundary
as a typed `StarsGenerateWorldGameState` view. The adapter checks the little-
endian fields at offsets 0 (`lid`), 4 (`mdSize`), 6 (`mdDensity`), 8
(`cPlayer`), 10 (`cPlanMax`), 12 (`mdStartDist`), 14 (`fDirty`), 16 (`wGen`),
and 18 (`turn`), while preserving the separate two-word `StarsSeeds` state.
The RNG callback uses the reviewed `Randomize2`/`Random` implementation,
including seed advancement for nonpositive counts, and feeds that state into
the real GenerateWorld layout phase.

The strict host probe reports `GENERATE_WORLD_E1_CHECKS=16 FAILURES=0
RESULT=PASS`. A fresh VC++ 1.52c / Win98 run in `C:\GWINT` reports the same
result; compile and link passed, the process exited 0, and the final window
inventory contained no Program Error dialog. This closes E1—packed state,
seed/RNG, and turn-field handling.

## E2 state-backed pipeline closure

The remaining integration unit was reduced to one state-backed pipeline
boundary rather than an unspecified “full integration” task. A packed 64-byte
game record is decoded into `StarsGenerateWorldGameState`; its copied seed
state then drives the complete local pipeline through layout, planet
initialization, start selection, player/homeworld setup, startup ships, battle
plans, wormholes, player-status finalization, file records, and local UI
finalization. The host probe reports
`GENERATE_WORLD_E2_CHECKS=13 FAILURES=0 RESULT=PASS`.

The same 13-check probe compiles and links with VC++ 1.52c in a fresh
`C:\GWINT2` directory, runs in Win98, exits 0, and writes
`runs/vmctl-vc152gwint2/GWE2.TXT` with the same pass result. The guest window
inventory contains no Program Error dialog. The checks cover the generated
point count, owned homeworld, four startup ships, five battle plans, main
stream open/close, two data files, point-plus-header record count, player
record, selected player/single-player state, and preserved game id/turn.

Together with the original Win98 New Game → generated-game observation and
the preserved `.xy`, `.M1`, `.M2`, `.M3`, and `.HST` artifacts above, this
closes the GenerateWorld acceptance boundary. The procedure is promoted to
Phase 2 step 6/6; there is no remaining GenerateWorld evidence unit.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/generate_world.c`
- `reconstructed/src/utilgen.c`
- `tests/generate_world_packed_probe.c`
- `compat/vc152/generate-world/E1PROBE.C`
- `compat/vc152/generate-world/GW.C`
- `compat/vc152/generate-world/SP.H`
- `compat/vc152/generate-world/SEED.INC`
- `compat/vc152/generate-world/BUILDE1.BAT`
- `compat/vc152/generate-world/RUNVC-E1.BAT`
- `compat/vc152/generate-world/RUNPROBE-E1.BAT`
- `tests/generate_world_e2_probe.c`
- `compat/vc152/generate-world/E2PROBE.C`
- `compat/vc152/generate-world/GWINT2.DEF`
- `compat/vc152/generate-world/GWINT2.LNK`
- `compat/vc152/generate-world/BUILDE2.BAT`
- `compat/vc152/generate-world/RUNVC-E2.BAT`
- `compat/vc152/generate-world/RUNPROBE-E2.BAT`
- `runs/vmctl-vc152gwint/GWINTSTA2.TXT`
- `runs/vmctl-vc152gwint/GWINTCL2.LOG`
- `runs/vmctl-vc152gwint/GWINTLINK2.LOG`
- `runs/vmctl-vc152gwint/GWE1.TXT`
- `runs/vmctl-vc152gwint/native-screen-e1.bmp`
- `runs/vmctl-vc152gwint2/GWINT2STA2.TXT`
- `runs/vmctl-vc152gwint2/GWINT2CL2.LOG`
- `runs/vmctl-vc152gwint2/GWINT2LINK2.LOG`
- `runs/vmctl-vc152gwint2/GWE2.TXT`
- `runs/vmctl-vc152gwint2/native-screen-e2.bmp`
