# VC++ race compiler/output acceptance

This is the buildable successor to the RACEACC1 fixture/recipe package. It
compiles all 20 recovered component source files plus a small automatic runner.
The runner drives the actual recovered wizard procedures, saves new race files
through recovered game code and real CRT file I/O, and compares their outputs
with frozen original-x86 observations. **The first native VC++ build/run remains
pending.** The [guest instructions](README.TXT) describe that next action.

The exported package is `ftp-share/RACEVC1/`, also available as
`ftp-share/RACEVC1.ZIP`. Build a fresh copy inside the user's Windows 98 SE VM
with `BUILD C:\MSVC`, then run `RACEVC.EXE` and return the entire folder. Preserve
on-screen diagnostics in `OUTPUT.TXT` as well as the generated logs. No manual
wizard replay or mouse automation is required for this experiment.

The program runs **88 scenarios / 89 wizard sessions**. Success produces 61
`.R1` files and 89 full 192-byte `.STA` snapshots in `ACTUAL`, plus a
`RESULTS.TXT` report. It expects 60 successful reloads, one predicted U020B
checksum rejection, and 28 sessions without a saved race. Native success should
report **816 checks, zero failures**; the host version has 812 checks because
four native ABI checks are conditional. The report marks host and native modes
explicitly. A failed assertion remains a failure; U020B's rejection is an
explicit expected outcome.

## What this proves, and what it leaves open

Running the returned VC build will test this component under the old compiler,
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
python3 tools/package_racevc.py --output ftp-share/RACEVC1
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

Local validation on 2026-09-14 passed four tests: the strict C89 host build and
all 88 scenarios; frozen provenance, package integrity and DOS filename/text
checks; a deliberately wrong expected file that fails comparison while the
generated file stays unchanged; and truncated input that fails clearly. The
full run also verifies every `.STA` snapshot and `.R1` output independently and
confirms that rerunning preserves existing results. Native compiler diagnostics,
native assertions and a returned artifact review are still required.
