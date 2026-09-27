# Step 5: memory, startup resources and linked runtime

Reviewed 2026-09-14. This completes **all 11 routines in memory.obj**, the
font/color/tile/terminal-cleanup resource lifecycle, six additional common
helpers, the DOS entry stub, and an audit of every code-bearing CRT module
record. The new C contribution is **23 named routines / 7,264 original bytes**;
the project total at that checkpoint was **122 / 846 named game routines /
53,170 original bytes**. Current totals are in [TODO5](../../TODO5.md), with
the separate Phase 2 gates in [TODO7](../../TODO7.md).
DOS and CRT evidence are counted separately from named game reconstruction.

The C sources compile together with the earlier race component under strict
C89, with warnings treated as errors. They implement game behavior using explicit
state and operating-system adapters. These new adapters have not yet had a
native VC++/Win16 acceptance run. The previous RETURN5 result remains the
accepted native race-component result; its frozen package is unchanged.

## Completed source mapping

Ranges below are hexadecimal NE ordinals and half-open offsets. Original
signatures, parameter names, type IDs and source-line mappings remain in
[procedures.json](../debug/procedures.json), [functions.json](../functions.json)
and [sfuncs.h](../../reconstructed/include/sfuncs.h). All 23 original routines
use far C calls with caller cleanup, stack parameters at entry SP+4, and AX or
DX:AX results as appropriate. The host API names and explicit context parameters
are reconstruction choices; they are not claims about the original source text.
Every listed body was reviewed against its complete disassembly; the raw
export's identity is retained in the [export manifest](../decompiler/export-manifest.json).

| Original routine | NE range | Source / host entry |
|---|---|---|
| LphbAlloc | 000d:0000–0108 | memory.c / stars_heap_alloc |
| LphbReAlloc | 000d:0108–02d8 | memory.c / stars_heap_realloc |
| FreeHb | 000d:02d8–0348 | memory.c / stars_heap_free |
| ResetHb | 000d:0348–03b2 | memory.c / stars_heap_reset |
| LpAlloc | 000d:03b2–058c | memory.c / stars_alloc |
| LphbFromLpHt | 000d:058c–065f | memory.c / stars_heap_from_pointer |
| LpReAlloc | 000d:0660–07a8 | memory.c / stars_realloc |
| FreeLp | 000d:07a8–0835 | memory.c / stars_free |
| LpplReAlloc | 000d:0836–088b | memory.c / stars_list_realloc |
| LpplAlloc | 000d:088c–0918 | memory.c / stars_list_alloc |
| FreePl | 000d:0918–0959 | memory.c / stars_list_free |
| PszFromInt | 0009:2274–22b6 | common.c / stars_number_int |
| PszFromLong | 0009:22b6–22fd | common.c / stars_number_long |
| PszFromLongK | 0009:22fe–2440 | common.c / stars_number_scaled |
| CommaFormatLong | 0009:2440–2533 | common.c / stars_number_commas |
| BoundPoints | 0009:2f70–3094 | common.c / stars_bound_points |
| WPackLong | 0008:4ba2–4c01 | common.c / stars_pack_long |
| HbrGet | 0009:4448–4531 | startup.c / stars_brush_get |
| FreeHbr | 0009:4532–45a0 | startup.c / stars_brush_free |
| FGetSystemColors | 0004:08d2–0bae | startup.c / stars_system_colors |
| FCreateFonts | 0001:0ab2–0d70 | startup.c / stars_create_fonts |
| InitTiles | 0001:0eaa–101f | startup.c / stars_init_tiles |
| FreeStuff | 0004:0bae–1211 | startup.c / stars_free_stuff |

Source: [memory.c](../../reconstructed/src/memory.c),
[common.c](../../reconstructed/src/common.c),
[startup.c](../../reconstructed/src/startup.c).
The small gaps between procedure ranges are not credited as reconstructed
instructions. The editable [review ledger](../../tracking/reviews.json)
credits only the procedure extents.

## How the allocator works

The program maintains 12 linked chains of movable Windows global-memory blocks.
Each block begins with a 16-byte header: total free bytes, block size, unused
tail bytes, tail offset, next selector:offset, Windows handle and heap type.
Byte 15 is retained rather than initialized. Each allocation has a two-byte
size prefix whose low bit marks a freed chunk. Heap heads begin at DGROUP:0d34;
the 12-word growth table at 0d64 contains
63488, 4096, 4096, 4096, 8192, 63488, 65280, 17472, 4096, 6144, 2048, 65280.

Allocation first uses untouched space at a block's tail. When that cannot
satisfy the request, it scans freed chunks and combines adjacent holes only
until the requested size fits. It consumes that entire hole without splitting
off the excess. Freeing the last chunk moves the tail backwards; earlier holes
are not immediately absorbed. Reset discards logical allocations while leaving
their bytes and the block chain intact. This is a compact allocator tailored
to 16-bit address spaces, with explicit reuse of existing contents.

