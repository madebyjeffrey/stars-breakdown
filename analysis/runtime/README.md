# Original-game runtime reference — step 4

This report records the initial Step-4 runtime baseline. Its forward-looking
statements describe that checkpoint. For current reconstruction and acceptance
status, use [TODO5](../../TODO5.md) and [TODO7](../../TODO7.md).

**Step 4's initial behavioral reference is established.** Windows for Workgroups 3.11 runs the original executable. The study now has game, turn, race and malformed-file fixtures; a verified live function trace; and two tutorial creations that match all 3,213 output bytes under a documented clock intervention. This is a foundation for reconstruction, not whole-program equivalence or exhaustive testing.

The user resolved registration after the initial experiments. Subsequent clean game creation, turn generation, separate batch launches, tutorial creation and race-file reload completed without renewed registration prompts. Earlier SMOKE runs remain provisional historical evidence and are excluded from the accepted baseline.

## Environment and reproduction

For upcoming acceptance checks, the user has Stars! and VC++ 1.52c in an
86Box/Windows 98 SE VM and prefers to perform its checks from precise
instructions. Give the actions, expected results and files to return through
the existing FTP share when needed. DOSBox remains authorized for checks the
agent can perform directly (user preference recorded 2026-09-14).

The user confirmed the DOSBox VC++ installation at `C:\MSVC` on 2026-09-14.
Its host directory is `runs/step4/guest/MSVC`. CL.EXE, LINK.EXE, LIB.EXE,
the Windows/C headers and MLIBCEW.LIB/LIBW.LIB are present. The two library
hashes still match the [verified runtime audit](../reconstruction/runtime-library.json).
Use this installation for upcoming agent-operated native builds; installation
confirmation is separate from successfully compiling/running each new component.

The subsequent [race acceptance package](../../compat/race/README.md) records
88 generated scenarios and a first live T000 baseline with matching plaintext
and successful cross-loading in both directions. For title-screen race creation,
use New Game (Alt+N), then Customize Race (Alt+C), without generating a world.
For loading, navigate the Open dialog into the fixture directory before entering
the bare filename. Refresh DOSBox's mounted C drive after host-side file copies.

`environment.json` records the emulator binary, configuration and driver hashes. `install-media.json` identifies user-provided Windows installation media. Original executable identity remains the step 0 SHA-256; only that executable was copied into the guest. Windows, settings, raw fixtures and captures stay in ignored local directories. User will provide serial number.

1. Install Homebrew DOSBox-X 2026.08.31 with its heavy debugger. Run `python3 -m tools.runtime.prepare --windows-media /path/to/W311` to prepare a fresh guest. It verifies pinned input hashes and refuses to replace an existing guest. The macOS app wrapper depends on this Homebrew installation's libraries; it is not a self-contained emulator distribution.
2. From `runs/step4`, launch `../../.tools/StarsRuntime.app/Contents/MacOS/dosbox-x -conf ../../runtime/dosbox-x.conf -nopromptfolder -fastlaunch -c "cd INSTALL" -c SETUP`. Install to `C:\WINDOWS`: custom setup, VGA initially, US English keyboard, no printer or network, skip tutorial. Setup identity: Stars Study / Research. Accept guest startup-file creation and return to DOS. This uses DOSBox-X's integrated DOS, not a separately installed DOS system. The clean installation snapshot is `runs/step4/clean-install/WINDOWS`.
3. In DOS Windows SETUP choose Super VGA 800×600, 256 colors, small fonts. The committed configuration uses the compatible ET4000 adapter. Initial installation used S3 with VGA; early smoke tests used 640×480, 16 colors.
4. The DOSBox-X mouse integration driver came from doslib release `doslib-20250729`, commit `473980b3f58d2dc4a6e614b843aba1cec4c03df1`. Extract `windrv/dosboxpi/bin/win31/DBOXMPI.DRV` and `OEMSETUP.INF`. Install as an Other mouse driver, or copy it into WINDOWS\SYSTEM and set `[boot] mouse.drv=dboxmpi.drv`, `[boot.description] mouse.drv=DOSBox-X Mouse Pointer Integration`, `[386Enh] mouse=*vmd`. The configuration enables its integration device. Automated coordinate clicks still target the wrong location; this reference uses keyboard navigation. The user reports that physical mouse input works normally. The remaining follow-up is automated pointer positioning, not general mouse usability or an established game bug.
5. Start with `python3 tools/runtime/launch.py`. It verifies both executable copies before launching Windows. In Program Manager use Alt+F, R and run `C:\STARS\STARS.EXE`. Title screen: Alt+N for New Game, Alt+O for Open. Game: Alt+T, G generates; Alt+F, C closes; Alt+F, X exits. Use menu accelerators when the host intercepts function keys. Closing a tutorial asks for confirmation; Y returns to the title screen. A coordinate click may activate the system menu at the upper left; Escape dismisses it, after which menu accelerators work.

