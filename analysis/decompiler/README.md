# Step 3 — tested disassembly and decompilation workflow

The supplied 2.70j RC3 executable now has a reproducible Ghidra analysis with **1,043 identified functions and 1,043 initial C exports**. This includes all 846 debug procedures and 197 additional functions, mostly linked runtime code. The importer reports no metadata errors, the loader comparison has zero differing bytes, and the final instruction audit has zero direct-flow targets outside valid instructions or imported-function placeholders.

These are automatic analysis results, not 1,043 completed source reconstructions. The baseline review ledger remains unchanged. Original-game execution, native VC1.5 compilation, and behavioral comparison have not happened. The separate real-mode DOS rejection stub also has C/assembly exports for its two entry routines (15 instruction bytes); these are excluded from the NE function/byte counts.

## Reproduce

Run from the repository root after reproducing steps 1–2:

```sh
python3 tools/setup_decompiler.py
python3 tools/run_decompiler.py
python3 -m unittest discover -s tests
python3 tools/baseline.py --check
```

The setup downloads checksum-pinned official Ghidra 12.1.3 and Temurin JDK 21.0.12.1+1 into `.tools/`. It is tested on macOS arm64 and requires the system C++ compiler and `make` to build Ghidra's bundled native decompiler source. It does not need Visual C++ 1.5 and does not install Java or Ghidra system-wide. Downloads and package hashes are pinned in the setup script and [tool references](../../references/decompiler_sources.json).

Each analysis creates a fresh project under `ghidra_projects/`; an existing export is moved aside before a new run. Java settings/caches are isolated under `.tools/user`. The current successfully checked project is `ghidra_projects/stars-repro-check`. Use `tools/decompiler_report.py` to recheck and reindex an existing export without rerunning Ghidra. This report is maintained alongside the generated JSON evidence.

## What is retained

| Artifact | Purpose |
|---|---|
| [Function inventory](functions.json) | Entries, exact body ranges, names, conventions, parameter storage, return registers, and export status |
| [Automatic coverage ledger](coverage.json) | Every static segment byte classified as instruction, data, or undefined; preserves debug-procedure identity and discovered function ownership |
| [Call graph](callgraph.json) | Direct/computed call references, names and source instruction addresses |
| [Data references](data-references.json) | Reads, writes and data references grouped by target address |
| [Code-pointer candidates](code-pointers.json) | Relocation-backed address-taking sites for callback/dispatch investigation |
| [Switch tables](switches.json) | Decompiler-recovered switch cases and targets; repeated targets are intentional |
| [Runtime dispatch tables](runtime-dispatch.json) | Thirteen named floating-point tables and the checked overrides derived from them |
| [Indirect-transfer ledger](indirect-targets.json) | Every detected indirect transfer, recovered targets, unresolved status and follow-up task |
| [Public-symbol candidates](public-candidates.json) | All code-segment public addresses classified as functions or defined data |
| [Acceptance exports](acceptance/) | Small retained selection of actual C and assembly outputs used for checks |
| [Export manifest](export-manifest.json) | Sizes and SHA-256 hashes of every raw C, assembly and segment-image export |
| [Loader verification](loader-check.json) | Independent comparison with the step-1 parser and relocation chains |
| [Analysis settings](analysis-settings.json) | Actual analyzer settings used by the saved program |

`export/raw/` contains a `.c` and `.asm` file for every identified function, named by Ghidra selector and offset. `export/memory/` contains all 37 loaded segment images. Full raw references, code-unit inventory, settings, and logs also live in `export/`. `export/dos/` and `acceptance/dos/` contain the separately mapped DOS entry and print/exit helper. Its near CALL consumes the message address, and the final INT 21h/AX=4c01 is marked nonreturning; intervening text is data. These large, reproducible products and Ghidra databases are ignored by Git; the indexes, hashes, tools and acceptance fixtures are tracked. All raw C files carry an explicit unreviewed-output marker.

## Loader and addressing checks

Use `x86:LE:16:Protected Mode`, with the NE loader and the patched `x86-16.cspec`. Real-mode `segment * 16` addressing is inappropriate for this protected-mode Windows program.

