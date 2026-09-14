# Baseline and progress tracking

Step 0 is complete when the retained inventory is verified, the ledgers exist, and a local Git baseline records the project. Later analysis questions remain open in [UNCERTAINTIES.md](UNCERTAINTIES.md).

## Input identity

[Input inventory](../analysis/input_inventory.json) records the explicitly selected original inputs, including their original macOS metadata, and every member of the selected ZIP archive with size and SHA-256. Verification uses that fixed allowlist; additional files are never automatically inventoried. ZIP timestamps are untrusted provenance metadata, not asserted build dates. Archive members are read without extracting or executing anything.

`target/stars.exe` is byte-identical to the archive's `stars.exe`. Both on-disk copies of `26JFIN.txt` match its archive member. There is only one distinct supplied executable. No comparison with unprovided releases has been performed; a different hash must receive a separate inventory and analysis baseline.

The user identified this executable on 2026-09-13 as **2.70j RC3**, the third release candidate of the retail patch. They clarified that 2.60 was the shareware release, 2.70 was retail, and the releases were binary compatible. U001 is resolved on that basis. The archive name and release-note labels remain preserved as provenance. SzVersion independently formats **Version 2.60j**, as confirmed by its instructions and the reconstructed native screen; that displayed text is preserved and does not replace the user's provenance clarification.

## Coverage

[Coverage ledger](../analysis/coverage.json) partitions each NE segment into non-overlapping half-open ranges: the start belongs to the range, the end does not. It records NE ordinal, offsets, storage, classification, names/modules, evidence, reconstruction location, review status and validation status. Every byte in the static segment memory extent belongs to one row, including zero-fill and unresolved gaps.

Initial coverage: 37 segments, 1,305 ranges, 1,042,584 memory bytes. Debug procedure ranges cover 890,384 bytes; 126,146 code-segment bytes remain unclassified and 26,054 bytes belong to the data segment. These are inventory measurements, not a decompilation percentage. Procedure extents may themselves contain inline data.

Step 5 has 130 reviewed routines covering 55,942 original procedure bytes, compiled
as host C89 semantic adapters and compared with original x86 execution. See
[the reconstruction report](../analysis/reconstruction/core.md). A partial
Win16 application now opens the starting screen, with original resources and
recovered drawing code; see the [milestone](../analysis/reconstruction/starting-screen.md).
Fresh native acceptance passed bitmap/fallback repaint, New/Open notices and
cleanup through automatic exit, Alt+X and Alt+F4; see the
[acceptance record](../analysis/compat/startup1-acceptance.json). Full application
behavior and the earlier intermittent guest fault (U023) remain open.
The historical GetRaceStat note has been superseded
by reviewed source and the recorded static/runtime/differential evidence.
The [race-component audit](../analysis/reconstruction/race-dependencies.json)
also records caller-specific helper paths separately, without crediting their
full procedure ranges. The [race compiler/output package](../compat/racevc/README.md)
compiled and linked cleanly in RETURN4; its returned EXE passed all 816 native
checks in DOSBox / WfW 3.11, with 61 matching race files and 89 matching states.
See the [review](../analysis/compat/racevc-return4.md). RETURN5 repeats all 816
checks in the user's Win98 SE workflow with revision 3 and a complete trace; its
returned EXE also passes visible progress and keyboard cancellation checks in
WfW 3.11. See the [RETURN5 review](../analysis/compat/racevc-return5.md). START2 now supplies real
race-wizard windows and save/open dialogs as a separate integration milestone;
see the [START2 report](../analysis/reconstruction/start2.md).

The [foundation work](../analysis/reconstruction/foundations.md) completes all
11 memory-manager routines, the startup resource lifecycle and six additional
numeric/bounds helpers. Their 23 routines / 7,264 bytes have original-instruction
comparisons and host C89 builds; the starting screen now exercises native
font/color/cleanup and heap/resource integration. Separate
from named-game coverage, the DOS entry assembles to the exact 98 bytes and
the [runtime library audit](../analysis/reconstruction/runtime-library.json)
maps all 101 code-bearing CRT records (15,192 bytes / 856 fixups). Seven records
without code are now identified in the [data audit](../analysis/reconstruction/runtime-data.md):
six map fully and one retains a Windows stub slot. Common message-padding
placement and whole-link layout remain open. The broad startup/common subsystem boxes remain open.

Manual progress belongs in [reviews.json](reviews.json), keyed by the ledger range ID. Generation merges those annotations into the ledger without overwriting the editable source. If segmentation changes, generation rejects obsolete keys so a human must reconcile their evidence. Schema 2 counts `host_c89_compiled` rows separately and states their scope explicitly; this is original-byte mapping coverage, not generated object size or whole-program equivalence.

The ledger excludes non-segment file regions and dynamic heap/stack growth. Whole-file classification and static relocation interpretation are now completed in [the step 1 layout](../analysis/ne/README.md). Discovery and interpretation of unnamed code remain later work, tracked by U004/U010. The baseline coverage denominator itself is unchanged.

## Version-control policy

Use local Git only; no remote or publication is configured by this step. Track tools, tests, documentation, ledgers, future reconstructed source and small curated validation fixtures. Track the current deterministic analysis JSON/Markdown snapshots, including the NE and debug recovery subdirectories and their reports so changes can be reviewed against the tools that produced them.

Preserve `target/` in place but exclude original binaries/archives and OS metadata from Git. The tracked input manifest identifies required originals; a checkout needs those same originals restored locally before regeneration. Git is not a backup of the supplied archive. Keep the original archive in your normal backed-up storage.

Ignore interpreter caches, tool caches, decompiler project databases and build outputs. Raw extracted resources and synthetic memory images under `analysis/ne/resources/` and `analysis/ne/memory/` are ignored and reproducible from the original; the tracked layout records their hashes. Future large/raw run captures go under `runs/` and are excluded; promote minimal deterministic fixtures into `tests/fixtures/` with provenance and expected results. Never put a copyrighted compiler/OS installation or private gameplay data into the repository by accident.

## Reproduce and verify

Run from the project root:

```sh
python3 tools/baseline.py --check
python3 tools/extract_debug.py
python3 tools/render_layouts.py
python3 tools/ne_layout.py
python3 tools/recover_debug.py
python3 tools/debug_headers.py
python3 tools/baseline.py --check
python3 -m unittest discover -s tests
```

`--check` compares both retained artifacts without rewriting them and detects changed input files. For an intentional, reviewed inventory or progress change, run `python3 tools/baseline.py`, inspect the diff, and then run the checks. Regeneration must not silently bless an unexpected changed original.

Step 2 adds `analysis/debug/coverage.json`, preserving the baseline ranges and annotating public-symbol anchors. It does not inflate completed-code coverage. Signature corrections live in `tracking/debug_signature_overrides.json`; generated headers and their check source are tracked under `reconstructed/include/`.

Step 3 adds [the automatic disassembly coverage ledger](../analysis/decompiler/coverage.json) and [the tested decompiler workflow](../analysis/decompiler/README.md). Its instruction/data classifications, 1,043 function exports and graphs are separate from manual/compiled reconstruction progress. The original baseline and `reviews.json` are unchanged. Full raw exports and local tools/projects are ignored; metadata indexes, the complete export hash manifest and selected acceptance outputs are tracked.
