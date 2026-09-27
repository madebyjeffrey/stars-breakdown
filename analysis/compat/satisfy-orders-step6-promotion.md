# SatisfyOrders Step-6 promotion audit

Status: **promoted for the documented procedure scope, 2026-09-26**.
Original function: NE `0017:6798–9eb8`, `void SatisfyOrders(int iPass)`,
far cdecl, debug source `turn3.c`. The three named Step-5 units in
`EVIDENCE.md` are closed; this audit verifies the separate Step-6 decision.

| Definition-of-done requirement | Current evidence | Decision |
|---|---|---|
| 1. Original boundary and ABI | The original disassembly and raw decompilation (`analysis/decompiler/export/raw/10b0_6798*`), original `turn3.c` audit, TODO5 address/signature entry, and the reviewed packed 124-byte fleet/18-byte order bridge identify the pass gates, state, service dispatch, messages, and data contracts. | Met for the recorded source/adapters |
| 2. Readable buildable 16-bit source | `reconstructed/src/satisfy_orders.c` and related typed services are compiled by the strict-C89 host probes and the retained VC++ 1.52c packages. E1/E2 audits cover packed widths, costs, transfer and failure paths, and exact message frames; the E3 provenance audit covers full-turn ordering and shared RNG. | Met for the documented source scope |
| 3. Host normal/boundary/failure probes | E1 isolated task-5 accounting passes 19/19; E2 exact-input repricing passes 74/74, alongside the corrected 141-check raw-order sweep and individual transfer/failure packets. The E3 12-module regression passes 35 tests with one expected architecture skip, including both complete pinned turns and 100 scout rolls. | Met |
| 4. Native VC++ 1.52c / Win98 checkpoint | `runs/vmctl-soe1g/` retains corrected E1 build status/logs/report (19/19); `runs/vmctl-soe2s/` retains exact-input E2 manifest/report (74/74); `runs/vmctl-vc152syfte4/` retains the four-pass build status/logs/report (18/18). The narrower native packets are itemized in the E2 closure audit. | Met at the existing native checkpoints; the full six-file replay is host-only |
| 5. Original/integration comparison | The pinned original control/action input audit and final provenance audit bind the same-input task-0/task-5 turn. The Gates 3–4 audit decodes and compares all 274 turn-1 records and verifies all six HST/M1/M2 files byte for byte. E1 and E2 retain their separate original and native oracles. | Met for the pinned acceptance boundary |
| 6. Ledger and note synchronization | The `0017:6798-9eb8` review record carries E1/E2/E3 and promotion evidence; regenerated `analysis/coverage.json` and TODO5 carry its final validation scope; `analysis/reconstruction/satisfy-orders.md`, TODO6, TODO7, EVIDENCE, README, and `tracking/README.md` record promotion and updated counts. | Met after regeneration/checks below |

The current `reconstructed/src/satisfy_orders.c` SHA-256 matches the source
hash in `runs/vmctl-soe2s/MANIFEST.JSON`, binding that native result to the
current procedure source. The subsequently expanded `reprice_shdef.c` helper
has a different hash; the six-file host regression exercises its current
version. The native packet therefore establishes the `SatisfyOrders` source
checkpoint and its pinned v4 repricing case, not a native claim for every
later helper extension.

The accepted full-turn result is specific to `vmctl-sye3k0` control and
`vmctl-sye3k1` action with recovered external clock residues. It is not a
universal whole-game or fresh native six-file run claim. The separate Step-4
native requirement is satisfied by the retained source-linked Win98 packets;
the Step-5 original comparison is the strict-C89 integrated replay against
the pinned original files. A future mismatch outside this scope should be
tracked as a new issue, not silently absorbed into this promotion.

Verification: `python3 tools/baseline.py --check` validates the regenerated
coverage ledger; `python3 tools/render_todo5.py` regenerates TODO5 with the
review entry; the 12-module SO-E3 regression returns 36 tests, 35 passes and
one expected Unicorn skip; `git diff --check` passes.