Ghidra uses selector `0x1000 + 8 * (NE ordinal - 1)`. The step-1 model uses `0x1000 + 8 * NE ordinal`. Thus NE 29:30d2 is Ghidra **10e0:30d2**, not the step-1 synthetic address. Ghidra import placeholders occupy selector 14f8, with four bytes per imported identity. Neither convention claims to reproduce actual Windows runtime selectors.

The independent checker reconstructs the Ghidra address convention from the original file and **all 15,327 relocation sites**, then compares all **1,042,584 bytes in 37 segment images**. It retains hardware-x87 instructions for all 1,313 OS FPU fixups. Ghidra preserves the original exported prologue; the separate Windows-loader prologue rewrite described in step 1 is not applied to this disassembly image.

Three imported identities are absolute exports rather than callable functions. The 13 operand sites using `KERNEL.__AHSHIFT` and `KERNEL.__AHINCR` are set to 3 and 8 for the documented protected-mode selector model, instead of treating placeholder addresses as scalar values. `KERNEL.__WINFLAGS` initializes the word `__wflags` at NE 36:0010. That word is explicitly data and conservatively volatile: its placeholder bytes are **not** an observed Windows flag value. Runtime observation remains necessary.

CS is set per code segment. The loader supplies DS as DGROUP (NE 37); SS-relative accesses remain stack-relative, and ES remains scratch for far data access. These assumptions are checked on the examples below and are not assertions that arbitrary runtime helpers preserve every segment register.

## Metadata and ABI import

The scripts import all 846 procedure entries, their recorded extents, effective signatures and source comments; 192 complete structure layouts with overlapping/bit-field views; the recovered arrays, 2-/4-byte pointers, procedure types and 604 aliases; globals and public aliases. Source line records are imported as address comments. The complete scoped-local/register-location records remain in `analysis/debug/symbols.json`, referenced by each function comment; they are not flattened into misleading function-wide local variables. Original qualifier records also remain authoritative in the debug JSON and generated headers.

Far cdecl uses a four-byte return address and caller cleanup. Pascal callbacks use explicit stack storage from the debug BP offsets, rather than assuming C argument order. FrameWndProc's register-record discrepancy and AskSaveDialog's incorrect raw argument-list record use the already documented step-2 resolutions.

Ghidra's stock far-cdecl specification only returns AX. The isolated patch adds **DX:AX** for four-byte values and removes ES from the unaffected-register list. The stock near/far stdcall and near cdecl conventions already provide DX:AX. Runtime shift helpers use DX:AX and CL; the checked multiply/divide/remainder helpers consume two four-byte stack arguments and return with `RETF 8`.

Width/order signatures for **190 imports** come from the same checksum-pinned Wine specifications used for step 1. These specifications describe ABI widths, not complete original Windows typedefs or return signedness. The three register-interface imports (`__fpMath`, `InitTask`, `DOS3Call`), FatalExit stub and three absolute exports are explicitly not assigned invented ordinary prototypes. Automatic generic C-library signature application and convention guessing are disabled; not every runtime helper has a verified prototype.

## Acceptance checks

The retained assembly and C files are compared with the earlier byte/structure evidence. Assembly addresses below use NE ordinals; filenames use Ghidra selectors.

