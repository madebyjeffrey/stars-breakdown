# Baseline and progress tracking

Step 0 is complete when the retained inventory is verified, the ledgers exist, and a local Git baseline records the project. Later analysis questions remain open in [UNCERTAINTIES.md](UNCERTAINTIES.md).

## Input identity

[Input inventory](../analysis/input_inventory.json) records every current file under `target/`, including macOS metadata, and every ZIP member with size and SHA-256. ZIP timestamps are untrusted provenance metadata, not asserted build dates. Archive members are read without extracting or executing anything.

`target/stars.exe` is byte-identical to the archive's `stars.exe`. Both on-disk copies of `26JFIN.txt` match its archive member. There is only one distinct supplied executable. No comparison with unprovided releases has been performed; a different hash must receive a separate inventory and analysis baseline.

The archive label and notes support 2.60j RC3 provenance. The `2.7j` source path is not sufficient to override that evidence. Exact embedded product-version confirmation remains U001.

## Coverage

[Coverage ledger](../analysis/coverage.json) partitions each NE segment into non-overlapping half-open ranges: the start belongs to the range, the end does not. It records NE ordinal, offsets, storage, classification, names/modules, evidence, reconstruction location, review status and validation status. Every byte in the static segment memory extent belongs to one row, including zero-fill and unresolved gaps.

Initial coverage: 37 segments, 1,305 ranges, 1,042,584 memory bytes. Debug procedure ranges cover 890,384 bytes; 126,146 code-segment bytes remain unclassified and 26,054 bytes belong to the data segment. These are inventory measurements, not a decompilation percentage. Procedure extents may themselves contain inline data.

Only `GetRaceStat`'s 34 bytes have manual review and static evidence. That example is a note, not compiled or runtime-tested source. Compiled reconstruction coverage starts at zero.

Manual progress belongs in [reviews.json](reviews.json), keyed by the ledger range ID. Generation merges those annotations into the ledger without overwriting the editable source. If segmentation changes, generation rejects obsolete keys so a human must reconcile their evidence. Extend the status schema and summary calculation when compiled/runtime-validated ranges first exist; do not inflate progress by assigning a status without evidence.

The ledger excludes non-segment file regions and dynamic heap/stack growth. Whole-file classification, relocation interpretation, and discovery of unnamed code belong to later stages, tracked by U003/U004/U010.

## Version-control policy

Use local Git only; no remote or publication is configured by this step. Track tools, tests, documentation, ledgers, future reconstructed source and small curated validation fixtures. Track the current deterministic `analysis/*.json` and `analysis/*.md` snapshots so changes can be reviewed against the tools that produced them.

Preserve `target/` in place but exclude original binaries/archives and OS metadata from Git. The tracked input manifest identifies required originals; a checkout needs those same originals restored locally before regeneration. Git is not a backup of the supplied archive. Keep the original archive in your normal backed-up storage.

Ignore interpreter caches, tool caches, decompiler project databases and build outputs. Future large/raw run captures go under `runs/` and are excluded; promote minimal deterministic fixtures into `tests/fixtures/` with provenance and expected results. Never put a copyrighted compiler/OS installation or private gameplay data into the repository by accident.

## Reproduce and verify

Run from the project root:

```sh
python3 tools/baseline.py --check
python3 tools/extract_debug.py
python3 tools/render_layouts.py
python3 tools/baseline.py --check
python3 -m unittest discover -s tests
```

`--check` compares both retained artifacts without rewriting them and detects changed input files. For an intentional, reviewed inventory or progress change, run `python3 tools/baseline.py`, inspect the diff, and then run the checks. Regeneration must not silently bless an unexpected changed original.
