# NewGameWizard

`NewGameWizard` is represented as a typed state machine in
`reconstructed/src/new_game_wizard.c`. The reconstruction preserves the
simple-game entry choices, the three-page dialog progression, page restart and
name-retry behavior, read-only player seeding, cancel/tutor exits, temporary
player copying, computer/race/AI type decoding including bounded random
selection for out-of-range race and AI values, packed computer-marker/AI-level/
AI-id writeback, player-count finalization,
five-bit relation-group duplicate repair and bounded random filling, including
the original packed relation-exclusion marker and relation-group bit
extraction/writeback when a packed player-flag word is supplied, relation assignment, title teardown, world
generation, host dispatch, and restoration of the saved game-bit flag.

The Win16 dialog procedures and localized string resources remain callback-owned.
An optional packed `_player` array now receives the wizard-owned flags, salt,
dead marker, and name fields while unrelated record bytes are preserved; typed
temporary- and
default-player record copies are represented directly. Random race/AI selection,
non-computer advantage-point repair, and the all-ones salt for randomized computer races are
represented in typed C; its failure path also sets the typed
dead marker after copying the default record; packed computer-marker/AI-level/AI-id and
relation-exclusion/group flag extraction and writeback are
represented when `packed_flags_valid` is set, while other packed Win16 player
fields remain callback-owned. The full `GenerateWorld` implementation remains
a separate procedure; the wizard's accepted boundary is its callback dispatch
and lifecycle ordering. The host probe covers normal, retry, read-only, cancel, and
cleanup paths. A fresh VC++ 1.52c/Win98 harness in
`runs/vmctl-vc152nwt/` compiles and links the current source and reports
`NEW_GAME_WIZARD_CHECKS=30 FAILURES=0 RESULT=PASS` without a runtime error
dialog. The packed-player E1 follow-up in `runs/vmctl-vc152nwp/` reports
11/11 checks with compile/link success and no Program Error dialog. The E2
resource follow-up now checks all 45 controls across resources 0x186, 0x187,
and 0x188, including exact rectangles and resource-text checksums; its host and
native probes report 366/366 with no Program Error dialog. Evidence is in
`analysis/compat/new-game-wizard-resource-integration.md` and
`runs/vmctl-vc152nwres/`.

The original executable has now been exercised through the shared creation
boundary in Win98: `New Game` opened the three-page Advanced Game wizard,
player rows exposed the race/computer/no-player menu, and `Finish` reached the
game-name finalization dialog without a Program Error. The shared creation
probe additionally closes E3 with 11/11 host and native checks for page
transition, packed player commit, name acceptance, world-generation, and host
dispatch. Evidence is in
`analysis/compat/new-game-creation-integration.md` and
`runs/vmctl-vc152ngint/`, alongside the original replay. `NewGameWizard` now
has all three numbered Step-5 evidence units closed (packed player state,
resources/control geometry, and complete creation replay), and the final
ledger promotion is recorded in `tracking/reviews.json`; it is complete at
6/6. The actual `GenerateWorld` implementation is tracked separately.
