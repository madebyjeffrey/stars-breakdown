# DoCyberAiTurn packed/integration checkpoint

The reconstructed Cyber boundary was exercised through recovered packed Win16
records in a fresh VC++ 1.52c / Win98 harness. The adapter binds the 56-byte
`_planet` record at offsets 0 (`id`), 2 (`iPlayer`), 4 (the raw flag word used
for visibility/scanned and starbase checks), and 18 (the low packet-strength
value used by the typed threat view). It binds the 124-byte `_fleet` record at
offsets 0 (`id`), 2 (`iPlayer`), 6 (`idPlanet`), 12–43 (sixteen 16-bit `rgcsh`
entries), and 98 (`cord`).

Habitat and mineral/service values, production resources/costs, and detailed
order decisions remain typed service results. They are not claimed to be
stored fields in the raw records. This preserves the original service
boundaries while exercising the recovered record conversion and Cyber policy
surface.

The integration probe checks all five merge masks, the three shared
design-status calls and their first split-mask consumption, all four late split
masks, research-order and shdef handoff, packed planet and fleet conversion,
scanned low-mineral classification, hostile packet-strength threat marking,
four-resource production affordability, fleet-role aggregation, friendly-fleet
filtering, and ordered basic-task/packet/queue finalization. Host strict C89
and native reports are:

`CYBER_AI_TURN_INTEGRATION_CHECKS=25 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152cybint/`. The Win98 process exited 0, the window inventory
returned no Program Error dialog, and the final screen is preserved as a
runtime safety record. The raw instruction listing and CodeView type recovery
provide the original-behavior boundary evidence; this is not a claim of a
complete original AI-turn replay.

This advances `DoCyberAiTurn` from Phase 2 step 4/6 to step 5/6, not
promotion. Packet routing, freighter behavior, detailed fleet-order repair,
direct packed `PLORD` mutation, complete AI-table integration, and full
original-behavior evidence remain open.

Evidence:

- `tests/do_cyber_ai_turn_integration_probe.c`
- `compat/vc152/do-cyber-ai-turn-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/10a8_002a.asm`
- `analysis/decompiler/export/raw/10a8_002a_DoCyberAiTurn.c`
- `runs/vmctl-vc152cybint/CYBISTA.TXT`
- `runs/vmctl-vc152cybint/CYBICL.LOG`
- `runs/vmctl-vc152cybint/CYBILINK.LOG`
- `runs/vmctl-vc152cybint/CYBIR.TXT`
- `runs/vmctl-vc152cybint/CYBINT.MAP`
- `runs/vmctl-vc152cybint/native-screen.bmp`
