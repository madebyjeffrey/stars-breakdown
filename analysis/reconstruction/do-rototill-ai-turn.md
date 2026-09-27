# DoRototillAiTurn reconstruction (partial)

`reconstructed/src/do_rototill_ai_turn.c` transfers the Rototill research,
planet classification, dispatch, and finalization phases of `DoRototillAiTurn` (NE
`0014:1e22–301f`) into typed C.  It preserves the early/late AI order,
Turin-shdef maintenance boundary, mineral concentration scoring, visible
unowned-planet counting with the positive optimal-value gate,
owned-desirability tracking, productive-planet
selection, typed friendly-fleet ownership filtering and role classification,
order-repair callback dispatch,
the `IroEnsureAi` research-order
handoff, and planet/fleet dispatch boundaries.

A packed integration adapter now binds the recovered 56-byte `_planet` and
124-byte `_fleet` layouts, including the 16-bit ship-count array, and checks
the classification, fleet-role, repair, and finalization boundary in both
host C89 and VC++ 1.52c / Win98. Optimal value and desirability remain typed
service results because the original procedure calls those services rather
than loading invented fields from `_planet`.

The source probes pass the early/late setup, research-order handoff, ordered
finalization, planet classification including the optimal-value boundary,
typed fleet-policy paths, and null-table entry rejection under strict host
C89. A fresh VC++ 1.52c / Win98 source checkpoint passes all 15 native checks,
and the packed integration checkpoint passes 12 additional native checks in
fresh `C:\\ROTINT3`, without a runtime error dialog. Complete queue
construction, colonizer/wormhole and mineral-fleet routing, raw `PLORD`
mutation, complete AI-table integration, and full original-turn differential
evidence remain open. The procedure is now at Phase 2 step 5/6, not promoted.