Reallocation can extend the last allocation in place. Ordinary objects otherwise
move to a newly allocated chunk, copy their old payload and free the original.
Heap types 3 and 10 instead grow their owning Windows block repeatedly; callers
must use the sole payload at block+18. A moved block is reattached correctly at
the head, middle or tail of the ownership chain. Growth stops at ffDCh bytes.
GlobalAlloc/ReAlloc flags are 0022h, without implicit zero initialization.

Out-of-memory handling runs the recovered message formatter and alert path,
then performs the original nonlocal abort. Failed GlobalReAlloc follows an
unlock and does not relock the old block. FreeHb frees a whole chain but leaves
the global head stale for its caller to manage. List storage retains packed
flags, one-byte capacity/count fields, the previous upper nibble and the
original 16-bit size-calculation wrap.

[memory.h](../../reconstructed/include/memory.h) represents a far address as a
32-bit selector:offset token. Its resolver supplies a complete writable 64 KiB
image; GlobalAlloc/ReAlloc/Lock/Unlock/Free are the only heap service boundaries.
Game allocation policy is implemented in C, not hidden in those callbacks.
Far arithmetic wraps the offset without carrying into the selector. Owner
lookup compares selector then offset, with both block boundaries excluded.

An explicit test confirms that a zero-byte allocation consuming the final
two-byte prefix can return the block's excluded end address. Owner lookup
then fails. This is preserved and tracked as U021; the caller reachability of
the corresponding invalid free is still open.

## Startup resources and common helpers

The startup component now creates all nine font variants, retains configured
font names or expands the four defaults, applies device DPI, measures four
fonts and restores the original selected object. It refreshes eight system
brushes and screen metrics, edits two bitmap palettes in BGR byte order, and
calculates the two fixed tile arrays with the original grouping and 16-bit
overflow behavior.

The 32-entry brush cache chooses the last unused slot, uses byte-sized
reference counts that can wrap, and returns an uncached brush when full.
Freeing a zero or untracked handle still reaches DeleteObject. Font creation
and color refresh retain the original unconditional success result even when
individual GDI creation calls fail.

FreeStuff is reconstructed in full, including the ordered release of brushes,
callback thunks, region, cursors, icon, bitmap resources, fonts, pens, logging
and message allocations, and all 12 heap chains. Its heap operations use the
new memory implementation. Only the log and message pointers are cleared;
other handles and heap heads are left stale. It is terminal cleanup, not an
idempotent reset. Tests cover the combined font creation → color refresh →
cleanup path as well as a populated synthetic resource/heap state.

The six common helpers preserve several details that a straightforward rewrite
would miss: PszFromLong tests the *value* at its count pointer, PszFromLongK
overwrites the string terminator with k/M without adding another terminator,
and large positive rounding can overflow into negative values. Negative small
values are narrowed to 16 bits. Bounding rectangles read point zero even for
nonpositive counts and wrap their one-pixel expansion at signed limits.
WPackLong uses logical two-bit shifts and allows the packed exponent to truncate.
Comma formatting groups the minus sign with the first digits.

## DOS entry and runtime-library mapping

[dos_stub.s](../../reconstructed/src/dos_stub.s) assembles to the exact **98
bytes at file offsets 0200h–0262h**, comprising 15 instruction bytes and 83
inline message/padding bytes. Its near CALL supplies the message pointer;
DOS interrupt 21h function 09h prints the Windows requirement message, then
function 4Ch exits with code 1. Both original and rebuilt bytes were executed
against the same two-service DOS model. This path is separate from the NE
Windows entry and outside the NE coverage denominator.

The installed VC++ libraries allowed a much wider comparison than the five
arithmetic modules checked in RETURN2. The [runtime audit](runtime-library.json)
records a unique, relocation-consistent match for **all 101 code-bearing CRT
module records: 15,192 distinct original code bytes**. Of these, 13,331 bytes
match literally and 1,861 occupy linker fields. Every one of the **856 fixups**
is accounted for: 517 linked words, 249 floating-point literal/OS fixups,
53 imports and 37 internal relocations. All 896 initialized data literal bytes
checked at 41 supporting data anchors also match.

The audited runtime library is MLIBCEW.LIB, SHA-256
`cd162afbba20b1a2799d5ac71b9aab34f55ea48c8b7a47af648bddebc797214e`;
the import library is LIBW.LIB, SHA-256
`234101bae0f50884a6e2f5f932ac2a53dd79c017ce174772b449a9d98e049c06`.
No library code or compiler installation files are committed.

