# DoTurinDroneAiTurn packed/integration checkpoint

The reconstructed Turin boundary was exercised through recovered packed Win16
records in a fresh VC++ 1.52c / Win98 harness. The adapter binds the CodeView
layouts for the 56-byte `_planet` record and the 124-byte `_fleet` record. It
round-trips `_planet` offsets 0 (`id`), 2 (`iPlayer`), 4 (the low-byte
visibility value), and 9–11 (`rgMinConc`), plus `_fleet` offsets 0 (`id`), 2
(`iPlayer`), 6 (`idPlanet`), 12–43 (sixteen 16-bit `rgcsh` entries), and 98
(`cord`).

Production resources, queued costs, hull costs, design selections, and the
fleet order discriminator are deliberately supplied as typed service results.
They are not claimed to be stored fields in the raw records. This preserves
the original call boundary while still exercising the recovered record
conversion and the five-hull affordability path.

The integration probe checks raw-record conversion, all four merge masks, the
shared design-status mask and technology-independent selection, two-pass
visible/unowned and owned/desirability classification, production preparation
and queueing, four-channel affordability, fleet-role totals and repair policy,
and the ordered final callbacks. Host strict C89 and native reports are:

`TURIN_DRONE_AI_TURN_INTEGRATION_CHECKS=20 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152turint/`. The Win98 process exited 0, the window inventory
returned no Program Error dialog, and the final screen is preserved as a
runtime safety record. The raw instruction listing and CodeView type recovery
provide the original-behavior boundary evidence; this is not a claim of a
complete original AI-turn replay.

This advances `DoTurinDroneAiTurn` from Phase 2 step 4/6 to step 5/6, not
promotion. Detailed production policy, fleet-order mutation, colonizer and
transport routing, complete AI-table integration, and a full original-turn
differential remain open.

Evidence:

- `tests/do_turin_drone_ai_turn_integration_probe.c`
- `compat/vc152/do-turin-drone-ai-turn-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1088_3670.asm`
- `analysis/decompiler/export/raw/1088_3670_DoTurinDroneAiTurn.c`
- `runs/vmctl-vc152turint/TURSTA.TXT`
- `runs/vmctl-vc152turint/TURCL.LOG`
- `runs/vmctl-vc152turint/TURLINK.LOG`
- `runs/vmctl-vc152turint/TURR.TXT`
- `runs/vmctl-vc152turint/TUR.MAP`
- `runs/vmctl-vc152turint/native-screen.bmp`
