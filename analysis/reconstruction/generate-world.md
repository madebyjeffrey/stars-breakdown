# GenerateWorld reconstruction (partial)

`reconstructed/src/generate_world.c` transfers the deterministic galaxy-layout,
planet-initialization, player-start-selection, player/race-initialization, and
homeworld-assignment phases of the 17,753-byte
`GenerateWorld` procedure at NE `0010:0136–468f` into typed C. It covers map
extent and density calculation, random candidate-point generation,
minimum-distance rejection, compaction, optional randomly selected
nearest-neighbour smoothing, unique random planet-ID assignment including the
special-map terminal-ID wrap boundary,
environment generation, mineral concentration generation, special-world flags,
fixed-rate setup, bounded start-location search, final start-order shuffling,
race-tech defaults, transient player-state, research/resource/score, and relation resets, race-flag
adjustments, initial message accounting,
homeworld ownership flags, packed homeworld mine/factory/defense initialization,
starting population,
enhanced-mineral population scaling, advantage-point capping, and
race-stat-specific mineral-rate and mine/factory/defense adjustments, race-stat concentration
redistribution and capacity clearing, concentration floors, environment
selection, the ordinary race-specific
startup-ship design sequence, including the race-6 homeworld-starbase mutation,
the race-6/race-7 nearby-special startup callback and its ship-count effect,
the special nearby-planet population split, bounded fresh-environment and
mineral-rate generation,
production-slot mutation, and state mutation, and the per-player ship-design
component fallback walk, the race-8 maximum homeworld scanner mutation, and the
packed world-stream/per-player data-file output phase, and post-write
batch/hosted-game finalization control flow. The battle-plan phase also
preserves the raw cleanup of provisional mineral rates on an unowned first
planet.
The start selector widens its minimum/maximum distance band and retries the
complete selection when the original sequential fallback exhausts its cycle.
It also models the five battle plans allocated and initialized for each player
before wormhole generation.
It also models the two-ended wormhole creation loop, bounded position
validation, best-position fallback, and endpoint linking.
It also transfers the death-message, eligible-player selection, computer salt,
and single-player-relation setup that follows wormhole creation.

The layout phase preserves the original distinction between point position and
assigned planet ID: smoothing chooses a new point through the RNG on each
iteration, then compacted points receive non-repeating IDs from the original
0..998 domain, with the special-map collision walk admitting terminal ID 999;
packed point records now emit those IDs rather than array positions. The
layout and planet flag/threshold policy is expressed with named constants in
the C source, retaining the original packed values and fixed-point boundaries.
The top-level typed pipeline also materializes the compacted point
table into each typed planet before start/homeworld callbacks consume it, so
coordinate-dependent semantic callbacks see the same planet-to-point mapping.
Mineral-rate initialization also preserves the low-rate fallback,
enhanced-mineral adjustment, and the fixed-mineral mode's explicit suppression
of the later random concentration override. The game-file naming/loading details remain callback-owned, and the broader UI
integration remains open. The expanded source
compiles cleanly under host C89, and the source probe passes layout, unique
planet IDs, planet
initialization, three-player start selection, four-player race-tech
initialization, homeworld assignment, the ordinary four-player startup-ship
sequence, the special nearby-planet path, packed file-output callbacks, and
normal/hosted finalization callbacks.
The battle-plan, wormhole, and player-status orchestration probes also pass.
The point-ID mode probe confirms the ordinary wrap at 999 and special-map
acceptance of terminal ID 999.
The explicit typed top-level phase pipeline now materializes planet coordinates
before start selection and homeworld assignment, matching the original phase
boundary, and passes an additional seven-check probe covering the complete
layout-to-file/finalization order without using assembly or emitted-byte replay.
The fresh VC1.52 rebuild in `runs/vmctl-vc152gwt/` now compiles and links the
expanded harness and runs it in the Win98 guest without the prior argument-space
error dialog. The native report is
`GENERATE_WORLD_CHECKS=31 FAILURES=0 RESULT=PASS`. The package required a
directory-first wrapper, a local `WINSTUB.EXE`, and a report path matching the
fresh guest directory; those boundaries are now explicit in the harness.

The creation/file/UI acceptance boundary is now also exercised against the
original executable in Win98. The original title, New Game dialog, generated
game frame, and the resulting `.xy`, `.M1`, `.M2`, `.M3`, and `.HST` artifacts
are preserved in `runs/vmctl-vc152current/`. The original instruction order
for the type-7 game record, packed point stream, player-count record,
per-player data files, and post-write UI transition is documented in
`analysis/compat/generate-world-integration.md` and captured in
`analysis/compat/generate-world-integration.json`. This advances the
procedure to Phase 2 step 5/6 for that boundary; at that earlier checkpoint,
full packed game-state and broader resource/UI integration remained open.

The first named Step-5 unit is now closed: the source exposes the recovered
64-byte `_game` storage boundary through `StarsGenerateWorldGameState`, with
the recorded `lid`, map, player, planet-limit, start-distance, dirty, world-
generation, and turn offsets. The separate two-word `StarsSeeds` state is
copied into that view and consumed by the reviewed `Randomize2`/`Random`
implementation, including the original nonpositive-count advancement rule.
The strict host and fresh VC++ 1.52c/Win98 E1 probes each report
`GENERATE_WORLD_E1_CHECKS=16 FAILURES=0 RESULT=PASS`; compile/link pass and
the final guest window inventory has no Program Error dialog. `GenerateWorld`
is promoted at Phase 2 step 6/6. E2 closes the state-backed pipeline through
file records and local finalization, while the original Win98 generation
observation and artifacts provide the independent executable comparison.
Evidence is in `analysis/compat/generate-world-integration.{md,json}` and
`runs/vmctl-vc152gwint/` and `runs/vmctl-vc152gwint2/`.