| Example | What the checked output preserves |
|---|---|
| GetRaceStat, NE 29:30d2 | Near `_player` pointer, `rgAttr` at byte 62, signed-byte extension with CBW and 16-bit return. Agrees with [the manual reconstruction](../../notes/FIRST_RECONSTRUCTION.md). |
| GetFuelFree, NE 11:6004 | Four-byte far pointer loaded with LES; far call to LGetFleetStat at NE 11:40f2; caller removes six argument bytes; subtracts low/high words at fleet offsets 92/94 with borrow; returns DX:AX. Raw C retains split-word arithmetic. |
| GetRaceGrbit, NE 29:3176 | DX:AX/CL shift helper, both halves of the 32-bit `grbitAttr` field, and boolean result. |
| ICompLong, NE 9:1d74 | Machine code computes a 32-bit subtraction, but the function returns the low 16 bits in AX. It must not be rewritten as a full 32-bit comparator result merely because of its name. |
| AskSaveDialog, NE 15:432a | Pascal argument order, four parameters occupying ten bytes, `RETF 10`, correct EndDialog/WinHelp argument placement and message dispatch. |
| FrameWndProc, NE 5:06d6 | HWND/WPARAM entry values loaded into SI/DI, remaining BP arguments, Pascal cleanup and DX:AX LRESULT. |
| LpengineFromId, NE 2:5168 | 78-byte record stride and a far pointer into its own code segment; CS resolves to 1008 rather than an unexplained register. |
| ReadIniSettings | Internal cross-segment call to FSerialAndEnvFromSz and named KERNEL profile imports. |
| SetFileSeeds | Named 32-bit globals in the zero-filled part of DGROUP, including NE 37:2272/2276. |
| FLookupPart | CS-relative jump table at NE 2:5852–5874 is data, separate from instructions; 17 case slots resolve to valid instruction targets. |
| Runtime arithmetic and FPU dispatch | Explicit long-helper signatures; 13 real math tables; four error-dispatch targets and a conservative union of 39 math-dispatch targets. |

All 33 regression tests pass. Two fresh NE analyses produced identical hashes for every raw C, assembly and memory export. The tests check metadata and actual assembly/C products, including pointer widths, callback stack order, cleanup, return storage, sign extension, table boundaries and complete byte coverage. They are static acceptance tests, not game execution or equivalence tests.

## Corrections made during evaluation

The first unconfigured run produced a hidden return-pointer argument for long-returning far functions. The compiler-spec correction removes that error. Missing long-helper signatures also caused a decompiler merge failure in LInnateRaceHabitability; explicit, instruction-backed helper signatures fix it.

The FPU error dispatch at NE 36:213f indexes the four-word table at NE 37:1a8c using byte selectors from the math records. Automatic inference treated arbitrary adjacent bytes as offsets, producing impossible branch targets and bogus instructions. The importer supplies the actual four targets. The math dispatch at NE 36:2483 gets a conservative union from the 13 named records. These are static valid-table sets; later runtime work must verify which table/index pairs are reachable.

GenerateWorld exposed a separate Ghidra address-tied variable merge failure involving the far-pointer global `lpPlanets`. The analysis marks that one global conservatively volatile to retain separate accesses and allow initial C output. **This is a decompiler workaround, not a recovered source qualifier.** Its original type remains unchanged in the debug artifacts and reconstruction headers. U012 tracks the required per-function review. A diagnostic-only native patch adds the failing storage address to this error message; it does not suppress failed merges.

## Remaining work and coverage limits

Automatic classification currently identifies **901,135 instruction bytes**, **111,989 data bytes inside code segments**, and **3,406 undefined code-segment bytes**. The data segment has 21,608 defined and 4,446 undefined bytes. Every byte is represented in the ledger; undefined bytes are not silently called padding. All 1,131 public addresses in code segments are represented as functions or defined data. A discovered function boundary or data classification remains subject to later instruction review.

There are 100 distinct indirect-transfer instructions: 58 with static target sets, **20 unresolved transfers**, and 22 OS interrupts whose service behavior needs runtime review. The unresolved list includes game enumeration/drawing callbacks, bsearch/qsort comparators, CRT initialization/termination/allocation hooks, stack-check continuations and a remaining math dispatch. Address-taken relocation candidates are leads, not proof of a particular callback consumer. Dynamic/constructed pointers may reveal more candidates during execution.

Raw C still contains segmented-pointer casts, split-word operations, imperfect inferred runtime signatures, and decompiler-specific expressions. Creating C output is not evidence that every expression is correct or compilable. Further discovery can also change the function count. Step 4 supplies the execution reference; step 5 reviews and reconstructs each function. VC1.5 setup can proceed independently.
# Segment-register instruction display correction

Step 5 found a display-only operand-order defect in pinned Ghidra's register-form
MOV-to-segment instructions. P-code assignment direction is correct. Consult
`analysis/reconstruction/segment-moves.json` for byte-derived corrections before
using those raw instruction labels. Reproduce with
`python3 -m tools.audit_segment_moves`; the original exports and hashes are retained.
See `analysis/reconstruction/race-validation.md` for the audit evidence.
