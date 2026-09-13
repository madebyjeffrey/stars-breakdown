# Full decompilation TODO

This checklist targets the exact supplied `target/stars.exe`. Complete the stages in order, revisiting earlier stages when later analysis exposes mistakes. Check an item only when its output and supporting evidence exist in this repository.

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
- [ ] Audit every debug subsection and symbol/type record kind. Decode all semantically relevant records; document why any retained raw records can be excluded.
- [ ] Recover complete function signatures, typedef aliases, qualifiers, argument lists, pointer kinds, arrays, and forward-type relationships.
- [ ] Recover globals, file-static variables, constants, register variables, lexical scopes, and local-variable lifetimes where recorded.
- [ ] Reconcile duplicate names, public symbols, procedure records, and source-line records without merging distinct functions or variable scopes.
- [ ] Validate structure sizes, overlapping fields, bit fields, and alignment against actual accesses. Preserve unresolved layout questions explicitly.
- [ ] Generate headers suitable for the chosen reconstruction compiler, with checks for sizes, offsets, integer widths, and pointer representations.
- [ ] Measure debug coverage and identify code/data without usable symbols.

**Exit:** recovered metadata is usable for decompilation and compilation, with unresolved records and coverage gaps explicitly tracked.

## 3. Establish a trustworthy disassembly/decompilation workflow

- [ ] Evaluate a decompiler's actual Windows NE and 16-bit x86 handling on this binary; select and record a tested version and configuration.
- [ ] Import the relocation-aware program and automate symbol, function-boundary, type, global, and source-location import.
- [ ] Verify near/far calls and pointers, DS/SS/ES assumptions, argument order, caller/callee cleanup, multi-register returns, and signedness.
- [ ] Identify compiler/runtime idioms, including long arithmetic, floating-point support, switch tables, indirect calls, and thunks where present.
- [x] Manually reconstruct `GetRaceStat` and anchor its metadata to exact machine bytes.
- [ ] Use that reconstruction plus representative cross-segment, arithmetic, callback, and switch functions as acceptance checks for the selected tools.
- [ ] Discover functions beyond the procedure records using entry points, relocation targets, callbacks, pointer tables, and control-flow analysis.
- [ ] Separate inline data and jump tables from instructions; resolve overlapping or incorrect function boundaries.
- [ ] Build call graphs, data-reference maps, and callback/dispatch tables. Enumerate every unresolved indirect target.
- [ ] Export a reproducible disassembly and an initial decompilation for every identified function, keeping raw tool output separate from reviewed source.

**Exit:** the analysis can be recreated, known examples decompile correctly, and the coverage ledger accounts for all candidate code ranges.

## 4. Set up original-game execution and comparison

- [ ] Establish a reproducible environment capable of running this executable and document its OS, emulator/VM, configuration, and required companion files.
- [ ] Capture startup and basic interaction as baseline smoke tests, including failure behavior for missing dependencies.
- [ ] Create small, controlled game/race/save/turn fixtures and record all inputs, outputs, seeds, and relevant environment state.
- [ ] Identify randomness, time, configuration, and other nondeterministic inputs; establish how to control or record them.
- [ ] Add observation methods appropriate to the environment: memory snapshots, breakpoints, function traces, file comparisons, and UI captures.
- [ ] Build a comparison harness for original versus reconstructed behavior, documenting any normalization of timestamps or other nondeterministic fields.
- [ ] Record original crashes, arithmetic quirks, and malformed-input behavior as compatibility evidence rather than silently correcting them.

**Exit:** there is a practical behavioral reference for validating source as it is reconstructed. Begin this stage before substantial subsystem rewriting.

## 5. Reconstruct the program, subsystem by subsystem

For each group below, follow the per-function checklist. The order is provisional: use the recovered dependency graph to adjust it. Module names identify initial study areas, not verified architectural boundaries.

- [ ] **Startup and runtime:** NE/DOS entry paths, initialization/termination, compiler helpers, memory model, and linked runtime code.
- [ ] **Memory and common utilities:** `memory`, `utilgen`, relevant parts of `util`, allocation/ownership, collections, arithmetic helpers, and random-number generation.
- [ ] **Core data and component definitions:** `parts`, `thing`, shared globals, tables, identifiers, and resource-backed constants.
- [ ] **Files and persistence:** `file`, `save`, `log`; reconstruct record formats, encoding, checksums, seeds, version handling, and validation paths as found.
- [ ] **Game/race creation:** `create`, `race`, `init`, `build`, including configuration and command-line paths.
- [ ] **Simulation:** `planet`, `ship`, `ship2`, `mine`, `produce`, `research`, `turn`, `turn2`, `turn3`; establish the exact ordering and state changes of turn processing.
- [ ] **Combat and playback:** `battle`, `vcr`; recover both battle resolution and its displayed/replayed representation.
- [ ] **AI:** `ai`, `ai2`, `ai3`, `ai4`, `aiutil`; recover decision rules, state, tie-breaking, and randomness.
- [ ] **Windows interface:** `stars`, `mdi`, `scan`, `popup`, `tb`, `report`, `msg`, `strings`, and UI code in other modules; map every dialog, message handler, timer, command, and callback.
- [ ] **Tutorial and remaining behavior:** `tutor`, `tutor2`, `dummy`, `debug`, and every module or unnamed routine not covered above.
- [ ] **Resources and non-code data:** incorporate extracted assets, dialogs, menus, strings, and tables into the build with preserved identifiers and semantics.

### Per-function completion checklist

- [ ] Record address range(s), names, source mappings, signature, calling convention, and evidence links.
- [ ] Resolve control flow, call targets, global references, structure accesses, and local-variable roles.
- [ ] Replace tool-generated pseudocode with readable source that preserves observed behavior; document inferred names separately from recovered names.
- [ ] Preserve integer widths, overflow/truncation, shifts, signed comparisons, rounding, pointer arithmetic, and aliasing. Model machine behavior explicitly where modern C would be undefined or implementation-dependent.
- [ ] Review each basic block against the disassembly, including early exits, errors, and rare branches.
- [ ] Validate with representative and boundary inputs against the original where practical. Record the validation method and any limits; pure UI/runtime routines may need integration checks.
- [ ] Compile the function within the reconstruction, resolve its dependencies, and update the coverage ledger.

**Exit:** every identified routine has reviewed source or an explicitly documented runtime/library implementation with an audited mapping. No unresolved behavior is hidden behind placeholders or silently omitted modules.

## 6. Build a complete replacement executable

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
