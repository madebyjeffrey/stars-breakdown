# DoMacintiAiTurn packed/integration checkpoint

The reconstructed Macinti boundary was exercised through recovered packed
Win16 records in a fresh VC++ 1.52c / Win98 harness. The adapter binds the
56-byte `_planet` record at offsets 0 (`id`), 2 (`iPlayer`), 4 (starbase flag
word), and 18 (the low packet-strength value used by the typed threat view).
It binds the 124-byte `_fleet` record at offsets 0 (`id`), 2 (`iPlayer`), 6
(`idPlanet`), 12–43 (sixteen 16-bit `rgcsh` entries), and 98 (`cord`).

Mineral/service values, map points, production costs, design selections, order
state, and detailed AI table choices remain explicit typed service results.
They are not claimed to be stored fields in the raw records. This preserves
the original service boundaries while exercising the recovered record
conversion and the queue/order integration surface.

The integration probe checks the four merge masks, all six design-status calls
including the 5000-budget group, the two status-info calls, all five split
masks, starter/shdef/starbase/colonizer handoff, packed planet and fleet
conversion, mineral/threat and production policy, production plan/queue,
fleet-role aggregation, attack action dispatch, transport routing, final fleet
callback, and ordered basic-task/queue finalization. Host strict C89 and
native reports are:

`MACINTI_AI_TURN_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152macintiint/`. The Win98 process exited 0, the window
inventory returned no Program Error dialog, and the final screen is preserved
as a runtime safety record. The raw instruction listing and CodeView type
recovery provide the original-behavior boundary evidence; this is not a claim
of a complete original AI-turn replay.

This advances `DoMacintiAiTurn` from Phase 2 step 4/6 to step 5/6, not
promotion. Per-planet table selection, exact retarget/colonizer/transport
predicates, direct packed order mutation, and complete original-behavior
evidence remain open.

## Packed first-order repair follow-up

The readable Macinti fleet view now accepts an optional packed `PLORD`
allocation. When the recovered fleet policy marks an owned fleet for first-order
repair, the caller applies the shared raw repair before fleet-task/action
planning. The strict-C89 host probe reports
`MACINTI_AI_PLORD_REPAIR_CHECKS=5 FAILURES=0 RESULT=PASS`; it checks the count,
order-kind nibble, repair words, policy-visible ordering, and preservation of
unrelated bytes. This is host supporting evidence only: a fresh VC++ 1.52c
replay and complete personality/original-turn differential remain open.

Evidence:

- `tests/do_macinti_ai_turn_integration_probe.c`
- `tests/do_macinti_ai_turn_plord_repair_probe.c`
- `compat/vc152/macinti-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/10a0_0008.asm`
- `analysis/decompiler/export/raw/10a0_0008_DoMacintiAiTurn.c`
- `runs/vmctl-vc152macintiint/MAISTA.TXT`
- `runs/vmctl-vc152macintiint/MAICL.LOG`
- `runs/vmctl-vc152macintiint/MAILINK.LOG`
- `runs/vmctl-vc152macintiint/MAIR.TXT`
- `runs/vmctl-vc152macintiint/MAI.MAP`
- `runs/vmctl-vc152macintiint/native-screen.bmp`
