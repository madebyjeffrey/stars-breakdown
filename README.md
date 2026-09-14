# Stars! executable study

A reverse-engineering study of `target/stars.exe`, preserving the supplied executable unchanged. The project has a tested NE/CodeView/Ghidra workflow and 99 reviewed C89 routines, including an integrated race save/load and creation component. It is not yet a complete decompilation or a Win16 replacement application.

## Start here

- [Race component](analysis/reconstruction/race-component.md): completed game-code dependencies, original-instruction comparisons, and the Windows contract for the next native build.
- [Reviewed semantic core](analysis/reconstruction/core.md): current source coverage and validation scope.

- [Decompiler workflow](analysis/decompiler/README.md): tested import, initial C/assembly exports, call/data maps, acceptance checks, and remaining analysis limits (step 3 complete).

- [Debug recovery report](analysis/debug/README.md): complete semantic record recovery, effective signatures, and generated storage headers (step 2 complete).

- [Executable layout report](analysis/ne/README.md): complete file map, imports, resources, and the static loader model (step 1 complete).

- [Baseline and progress tracking](tracking/README.md): verified input inventory, coverage ledger, uncertainty log, and version-control policy.

- [Full decompilation TODO](TODO.md): staged checklist, per-function requirements, and completion criteria for a buildable, validated reconstruction.
- [First reconstructed function](notes/FIRST_RECONSTRUCTION.md): `GetRaceStat`, with every instruction explained.
- [Function index](analysis/FUNCTIONS.md): 846 procedure records with original names, mapped NE addresses, byte lengths, and original source locations.
- [Data layouts](analysis/DATA_LAYOUTS.md): 192 complete structure records with member names, byte offsets, array sizes, and bit positions. Step 2 confirms that none is a forward declaration.
- [Summary](analysis/summary.json): file identity, counts, and original C source paths.

## Confirmed findings

The file is a 4,218,116-byte Windows NE executable with 37 loadable segments. Its embedded CodeView NB09 block starts at `0x35e0c0` and occupies 687,172 bytes including its trailer. The SHA-256 is `ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`.

The debug block contains 148 module records (including runtime modules), 846 procedure records, 1,678 type records, 8,098 BP-relative local/parameter records, and 46,190 line mappings referencing 38 C source files. These are record counts, not claims that every linked function or original source line survives. Compiler records report version string `8.00c`.

The original source tree appears as `c:\user\jeffl\2.7j\`. The user identifies this executable as **2.70j RC3**, the third release candidate of the retail patch, and clarifies that 2.60 was shareware, 2.70 was retail, and the releases were binary compatible. The bundled notes use the 2.60j label and are dated April 26, 2000; those labels are retained as provenance. This product identification comes from the user's clarification, not an independent decoding of version resources.

The modules expose distinct areas for AI (`ai`, `ai2`, `ai3`, `ai4`, `aiutil`), simulation (`turn`, `turn2`, `turn3`, `battle`, `produce`, `research`), Windows interface (`mdi`, `scan`, `popup`, `vcr`), and storage/memory (`file`, `save`, `log`, `memory`). These are useful starting points for studying the design; function names alone do not establish their algorithms.

Examples of compact recorded layouts: `_planet` is 56 bytes, `_fleet` 124 bytes, `_player` 192 bytes, and `_order` 18 bytes. The layouts preserve extensive bit fields and overlapping members. Near/far pointers, 16-bit integers, and the original packing rules matter when reconstructing these on a modern machine.

## Reproduce

The metadata extractors use Python 3. Differential tests additionally use the
pinned dependencies in `requirements-validation.txt` and a host C compiler:

```sh
python3 tools/extract_debug.py
python3 tools/render_layouts.py
python3 tools/ne_layout.py
python3 tools/recover_debug.py
python3 tools/debug_headers.py
python3 tools/baseline.py --check
python3 -m unittest discover -s tests
```

The extractor reads the input only and writes derived artifacts to `analysis/`. It retains raw symbol and type bytes for auditing. Unknown symbol records are retained rather than assigned speculative meanings. The initial extractor provides a compact first-pass inventory. The step 2 artifacts in `analysis/debug/` add nested lexical scopes, register variables, reference resolution and effective signatures.

CodeView logical segment numbers are translated through `sstSegMap` to NE segment ordinals. All procedure ranges are checked against segment memory extents. A procedure may include a trailing zero-filled byte beyond the initialized file bytes; `file_backed_bytes` makes this explicit. Source paths and line numbers are metadata, not embedded source text.

## Next decompilation stage

Steps 0–4 are complete for the recorded baseline. Step 5 is in progress. The
save/load and race-creation dependency cluster is integrated; its native Windows
acceptance build comes next. The full checklist is in [TODO.md](TODO.md).

The NE extractor accounts for every file byte, decodes all relocation records, resolves imports to symbolic identities, extracts every resource, and emits static memory images with synthetic selectors. Its hardware-x87 policy and remaining runtime limitations are documented in [the layout report](analysis/ne/README.md). Original execution evidence is in the [runtime report](analysis/runtime/README.md); the early native compiler results are in the [VC++ review](analysis/compat/vc152-return2.md).

Debug information greatly improves names and layouts, but comments, macros, original expressions, and optimized-away code generally require interpretation or are unrecoverable.

## Format references

The parser follows the 16-bit records in [Microsoft's CodeView definitions](https://github.com/microsoft/microsoft-pdb/blob/master/include/cvinfo.h) and the directory/module definitions in [Wine's CodeView header](https://github.com/wine-mirror/wine/blob/master/include/wine/mscvpdb.h). The tested decompiler is version 12.1.3 of the [official Ghidra project](https://github.com/NationalSecurityAgency/ghidra).