Sources: [DOSBox-X installation and driver guide](https://dosbox-x.com/wiki/Guide:Installing-Windows-3.1x), [official doslib release](https://github.com/joncampbell123/doslib/releases/tag/doslib-20250729), [debugger documentation](https://github.com/joncampbell123/dosbox-x/blob/dosbox-x-v2026.08.31/README.debugger).

## Accepted smoke and error observations

| Test | Actual result |
|---|---|
| EXE without companion game/help files | Starts and displays Version 2.60j. Keep this observed label separate from the user's 2.70j RC3 retail provenance. |
| Initial VGA mode | Warns that it prefers at least 800×600 with 256 colors; allows continuation. |
| New clean game | Standard difficulty, Tiny universe, Humanoid race, remaining defaults. CLEAN.XY; game Duck Hunt; homeworld Spitfire; year 2400; population 25,000; Armed Probe #1 fuel 50/50 mg. |
| Clean turn generation | Year 2401; population 28,700; ordinary empty-production-queue message, no validation warning. |
| Exit/relaunch and reload | Separate batch sessions run; CLEAN.M1 subsequently reopens at 2401 without registration interruption. |
| F1 with no help companion (initial session) | Windows Help: “Cannot open Help file.” Dismissal and closing Help return to the game. |
| Open BAD.HST, 16 zero bytes, without BAD.XY | Reports it cannot open BAD.xy, establishing map dependency before host-content rejection. |
| Repeat with valid CLEAN.XY copied as BAD.XY | Reports that BAD.hst does not belong to this version of Stars. No crash; CLEAN.M1 reopens afterward. |
| Custom race save/reopen | Default Humanoid, names Study / Studies, no password, 25 remaining points assigned to surface minerals. Finish immediately; STUDY.R1 is 149 bytes. Reopening restores those settings. |

No crash occurred in these limited checks. The malformed file's message is observed behavior, not a reliable diagnosis of every corrupt file. No original error behavior was repaired. Broader failures, numeric boundaries, combat and long games belong to subsequent reconstruction/validation work.

## Fixtures and comparison harness

`fixtures.json` records accepted local snapshot identities. Raw files remain under `runs/step4/fixtures`:

- `clean2400`, `clean2401`: clean game and generated turn; XY/HST/M1/M2.
- `clean-replay-a`, `clean-replay-b`: independent ordinary batch runs; still nonidentical.
- `tutorial-a`, `tutorial-b`: fixed-seed tutorial creation without clock control; nonidentical file encoding.
- `tutorial-controlled-a`, `tutorial-controlled-b`: all four files match exactly with the intervention below.
- `race`: STUDY.R1. `malformed`: BAD.HST and the deliberately supplied BAD.XY.

To capture selected files: `python3 tools/runtime/compare.py capture runs/step4/guest/CLEANA runs/step4/fixtures/new-a CLEAN.XY CLEAN.HST CLEAN.M1 CLEAN.M2`.

To compare: `python3 tools/runtime/compare.py compare runs/step4/fixtures/tutorial-controlled-a runs/step4/fixtures/tutorial-controlled-b --output analysis/runtime/tutorial-controlled-comparison.json`.

Exit 0 means exact equality; exit 1 means mismatch. Snapshot integrity is verified first. The harness rejects unselected paths, unrelated extensions and empty snapshots. It compares every byte and reports only sizes, offsets and counts. **No output fields are masked or normalized.** Capture reconstructed output through the same interface once it exists. No reconstructed executable has been compared yet.

For ordinary replay, copy the entire verified clean2400 snapshot into a fresh guest folder and run `C:\STARS\STARS.EXE -g C:\CLEANA\CLEAN.HST`. Repeat in CLEANB. `clean-replay-comparison.json` retains the mismatch. UI and batch modes are not interchangeable: batch M1 files append another year's data, producing a different length from the UI fixture.

## Reproducible tutorial and time control

The source-linked original routines provide a useful bounded scenario: CreateTutorWorld at NE 16:5E5E sets game ID 008CEF49 and seeds Randomize with decimal 1234567890. New Game → Begin Tutorial (Alt+N, then Alt+B) invokes this path. We launched the title screen using `STARS.EXE -t`; that switch alone did not begin the tutorial. Initial tutorial: year 2400, Stove Top, population 100,000.

Fixed simulation seed alone did **not** yield identical files. WriteBOF at NE 15:8EA4 calls Random(2000), then GETTICKCOUNT, and uses their sum in the file-encoding salt. The matching live instruction after that API call is NE 15:8F64. Its 16-byte fingerprint is `8b4eea03c18946ec8b46ec25ff07d1e0`.

Controlled reproduction:

1. Start a tutorial once so the save segment is loaded. Enter debugger with Alt+F12. Use `LDT` and code bytes to identify the live selector for NE segment 15; its recorded allocation is 51,282 bytes. Do not identify a segment from descriptor size alone. `MEMDUMPBIN selector:8F64 10` must match the fingerprint above. In the controlled session it was selector 1297.
2. Close the tutorial through File → Close, confirm Y. Move only TUTORIAL.XY/HST/M1/M2 and any generated tutorial H/X files to an ignored local archive. Do this while the game is closed; do not copy settings into fixtures. Keep the same Stars process open so the observed selector remains meaningful.
3. Set `BP 1297:8F64`, then `RUN`. New Game → Begin Tutorial. At each stop first verify CS:IP is 1297:8F64, then `SR AX 0 DX 0` and `RUN`. There were exactly four hits per creation. This substitutes a zero result for this API call only. It leaves RNG calls, arithmetic, encoding and file writing in the original code.
4. Once the tutorial screen appears, capture the four files. Remove the breakpoint (`BPDEL *`) before closing the game. Archive that session's tutorial files, then repeat steps 2–3 for the second creation in the same process. Recheck mapping if the process/segments change.
5. Compare all four files. Both runs produced 184-byte XY, 1,635-byte HST, 818-byte M1 and 576-byte M2: **3,213 bytes exactly equal**, without normalization. `clock-control.json` and `tutorial-controlled-comparison.json` retain the result. Breakpoints were removed afterward.

This is a controlled environmental-input experiment, not a stock-run equivalence claim. No executable bytes on disk were modified. It does not establish deterministic ordinary turn generation. For those routines, separately capture/restore RNG state at a verified call boundary or recover an explicit seed recipe before comparing results.

The configuration requests a DOS clock reset and disables host synchronization, but the debugger's internal date remained 2026-09-13 during the earlier session. A requested 2000 date is **not** an established RTC baseline. DOS date, emulator RTC and Windows tick count are distinct observations; the accepted controlled comparison depends only on the scoped API return above. Fixed CPU cycles alone do not freeze ticks.

## Live function trace and observation methods

`getracestat-observation.json` maps NE 29:30D2 to live selector 1FEF in an earlier session, confirmed by all 34 original bytes. At entry DS=SS=141F, SP=A0A2; arguments are near player pointer 59A2 and statistic index 14. The selected byte at 141F:59EE is 9. The original returns AX=9. RETF advances SP to A0A6, leaving four argument bytes for caller cleanup. The 18-instruction CS:IP trace validates one naturally reached positive case, not all boundary inputs.

The heavy debugger was exercised with HELP, CPU, EV, LDT, MEMDUMPBIN, BP/BPDEL, LOGC, SR and RUN. All numeric parameters are hexadecimal (`LOGC 12` means 18 instructions). `BP selector:offset` is an instruction breakpoint; `BPPM selector:offset` watches protected-mode memory changes. `SELINFO`, narrow memory dumps and register observations can extend this workflow. Ghidra's synthetic selectors are never runtime addresses. Segments can move or be discarded; revalidate mappings each session.

UI captures are local under `runs/step4/captures`: clean2401.png, tutorial2400.png, missing-map.png, malformed-host.png, race-reload.png. Keep memory dumps and settings local: full guest memory can contain private values. Prefer narrowly selected non-sensitive code, arguments and state.

## Validation and next-stage limits

All 38 repository tests pass; preserved-input and coverage checks pass. The comparison harness tests detect changed bytes, truncation, missing files and tampered manifests. The original EXE remains unchanged. The accepted reference combines controlled whole-file creation, unmodified clean turn behavior, error/recovery tests, race persistence and a source-linked function trace.

Open follow-ups: ordinary-game deterministic simulation, additional function boundaries and signed inputs, robust mouse automation, RTC setup, complete companion-help behavior, callbacks and subsystem coverage. These remain explicit in the uncertainty log and steps 5/7. The initial step 4 exit criterion—a practical reference for validating reconstructed source—is met without claiming these broader questions are resolved.
