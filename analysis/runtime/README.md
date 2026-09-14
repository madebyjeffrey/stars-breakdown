# Original-game runtime reference — step 4 in progress

Windows for Workgroups 3.11 is installed locally and the original executable runs. Startup, creation of a small game, turn generation, exit and reload have been exercised. **The reference is not yet accepted:** registration was requested again after reload, and the first generated turn reported that the copy needed validation. Treat current simulation outputs as provisional. User will provide serial number. After the second user-confirmed entry, another turn advanced population to 28,800 and prompted again for a unique serial to prevent further adverse events. No registration bypass or executable patch was attempted. Further game-level acceptance needs this resolved.

## Environment and reproduction

`environment.json` records the emulator binary, configuration and driver hashes. `install-media.json` identifies user-provided Windows installation media. Original executable identity remains the step 0 SHA-256; only that executable was copied into the guest. Windows, drivers, settings, raw game fixtures and captures stay in ignored local directories.

1. Install Homebrew DOSBox-X 2026.08.31 (debugger enabled), then use `python3 -m tools.runtime.prepare --windows-media /path/to/W311` to prepare a fresh guest. The preparer refuses to replace an existing guest and verifies the pinned input hashes. The copied macOS app executable depends on the Homebrew libraries of this installation; the wrapper is not a self-contained emulator distribution.
2. From `runs/step4`, start `../../.tools/StarsRuntime.app/Contents/MacOS/dosbox-x -conf ../../runtime/dosbox-x.conf -nopromptfolder -fastlaunch -c "cd INSTALL" -c SETUP`. Install to `C:\WINDOWS`, use custom setup, VGA initially, US English keyboard, no printer or network, skip tutorial. Setup identity used: Stars Study / Research. Accept guest startup-file creation, then return to DOS. No separately installed DOS is required: this run uses DOSBox-X's integrated DOS.
3. A clean installation snapshot is retained at `runs/step4/clean-install/WINDOWS`. Do not replace it with registered settings.
4. In the DOS Windows SETUP program choose Super VGA 800×600, 256 colors, small fonts. The committed configuration uses the compatible ET4000 adapter. Initial installation used S3 with the default VGA driver; the first smoke test therefore used 640×480, 16 colors.
5. The DOSBox-X integration driver came from doslib release `doslib-20250729`, commit `473980b3f58d2dc4a6e614b843aba1cec4c03df1`. Extract `windrv/dosboxpi/bin/win31/DBOXMPI.DRV` and `OEMSETUP.INF`. Install as an Other mouse driver, or copy the driver into WINDOWS\SYSTEM and set `[boot] mouse.drv=dboxmpi.drv`, `[boot.description] mouse.drv=DOSBox-X Mouse Pointer Integration`, `[386Enh] mouse=*vmd`. The configuration enables its integration device. Host-driven coordinate clicks still targeted the wrong location during our test; mouse usability is **not yet verified**. Keyboard menu accelerators work.
6. Start with `python3 tools/runtime/launch.py`. It checks both executable copies before launching Windows. In Program Manager use Alt+F, R to run `C:\STARS\STARS.EXE`. Alt+N opens New Game on the title screen. Within a game, Alt+T, G generates a turn; Alt+F, X exits. Use menu accelerators when function keys are intercepted by the host.

