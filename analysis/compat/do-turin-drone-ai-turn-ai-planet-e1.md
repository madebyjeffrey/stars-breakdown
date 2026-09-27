# DoTurinDroneAiTurn E1 packed AI-planet mutation

Original `1088:3670` writes `vlpbAiPlanet[id * 16 + 9] = 1` for every
planet in its second pass. It writes byte `+1` with the capped three-mineral
score for visible unowned planets, and byte `+2 = 1` for owned planets whose
`PctPlanetDesirability` is negative (raw export lines 166–204).

The production Turin source now optionally binds that 16-byte-per-ID table.
It validates every planet ID and table extent before the turn mutates any
record, then makes the three instruction-derived writes at the original
second-pass positions. With no table pointer, the existing typed behavior is
unchanged. This does not infer other AI bytes or manufacture a table address.

The strict-C89 and fresh VC++ 1.52c/Win98 `C:\TURAI1` probes pass **26/26**:
packed planet/fleet/PLPROD decoding and support production from the prior
probe, plus normal owned, visible unowned, negative-desirability, undersized
table, and negative-ID mutation cases. The fresh guest build reports
`COMPILE=PASS LINK=PASS`, process `EXIT 0`, and
`TURIN_PACKED_SERVICE_CHECKS=26 FAILURES=0 RESULT=PASS`; no Program Error
window appeared. The full host suite passes 539 tests with 153 expected
skips. The package is `compat/vc152/turin-ai-planet-e1/`; the build logs,
link map, object, executable, report, and agent log are in
`runs/vmctl-turai1/`.

This is supporting E1 evidence only. Live guest table enumeration, remaining
AI/production writes, growth/resource services, queue/finalization effects,
and an original-turn differential still prevent E1 closure. No named
Step-5 unit or promotion closes.
