# Stars! executable study

A first reverse-engineering pass over `target/stars.exe`, preserving the supplied executable unchanged. This repository currently contains a debug-information extractor, a function index, recovered data layouts, and one manually reconstructed function. It is not a complete decompilation or a buildable recreation of the game.

## Start here

- [Baseline and progress tracking](tracking/README.md): verified input inventory, coverage ledger, uncertainty log, and version-control policy.

- [Full decompilation TODO](TODO.md): staged checklist, per-function requirements, and completion criteria for a buildable, validated reconstruction.
- [First reconstructed function](notes/FIRST_RECONSTRUCTION.md): `GetRaceStat`, with every instruction explained.
- [Function index](analysis/FUNCTIONS.md): 846 procedure records with original names, mapped NE addresses, byte lengths, and original source locations.
- [Data layouts](analysis/DATA_LAYOUTS.md): 192 structure records with member names, byte offsets, array sizes, and bit positions. Some records are forward declarations or alternate type records, so this is not a count of unique complete structures.
- [Summary](analysis/summary.json): file identity, counts, and original C source paths.

## Confirmed findings

The file is a 4,218,116-byte Windows NE executable with 37 loadable segments. Its embedded CodeView NB09 block starts at `0x35e0c0` and occupies 687,172 bytes including its trailer. The SHA-256 is `ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`.

The debug block contains 148 module records (including runtime modules), 846 procedure records, 1,678 type records, 8,098 BP-relative local/parameter records, and 46,190 line mappings referencing 38 C source files. These are record counts, not claims that every linked function or original source line survives. Compiler records report version string `8.00c`.

The original source tree appears as `c:\user\jeffl\2.7j\`. The bundled release notes describe 2.60j RC3 and are dated April 26, 2000. The source directory name alone does not establish the executable's product version.

The modules expose distinct areas for AI (`ai`, `ai2`, `ai3`, `ai4`, `aiutil`), simulation (`turn`, `turn2`, `turn3`, `battle`, `produce`, `research`), Windows interface (`mdi`, `scan`, `popup`, `vcr`), and storage/memory (`file`, `save`, `log`, `memory`). These are useful starting points for studying the design; function names alone do not establish their algorithms.

Examples of compact recorded layouts: `_planet` is 56 bytes, `_fleet` 124 bytes, `_player` 192 bytes, and `_order` 18 bytes. The layouts preserve extensive bit fields and overlapping members. Near/far pointers, 16-bit integers, and the original packing rules matter when reconstructing these on a modern machine.

## Reproduce

From this directory, with Python 3 (no third-party dependencies):

```sh
python3 tools/extract_debug.py
python3 tools/render_layouts.py
python3 -m unittest discover -s tests
```

The extractor reads the input only and writes derived artifacts to `analysis/`. It retains raw symbol and type bytes for auditing. Unknown symbol records are retained rather than assigned speculative meanings. Local records are associated with their containing procedure, but lexical block scopes and register locals are not fully modeled.

CodeView logical segment numbers are translated through `sstSegMap` to NE segment ordinals. All procedure ranges are checked against segment memory extents. A procedure may include a trailing zero-filled byte beyond the initialized file bytes; `file_backed_bytes` makes this explicit. Source paths and line numbers are metadata, not embedded source text.

## Next decompilation stage

Use a decompiler with a Windows NE loader and 16-bit x86 support, then import the extracted function names and types. Ghidra is a candidate, but it is not installed in the current command environment and no Ghidra import or decompilation has been run here. Before automating it, verify its NE address mapping, loader relocation handling, and calling conventions against the checked `GetRaceStat` example.

1. Apply NE relocations and recover Windows API imports and call targets.
2. Import symbols using the segment map, then reconstruct types with explicit near/far pointer widths and bit-field layouts.
3. Study `memory.c` and `produce.c` first for memory constraints and simulation techniques, or `battle.c` for combat. Recover one subsystem at a time with assembly-backed explanations.
4. Compare reconstructed behavior against the original under a suitable Windows 3.x environment when runtime validation becomes useful.

Debug information greatly improves names and layouts, but comments, macros, original expressions, and optimized-away code generally require interpretation or are unrecoverable.

## Format references

The parser follows the 16-bit records in [Microsoft's CodeView definitions](https://github.com/microsoft/microsoft-pdb/blob/master/include/cvinfo.h) and the directory/module definitions in [Wine's CodeView header](https://github.com/wine-mirror/wine/blob/master/include/wine/mscvpdb.h). The decompiler candidate is the [official Ghidra project](https://github.com/NationalSecurityAgency/ghidra).
