# File headers and stream control flow

Five reviewed C89 semantic adapters add 989 original procedure bytes. Their
original signatures, source mappings and locals are in `analysis/functions.json`
and `analysis/debug/procedures.json`. Ranges below are NE ordinals and half-open
offsets. All five are far C functions with caller cleanup and void returns.

| Original | NE 15 range | Host entry / source |
|---|---|---|
| WriteBOF | 8EA4–9049 | stars_write_bof / records.c |
| StreamOpen | 52AE–53CC | stars_stream_open / stream.c |
| StreamClose | 53CC–53F4 | stars_stream_close / stream.c |
| RgFromStream | 53F4–547E | stars_stream_read / stream.c |
| RgToStream | 9554–95BC | stars_stream_write / stream.c |

These extend [player-record serialization](player-records.md). They are not
a linked Win16 application. This checkpoint left FileError, PszFormatIds,
AlertSz, FCreateFile, FSaveRace and FWasRaceFile unreconstructed. Subsequent
[file-creation work](file-creation.md) supplies FileError, AlertSz and FCreateFile
at explicit boundaries; the other three still require reconstruction.

## Beginning-of-file record

WriteBOF clears a local 16-byte body and copies at most four bytes from the
original marker at DGROUP:0A1C using strncpy semantics, including NUL padding.
It then writes:

| Body offset | Value |
|---|---|
| +4, four bytes | Game id from DGROUP:0070 |
| +8, two bytes | Version word 2A60h |
| +10, two bytes | Turn from DGROUP:0082 |
| +12, bits 0–4 | Player argument masked to five bits |
| +12, bits 5–15 | Low eleven bits of Random(2000) + low word of GetTickCount |
| +14, bits 0–7 | File-type argument masked to eight bits |
| +14, bit 8 | DGROUP:07CA bit 4 |
| +14, bit 9 | DGROUP:07CA bit 3 |
| +14, bit 11 | Full signed type argument equals 2 and DGROUP:07CC bit 0 is set |
| +14, bits 13–15 | DGROUP:0080 bits 9–11 |

Other bits stay zero, including bit 12 used by the file-stream seed setup.
The `fMulti` argument exists in the recovered signature but is never read.
Type 258 therefore emits the same low type byte as 2 while not setting bit 11.

Random runs before the tick callback and advances the game RNG at 1370/1374.
The routine finally calls WriteRt(8,16,body), initializing the separate file RNG
and emitting a plain BOF record. Neither the tick's high word nor any wall-clock
date is used. These are distinct RNG state changes, both retained by the adapter.

## Open, retry and close

StreamOpen extracts mode bit 4000h as an error-suppression flag and removes it
before calling OpenFile. It always stores the returned handle at DGROUP:0A22.
Any value other than FFFFh is accepted, including other negative signed values.
There is no prior close and no reset of the in-memory stream pointer.

On failure it retries only when DGROUP:07CA bit 9 is set and the OpenFile
error word is not 2. The first clock reading establishes an unsigned 32-bit
deadline at now+4000. Each retry busy-polls GetTickCount until now+500, then tries
OpenFile again. The comparisons use unsigned **absolute** ticks; replacing them
with elapsed-time subtraction would change behavior near 32-bit rollover.
Tests confirm immediate failure when the first four-second deadline wraps.
No sleep, backoff or retry-count cap is added. A nonadvancing clock can hang,
as in the original; test clocks are scripted and bounded.

Final failure calls FileError(4) unless mode bit 4000h was set, then invokes
longjmp(penvMem,-1), with penvMem reloaded from DGROUP:006E after error reporting.
Suppressing the message does not suppress the nonlocal exit. StreamClose calls
_lclose only for a handle other than FFFFh, ignores its return and sets the
handle to FFFFh. Calling it again performs no OS operation.

## Transfers and memory streams

Both transfer routines return without any callback when count is zero.
RgToStream otherwise performs one _lwrite. Any return different from the
requested 16-bit count, including FFFFh, calls PszFormatIds(22,NULL), displays
the result through AlertSz(...,16), then longjmps through the current penvMem.
It does not retry a partial write or consult the file-error silent flag.

RgFromStream first checks the full offset:selector pair at 099C/099E. When
non-null it uses the CRT far-memory copy and advances **only the offset word**
by count, with 16-bit wrap. The original copies forward in words then one odd
byte. Consequently overlapping copies can differ from both memmove and a
byte-at-a-time loop. The host adapter preserves those word-copy effects within
contiguous spans. It reloads 099C after the copy before adding count, retaining
the case where the destination overwrites the pointer itself.

