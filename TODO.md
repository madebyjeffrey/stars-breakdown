# Full decompilation TODO

This checklist targets the exact supplied `target/stars.exe`. The stages describe completion criteria, not a ban on overlapping work: bring compiler checks and integration scaffolding forward during step 5, and revisit earlier stages when later analysis exposes mistakes. Check an item only when its output and supporting evidence exist in this repository.

## What “complete” means

A complete result is a readable, buildable reconstruction of all executable behavior in this binary, with recovered data layouts and resources, an auditable mapping back to machine code, and validation against the original. It includes startup, game rules, AI, file handling, UI, and error paths. Recovering 846 named procedures alone is insufficient: unnamed code, runtime helpers, callbacks, and embedded code must also be accounted for.

This does not promise the author's exact source text, comments, macros, or code absent from this build. Matching executable bytes is a separate, optional milestone. Behavioral equivalence must be supported by evidence; a passing test suite alone cannot prove equivalence for every possible game state.

## 0. Preserve the baseline and record progress

- [x] Identify the input format, file size, and SHA-256 in `analysis/summary.json`.
- [x] Preserve the input executable and generate analysis into separate files.
- [x] Create a repeatable initial CodeView extractor and known-binary verification checks.
- [x] Inventory all supplied files, archive contents, versions, and hashes; distinguish this executable from other Stars! releases.
- [x] Establish version control for tools, notes, reconstructed source, and validation fixtures; define which generated outputs are retained.
- [x] Create a coverage ledger keyed by NE segment and address range. Record code/data classification, original names, module, evidence, reconstruction path, review status, and validation status.
- [x] Create an uncertainty log with evidence, impact, and a concrete resolution task for each open question.

**Evidence:** [baseline policy and reproduction](tracking/README.md), [input inventory](analysis/input_inventory.json), [coverage ledger](analysis/coverage.json), and [uncertainty log](tracking/UNCERTAINTIES.md). Local Git records the baseline.

**Exit:** the original input is identifiable, all work is reproducible, and progress can be measured without treating symbol counts as code coverage.

## 1. Recover the complete executable layout

- [x] Read the NE segment table and CodeView logical-to-NE segment map; check named procedure ranges against segment memory extents.
- [x] Parse and validate the NE entry table, exported names, imported modules/names/ordinals, segment flags, initial registers, stack, heap, and automatic data segment.
- [x] Decode every relocation record and relocation chain, including internal targets, imported targets, and additive fixups where present.
- [x] Produce a relocation-aware memory image and address translation helpers. Keep file offsets, CodeView segments, NE ordinals, and runtime selectors distinct.
- [x] Account for zero-filled memory, discarded/loadable segments, and any special segment behavior actually present.
- [x] Extract all resources with their identifiers, types, offsets, sizes, and hashes. Decode dialogs, menus, strings, accelerators, icons, and bitmaps where present.
- [x] Classify the DOS stub, executable overlays/trailing data, padding, and debug block. Determine whether any additional executable content needs reconstruction.
- [x] Verify representative code, data, cross-segment calls, imports, and resource references against independent byte inspection or runtime observations.

**Evidence:** [executable layout report](analysis/ne/README.md), [machine-readable layout](analysis/ne/layout.json), `tools/ne_layout.py`, and `tests/test_ne_layout.py`. Memory images use synthetic selectors and a documented hardware-x87 policy; runtime emulation is step 4.

**Exit:** every file region and loaded segment has a documented role; relocation-bearing operands resolve correctly.

## 2. Finish debug-information recovery

- [x] Extract module records, 846 procedure records, raw symbols/types, and source-line mappings.
- [x] Render initial structure layouts, including field offsets and bit positions.
- [x] Audit every debug subsection and symbol/type record kind. Decode all semantically relevant records; document why any retained raw records can be excluded.
- [x] Recover complete function signatures, typedef aliases, qualifiers, argument lists, pointer kinds, arrays, and forward-type relationships.
- [x] Recover globals, file-static variables, constants, register variables, lexical scopes, and local-variable lifetimes where recorded.
- [x] Reconcile duplicate names, public symbols, procedure records, and source-line records without merging distinct functions or variable scopes.
- [x] Validate structure sizes, overlapping fields, bit fields, and alignment against actual accesses. Preserve unresolved layout questions explicitly.
- [x] Generate headers suitable for the chosen reconstruction compiler, with checks for sizes, offsets, integer widths, and pointer representations.
- [x] Measure debug coverage and identify code/data without usable symbols.

