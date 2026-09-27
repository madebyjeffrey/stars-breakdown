# DoRototillAiTurn RT-E1 native packet

The fresh compat/vc152/rototill-production-e1-r1/ package copies current
production ROT.C and CORE.H, the 15-check strict-C89 packed production
probe, a Win16 report driver, and DOS CRLF wrappers for C:\RTE1N1. Its
BUILD.BAT refuses a directory with BLDSTAT.TXT, compiles source/probe/
driver separately, records compile/link status and logs, and links a
Win16 RTE1.EXE. The executable writes RTE1RES.TXT with the probe result.
The package-layout test checks ten files, CRLF, wrapper directory,
entrypoint rename, conservative line length, and overwrite refusal.

No VC++ 1.52c compile/link, guest run, or original-turn comparison has
occurred. On STARSAGT reconnection, transfer all ten files to a fresh
C:\RTE1N1, run RUNBLD.BAT via command.com /c, retrieve status and every
compile/link log, run RUN.BAT, and retrieve RTE1RES.TXT, executable,
objects, and map to a unique host runs/ directory. Process exit alone
is not a pass.

The current replacement is `compat/vc152/rototill-production-e1-r2/`
for fresh guest directory `C:\RTE1N2`. It contains the updated 20-check
probe and packed-planet/AI-table source; the r1 package described above is
retained for history and must not be used to validate current source. The
same build/run/return checklist applies to r2. No r2 guest return exists.

The current packet is r3 for `C:\RTE1N3`. It additionally compiles
`FLPROBE.C`, checks the packed fleet/flag seam (5 checks), and reports both
the production and fleet probe results. The prior r1/r2 packets are stale
for the current source; preserve them, but do not use them as current
native validation. Retrieve `FLEETCL.LOG` along with the other compile,
link, executable, and report artifacts. No r3 guest return exists.

The current packet is r4 for `C:\RTE1N4`, with the expanded 12-check
packed-fleet probe (including the unowned-destination marker). The same
build/run/return checklist applies; r1–r3 are retained but stale for
current source. No r4 guest return exists.

The current packet is r5 for `C:\RTE1N5`. Its packed-fleet probe has
17 checks, including the occupied-destination select/order/refresh/task
sequence. Retain older packets but use r5 for a current native run and
retrieve `FLEETCL.LOG` with all other build and runtime artifacts.
No r5 guest return exists.

The current packet is r6 for `C:\RTE1N6`. It adds `LQPROBE.C`, a
7-check post-init live-PLPROD probe, to the 20-check production and
17-check packed-fleet probes. Retrieve `LQCL.LOG` as well as the other
compile/link logs, map, objects, executable, and result report. Older
r1–r5 packets are retained but stale for the current source. No r6
guest return exists.

The current packet is r7 for `C:\RTE1N7`. Its live-queue probe has
12 checks, including post-add relocation and malformed post-add bounds.
It supersedes r6 and earlier packets for the current source. No r7
guest return exists.

## Guest execution, 2026-09-26

The current-source r8 packet compiled all five objects with VC++ 1.52c,
but its Win16 link failed with unresolved `_printf` references from the
three probes. Preserve the failed build and link logs in
`runs/rototill-production-e1-r8/`; no executable result exists for r8.

The r9 package adds a Win16 driver stub for the probes' host-only console
summary. It was transferred to fresh `C:\RTE1N9`. The guest controller
reported `STARSAGT 0.4.2`, `PONG`, and the desktop windows before the run.
`runs/rototill-production-e1-r9/BLDSTAT.TXT` records `COMPILE=PASS` and
`LINK=PASS`. The linked program exited 0 and `RTE1RES.TXT` reports
`PRODUCTION_CHECKS=20 FLEET_CHECKS=17 LIVE_QUEUE_CHECKS=12 RESULT=PASS`
(49/49). The return directory retains compile/link logs, map, five objects,
executable, report, and agent log. The package-layout test also passes.

This is a focused native source probe. It does not bind the original live
`lpplProdGlob` identity, queue costs, or original turn result, so RT-E1
remains open.

The current r10 packet adds an 11-check three-order PLORD/AI-table probe
for RT-E3 at `C:\RTE1N10`. It compiled, linked, and ran in StarDev; the
combined report passes 60/60. The [RT-E3 record](do-rototill-multi-order-e3.md)
separates the covered count/marker branches from the unbound reroute writes.

Fresh r11 added the bounded 12-check transport reroute packet at
`C:\RTE1N11`, passing 72/72. Fresh r12 adds malformed-target preflight
to the current source and tests it with a 14-check reroute probe at
`C:\RTE1N12`. It compiled, linked, and ran in StarDev with a combined
[74/74 PASS report](../../runs/rototill-production-e1-r12/RTE1RES.TXT). The
[RT-E3 continuation](do-rototill-multi-order-e3.md#transport-reroute-continuation-2026-09-26)
records the exact write and remaining limits.

Fresh r13 adds the bounded deep-space first-order-kind branch in the
second fleet sweep. The 10-check branch probe and previous packets compiled,
linked, and ran in StarDev at `C:\RTE1N13`; the combined
[report passes 84/84](../../runs/rototill-production-e1-r13/RTE1RES.TXT).
The [RT-E3 record](do-rototill-multi-order-e3.md#deep-space-first-order-kind-continuation-2026-09-26)
states the covered predicate and outstanding branches.

Fresh r14 extends the first-kind branch to selected current planets and
nearest-starbase outcomes, and corrects the deep-space source call order to
two selections. It compiled, linked, and [passed 93/93](../../runs/rototill-production-e1-r14/RTE1RES.TXT)
at `C:\RTE1N14`. The [RT-E3 continuation](do-rototill-multi-order-e3.md#selected-planet-and-starbase-continuation-2026-09-26)
records the predicate and remaining original-state limits.

Fresh r15 adds the 10-check scout dispatch probe and passes
[103/103](../../runs/rototill-production-e1-r15/RTE1RES.TXT) at
`C:\RTE1N15`; the [scout caller record](do-rototill-scout-dispatch-e2.md)
defines its scope. Fresh r16 adds the 9-check owned-planet colonizer marker
probe. The guest reported `STARSAGT 0.4.2`, `PONG`, and Win98 desktop
windows; r16 then compiled, linked, and exited 0 at `C:\RTE1N16`. Its
[returned report](../../runs/rototill-production-e1-r16/RTE1RES.TXT)
passes 112/112. The unique return directory includes build status, all
compile/link logs, map, objects, executable, report, agent log, and a
[SHA-256 manifest](../../runs/rototill-production-e1-r16/MANIFEST.JSON).
The [RT-E3 record](do-rototill-multi-order-e3.md#colonizer-ai-byte-15-marker-continuation-2026-09-27)
specifies the bounded marker branch and remaining original-state gaps.