When that pair is null, it performs one _lread. A non-exact result calls
FileError(3), then longjmps. A partial transfer is retained; no rollback occurs.

## Host boundaries and differential tests

`StarsStreamApi` provides OpenFile, close, read, write and tick callbacks. Its
memory resolver maps an original far address to a contiguous host span. The
read/write callbacks return raw 16-bit API results; they differ from the
exact-transfer callbacks used by the higher record layer. The open callback
must supply the OpenFile error word whenever it returns FFFFh; names must be
NUL-terminated within an accessible span.

The `file_error` callback represents the complete FileError call (now supplied
by [the alert adapter](file-creation.md)). The
`write_error` callback represents formatting message 22 and AlertSz with type
16. Their UI/text/global-scratch effects must be supplied by later integration;
they are not silently treated as no-ops in production. The `abort` callback
represents the nonlocal exit, receives the original environment offset and -1,
and **must not return**. The test-only bridge catches it with host setjmp/longjmp.

`tests/test_stream_machine.py` compares the five original routine bodies with
compiled C89 code. It checks all application data below E000, output bytes,
callback order, arguments, return-dependent branches, stack balance and saved
registers on ordinary returns. Error exits are compared at the longjmp boundary,
including its arguments. Specialized unit tests explicitly substitute FileError,
PszFormatIds and AlertSz at their call entries; those callees are outside this
unit-equivalence claim. Original function bodies and successful CRT callees
remain unchanged. The separate original-caller tests below disable these helper
substitutions and execute the actual CRT nonlocal exit as well.

Cases cover signed handles/modes, zero and partial transfers, open errors,
retry success/deadline expiry, 16-bit and 32-bit tick boundaries, repeated close,
overlapping copies, pointer-slot aliasing, and offset wrap at an exact span end.
Header cases vary RNG seeds, player/type masks, flags, markers and ignored multi
arguments. Changing only the demo clock changes encoding while preserving the
decoded player and the RNG state after BOF generation.

Memory spans must be accessible, contiguous and not cross an original selector
boundary during copying; exact-end offset wrap is covered. Original far-CRT
selector advancement, protected-mode faults, private-stack aliasing and invalid
host pointers are outside this adapter. OS scheduling, actual filesystem errors,
message rendering and live dialogs have not been validated by these CPU tests.
Unmapped imports/segments still trap; the harness uses F000 as their sentinel so
the additional loaded segments cannot accidentally satisfy an unresolved call.

## Observations through original save/load callers

Additional tests run **unchanged** FSaveRace, FCreateFile and FWasRaceFile with
original game/CRT callees. Only OS boundaries receive synthetic dialog choices,
files, handles and ticks. A clean 192-byte synthetic player passes the complete
save/read/checksum sequence. The save dialog's 72-byte structure and 8806h flags
are checked. Canceling the dialog leaves the player and stream handle unchanged.
No UI is displayed and no filesystem content is created by this oracle.

FSaveRace calls WriteRtPlr and checksums the resulting source record directly;
it does not canonicalize unused name bytes. Tests setting byte player+134 to
55h after the name terminator, or setting player+84 bit 3, save successfully but
fail the subsequent original FWasRaceFile checksum check. This narrows U020 to
the initialization and editing callers before saving; the save/load helpers do
not themselves repair the inconsistency.

The loader accepts BOF version major 2 and minor-field values 49 through 83,
inclusive. These are encoded fields, not inferred marketing release numbers.
It next requires file type 5, a player record, an EOF record and a matching
whole-player checksum. This path does **not** compare the four marker bytes:
changing them to a synthetic value still passes. Unsupported version or first
record type returns -1; wrong file/player/EOF type or checksum returns 0.
Successful loading returns 1 and installs the player at DGROUP:4F9A.

The failure tests run with messages silent and verify preservation of the prior
player, restoration of penvMem and the silent flag, and stream cleanup. Scripted
open/read failures execute original FileError and CRT longjmp before returning
0. Password-UI branches are not exercised; successful checks use zero salt or
the caller's explicit no-check argument. Full review/reconstruction of those
callers and their remaining UI/error branches is still required.

## Runnable example

```sh
python3 tools/race_demo.py --seed 0 --record-roundtrip --tick 0
```

The demo now uses reconstructed WriteBOF instead of a hand-built header. Game
id, turn, flags and the tick callback are controlled; the generated race's live
RNG supplies the salt contribution. Seed 0 and tick 0 produce 144 encoded bytes,
salt 1459, equal restored player bytes and checksum 18462. The stream SHA-256 is
`abd3586a3e6553df840ab88f2904947cf71f7a54bb33015557e5be88d1629447`.
No emulator is needed for the demo, and it writes no race files.
