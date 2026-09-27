# DoAutomitronAiTurn E1 AI-planet record writes

The original `analysis/decompiler/export/raw/1098_01e0_DoAutomitronAiTurn.c` writes a 16-byte `vlpbAiPlanet` record during the second planet loop (lines 160–176 and 365–374). Every visited planet gets byte 9 set to 1. An owned planet whose desirability is negative gets byte 2 set to 1. A foreign-owned planet gets byte 10 set to `((planet.flags >> 9) & 1) + 1`; if its optimal value is positive, byte 3 is set to 1. Unowned planets do not take that foreign-planet branch.

Production `reconstructed/src/do_automitron_ai_turn.c` now applies those writes to an optional caller-bound 16-byte AI-planet record before visiting the planet. A bound record shorter than 16 bytes fails safely. The strict-C89 host probe `tests/do_automitron_ai_planet_e1_probe.c` passes 14 checks across bad/good owned, unowned, and two foreign cases, plus the short-record guard. The existing unbound callers retain their prior behavior.

This is an original-instruction/host subboundary, not AU-E1 closure. It does not prove the live global table pointer, the rest of the mineral/planet action graph, the exact caller's service state, or native/original-turn behavior. AU-E1 and the final promotion remain open.
