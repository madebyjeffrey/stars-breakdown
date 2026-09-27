# FGenerateTurn

`FGenerateTurn` is the turn coordinator. Its most important observable
property is the ordered transaction across the turn subsystems, so
`reconstructed/src/generate_turn.c` expresses that order directly instead of
replaying packed global-memory instructions.

The typed pipeline preserves the load gate, shuffled per-player log loading,
two order passes, pre- and post-production movement, mine-field and fleet
phases, production, healing and terraforming, cheating/waypoint/guess updates,
scoring, per-player output, sentinel output, progress milestones, and cleanup
on both success and failure. The player permutation follows the original
forward Fisher-Yates loop and calls the shared RNG with limits
`cPlayer, cPlayer-1, ..., 1`; the final `Random(1)` call still advances both
seed words. A missing or out-of-range RNG result fails before log processing,
so the coordinator does not silently substitute numeric player order. The
callbacks own the shared fleet, planet, message, file, and allocation records
that the original routine coordinates.

The strict-C89 source probe checks the exact `[3,2,1]` shuffle call limits,
shuffled log order, and the full callback sequence from load and initialization
through both order passes, movement, production, turn advancement, player and
aggregate file writes, and cleanup. It also checks the early log-failure
sequence, invalid/missing RNG handling, and mode arguments. The full turn state
remains behind callbacks; this coordinator binding alone is not a differential
against a complete original turn. The automated host test compiles and runs
this probe under strict C89 and passes.
