# Step 5: reviewed semantic core

Status: **127 of 846 named game routines reconstructed**, covering 55,129 original
procedure bytes. Step 5 remains in progress. A separate
[CRT audit](runtime-library.json) maps the linked runtime code; other unnamed
code and remaining CRT data still need review. These routines compile together
as a host C89 library and now support a partial native
[Win16 starting-screen application](starting-screen.md). The complete application
and original far-call ABI remain unfinished. Innate race
scoring uses explicit bounded x87 rounding; see the U018 resolution.

Additional routines and their limits are described in [codec](codec.md),
[text utilities](text.md), [game creation](create.md), [race validation](race-validation.md),
[planet habitability](habitability.md), [innate habitability](innate-habitability.md),
[race scoring](race-scoring.md), [game strings](game-strings.md),
[random race creation](random-race.md), [player records](player-records.md),
[file headers and stream handling](file-streams.md), and
[file creation and alerts](file-creation.md), the integrated
[race component](race-component.md), [message formatting](message-formatting.md),
and [memory, startup resources and runtime mapping](foundations.md), plus the
[starting-screen milestone](starting-screen.md).
The table below covers the initial 15 routines.

A complete host-side race-generation example is available with
`python3 tools/race_demo.py --seed 0`. It runs reconstructed C code with static
original data and needs no emulator. Player-record serialization is now available through
`--record-roundtrip --tick 0`, including the reconstructed clock-dependent BOF
header. Stream retry/transfer control flow is also reconstructed with explicit
OS, error-presentation and nonlocal-exit boundaries. Filename generation, file
creation recovery, error classification and dialog/log selection now have
reviewed source as well. Message expansion, logging, passwords, race save/load
and all six wizard pages now execute as an integrated host component. Its
[dependency audit](race-dependencies.json) and [Windows contract](race-window-api.md)
define the scope of the forthcoming native acceptance build.

## Source and address mapping

Addresses are NE ordinals and half-open offsets, not runtime selectors.
Recovered declarations, parameter types, and source mappings are in
`analysis/debug/procedures.json`, `analysis/source_lines.json`, and
`reconstructed/include/sfuncs.h`. The `stars_` API names and explicit state
parameters are reconstruction choices. Generated storage headers remain separate.

| Original | NE range | Reviewed source / host entry |
|---|---|---|
| PushRandom | 9:1440–14A2 | near_state.c / stars_push_random |
| PopRandom | 9:14A2–14EA | near_state.c / stars_pop_random |
| Randomize | 9:15EA–165A | utilgen.c / stars_randomize |
| Randomize2 | 9:165A–16D2 | utilgen.c / stars_randomize2 |
| Random | 9:16D2–1889 | utilgen.c / stars_random |
| GetFileSeeds | 9:1A4E–1A7B | utilgen.c / stars_get_file_seeds |
| SetFileSeeds | 9:1A7C–1AA5 | utilgen.c / stars_set_file_seeds |
| SetFileXorStream | 9:1AA6–1B53 | utilgen.c / stars_set_file_xor_stream |
| LGetNextFileXor | 9:1B54–1CC3 | utilgen.c / stars_next_file_xor |
| XorFileBuf | 9:1CC4–1D74 | near_state.c / stars_xor_file_buffer |
| ICompLong | 9:1D74–1D96 | utilgen.c / stars_compare_long |
| ExpandRc | 9:2F0C–2F3E | utilgen.c / stars_expand_rect |
| OffsetRc | 9:2F3E–2F70 | utilgen.c / stars_offset_rect |
| LDistance2 | 9:685C–68BA | utilgen.c / stars_distance_squared |
| GetRaceStat | 29:30D2–30F4 | race.c / stars_get_race_stat |

All listed routines use far C calls, with arguments starting at entry SP+4 and
caller cleanup. Returns use AX (16-bit) or DX:AX (32-bit); void routines have no
defined return. Original near pointers address DGROUP. The host adapters use
native pointers, explicit state, or a complete 65,536-byte DGROUP image.

## Behavioral findings

* Random uses two signed 32-bit seed recurrences with truncation toward zero.
  It updates both seeds before testing its signed 16-bit bound. A zero or
  negative bound returns zero but still changes state. Nonpositive seed
  difference gets 2,147,483,562 added before the unsigned remainder operation.
* File encoding uses the same recurrences but returns the uncorrected seed
  difference. It must not reuse Random's corrected output. The seed table is
  the 128 original words in NE 9:14EA–15EA, recovered into `seed_table.inc`.
* Randomize uses two six-bit indices; Randomize2 uses seven-bit indices with
  different XOR masks. Equal indices are separated. Ghidra's apparent global
  pointer reference was resolved against the actual CS-relative table load.
