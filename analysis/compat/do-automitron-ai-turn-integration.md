# DoAutomitronAiTurn packed/integration checkpoint

The reconstructed Automitron boundary was exercised through recovered packed
Win16 records in a fresh VC++ 1.52c / Win98 harness. The adapter binds the
56-byte `_planet` record at offsets 0 (`id`), 2 (`iPlayer`), and 4 (the raw
flag word used for visibility/scanned checks). It binds the 124-byte `_fleet`
record at offsets 0 (`id`), 2 (`iPlayer`), 6 (`idPlanet`), and 12–43 (sixteen
16-bit `rgcsh` entries), converting only values that fit the typed byte view.

Planet class, optimal/desirability values, mineral service values, production
resources/costs, design selections, and detailed order decisions are supplied
as typed service results. They are not claimed to be stored fields in the raw
records. This preserves the original service boundaries while exercising the
recovered record conversion and the five-hull affordability path.

The integration probe checks the three late merge masks, all four shared
design-status mask writes, research-order and shdef handoff, scanned/mineral
production gating, four-channel queue affordability, unowned/owned planet
classification, current-player fleet filtering, the four typed fleet-slot
aggregates, and ordered basic-task/queue finalization. Host strict C89 and
native reports are:

`AUTOMITRON_AI_TURN_INTEGRATION_CHECKS=20 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152autoint/`. The Win98 process exited 0, the window inventory
returned no Program Error dialog, and the final screen is preserved as a
runtime safety record. The raw instruction listing and CodeView type recovery
provide the original-behavior boundary evidence; this is not a claim of a
complete original AI-turn replay.

This advances `DoAutomitronAiTurn` from Phase 2 step 4/6 to step 5/6, not
promotion. Colonizer and mineral fleet handling, fleet-order mutation, the
remaining packed order integration, and complete original-behavior evidence
remain open.

Evidence:

- `tests/do_automitron_ai_turn_integration_probe.c`
- `compat/vc152/do-automitron-ai-turn-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1098_01e0.asm`
- `analysis/decompiler/export/raw/1098_01e0_DoAutomitronAiTurn.c`
- `runs/vmctl-vc152autoint/AUTOSTA.TXT`
- `runs/vmctl-vc152autoint/AUTOCL.LOG`
- `runs/vmctl-vc152autoint/AUTOLINK.LOG`
- `runs/vmctl-vc152autoint/AUTOR.TXT`
- `runs/vmctl-vc152autoint/AUTO.MAP`
- `runs/vmctl-vc152autoint/native-screen.bmp`
