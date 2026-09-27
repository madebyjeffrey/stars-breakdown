# DoRobotoidAiTurn packed/integration checkpoint

The reconstructed Robotoid boundary was exercised through recovered packed
Win16 records in a fresh VC++ 1.52c / Win98 harness. The adapter binds the
CodeView layouts for the 56-byte `_planet` record and the 124-byte `_fleet`
record. It round-trips `_planet` offsets 0 (`id`), 2 (`iPlayer`), 4 (the
starbase/visibility word), and the low twelve packet-strength bits at offset
18 (`uPopGuess`), plus `_fleet` offsets 0 (`id`), 2 (`iPlayer`), 6
(`idPlanet`), 12–43 (sixteen 16-bit `rgcsh` entries), and 98 (`cord`).

Production resources and costs are deliberately supplied as typed service
results. The original procedure obtains them through
`GetResourcesAvailable`, `GetProdQCost`, and `GetTrueHullCost`; the packed
planet record does not expose those values as the typed source's direct
fields. Design selections and split masks likewise enter through the
`CheckAiShdefStatus` service boundary. The adapter therefore checks the
recovered record layout without inventing offsets for service-owned data.

The integration probe checks raw-record conversion, the shared status/split
mask sequence, armada potency, hostile threat-band calculation, production
affordability and five-hull queueing, war-fleet counting, typed fleet-role
classification, research/design handoff, and final callback ordering. Host
strict C89 and native reports are:

`ROBOTOID_AI_TURN_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152robint/`. The Win98 process exited 0, the window inventory
returned no Program Error dialog, and the final screen is preserved as a
runtime safety record. The raw instruction listing and CodeView type recovery
provide the original-behavior boundary evidence; this is not a claim of a
complete original AI-turn replay.

This advances `DoRobotoidAiTurn` from Phase 2 step 4/6 to step 5/6, not
promotion. Fleet-order repair, colonizer/transport routing, raw `PLORD`
mutation, complete AI-table integration, and full original-turn differential
remain open.

Evidence:

- `tests/do_robotoid_ai_turn_integration_probe.c`
- `compat/vc152/do-robotoid-ai-turn-integration/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1088_0312.asm`
- `analysis/decompiler/export/raw/1088_0312_DoRobotoidAiTurn.c`
- `runs/vmctl-vc152robint/ROBSTA.TXT`
- `runs/vmctl-vc152robint/ROBCL.LOG`
- `runs/vmctl-vc152robint/ROBLINK.LOG`
- `runs/vmctl-vc152robint/ROBR.TXT`
- `runs/vmctl-vc152robint/native-screen.bmp`