* File-stream setup selects table halves with salt bit 10. Its warmup count
  wraps to 16 bits and is tested as signed. GetFileSeeds preserves assignment
  order so aliased outputs retain the original behavior.
* PushRandom and PopRandom do not check their index. The source preserves
  index reloading between operations and writes through an explicit memory
  image, including aliasing with the seed/index globals. Their positions are
  DGROUP:1370/1374 (seeds), 1378 (index), and 5238 (saved pairs).
* XorFileBuf compares unsigned near pointers against a wrapping end pointer.
  Replacing this with a normal count loop changes behavior. Full words use
  one XOR value each; the tail consumes its low bytes. File seeds are at
  DGROUP:2272/2276. Seed updates precede buffer writes, including overlap.
* ICompLong returns the signed low word of a long subtraction. It is not a
  normalized ordering comparison: 0 and 65,536 compare equal.
* Rectangle operations wrap each coordinate to 16 bits. LDistance2 subtracts
  coordinates in 16 bits before sign extension and multiplication; widening
  the subtraction first would change large-distance results.
* GetRaceStat adds player offset, index, and 62 with near-pointer wrap, then
  sign-extends the selected byte. It does not range-check the index.

## Validation

`tests/test_core_machine.py` builds the C source with strict C89 warnings as
errors and optimization enabled, then compares it with original x86 execution.
`tools/core_machine.py` verifies the original executable's SHA-256, loads only
the selected original NE segments, and applies their real relocation records
to isolated segment addresses. Original compiler arithmetic helpers execute
unchanged. In the base harness, imports and unavailable segments are unmapped
and fail on use; there are no substitute Windows calls. Far return, stack balance, and SI/DI
preservation are checked on every invocation.

The ten differential test groups cover all 4,096/16,384 effective seed inputs;
281 seed pairs with eight bounds and file-XOR comparisons; all 2,048 effective
salts with varying game/player/turn values and signed warmup boundaries; all
8,192 effective RNG-stack positions, memory aliasing and empty-pop underflow;
625 distance combinations; 774 signed-byte/near-wrap race reads; file-seed
transfers and aliased outputs; comparison truncation; rectangle extremes;
and file-buffer tails, seed overlap, negative lengths, and end-pointer wrap.
Random test generation has a fixed seed. Full synthetic DGROUP images are
compared for routines whose writes can alias their own state.

This is real-mode CPU execution of selected integer code, not an emulation of
Windows protected-mode selector limits. Host pointers must denote valid objects;
the near-memory APIs require complete DGROUP storage. XorFileBuf word accesses
must stay inside the segment. Original faults from invalid selectors, segment
crossings, stack/code aliasing, or unbounded corrupt-memory execution are not
certified by these tests. A reconstructed Windows adapter and integration tests
remain necessary. Original runtime helpers used as the oracle are **not** counted
as reconstructed C source. The separate [library audit](runtime-library.json)
now supplies implementation mappings for all 101 code-bearing CRT records;
its data/provenance limits are recorded in [foundations](foundations.md).

The step 4 GetRaceStat trace independently anchors that routine in the live
Windows runtime. A new attempt to rewind a live Random call did not reach its
expected return breakpoint and left an application protection fault reported at
0009:175D. This debugger-induced failure is not accepted as behavioral evidence.
Its temporary breakpoints were removed and the failed process was closed.
The isolated comparisons above are the accepted new evidence; original files
were not patched. A fresh copy of the hash-verified clean2401 fixture is used
when reopening the game after this experiment.

## Reproduce

From the repository root, with the retained original executable present:

```sh
python3 -m pip install --target .tools/python-validation -r requirements-validation.txt
python3 tools/build_core.py
python3 -m unittest tests.test_core_machine -v
python3 tools/baseline.py --check
```

Unicorn is pinned to 2.1.4. On macOS its JIT needs execution permission outside
the restricted process sandbox. Output is `build/libstars_core.dylib` on macOS
and `build/libstars_core.so` on other supported hosts. The initial native
VC++ 1.52c [compatibility checks](../compat/vc152-return2.md) passed for their
selected source subset. The race component subsequently compiled and linked
with VC++ 1.52c and passed all 816 native checks in the
[RETURN4 replay](../compat/racevc-return4.md), with exact file/state matches for
the 88-case catalog. [RETURN5](../compat/racevc-return5.md) also passes all 816
checks in Win98 SE, with native progress/keyboard cancellation checked in
WfW 3.11. Its race-wizard Windows services remain modeled; a real wizard UI
adapter is still required.