**Evidence:** [debug recovery report](analysis/debug/README.md), [effective signatures](analysis/debug/SIGNATURES.md), `reconstructed/include/`, and `tests/test_debug_recovery.py`. Headers pass host C89 storage checks; native VC1.5 validation remains U002/toolchain work. Unmapped library-index provenance is explicitly retained as U011.

**Exit:** recovered metadata is usable for decompilation and compilation, with unresolved records and coverage gaps explicitly tracked.

## 3. Establish a trustworthy disassembly/decompilation workflow

- [x] Evaluate a decompiler's actual Windows NE and 16-bit x86 handling on this binary; select and record a tested version and configuration.
- [x] Import the relocation-aware program and automate symbol, function-boundary, type, global, and source-location import.
- [x] Verify near/far calls and pointers, DS/SS/ES assumptions, argument order, caller/callee cleanup, multi-register returns, and signedness.
- [x] Identify compiler/runtime idioms, including long arithmetic, floating-point support, switch tables, indirect calls, and thunks where present.
- [x] Manually reconstruct `GetRaceStat` and anchor its metadata to exact machine bytes.
- [x] Use that reconstruction plus representative cross-segment, arithmetic, callback, and switch functions as acceptance checks for the selected tools.
- [x] Discover functions beyond the procedure records using entry points, relocation targets, callbacks, pointer tables, and control-flow analysis.
- [x] Separate inline data and jump tables from instructions; resolve overlapping or incorrect function boundaries.
- [x] Build call graphs, data-reference maps, and callback/dispatch tables. Enumerate every unresolved indirect target.
- [x] Export a reproducible disassembly and an initial decompilation for every identified function, keeping raw tool output separate from reviewed source.

**Evidence:** [tested workflow and acceptance checks](analysis/decompiler/README.md), [function exports/index](analysis/decompiler/functions.json), [automatic byte coverage](analysis/decompiler/coverage.json), and [unresolved indirect transfers](analysis/decompiler/indirect-targets.json). Ghidra 12.1.3 exports 1,043 identified functions; static acceptance passes. Raw output remains separate from reviewed source, and runtime/tool limitations remain explicit.

**Exit:** the analysis can be recreated, known examples decompile correctly, and the coverage ledger accounts for all candidate code ranges.

## 4. Set up original-game execution and comparison

**Complete for the initial reference:** clean execution, fixtures, error/recovery checks, a live function trace and exact controlled tutorial comparison are recorded in the [runtime report](analysis/runtime/README.md). Ordinary-game RNG, broader behavioral coverage and mouse automation remain explicit follow-ups.

- [x] Establish a reproducible environment capable of running this executable and document its OS, emulator/VM, configuration, and required companion files.
- [x] Capture startup and basic interaction as baseline smoke tests, including failure behavior for missing dependencies.
- [x] Create small, controlled game/race/save/turn fixtures and record all inputs, outputs, seeds, and relevant environment state.
- [x] Identify randomness, time, configuration, and other nondeterministic inputs; establish how to control or record them.
- [x] Add observation methods appropriate to the environment: memory snapshots, breakpoints, function traces, file comparisons, and UI captures.
- [x] Build a comparison harness for original versus reconstructed behavior, documenting any normalization of timestamps or other nondeterministic fields. (`tools/runtime/compare.py`; all bytes compared, no normalization; reconstruction not yet available.)
- [x] Record original crashes, arithmetic quirks, and malformed-input behavior as compatibility evidence rather than silently correcting them.

**Exit:** there is a practical behavioral reference for validating source as it is reconstructed. Begin this stage before substantial subsystem rewriting.

## 5. Reconstruct the program, subsystem by subsystem

