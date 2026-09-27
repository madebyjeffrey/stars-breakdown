# SO-E3 Gate 1 — complete-input closure audit

Status: **closed, 2026-09-25**. This closes loading both pinned input sets,
including coordinator integration. SO-E3 itself remains open: Gates 2–4
still require complete turn execution, serialization, and differential checks.

## Requirements and inspected evidence

| Requirement | Evidence | Result |
|---|---|---|
| Use each run's own complete starting files | `InputReader.load_run` checks `RUN.JSON` sizes/SHA-256 for XY/HST/M1/M2 in `runs/vmctl-sye3k0` and `runs/vmctl-sye3k1`; starting X2/H2 are pinned absent. HST/M files come from `BACKUP`, not turn-1 outputs. | Pass |
| Consume every input byte/record | Source `stars_read_record` consumes 274 bounded frames; the XY adapter consumes 64 packed coordinates. Independent stream decoding checks framing/body equality. BOF identities match. XY EOF carries cPlayer=2; saved-file EOF carries turn=0. | Pass |
| Initialize every object group | Eight player records, 44 designs, 40 battle plans, 72 planets, 40 fleets, 40 orders, eight things, and two message blocks are initialized and checked. Two thing-count prefixes and 18 metadata records complete the 274-record census. | Pass |
| Preserve the action input difference | The action HST's type-19 task-5 order retains all 18 bytes; the other 39 compact orders expand into zeroed 18-byte entries. Its task-5 coordinator path now uses that loaded order, fleet, and design. | Pass |
| Complete load-time initialization | Names and design normalization/cost/mass/damage/cache/free slots are initialized; host/client first-year flags remain distinct; discovery messages are generated and serialized; player planet/fleet/design counts are rebuilt; THING turn is stamped from BOF. | Pass |
| Bind complete inputs to the coordinator | `complete_input_header` exports only `InputReader` results. `TurinTrace.complete_input` receives the selected control/action aggregate before execution. All HST/M1/M2 tables remain available; load adapters use those tables. `DoOrders` receives all 32 host planets. | Pass |
| Verify the C transfer, not only Python objects | A strict-C89 shared probe exposes each complete C input structure. The test compares every initialized table/field in all six file views plus game metadata and coordinates against the source loader. | Pass |
| Audit every record's runtime destination | `record_runtime_references` identifies each record's concrete player, planet, design, fleet/order, battle-plan, thing, message, or metadata destination. Missing runtime objects fail the audit. The generated inventory stores each reference. | Pass |
| Run the integrated regression | Both seeded control/action coordinator replays pass, as do the selector's existing 100 scout-roll cases. Their broad turn expectations remain supporting regression evidence, not Gate 2 closure. | Pass |

## Initialization details checked during closure

- `ReadRtPlr` wire state is retained separately from finalized player state.
  Saved HST planet counts 32/0 and M1 count 3 describe file distribution;
  runtime ownership gives one planet per included player. Ship and starbase
  counts come from occupied initialized banks; fleet counts come from fleets.
- The M1 runtime message table contains five saved messages and two planet
  discoveries. Message headers include both the message/mask and goto words.
  Discovery habitabilities 33/17 are multiplied by loaded growth, and the
  source serializer produces all seven messages, sent bits, and selection.
- Full design banks preserve free entries and separate starbases from ships.
  Fleet/order state preserves cargo, damage, battle-plan selection, order
  payloads, and zeroed unused storage. Clearing ORDER bit 13 no longer changes
  warp bit 5.
- Source resource preparation and AI research initialization now read loaded
  planet/player fields. The resource adapter packs the initialized planet
  fields rather than copying a selected planet fixture.
- Empty starting score/production-queue/incoming-message sections contribute
  no input records. Later score, queue, and message generation belongs to the
  remaining turn-service and output gates.

## Verification and boundaries

Command:

```sh
python3 -m unittest tests.test_satisfy_orders_input_corpus tests.test_satisfy_orders_turin_binding -v
```

Result: **20 tests: 19 passed, 1 skipped**, 14.915 seconds. The skipped test
executes original x86 player instructions through Unicorn, disabled on this
macOS ARM host. Full player-byte verification through the independent wire
oracle passed. This audit claims host source initialization and integration;
it does not claim new Win98 or original-machine execution.

Inventory reproduction:

```sh
python3 -m tools.satisfy_orders_input_corpus --output analysis/compat/satisfy-orders-e3-input-corpus.json
```

The inventory has zero uninitialized records and explicit runtime references.
The loader is bounded to these pinned inputs. Unsupported formats/callback
branches fail explicitly; this is not certification of an arbitrary game-file
loader. Existing post-load event witnesses, retained turn-1 guess/score fields,
unfinished services, and incomplete output streams remain Gate 2–4 work.