Sources: [DOSBox-X installation and driver guide](https://dosbox-x.com/wiki/Guide:Installing-Windows-3.1x), [official doslib driver release](https://github.com/joncampbell123/doslib/releases/tag/doslib-20250729), [DOSBox-X debugger documentation](https://github.com/joncampbell123/dosbox-x/blob/dosbox-x-v2026.08.31/README.debugger).

## Observed smoke tests

| Test | Actual result |
|---|---|
| EXE with no game companion files | Starts and displays its title screen; built-in display says Version 2.60j. Preserve user's 2.70j RC3 retail provenance separately. |
| Initial VGA mode | Warns that it prefers at least 800×600 with 256 colors; permits continuation. |
| New Game | Standard difficulty, Small universe, Humanoid race; other settings left at defaults. Saved as SMOKE.XY. Year 2400 opened with homeworld Horselover Fat, population 25,000 and Armed Probe #1 fuel 50/50 mg. |
| Save without changed orders | No X1 file appeared; original five game-file bytes remained unchanged. |
| F1 without help companion | Windows Help reports “Cannot open Help file.” Dismissal and closing Help return to the game. |
| Turn / Generate | Advances to 2401; population 26,800; host and player files change. A validation warning in the turn makes this a provisional result. |
| Exit, restart Windows, open SMOKE.M1 | Loads year 2401 at 800×600, but asks for registration again. |
| Two separate `-g` invocations from copied year-2400 files | Both produce outputs and exit; XY matches, HST/M1/M2/M3 differ. No normalization applied. Registration and RNG control must be resolved before semantic conclusions. |

## Fixtures and comparison

Local snapshots under `runs/step4/fixtures`: `initial`, `pre-turn`, `ui-year2401`, `replay-a`, `replay-b`. Each contains explicit game files and an integrity manifest. No raw fixture is committed. The base has SMOKE.XY, SMOKE.HST and SMOKE.M1–M3. For a replay, copy the complete verified base to a fresh guest directory and run `C:\STARS\STARS.EXE -g C:\REPLAYA\SMOKE.HST`; repeat in REPLAYB. Do not compare a UI run with a batch run as if their modes were equivalent: M1 lengths differ substantially in this test.

Capture with `python3 tools/runtime/compare.py capture runs/step4/guest/REPLAYA runs/step4/fixtures/new-a SMOKE.XY SMOKE.HST SMOKE.M1 SMOKE.M2 SMOKE.M3`. Compare with `python3 tools/runtime/compare.py compare runs/step4/fixtures/replay-a runs/step4/fixtures/replay-b --output analysis/runtime/replay-comparison.json`. Exit status 1 means a mismatch, 0 an exact match. The harness verifies snapshot integrity, checks names and allowed file extensions, rejects empty comparisons, and reports only sizes and differing offsets/counts. Every byte is compared; **no timestamps, seeds or registration fields are masked**. Supply reconstructed outputs using the same capture interface when source exists. No reconstructed executable has been compared yet.

## Time, randomness and debugger observations

Fixed CPU cycles and disabling host clock synchronization do not freeze Windows tick counts. The configuration attempts a DOS date/time reset at startup, but the debugger reported its internal date as 2026-09-13 and time 12:01:37 in this session. The requested 2000 date is therefore not an established RTC baseline. Record and distinguish DOS date, emulator RTC and Windows GETTICKCOUNT before claiming clock control.

Static leads, not yet runtime-validated: WinMain calls Randomize2(GETTICKCOUNT); GenerateWorld can use GETTICKCOUNT for game.lid; FGenerateTurn conditionally calls Randomize(1234567890) when bit 11 of gd.grBits is set. The original release notes describe a fourth DEF universe parameter for a generation seed. Explicit seed creation still needs a tested DEF fixture. Both RNG-state values and the file-encoding seeds must be accounted for; changing file bytes alone is not proof of different simulation semantics.

Alt+F12 successfully enters the heavy debugger; commands can be supplied through the emulator's terminal. `HELP`, `CPU`, `EV CS IP DS SS SP`, `DATE`, `TIME`, and `RUN` were exercised. One paused Windows-desktop observation: CR0=80000013, CR3=0024C000, CPL=0, GDT base=8003DDA8, LDT selector=0070/base=80CAA000. These describe one live session, not persistent Stars segment mappings.

The debugger supports `BP selector:offset` for instruction breakpoints, `BPPM selector:offset` for protected-mode memory-change breakpoints, `LDT`, `SELINFO`, `MEMDUMPBIN selector:offset length`, and `LOGC count` for CS:IP traces. All numeric arguments are hexadecimal. Keep dumps narrowly scoped to verified code or selected non-sensitive state and retain them only in ignored runs. Full memory may contain private settings. A live mapping and entry/return trace are now recorded in `getracestat-observation.json`: NE 29:30D2 maps to selector 1FEF in this session, with all 34 code bytes identical. Stat index 14, near player pointer 59A2 in DS=141F, read signed byte 9 at 59EE and returned AX=9. SP advances from A0A2 to A0A6 across RETF, leaving the four argument bytes for the caller. Eighteen instruction addresses were captured with LOGC 12 (hex). This is one positive input, not a boundary-value proof; **Ghidra's synthetic selectors must never be used as runtime selectors**.

## Remaining acceptance work

- Resolve repeated registration/validation behavior; repeat smoke and replay tests with a validated copy.
- Verify usable mouse input and close the clock-control discrepancy.
- Establish a controlled seed/state recipe and repeatable turn or function results.
- Extend the demonstrated GetRaceStat selector mapping and argument/return observation to negative/boundary values and additional routines.
- Add a race fixture, missing/corrupt game-file tests, and retained clean UI captures; record original errors without repairing them.
- Update the main checklist only when these results constitute an accepted behavioral reference.