**Detailed function checklist:** [TODO5.md](TODO5.md) enumerates every identified
game procedure, runtime/helper entry and DOS-stub entry, grouped by subsystem,
with signatures, dependencies, reviewed/partial/open status and evidence links.

**In progress:** 130 named routines (55,942 original procedure bytes) now have
reviewed C89 source and original-x86 differential tests. See
[the core reconstruction report](analysis/reconstruction/core.md).
Completed components are checked under their subsystem below. A parent checkbox
stays open until the whole subsystem meets the Step 5 completion criterion.
The race scoring/random-generation chain now runs independently of Windows;
`python3 tools/race_demo.py --seed 0 --record-roundtrip` also exercises player
serialization, header generation and record encryption. Stream retry/transfer
control flow has explicit OS/error boundaries. Filename generation, file-creation
recovery and alert selection are reconstructed. The save/load and race-creation
dependencies now form an integrated host component, including message expansion,
logging, passwords and all six wizard pages. START2 now connects this component
to real Win16 dialogs, drawing, file pickers and settings. Current work targets startup,
runtime, memory and common utilities; see the completed component milestones
below and the [foundation report](analysis/reconstruction/foundations.md).

### Work cadence and next milestones — revised 2026-09-14

The user requested larger, less tedious work sessions. The preceding batches of
roughly five routines were checkpoint choices, not a technical requirement.
Continue toward a coherent subsystem outcome; do not end a work session merely
because a handful of functions or a local commit is complete. Internal commits
remain useful recovery points. Report functioning paths, dependencies closed,
remaining boundaries and validation results, with function counts as supporting
coverage rather than the work quota.

Use the existing complete raw exports and call graph to assemble each dependency
cluster before implementation. Reuse common ABI, state and comparison machinery
across related routines. Run focused checks during development and the full
regression suite at a substantial integration checkpoint or when shared changes
require it. Retain the per-function review standard; raw decompiler output and
untested callbacks must not be promoted to completed reconstruction to increase
throughput. Continue independent work while a manual VM experiment is pending.

The startup/runtime and memory/common component checklists are now grouped
under **Subsystem completion** below. Earlier build and race milestones follow.

- [x] **Early VC++ compatibility check:** prepare one small Win16 fixture package
  for the user's VC++ 1.52c / Windows 98 SE / 86Box environment. Capture compiler
  and linker banners, commands and outputs; test integer widths, near/far pointers,
  representative recovered layouts and calling conventions, plus a small group
  of reconstructed routines. Record what the comparison establishes under U002.
  This is a step-5 integration check, not completion of the step-6 application.
  **Complete with RETURN2, 2026-09-14:** all 563 utility and 1,491 layout checks
  passed natively, including all 1,469 recovered field offsets. Every type-size
  assertion compiled. All 23 returned inputs match package 2. Compiler records
  match the original and five CRT modules match exactly; see the
  [review](analysis/compat/vc152-return2.md). The console-spacing workaround in
  `ftp-share/VC152C/` changes status printing only; no repeated build is needed.
- [x] **Race-file save/load dependencies and host integration:** reconstruct and integrate FSaveRace and
  FWasRaceFile with the existing creation, record, checksum and stream code.
  Close their message-formatting, logging and password dependencies, including
  PszFormatIds/PszFormatString, OutputSz/OutputFileString/SzVersion and
  FCheckPassword/LSaltFromSz/PasswordDlg/Delay as required by the verified graph.
  Account for additional dependencies and indirect callbacks rather than assuming
  this initial list is exhaustive. Exercise save, load, cancel, invalid files,
  password outcomes and I/O failure recovery with synthetic inputs, and check
  files in both directions against the original. Report any remaining OS boundary
  explicitly; a callback-only path does not complete its underlying behavior.
  **Complete, 2026-09-14:** actual game helpers and password callbacks execute in
  both implementations. Saved bytes match; invalid/truncated files, password
  outcomes and I/O recovery are compared. See [the component report](analysis/reconstruction/race-component.md).
