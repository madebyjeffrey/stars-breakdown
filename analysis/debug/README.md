# Completed debug-information recovery

Step 2 recovers the semantic information present in this binary's NB09 records. It does not recover absent source text or establish behavior for unnamed routines. Raw records remain available with exact file offsets; inferred corrections are separate.

## Results

- Audited all 234 debug subsections and all 18 present symbol kinds and eight type kinds. No present semantic record kind is silently skipped.
- Recovered 846 procedure records, 637 nested lexical blocks, 8,098 stack-relative variable records, two register-variable records, and 431 labels. Scope identities remain distinct even when names repeat.
- Resolved all 846 procedure references and 55 data references to their actual module records; those references are not additional functions or variables.
- Recovered 604 distinct typedef names and 664 data records (655 distinct name/type/address tuples). Co-located aliases and duplicate records remain auditable in `address_groups.json`; co-location alone is not treated as proof of identity.
- All 192 structure types are complete: 1,478 members, 63 structures with overlapping fields, and nine variable-tail members. No forward-reference structures appear. Array element counts, near/far pointer widths, qualifiers, and bit positions are decoded.
- Identified 23 absolute public linker symbols, including floating-point fixup constants. They are not interpreted as loaded memory addresses. No `S_CONSTANT` records or enum type records are present; source macros and enums cannot be invented from that absence.
- Cross-checked the source-file index against all 38 source files in line tables. Kept line entries distinct; a source line may map to multiple addresses.

## Signatures and scope

See [SIGNATURES.md](SIGNATURES.md) for the readable function inventory and `procedures.json` for effective signatures and evidence. Calling-convention types distinguish far C, near Pascal, and far Pascal. All 40 compiler records identify C language, 16-bit mode, ambient near data and far code; that is consistent with the medium memory model. It does not identify every command-line option.

`FrameWndProc` records `hwnd` in SI and `wParam` in DI. The first instructions load these from BP+14 and BP+10; `msg` and `lParam` remain at BP+12 and BP+6. Its effective signature agrees with the recorded procedure type.

`AskSaveDialog` is the substantive signature discrepancy: the raw type 0x1036 lists zero arguments, while local records show four arguments and the code ends in `RETF 10`. Its effective signature uses those four parameter records. The raw type remains unchanged; the adjustment and exact instruction evidence are retained in `tracking/debug_signature_overrides.json`. We do not assert why the compiler emitted that type record. Empty legacy type argument lists are emitted as C89 unspecified-argument function types, not automatically asserted as `(void)`.

Local-variable ranges describe lexical visibility. These older records do not establish precise optimized location-validity intervals at every instruction. Stack/register initialization, reuse and actual lifetime must be checked during per-function reconstruction.

## Reconstruction headers

Files are in `reconstructed/include/` with 8.3-compatible filenames:

- `stypes.h`: all 192 structures as packed storage views, scalar and pointer representations, array types, function types, typedef aliases, and member offsets/bit masks.
- `sfuncs.h`: 846 effective function declarations, prefixed by module to retain identity and avoid SDK name collisions.
- `sglobals.h`: declarations for every data record, with address/linkage comments and distinct record-based names. These are declarations, not allocations or initialized definitions.
- `sconst.h`: raw values for the 23 absolute linker symbols, explicitly distinct from loaded addresses or recovered source macros.
- `checklay.c`: a compilation-only entry point including all four headers.

The default host mode represents near pointers as 16-bit integers and far/huge pointers as 32-bit integers. These are encoded addresses, not host pointers you can dereference. `STARS_NATIVE16` selects actual VC-style near/far/huge declarations. Native compiler checks deliberately require two-byte near pointers and four-byte far pointers. Extended 80-bit floating storage is a ten-byte wrapper, not a claim that host `long double` has the right representation.

Overlaps are modeled as independent named views rather than guessed anonymous unions. For example, `player.rgAttr.value[index]` accesses the recorded array in a storage view; `player.raw` exposes its bytes. Bit-field views expose the backing storage, not C bit-field assignments: use recorded bit masks/positions, preserve unrelated bits, and sign-extend signed fields when implementing accessors. Variable tails have offsets and incomplete array typedefs, with no fake fixed element appended to their parent. The headers preserve storage and type information; final source spelling, ergonomic declarations, and native aggregate ABI behavior remain reconstruction work.

The host C89 check passes 2,241 generated type-size/member-offset assertions, plus primitive width assertions. All field extents and bit ranges are independently checked against metadata. This is not evidence that Visual C++ 1.5 has compiled them: the user's toolchain setup is still pending. Native validation belongs with that toolchain work; any difference must be resolved before relying on these declarations for a rebuilt executable.

## Machine-code validation

Existing and new tests tie layouts to concrete operations:

- `GetRaceStat`: `_player.rgAttr` at offset 62, one-byte elements, sign extension to 16-bit return.
- `GetRaceGrbit`: the 32-bit `grbitAttr` field at offsets 78/80.
- `GetFuelFree`: the fifth 32-bit element in `_fleet.rgwtMin`, at offsets 92/94, accessed through a far pointer.
- `LpengineFromId`: 78-byte `_engine` stride.
- `LphuldefSBFromId`: 143-byte `_huldef` stride.
- `SetFileSeeds`: CodeView segment 69's static globals map to data-segment offsets 0x2272 and 0x2276.

These are representative instruction-backed checks plus exhaustive structural/compile-time checks, not a claim that every member's meaning or use has been verified. Instruction-by-instruction validation continues during step 5.

## Provenance and remaining gaps

The library table contains `mlibcew.lib`, `libw.lib`, `commdlg.lib`, and `toolhelp.lib`. The 108 runtime module records use library index 1 (`mlibcew.lib`). All 40 game-object records use 256, outside the list. That number is retained as unresolved provenance; no nonexistent library or specific sentinel meaning is inferred. See U011.

The enriched `coverage.json` keeps the baseline's non-overlapping segment ranges and adds public-symbol anchors. It finds 323 public-symbol records in 44 otherwise-unclassified code ranges. Those anchors do not prove routine lengths or turn those ranges into decompiled code. The 126,146 unclassified code-segment bytes remain outstanding for step 3.

Hash tables are debugger lookup accelerators; sequentially decoded semantic records replace their lookup role. Their formats, lengths, and hashes are recorded in `subsection_audit.json`, with the exact bytes retained in the original input. Alignment records and record padding have no reconstructed source semantics. These exclusions are explicit, not unknown symbol kinds.

## Reproduce

From the project root:

```sh
python3 tools/baseline.py --check
python3 tools/extract_debug.py
python3 tools/render_layouts.py
python3 tools/recover_debug.py
python3 tools/debug_headers.py
clang -std=c89 -pedantic-errors -Wno-strict-prototypes -fsyntax-only reconstructed/include/checklay.c
python3 -m unittest discover -s tests
```

For an eventual native check, copy the five generated files to a short DOS path and compile `checklay.c` with `STARS_NATIVE16` defined. Record the compiler banner, command and diagnostics. This only checks declarations and storage: it does not build Stars!.

## Primary format references

- [Microsoft CodeView definitions](https://github.com/microsoft/microsoft-pdb/blob/master/include/cvinfo.h)
- [Microsoft calling-convention and register definitions](https://github.com/microsoft/microsoft-pdb/blob/master/include/cvconst.h)
- [Wine CodeView decoder](https://github.com/wine-mirror/wine/blob/master/tools/winedump/debug.c)

Reference URLs, access dates and downloaded-source hashes are retained in `references/debug_sources.json`.
