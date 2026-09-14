# VC++ race compiler/output acceptance

This is the buildable successor to the RACEACC1 fixture/recipe package. It
compiles all 20 recovered component source files plus a small automatic runner.
The runner drives the actual recovered wizard procedures, saves new race files
through recovered game code and real CRT file I/O, and compares their outputs
with frozen original-x86 observations. **RETURN5 compiled and linked revision 3
cleanly and passed all 816 checks in the user's 86Box / Windows 98 SE workflow.**
All 89 player snapshots and 61 saved race files independently match the original
expectations byte for byte. The report records 536 guest seconds (8:56) and
3,158 valid checkpoints. No replacement package or repeated full build is
required. See the [RETURN5 review](../../analysis/compat/racevc-return5.md).

The same returned executable also visibly rendered progress in DOSBox / WfW
3.11. Esc, Space on the focused Cancel button and Alt+F4 each stopped cleanly,
retaining partial reports and outputs. The button run preserved all matching
files/states from 13 completed sessions. These are native keyboard/control
checks; an automated pointer click did not activate Cancel. RETURN4's
[earlier review](../../analysis/compat/racevc-return4.md) retains the silent-run
investigation and complete revision-2 replay.

The current exported package is **`ftp-share/RACEVC3/`**, also available as
`ftp-share/RACEVC3.ZIP`. Revision 3 adds a real progress window with the current
scenario/action, elapsed time, completed sessions, checks and failures. It pumps
Windows messages between stages and periodically during modeled window calls.
Cancel, Esc and window close request a stop at the next checkpoint; the report
then says `STATUS=CANCELLED` and keeps partial artifacts. No game callback is
invoked by the real progress window's message procedure.

Build a fresh copy inside the Windows 98 SE VM with `BUILD C:\MSVC`, then run
`RACEVC.EXE` and return the entire folder. Preserve on-screen diagnostics in
`OUTPUT.TXT` as well as the generated logs. The progress window should appear
immediately; allow several minutes or longer for this unoptimized build. See
[the guest instructions](README.TXT) for the full run/return procedure. No manual
wizard replay or mouse automation is required.

The program runs **88 scenarios / 89 wizard sessions**. Success produces 61
`.R1` files and 89 full 192-byte `.STA` snapshots in `ACTUAL`, plus a
`RESULTS.TXT` report and `TRACE.TXT` stage log. It expects 60 successful
reloads, one predicted U020B checksum rejection, and 28 sessions without a saved race. Native success should
report **816 checks, zero failures**; the host version has 812 checks because
four native ABI checks are conditional. The report marks host and native modes
explicitly. A failed assertion remains a failure; U020B's rejection is an
explicit expected outcome.

## What this proves, and what it leaves open

Running the complete VC build tests this component under the old compiler,
16-bit integer arithmetic, far data pointers and Windows CRT. Game results are
computed by the recovered code. Expectations are read only for assertions,
after execution. The runner captures actual player bytes and actual files,
including outputs of rejected/cancelled sessions where applicable.

The runner models window services using the same deterministic answers as the
existing original/C comparison harness. It supplies edit/radio/checkbox input
and calls recovered callbacks with Windows messages. Mouse coordinates are
computed from rectangles laid out by the recovered code; queued mouse events
execute its tracking code. Scores, race fields, save bytes and dialog outcomes
are not supplied by the adapter. Rendering calls are modeled, so this does not
finish real USER/GDI control behavior, dialog thunks, graphics, actual mouse
input, or the live U020 edit-control question. That separate work remains in
the [native UI contract](../../analysis/reconstruction/race-window-api.md).

The controlled RNG state and tick value make whole-file byte comparisons
meaningful. This is **race-file byte equality**, not a byte-identical rebuilt
game executable. The compiler memory model is an experiment choice: `/AL`
allows the semantic interface's ordinary pointers to reach a separate 64 KiB
data image, while the executable's own DGROUP holds its stack and runner data.
It does not replace the earlier evidence about the original medium-model ABI.
No whole-game startup or other menu command is counted as reconstructed here.

## Reproduction and evidence

```sh
python3 tools/package_racevc.py --output ftp-share/RACEVC3
python3 -m unittest tests.test_racevc_package -v
```

The exporter initializes the established oracle's static memory model, requiring
Unicorn JIT initialization permission on this host. It does not execute original
game code during packaging. A new folder and ZIP are required; existing build
and return folders are preserved. Package text is ASCII/CRLF with DOS 8.3 names.
Only quoted include filenames change in exported recovered sources.

`vectors.json` freezes the verified RACEACC1 observations. Each player's hash
and each expected file hash is cross-checked against the already tracked
[acceptance audit](../../analysis/compat/race-acceptance.json). `INDATA.BIN`,
`STRINGS.BIN`, and `PLANETS.BIN` come from the pinned executable and the
synthetic initial state of that same oracle. Control geometry comes from its
original dialog resources. Expected file bytes live in `EXPECT`; actual files
are created in an initially empty output area. `MANIFEST.JSN` records source,
vector and package hashes.

Local validation of revision 3 on 2026-09-14 passes eight focused tests. The
strict C89 host build still passes all 812 assertions across 88 scenarios and
matches all 89 player snapshots and 61 saved files. Tests cover frozen original
provenance and DOS package encoding, deliberately incorrect expectations,
truncated input, preservation of prior reports/traces, and cancellation at
P002 with the first 11 completed sessions preserved. A Windows declaration
check continues to reject the earlier `chdir` alias error.

The Windows-only progress module is also compiled and exercised against a
small message shim: initial display/update, time/counters, bounded message
pumping, prevention of nested pumping, Cancel, Esc, close, and window-creation
failure. This verifies our routing logic on the host; it does **not** substitute
for VC headers, Win16 calling conventions, native linking or visible rendering.
RETURN5 now supplies the real VC build and complete Win98 SE run; the returned
executable's progress/cancel controls were also checked natively in WfW 3.11.

Each trace entry is written and closed **before** its named stage. The next
entry proves that stage returned. Closing matters: during the DOSBox replay,
revision 2's report initially remained zero length to the host while open, even as closed
race and snapshot files accumulated. A stopped run now leaves the exact
scenario, dialog resource and action on disk. Cancellation is handled outside
Windows callbacks, at stage/action boundaries, then closes files and releases
runner allocations. A long calculation can delay cancellation until it returns.
Game clocks, scripted input, arithmetic and original expectations are unchanged.