- [x] **Race-creation dependencies and host integration:** integrate the complete wizard and generation
  path, including name-buffer initialization and U020, validation/scoring,
  save/load integration and the UI/resource contracts. Keep arithmetic and legacy
  edge cases mapped to their evidence. Use the resulting runnable component as
  the unit of delivery before proceeding to the next subsystem cluster.
  **Complete, 2026-09-14:** New Race initialization, six dialog pages, shared
  drawing/input helpers, scoring and save integration are reconstructed. The
  original and C callbacks produce matching files and control/drawing events.
  Random templates preserve the original deferred-generation behavior. U020's
  affected callback sequence is reproduced under the documented OS model.
  The [audit](analysis/reconstruction/race-dependencies.json) accounts for all
  88 reachable game-code targets, including 12 caller-specific helper paths
  that are not counted as complete standalone procedures.
- [x] **Race acceptance scenarios:** prepare a generated catalog shared by the
  reconstructed component, original-instruction comparisons and live replay
  recipes. Include valid saves, expected rejections, U020 and file comparison
  that accounts explicitly for the BOF salt. See the [scenario package](compat/race/README.md).
  **2026-09-14:** 88 scenarios match under modeled Windows services; one live
  DOSBox baseline passes both file directions. The 15-case live pilot remains
  incomplete; the buildable compiler/output package is the next item.
- [x] **Buildable race compiler/output package:** export the recovered component,
  an automatic C runner, frozen original expectations and a VC++ 1.52c build
  recipe. Run all 88 scenarios locally with real file I/O and retain generated
  state/files for VM comparison. **2026-09-14:** [RACEVC1](compat/racevc/README.md)
  passes 812 host checks, generates 61 matching race files and 89 state snapshots,
  and detects deliberately incorrect expectations. It models Windows services;
  it does not complete real Windows UI acceptance.
- [x] **Native race compiler/output run:** build in VC++ 1.52c, execute
  RACEVC.EXE, return all logs/artifacts, and investigate compiler, linker and
  output mismatches. **Complete, 2026-09-14:** RETURN4 compiled and linked
  revision 2 cleanly. Its Win98 SE run stopped after 11 passing cases. The exact
  returned EXE then completed all 88 cases / 89 sessions in DOSBox / WfW 3.11:
  **816 checks, zero failures; all 89 states and 61 race files match.** The
  initial RETURN3 alias and signed-comparison issues are resolved. See the
  [returned-build and replay review](analysis/compat/racevc-return4.md).
  RETURN5 also completes the full revision-3 run in the user's Win98 SE VM.
- [x] **Native runner progress and cancellation:** revision 3 compiled/linked
  cleanly in RETURN5 and completed all **816 checks** in the user's Win98 SE
  workflow. All 61 race files, 89 player states and 3,158 trace checkpoints are
  verified; reported guest elapsed time is 536 seconds. The exact returned EXE
  visibly displayed progress in DOSBox / WfW 3.11. Esc, Space on Cancel and
  Alt+F4 each stopped safely with CANCELLED and zero failures; the 13 completed
  sessions in the button run retained all matching outputs. Mouse automation
  delivery is not credited by those keyboard checks. See the
  [RETURN5 review](analysis/compat/racevc-return5.md). No rebuild is needed.
- [x] **Native race workshop and initial acceptance (START2):** built the new
  VC++ 1.52c application with all six original-resource dialogs, real USER/GDI
  services, file dialogs, save/reopen, Recent Race, settings and complete cleanup.
  The final executable passes real six-page creation/save/load/cancel checks,
  bitmap repaint, interactive file exchange with the original, negative-point
  validation and invalid-file recovery. Its bundled regression passes all 816
  native checks, 61 files and 89 states. Source, prebuilt programs, automatic
  checks and manual instructions are in START2. See the
  [report](analysis/reconstruction/start2.md) and
  [evidence](analysis/compat/startup2-native.json).
- [ ] **Broader native race acceptance:** run the complete input matrix with
  actual Windows controls, including U020 text-tail behavior and password dialogs;
  verify physical mouse controls and the user's Windows 98 SE / 86Box run.
  CHECKS.TXT and REGRESS in START2 provide exact actions and expected output.
  Return the whole package in a new RETURN folder. DOSBox remains authorized
  for agent-operated reference checks.

