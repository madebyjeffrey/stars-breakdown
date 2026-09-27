# SO-E3 Gates 3 and 4 — output-stream closure audit

Status: **closed for the pinned control/action pair, 2026-09-26**. The scope is
`runs/vmctl-sye3k0` and `runs/vmctl-sye3k1`, each with turn-1 HST/M1/M2.

## Exact closure requirements

Gate 3 requires one complete, source-generated, ordered, encrypted output
stream for each of the six files. Each must contain every turn-1 record section,
including BOF, players, messages where present, planets, designs, fleets and
orders, production queues, scores, things, battle plans, and turn marker. M1
must preserve its 31-record turn-0 prefix before appending turn 1.

Gate 4 requires one retained regression to decode each generated stream and
compare **every** turn-1 record's kind and body against the corresponding
original output, without using original output records as runtime inputs.
This covers fleet/order/cargo, planet/population/resource/guess, ownership and
technology/research, message, score, design, BOF, and other emitted sections.

## Six-file census and result

| Run | File | Original bytes | Turn-1 records | Full decoded comparison | Entire raw file |
|---|---|---:|---:|---|---|
| Control | M1 | 1700 | 32 | Pass | Identical |
| Control | M2 | 808 | 24 | Pass | Identical |
| Control | HST | 2019 | 83 | Pass | Identical |
| Action | M1 | 1682 | 30 | Pass | Identical |
| Action | M2 | 815 | 24 | Pass | Identical |
| Action | HST | 1992 | 81 | Pass | Identical |

The 274 turn-1 records cover kinds 0, 6, 8, 12, 13, 14, 16, 20, 26, 28,
30, 43, and 45. The test checks expected record counts before comparing
ordered `(kind, body)` sequences. It then compares each complete encrypted
file byte for byte, which also checks framing, stream cipher progression,
record headers, file length, and the inherited M1 prefix.

## Source and verification boundary

The strict-C89 integrated probe in
`tests/satisfy_orders_turin_binding_e3_probe.c` sends each existing
current-state writer callback through `stars_write_record`; `stars_write_bof`
starts the file stream. The three files are exported only after the complete
seeded selector-1 turn passes its internal checks. The comparison in
`tests/test_satisfy_orders_turin_binding.py` uses independent `decode_stream`
and the retained original outputs **after** generation. The Gate 1 complete
input audit and Gate 2 final provenance audit establish the loaded input and
turn-service origin of writer state. They also document recovered external
clock inputs for the two deterministic replays.

Verification: `python3 -m unittest tests.test_satisfy_orders_turin_binding -q`
passed (one test, including both seeded runs and 100 scout rolls). No new
original Stars! VM run was needed. The historical corrupt `CLEAN.X2` case is
outside this pinned pair.

The broader 12-module Gate 2 regression was rerun after stream integration:
**36 tests, 35 passed and one expected Unicorn skip**. `git diff --check` passed.

**Remaining SO-E3 grouped gates: 0 of 4.**