The [OMF reader](../../tools/omf.py) follows the
[TIS OMF 1.1 record definitions](https://openwatcom.org/ftp/devel/docs/omf.pdf).
It verifies record framing/checksums, expands iterated data with fixup-site
mapping, handles threaded fixups, aliases, weak externals, imports and ordered
assembly patches. Unsupported members are excluded: 11 of the installed
library's 598 members contain unsupported records; none is a selected match.
All selected code bytes must be explicitly initialized. Original public
symbols distinguish otherwise identical floating wrapper bodies.

Some private data placements are inferred from linked references, then checked
for consistent references and initialized literals; the JSON labels these
separately from independently anchored public symbols. Data fixups, uninitialized
data, full linker placement and all whole-EXE bytes have not been certified.
Seven CRT records contain no code span: malloc.asm, huge.asm, windows/wnull.asm,
cmiscdat.asm, seekfast.asm, fixups.asm and ctype.asm. They are **not** claimed
as code-byte matches or as fully audited data-only modules. The preliminary
107-match count included six zero-length code segments; 101 is the final count
of modules with actual code.

This establishes a matching library implementation for the linked CRT code
spans. It does not uniquely identify the original IDE, maintenance release or
build flags, and does not establish an identical complete executable. U002
retains those provenance limits.

## Validation and remaining scope

The 31 focused tests pass: 11 stateful memory groups, four common-helper groups,
seven startup groups, one assembled DOS test and eight parser/audit tests.
Negative controls reject changed literal code, changed linker displacement and
wrong imports; malformed OMF data/fixup boundaries are also rejected.

The full regression run executed **206 tests in 416 seconds**: 204 passed and
two progress checks found stale coverage expectations/snapshots. Those were
corrected, the enriched debug coverage was regenerated, and all **18 related
baseline/debug/library checks passed on recheck**, including both failures.
There were no behavioral-test failures. The debug-report generator was also
updated to retain the previously established native compiler evidence instead
of reverting its report to the old pending-toolchain wording.

Memory tests compare returned addresses, abort outcomes, all live heap metadata,
the complete contents of every touched 64 KiB heap image, and Windows call order.
Heap images start with nonzero patterns. Cases include all 12 default block
sizes, nonzero offsets, moving selectors/handles, fragmented holes, exhausted
scans, size truncation, growth limits and 12 deterministic sequences of 80
allocation/free decisions. Startup tests compare full resource-call arguments,
ordering, palettes, font structures and state, including cache exhaustion,
reference-count wrap, creation failures and 100 varied tile layouts.
DGROUP comparisons exclude only the documented e000h–ffffh scratch/stack arena.
Bounds tests use the full DGROUP image and check aliased outputs.

The original-instruction harness runs actual game/CRT code, substituting
Windows services only at explicit boundaries; nonlocal abort is caught at the
runtime boundary. These tests do not emulate Windows selector protection,
font rendering or actual global/local heap behavior. Valid live pointers,
consistent ownership chains, legal heap types and adequate buffers remain
caller preconditions. GlobalLock/LocalAlloc failures unchecked by the original,
corrupt chains, invalid selectors and arbitrary segment-crossing accesses are
not certified. No recovery behavior has been invented for them.

The two broad Step 5 subsystem boxes remain open. Their next coherent work is:

- **Application startup and configuration:** FCreateStuff, InitInstance,
  GetIniWinRc, ReadIniSettings, ReadIniTileSettings, WinMain, batch/command-line
  setup and the class-registration/message-loop dependencies. The completed
  resource helpers are ready to be called from those reconstructed paths.
- **Common collections and geometry:** lookup/duplication/ownership of planets,
  fleets and things, the remaining reusable arithmetic/geometry such as
  FIntersectCircleLine, and their full dependencies. Numerous util/utilgen
  routines are UI, persistence or simulation code; module membership alone does
  not make them complete common utilities.
- **Runtime/data integration:** audit the remaining CRT data-only records and
  data fixups, connect the mapped NE entry to reconstructed WinMain, and add a
  native Windows heap/resource adapter before crediting live startup acceptance.

Reproduce from the project root:

```sh
python3 tools/build_core.py
python3 -m unittest -v tests.test_memory_machine tests.test_common_machine tests.test_startup_machine tests.test_dos_stub tests.test_runtime_library
python3 -m tools.review_runtime_library --check
python3 tools/baseline.py --check
python3 -m unittest discover -s tests
```

The installed-library comparisons require the recorded MLIBCEW.LIB and LIBW.LIB
under runs/step4/guest/MSVC/LIB; parser tests run without that installation and
installed-library tests report a skip when it is absent. DOS assembly uses
Clang's i386 ELF target without a linker or external assembler. CPU execution
requires the same macOS JIT permission as the earlier differential tests.