Step 5 is finished only when its full exit criterion is met. These milestones
change the size and usefulness of work sessions, not the definition of completion.

### Subsystem completion

Checked child items are complete for their stated scope. An unchecked parent
with checked children is **partly complete**, not unstarted. The order remains
provisional; module names identify study areas rather than architectural boundaries.

- [ ] **Startup and runtime — in progress:** NE/DOS entry paths, initialization/termination, compiler helpers, memory model, and linked runtime code.

  - [x] **DOS entry:** reviewed assembly reproduces the original 98 bytes and its
    print/exit behavior. Complete 2026-09-14.
  - [x] **Linked CRT code mapping:** all 101 code-bearing records match 15,192
    original bytes, including 856 checked linker fixups. Seven further records
    contain no code. See the [runtime audit](analysis/reconstruction/runtime-library.json).
  - [x] **Fonts, colors, brush cache, tiles and complete FreeStuff cleanup:**
    six routines / 3,785 bytes with original-instruction comparisons, including
    integrated font creation, color refresh and terminal cleanup. Complete
    2026-09-14; see the [foundation report](analysis/reconstruction/foundations.md).
  - [ ] **Application startup and configuration:** finish FCreateStuff,
    InitInstance, INI settings, WinMain, command-line/batch setup, class
    registration and message-loop dependencies; connect the mapped NE entry
    and recovered resource helpers and validate the integrated startup path.
  - [x] **START2 configuration component:** ReadIniTileSettings, GetWindowRc
    and SetWindowIniString add 813 reviewed bytes. Main-frame/layout/pane/tile/
    font settings paths and a new native settings dialog are integrated. This
    scoped component does not complete ReadIniSettings or WriteIniSettings.
    See [START2](analysis/reconstruction/start2.md).
  - [x] **START2A habitability color correction:** recover and connect the six
    original planet-attribute brushes; compare the startup slice against x86
    and require rendered blue/red/green range pixels in native acceptance.
    Standard VGA acceptance passed; the existing SVGA setup also misdraws the
    original game's bars. See [evidence](analysis/compat/startup2a-native.json).
  - [x] **Starting-screen source milestone:** recover large-resource loading,
    palette construction, saved-window parsing and the fallback starfield;
    connect recovered title creation, painting and palette handling to a new
    Win16 bootstrap. Fonts use the original configuration rules in a separate
    INI file. Five additional complete routines / 1,959 original bytes plus
    explicitly scoped TitleWndProc and ReadIniSettings paths. See the
    [starting-screen report](analysis/reconstruction/starting-screen.md).
  - [x] **First native starting-screen launch:** VC++ 1.52c compiled/linked the
    application; WfW 3.11 drew the original title, read 481,088 bitmap bytes,
    repainted and exited with successful cleanup logged. The source and prebuilt
    executable are packaged as START1. This does not complete WinMain or the
    full application initialization/command paths.
  - [x] **Repeat native startup acceptance:** fresh WfW 3.11 session passed
    /SMOKE, /FALLBACK /SMOKE, New/Open notices and dismissal repaints, Alt+X
    and Alt+F4. All four runs logged complete cleanup/exit; no guest fault
    appeared. The unchanged START1 executable was tested after user confirmation;
    see the [acceptance record](analysis/compat/startup1-acceptance.json).
  - [ ] **Intermittent guest graphics/fault investigation:** the first session's
    PROGMAN/USER.EXE fault did not recur during fresh acceptance or an original
    title-screen comparison. Page-4 colored fills now misdraw in both original
    and reconstruction under SVGA256; the same rebuilt EXE passes with standard
    VGA. The display path is implicated in that symptom, while its mechanism
    and the earlier fault remain open (U023).
  - [x] **CRT data member identification and bounded audit:** identify all seven
    records without code; six mappings are complete. Across all CRT records,
    compare 2,199 initialized literals and 98 of 99 data fixups. Preserve the
    Windows stub slot and common message-padding ambiguities in the
    [data audit](analysis/reconstruction/runtime-data.md).
  - [ ] **Remaining runtime data and linker placement:** independently reproduce
    the __fptaskdata zero-slot transformation and locate common PAD/EPAD markers.
    The mapped members do not prove the complete original linker layout.
