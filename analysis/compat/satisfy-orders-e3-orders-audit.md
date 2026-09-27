# SO-E3 G2-02 order and log closure audit

Status: **closed for the two pinned complete starting inputs**, 2026-09-26.
This covers the player-log load and four order passes, including their shared
world effects. Message serialization and output-file lifecycle remain G2-12;
whole-turn runtime provenance remains G2-14.

## Player logs before orders

- The original player shuffle uses the shared main RNG. Player 0's absent log
  retains the `-1` serial sentinel. Player 1's generated encrypted X2 is
  validated and read by the source file/log services, then all ten mutation
  records replay in source order. Research, designs, production, balance,
  cargo and orders publish into current host state. The starting merge is
  reconstructed from decoded input records; the old pre-AI snapshot can be
  poisoned without changing replay.
- The original post-log serial/duplicate pass checks loaded player flags,
  source serial validity and duplicate payloads. Both current players avoid
  the error-message branch. The source design-preparation service visits all
  32 ship slots; source fleet-flag preparation visits all ten fleet entries.
  Newly eligible follow-fleet orders reject.
- Merge replay publishes source ship counts, cargo, deleted-fleet state and
  compacted lookup. The host world view now receives those same counts and
  cargo. Its deleted flag comes from the live presence table, since deletion
  removes the source fleet without setting bit 10 in the raw body. Before
  waypoint/order processing, an integrated assertion compares all 32 ship
  words, five cargo lanes and effective flags for both merged fleet records
  against the current raw/presence state.

## Original order and world sequence

| Stage | Current-source evidence |
|---|---|
| Waypoint validation | Loaded current fleets/orders, planets and things enter the source validator before orders and at the later original validation boundary. Unsupported newly eligible fleet/thing retargeting rejects. |
| Passes 1 and 2 | `stars_do_orders(0)` updates planet flags, runs thing interactions, visits every current fleet in table order for each pass, consumes the trace-owned colonist-drop queue, applies source research and consumes the transfer queue. |
| Task 5 | Owner-1 merged miner scraps at its actual table position in both cases. The action case also runs owner-0 task 5. Both source consumers publish fleet deletion, planet minerals and messages. |
| Passive fleets | Source no-task handling visits the remaining 8 control / 7 action active fleets. Any other active task fails explicitly. |
| Passes 3 and 4 | `stars_do_orders(1)` runs source fleet grouping, battle eligibility and bombing, thing interactions, source planet turn reset, post-movement passes, empty drop queue and source research. Current combat eligibility yields no battle effect; an eligible unbound battle/bomb effect rejects. |
| Shared world | Planet flags/turn, minerals, research, fleet flags/counts/cargo/orders and task-5 retirement flow to subsequent production, movement and writer adapters. The integrated regression checks both original endpoints and downstream record bodies. |

The drop and transfer queues are initialized empty at host load. The reached
order handlers enqueue neither one; the source queue consumers still run at
their original boundaries. Separate nonempty queue probes cover the consumer
callbacks. All four order passes visit the complete current fleet table, and
the integrated assertions require pass order 1, 2, 3, 4 and the exact task-5
visit sequence.

## Verification

- `python3 -m unittest tests.test_satisfy_orders_turin_binding -q` — passing
  strict-C89 integration for 100 scout cases and both pinned full inputs,
  including 57/39 main-RNG draws and existing original record-body checks.
- `python3 -m unittest tests.test_satisfy_orders_task5_four_pass_delta tests.test_satisfy_orders_log_files tests.test_generate_turn_source -q` — 9 tests pass in 59.145 s. The X2 test decodes both archived original backups and checks source load/write framing and record bodies.
- `git diff --check` — pass.

The stream writer and its view/message/file lifecycle remain G2-12. The final
whole-turn no-oracle/callback audit remains G2-14 and can reopen this package
if a defect is found. Complete HST/M1/M2 stream assembly is SO-E3 Gate 3.
