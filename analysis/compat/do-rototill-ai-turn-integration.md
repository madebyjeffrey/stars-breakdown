# DoRototillAiTurn packed/integration checkpoint

The reconstructed Rototill boundary was exercised through recovered packed
Win16 records in a fresh VC++ 1.52c / Win98 harness. The adapter binds the
CodeView layouts for the 56-byte `_planet` record and the 124-byte `_fleet`
record. It round-trips the recovered fields at `_planet` offsets 0 (`id`), 2
(`iPlayer`), 4 (the low-byte visibility value), and 9–11 (`rgMinConc`), plus
`_fleet` offsets 0 (`id`), 2 (`iPlayer`), 6 (`idPlanet`), 12–43 (sixteen
16-bit `rgcsh` entries), and 98 (`cord`).

Optimal value, desirability, and the productive-planet gate are deliberately
passed as typed service results. They are not claimed to be stored fields in
the raw `_planet` record: the original instruction trace calls
`PctPlanetOptValue` and `PctPlanetDesirability`, then evaluates the starbase
and mineral-production gate. The adapter therefore tests the recovered record
boundary without inventing offsets for those service results. Fleet ship
counts are accepted only when their recovered 16-bit values fit the typed
byte-role view.

The integration probe checks raw-record conversion, the 127-point mineral
score clamp, visible/unowned and owned/desirability classification, friendly
fleet filtering, the six-ship role policy, repair dispatch, final basic-task
and queue ordering, and the research/starbase-design handoff. Host strict C89
and native reports are:

`ROTOTILL_AI_TURN_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152rotint3/`. The Win98 process exited 0, the window inventory
returned no Program Error dialog, and the final screen is preserved as a
runtime safety record. The raw instruction listing and CodeView type recovery
provide the original-behavior boundary evidence; this is not a claim of a
complete original AI-turn replay.

This advances `DoRototillAiTurn` from Phase 2 step 4/6 to step 5/6, not
promotion. Queue construction, colonizer/wormhole and mineral-fleet routing,
raw `PLORD` mutation, complete AI-table integration, and a full original-turn
differential remain open.

Evidence:

- `tests/do_rototill_ai_turn_integration_probe.c`
- `compat/vc152/do-rototill-ai-turn-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1098_1e22.asm`
- `analysis/decompiler/export/raw/1098_1e22_DoRototillAiTurn.c`
- `runs/vmctl-vc152rotint3/ROTSTA.TXT`
- `runs/vmctl-vc152rotint3/ROTCL.LOG`
- `runs/vmctl-vc152rotint3/ROTLINK.LOG`
- `runs/vmctl-vc152rotint3/ROTR.TXT`
- `runs/vmctl-vc152rotint3/native-screen.bmp`
