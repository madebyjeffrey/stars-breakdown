# Native tutorial instruction matching

This is an isolated compiler experiment, not a linked tutorial application. The
sources preserve the behavior of selected original tutorial procedures, and the
comparison determines which entire procedure extents match. Unmatched candidates
are explicitly listed in the report and must not be treated as recovered code.

The authoritative results are in
[the tutorial report](../../analysis/compat/tutorial-byte-match.md) and its
[JSON evidence](../../analysis/compat/tutorial-byte-match.json). TODO5 has a
separate byte-match column and per-function checkboxes. The semantic review
ledger and application integration status remain independent.

## Rebuild

A folder-backed DOS drive containing `MSVC/BIN/CL.EXE`, the compiler tools and
headers is sufficient. This experiment uses the installed VC++ 1.52c compiler;
it does not boot Windows or mount the Windows 95 disk images.

From the repository root:

```sh
python3 tools/prepare_tutorial_match.py --work runs/tutorial-match/rebuild
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
  .tools/StarsRuntime.app/Contents/MacOS/dosbox-x \
  -nogui -nomenu -nopromptfolder -fastlaunch \
  -conf runs/tutorial-match/compile.conf
python3 tools/compare_tutorial_match.py runs/tutorial-match/rebuild \
  --output analysis/compat/tutorial-byte-match.json \
  --fixture tests/fixtures/tutorial-byte-match.json
python3 tools/render_todo5.py
python3 -m unittest tests.test_tutorial_byte_match tests.test_byte_match tests.test_runtime_library
```

Pass `--compiler-drive /absolute/path` to the preparer if the directory containing
`MSVC` is elsewhere. The generated batch deletes each old object/listing before
compiling, stops on a compiler failure, and writes `PASS` or `FAIL` to `DONE.TXT`.
Each function has its own compiler log. The comparison refuses unknown symbols,
unsupported fixups, incomplete objects, or working sources different from the
repository candidates. An instruction mismatch is an experimental outcome, not
a compiler failure: inspect each function's exact-match flag.

Flags: `/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc`. `/Gs` reproduces the original
absence of stack-check calls. `/Fc` supplies procedure extents; actual code comes
from the OMF object, including the compiler's jump/frame backpatch records.

## Reading the sources

`TYPES.H` contains small native 16-bit views, not a complete application header.
Opaque regions retain offsets used by these functions. Its experimental
production-list count view is still unresolved for the unmatched queue candidate;
do not reuse it as a general game type definition. Only the accesses in credited
function bodies have byte-match evidence.

Some winning candidates retain trial basenames, length-preserving local names,
and local-identifier aliases.
VC152's unoptimized stack allocation changed with declaration ordering and
identifier storage lengths in these experiments. Where aliases are used, readable names expand to the exact compiled identifiers;
a clean rebuild verifies the resulting bytes.
These aliases do not change an instruction, operand, or object after compilation.
Do not simplify redundant branches, unused assignments, or loop structure merely
because a modern compiler would optimize them away. Several such operations are
present in the original but absent from raw decompiler pseudocode.

The functions have unresolved external definitions by design: the comparator
binds calls and global references to their original symbol identities. A match
proves the function's generated instructions in that address layout. It does not
supply the callees, construct a complete tutorial, or reproduce the final linker
segment ordering and executable bytes.