- [ ] **Memory and common utilities — in progress:** `memory`, `utilgen`, relevant parts of `util`, allocation/ownership, collections, arithmetic helpers, and random-number generation.

  - [x] **Complete memory manager:** all 11 memory.obj routines / 2,390 bytes,
    including block growth/movement, allocation/free/reset, owner lookup and
    packed list storage. Tests compare full heap contents, ownership links,
    allocation failures, nonlocal aborts and Windows-service order. Terminal
    cleanup uses this implementation. Complete 2026-09-14; see the
    [foundation report](analysis/reconstruction/foundations.md).
  - [x] **Reviewed RNG core and seed helpers:** initialization, seed handling,
    random generation and saved RNG state, with original-instruction comparisons.
    See the [core report](analysis/reconstruction/core.md).
  - [x] **Additional numeric and bounds helpers:** PszFromInt, PszFromLong,
    PszFromLongK, CommaFormatLong, BoundPoints and WPackLong; six routines /
    1,089 bytes. Preserve the original pointer/value tests, unterminated suffixes,
    integer wrap and aliasing. Complete 2026-09-14.
  - [ ] **Collection ownership and remaining shared helpers:** finish relevant
    planet/fleet/thing lookup, duplication and ownership paths, remaining
    arithmetic/geometry such as FIntersectCircleLine, and their dependencies.
- [ ] **Core data and component definitions:** `parts`, `thing`, shared globals, tables, identifiers, and resource-backed constants.
- [ ] **Files and persistence:** `file`, `save`, `log`; reconstruct record formats, encoding, checksums, seeds, version handling, and validation paths as found.
- [ ] **Game/race creation:** `create`, `race`, `init`, `build`, including configuration and command-line paths.
- [ ] **Simulation:** `planet`, `ship`, `ship2`, `mine`, `produce`, `research`, `turn`, `turn2`, `turn3`; establish the exact ordering and state changes of turn processing.
- [ ] **Combat and playback:** `battle`, `vcr`; recover both battle resolution and its displayed/replayed representation.
- [ ] **AI:** `ai`, `ai2`, `ai3`, `ai4`, `aiutil`; recover decision rules, state, tie-breaking, and randomness.
- [ ] **Windows interface:** `stars`, `mdi`, `scan`, `popup`, `tb`, `report`, `msg`, `strings`, and UI code in other modules; map every dialog, message handler, timer, command, and callback.
- [ ] **Tutorial and remaining behavior:** `tutor`, `tutor2`, `dummy`, `debug`, and every module or unnamed routine not covered above.
- [ ] **Resources and non-code data:** incorporate extracted assets, dialogs, menus, strings, and tables into the build with preserved identifiers and semantics.

  - [x] **Original resource embedding:** all 122 resources are included in the
    new executable with identical types, identifiers, flags, sizes and SHA-256
    hashes. Native RC 3.11 binding and NE roundtrip verification pass.
  - [x] **Starting-screen static data:** use preserved DGROUP initialization,
    all 1,414 compressed strings with executable string-segment code removed,
    and reviewed starfield tables. All extracted strings match the full carrier.
  - [ ] Audit and use remaining non-code tables, resource semantics and CRT
    data throughout the complete application.

The checked C components above have host builds and original-instruction tests.
The starting-screen milestone also exercises native Windows heap/resource
adapters. Broader native integration and repeat live acceptance remain open;
compiler installation alone does not complete those checks.

### Per-function completion checklist

This is a template for each routine, not a global progress checklist. Completed
routine reviews are recorded in [the coverage ledger](analysis/coverage.json);
130 named game routines currently meet the reviewed-source standard.

