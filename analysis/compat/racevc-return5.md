# RETURN5 race compiler/output and progress-runner acceptance

**RETURN5 passes the complete revision-3 native race test in the user's
86Box / Windows 98 SE workflow.** All 88 scenarios / 89 wizard sessions passed
816 checks with zero failures. Every one of the 89 player snapshots and 61
saved race files independently matches the frozen original-x86 expectation
byte for byte. No replacement package or repeat full build is required.

The returned folder is `ftp-share/RETURN5/Racevc3`. All **105** manifest-listed
package inputs match RACEVC3; the manifest itself is also unchanged. All 24
expected object files are nonempty. `BLDSTAT.TXT` reports compile and link PASS;
the returned compiler/linker logs contain no warnings or errors. There is no
separate `OUTPUT.TXT`, so this review makes no claim about unreturned console
text. The linker is 5.60.339 and the map includes the exported `ProgressWndProc`.

The returned executable SHA-256 is
`63d3fa1ab762fcc632656715e633ec06a209c5fe55782780fa9ea091011006cb`.
Artifact hashes, logs, individual output comparisons and the trace audit are
in [the evidence snapshot](racevc-return5.json). Raw returned files are preserved.

## Complete output and trace checks

The report ends with:

```
CASES=88 SESSIONS=89 CHECKS=816 FAILURES=0
ELAPSED_SECONDS=536 CHECKPOINTS=3158
STATUS=PASS
```

Its **8 minutes 56 seconds** is measured by the guest's `GetTickCount`, not an
independent wall-clock measurement. All 3,158 trace entries have consecutive
sequence numbers, nondecreasing times/check counts/completed-session counts,
and zero recorded failures. The 89 session-complete entries occur in the
expected order. No adjacent recorded timestamps differ by more than one
second. These are whole-second checkpoint timestamps; they do not measure
subsecond input latency or prove continuous repainting in the user's VM.

The final trace has 812 checks, followed by 816 in the report. This is expected:
the four final suite assertions run after the last session checkpoint. It is
unrelated to the host build's separate 812-check total, which excludes four
native ABI assertions.

All 60 expected reload successes passed. U020B's original checksum rejection
also passed as an expected outcome. The 28 negative-point/cancel sessions
created no race file, as expected. Every per-session filename, score and reload
result agrees with the frozen catalog. The warning text for those negative
balances, the synthetic incorrect-password attempt and U020B is intentional;
none indicates a failed acceptance assertion.

## Native progress and cancellation checks

The exact returned executable was run in three fresh copies of the unchanged
package under DOSBox-X / WfW 3.11. Its window appeared immediately and visibly
updated the scenario, completed sessions, page/action, elapsed time, checks and
failure count. The labels and Cancel button fit the window. A full native run
was already supplied by RETURN5; these runs test the previously unverified
progress/cancellation behavior rather than repeating that catalog.

| Input | Preserved test folder | Completed sessions | Outcome |
|---|---|---:|---|
| Esc | `runs/step4/guest/RVC3ESC` | 0 | Cancelled at a checkpoint; zero failures; final report and trace retained. |
| Space on the focused Cancel button | `runs/step4/guest/RVC3BTN` | 13 | Cancelled; all 13 snapshots and 10 race files retained and independently matched. |
| Alt+F4 window close | `runs/step4/guest/RVC3CLS` | 0 | Cancelled at a checkpoint; zero failures; final report and trace retained. |

Each displayed the cancelled summary, wrote `STATUS=CANCELLED` and returned
control to Program Manager. No source or executable patch was used. The
control checks ran in WfW 3.11; RETURN5 supplies the full Win98 SE result.
An automated coordinate click did not activate Cancel in the button test;
Space did. These checks establish native button command handling and keyboard
cancellation, not physical-mouse delivery through this emulator's automation.

## Milestone status

The native compiler/output and runner progress/cancellation milestones are
complete with the environment scopes above. The tested implementation still
models the race wizard's Windows services; its real progress window is runner
UI. A real six-page race-wizard adapter, original-game live acceptance and
whole-executable equivalence remain separate work. The source-coverage count
is unchanged. RACEVC3 and all return folders remain preserved as tested.
