# RETURN4 race build and interrupted-run review

The returned `ftp-share/RETURN4/Racevc2` matches all **104** manifest-listed
revision-2 inputs, including the unchanged manifest. VC++ 1.52c compiled all
23 C modules and linked the Win16 executable successfully. `BUILD.LOG` contains
no compiler warnings or errors. The previous launcher alias problem and the
two signed-comparison warnings are resolved. Hashes, logs and output comparisons
are retained in [the evidence snapshot](racevc-return4.json).

The user reported stopping after about **30 seconds to one minute**, with no
visible feedback. The returned report records 11 passing scenarios: B000,
T000 through T007, P000 and P001. It stops at `RUN P002 P002.R1`, without a final
status. Every one of the **11 player snapshots and nine race files** matches
the frozen original expectation byte for byte. Those nine files also reloaded
successfully according to the report. P000/P001 correctly reject saving because
their advantage-point balances are negative. RETURN4 alone is a partial run,
not an acceptance pass and not proof that P002 hangs.

## Why the runner looked frozen

Revision 2 runs synchronously in `WinMain`, creates no visible window, and only
shows a message after all 89 wizard sessions finish. Its modeled `PeekMessage`
supplies synthetic mouse input to the game; it never calls the real Windows
message queue. There is no explicit message pump or cancellation path during
the test. The missing feedback is a runner defect.

Windows 3.x and Win16 tasks on Windows 95 use cooperative scheduling; a task
that does not yield can prevent other 16-bit work from running. Microsoft's
[Q117567](https://jeffpar.github.io/kbarchive/kb/117/Q117567/) describes that
behavior. Its [Windows SDK guidance on background work](https://jeffpar.github.io/kbarchive/kb/074/Q74042/)
uses `PeekMessage`, `TranslateMessage` and `DispatchMessage` during lengthy
processing. These explain the responsiveness problem; they do not by themselves
diagnose any particular recovered function as stuck.

The supplied build deliberately uses `/Od` and a large-model semantic interface,
including software arithmetic for original floating-point behavior. It is a
diagnostic build, not a performance-equivalent rebuild of Stars. The timing of
this runner should not be taken as the speed of the original game.

## Independent replay

A fresh copy of revision-2 inputs and the **exact returned RACEVC.EXE** was
staged at `runs/step4/guest/RVC2T` and launched in DOSBox-X / WfW 3.11. The
executable SHA-256 is
`645d4e140f217473a3cc60a1a8d72484652f769c2f65beb60b14251f7edd5290`.
It progressed through P002 and later cases without any binary or input changes.
The run began at approximately 10:13:17 UTC on 2026-09-14 at the configured
20,000 cycles/ms, normal CPU core. At 10:19:41 UTC, Turbo fast-forward was
enabled to finish the remaining output checks. This is not a controlled
whole-suite performance benchmark or a timing estimate for the user's 86Box VM.

The run finished successfully, observed by 10:26:13 UTC: **88 scenarios,
89 sessions, 816 checks, zero failures; STATUS=PASS**. Every one of the 89
player snapshots and 61 saved race files was independently compared with the
frozen originals and matched exactly. All 60 expected successful reloads and
the intentional U020B rejection passed. No files appeared for the 28 fileless
sessions. The native completion dialog confirmed the same counts. Turbo was
then disabled, restoring normal speed. The guest report's filesystem timestamp
reflects its DOS clock and is not used as a wall-clock completion measurement.

This completes the native race compiler/output milestone for this catalog
using the user's VC-built executable in WfW 3.11. The user's Win98 SE run remains
partial. Real race-wizard USER/GDI acceptance and whole-game reconstruction are
still separate work; revision 3's new runner window has not yet run natively.

While the report initially remained open, DOSBox exposed a zero-length `RESULTS.TXT` to
host-side file inspection despite the source calling `fflush` after every RUN
and PASS. Closed `.STA` and `.R1` files accumulated normally. This is why the
new trace closes each checkpoint file write; a flushed, still-open report alone
was insufficient for observing this run externally.

## Revision 3

`ftp-share/RACEVC3` / `RACEVC3.ZIP` adds a real Win16 runner window with scenario,
wizard action, elapsed time, completed sessions, checks and failure counts.
Cancel, Esc and window close request a safe stop at the next stage/action
boundary. Cancellation preserves outputs and ends with `STATUS=CANCELLED`,
even if no assertion has failed. The window procedure is exported in the
module-definition file; its messages never call into recovered game code.

The runner pumps the real Windows queue between stages and every 64 modeled
window calls, with a bounded pump and a guard against nested pumping. A pending
cancel is acted on outside the native window procedure. `TRACE.TXT` records
each entered stage, including dialog resource and action number, and is closed
before that stage runs. Cleanup closes the data file and releases runner
allocations. A long calculation can delay cancellation until a checkpoint.

All 20 recovered game sources and all binary test inputs/expectations remain
unchanged from revision 2. The host build still passes **812 checks**, with all
89 player states and 61 race files matching. Eight focused tests cover the full
catalog, original provenance, mismatched expectations, truncated inputs,
preserving existing traces/results, partial-run cancellation, Windows launcher
declarations, and Windows-only progress routing against a small message shim.
The shim checks display updates, bounded/nested pumping, Cancel/Esc/close and
window-creation failure; it does not test native rendering, SDK ABI or linking.

Build a fresh RACEVC3 copy with `BUILD C:\MSVC`, run `RACEVC.EXE`, and return the
whole folder including `RESULTS.TXT`, `TRACE.TXT` and `ACTUAL`. The progress
window should appear immediately. Native revision-3 compilation and visible
progress/cancellation remain to be checked in the VM. Revision 2, both returned
folders and the independent replay artifacts are preserved.