- [ ] Record address range(s), names, source mappings, signature, calling convention, and evidence links.
- [ ] Resolve control flow, call targets, global references, structure accesses, and local-variable roles.
- [ ] Replace tool-generated pseudocode with readable source that preserves observed behavior; document inferred names separately from recovered names.
- [ ] Preserve integer widths, overflow/truncation, shifts, signed comparisons, rounding, pointer arithmetic, and aliasing. Model machine behavior explicitly where modern C would be undefined or implementation-dependent.
- [ ] Review each basic block against the disassembly, including early exits, errors, and rare branches.
- [ ] Validate with representative and boundary inputs against the original where practical. Record the validation method and any limits; pure UI/runtime routines may need integration checks.
- [ ] Compile the function within the reconstruction, resolve its dependencies, and update the coverage ledger.

**Exit:** every identified routine has reviewed source or an explicitly documented runtime/library implementation with an audited mapping. No unresolved behavior is hidden behind placeholders or silently omitted modules.

## 6. Build a complete replacement executable

A partial native executable now opens the starting screen; see the
[START1 instructions](compat/startup/README.TXT). The complete-program items
below remain open, including New/Open/Continue game behavior.

- [ ] Choose and document a reproducible compiler/toolchain and target. Prefer an original-compatible 16-bit build for direct comparison if feasible; if using a modern target, explicitly implement the original memory/arithmetic/OS semantics.
- [ ] Create build scripts, startup integration, compiler compatibility definitions, and resource compilation steps.
- [ ] Integrate all reconstructed modules and globals, resolving dependencies without behavioral stubs.
- [ ] For each linked runtime routine, either reconstruct it or map it to a validated implementation; record version and semantic differences. Replacements do not count as verbatim runtime-source recovery.
- [ ] Preserve file formats, command-line behavior, configuration, resource identifiers, and observable Windows interactions.
- [ ] Verify the complete program starts, creates/loads games, accepts orders, generates turns, displays/replays battles, saves, and exits through its reconstructed implementation.
- [ ] Document every external dependency and demonstrate a clean build from the recorded inputs.

**Exit:** a clean build produces a runnable, complete reconstruction with no required behavior delegated back to `stars.exe`.

## 7. Validate the whole program and close coverage gaps

- [ ] Compare original and reconstructed state across controlled turn-generation scenarios covering all recovered mechanics, race traits, AI modes, combat, and ordering interactions.
- [ ] Verify game/race/turn-file compatibility in both directions wherever the original supports it.
- [ ] Exercise long-running games, large supported inputs, numeric boundaries, empty states, allocation failures where observable, and error/cancellation paths.
- [ ] Exercise all UI commands, dialogs, shortcuts, reports, tutorials, and automation/command-line modes present in the binary.
- [ ] Resolve every mismatch or retain a precise, evidence-backed explanation of environmental differences. Intentional behavior changes belong in a separate follow-up, not the equivalence baseline.
- [ ] Audit all executable segment ranges: reconstructed code, identified runtime, thunks, embedded data, or padding. Investigate every unexplained byte range.
- [ ] Audit every export, callback, indirect-call table, import, resource, global, and function against the coverage ledger.
- [ ] Close all uncertainty items that affect behavior. Nonrecoverable original spelling/comments may remain documented without blocking completion.

**Exit:** no known behavioral discrepancies or unexplained executable regions remain, and the validation report states the scope and limits of the evidence.

## 8. Package the finished study

- [ ] Organize readable source and reconstructed headers by recovered module, retaining machine-code/source traceability.
- [ ] Document the program architecture, memory model, data packing, simulation order, AI techniques, rendering/UI, and persistence formats with concrete examples.
- [ ] Provide one documented workflow to regenerate extraction, analysis imports, builds, and validation results.
- [ ] Publish a local completion report with coverage totals, tool versions, input hashes, runtime/library substitutions, test results, and remaining nonbehavioral uncertainties.
- [ ] Mark this project complete only after the definition of completion and every required exit criterion above are satisfied.

## Optional: byte-identical rebuild

This is not required for the behaviorally complete decompilation above.

- [ ] Identify the exact original compiler, linker, libraries, flags, resource tools, and link ordering from evidence.
- [ ] Reproduce code generation, segment layout, resource ordering, debug output, and metadata; explain every remaining binary difference.
- [ ] Demonstrate a byte-identical executable, or report precisely which regions match and which differ. Do not equate similar decompiler output with a matching rebuild.
