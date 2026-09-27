# DoRototillAiTurn retained four-fleet AI phase

Status: **host and Win16 composition pass; RT-E1/E2/E4 remain open**.

[`test_rototill_retained_ai_phase.py`](../../tests/test_rototill_retained_ai_phase.py)
materializes all four owner-1 compact fleet/order pairs and planet 24 from
the [retained original turn oracle](do-rototill-turn-r1-oracle.json). For the
56-byte planet view it uses the existing `InputReader` load of Turin's M2
control input. The [variant audit](do-rototill-turin-variant-r1.md) proves
that this decoded input differs from Rototill's only in the player selector
byte; the probe also checks loaded minerals and concentrations against the
Rototill compact record. Fleet and PLORD records are semantic materializations
of the compact input, not captures of original Win16 heap allocations.

The [C89 probe](../../tests/do_rototill_retained_ai_phase_probe.c) calls
`stars_do_rototill_ai_turn` once with planet 24 and fleets 0–3. It binds the
production gate, transport classification, both fleet sweeps, scout adapter,
original hull/engine/XY/wormhole data, AI planet table, final basic-task
and queue-service callbacks. The retained 250 colonists occupy raw planet
bytes 40–43; the original per-planet gate checks the high word at byte 42
and the low word against 999. Thus this input does **not** reach the direct
`AddItemToQueue` branch. The later `FillProductionQueue` callback is invoked,
but its implementation and the persisted original type-28 body `011c9205`
are not reproduced here. The original output queue cannot be attributed to
the direct Rototill queue-add branch from this witness. The type-28 word is
one planet factory item, quantity 1, at 89% progress, while factories on
planet 24 increase from 10 to 12. The remaining source path spans the final
queue service and `Produce` before the output writer.

The 11 assertions cover one successful personality call, no direct queue
adds, the final queue callback, seven transport classifications, two fleet
selections and one refresh, owned planet AI mark, fleet 0's added target-26
warp-5 PLORD, unchanged first orders for fleets 1–3, and scout status
marks. The host strict-C89 test passes. The [fresh VC1.52c package](../../compat/vc152/rototill-retained-ai-phase-r1/)
was built and run in StarDev `C:\RTAIP1`: [`BLDSTAT.TXT`](../../runs/rototill-retained-ai-phase-r1/BLDSTAT.TXT)
has `COMPILE=PASS` and `LINK=PASS`; [`RTAIRES.TXT`](../../runs/rototill-retained-ai-phase-r1/RTAIRES.TXT)
has `AI_PHASE_CHECKS=11 RESULT=PASS`. The [run manifest](../../runs/rototill-retained-ai-phase-r1/RUN.JSON)
pins all package and return hashes, including compiler logs, map, objects,
executable, and agent log. The guest agent answered `PONG` before the run
and remained responsive afterward; the host listener was stopped normally.

The scout probe supplies a controlled `Random(100)=99`, so the actual
original RNG state and wormhole-call branch are not established. There is
no native movement, resource/production execution, compact output writer,
or full original turn differential in this probe. RT-E1 still needs live
global queue identity and the actual `FillProductionQueue` result; RT-E2
still needs original RNG and live allocator/global identity; RT-E4 still
needs the selector-3 full-turn harness and output comparison.
