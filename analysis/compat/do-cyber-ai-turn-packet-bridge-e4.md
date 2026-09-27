# DoCyberAiTurn packet-service bridge — E4

The readable Cyber turn now exposes the packet-routing phase with the typed
state that the original turn has prepared: player id, the complete mutable
converted planet table, planet count, and computed packet pressure. The existing
player-only callback remains the fallback for older callers, so this is an
additive source-fidelity seam rather than a guessed packet implementation.

The strict C89 integration probe passes
`CYBER_AI_TURN_INTEGRATION_CHECKS=30 FAILURES=0 RESULT=PASS`. It checks the
packet callback's two-planet table, the 501 packet-strength value from the
packed planet bridge, pressure 16, packet-strength write-back to 401, and
final callback ordering after the research/basic-task phase.

This is host-only supporting evidence and closes no named Step-5 unit. Fresh
VC++ 1.52c replay, direct `DoCyberPackets`/freighter composition, complete
packed `PLORD` mutation, AI-table integration, and original-turn differential
remain open.
