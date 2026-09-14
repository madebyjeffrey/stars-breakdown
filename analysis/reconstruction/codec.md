# User-string codec reconstruction

Four additional routines have reviewed, host-compiled C89 source in
`reconstructed/src/codec.c`:

| Original | NE range (half-open) | Host API |
|---|---|---|
| FCompressUserString | 9:45A0–46E8 | stars_compress_user_string |
| FDecompressUserString | 9:46E8–487F | stars_decompress_user_string |
| NybbleFromCh | 9:4880–49E9 | stars_nybble_from_char |
| ChFromNybble | 9:49EA–4A99 | stars_char_from_nybble |

These add 1,271 procedure bytes. All four use far C calls with caller cleanup.
Original argument types and source mappings remain in
`analysis/debug/procedures.json` and `reconstructed/include/sfuncs.h`.
Host names and explicit DGROUP/table parameters are adaptation choices.

## Encoding technique

The format is a fixed, variable-length character code, packed into nibbles
(four-bit values). It is not dictionary compression. A character's code is
emitted low nibble first, while bytes receive their high nibble first.

* Space and `aehilnorst` use one nibble, 0 through 10.
* Uppercase letters, digits, other lowercase letters, and selected punctuation
  generally use two nibbles. The first nibble selects an alphabet group.
* Other bytes use three nibbles: escape 15, then low and high character nibbles.
* An unused final low nibble is filled with 15.

For example, `Stars!` compresses to `c2 a1 89 e7`; `a` becomes `1f`.
The tables remain reads from original DGROUP: lowercase codes at 13AE, the
alphabet pointer at 13FE (initially pointing to 13E2), and punctuation at 1400.
The explicit memory parameter avoids silently inventing immutable globals or
discarding noncanonical decoder table reads. Tests use the original static
data image, not captured user/runtime data.

## Preserved edge behavior

Compression builds a temporary 1,024-byte buffer before copying. It fails when
completing the 1,024th byte, but a half-filled final byte can be padded to reach
exactly 1,024 bytes successfully. Thus 2,047 repeated `a` characters can succeed,
while 2,048 fail even with a large output capacity. Failure leaves output and
capacity unchanged. Success replaces capacity with the encoded byte count.

Decompression interprets capacity as decoded characters excluding the NUL.
It never updates capacity. A nonpositive encoded length succeeds and writes an
empty string, even with negative capacity. Nonempty output is checked after
each decoded byte. A padding nibble is recognized only in the final low position.
Escape continuation reads are not limited by the stated encoded length.
Tests supply accessible lookahead and preserve that observable behavior rather
than treating every truncated escape as an error.

The final copy is the original string-copy operation: an embedded decoded zero
truncates copied output even though decoding/capacity checks continued beyond
it. `NybbleFromCh(0)` also reflects the original strchr finding the punctuation
table terminator. `ChFromNybble` uses a signed initial comparison and can read
negative alphabet indices; all 65,536 input bit patterns have been compared.

## Validation and limits

Initial caller audit found eight direct decompression calls in the complete
instruction export. Their explicit capacity assignments are below; all are
below the internal temporary-buffer limit. This reduces the concern about
ordinary callers overrunning that temporary, but does not establish destination
allocation sizes, return-value handling, or indirect-call reachability.

| Caller | Call site | Capacity assignment | Characters |
|---|---|---|---:|
| MessageWndProc | 7:6757 | 7:672B | 1000 |
| SetMsgTitle | 7:74DE | 7:74B9 | 1000 |
| FRunLogRecord | 10:A8EA | 10:A852 | 32 |
| FReadShDef | 15:01CB | 15:0181 | 32 |
| ReadRtPlr | 15:0706 | 15:06D2 | 32 |
| ReadRtPlr | 15:07EF | 15:07BB | 32 |
| FReadFleet | 15:4061 | 15:4047 | 32 |
| UnpackBattlePlan | 15:415D | 15:412A | 32 |

The seven direct compression calls occur in FFinishPlrMsgEntry, LogChangeName,
WriteRtPlr (twice), WriteRtShDef, WriteRtString, and WriteBattlePlan. This anchors
the codec in messages, names, and persisted player/ship/battle-plan records;
the enclosing record formats still need reconstruction.

`tests/test_codec_machine.py` compares return values, output bytes including
untouched sentinels, and capacity against original instructions. It covers:

* all 256 encoder inputs and all 65,536 decoder inputs;
* all byte categories, deterministic random strings, empty input, negative and
  exact capacities, and the 1,024-byte compression boundary;
* all 256 single-byte compressed inputs at six capacities, truncated escapes,
  nonpositive input lengths, random compressed data, and 1,023 decoded bytes.

Original CRT search, memory-copy and string-copy helpers execute in the oracle;
they are not replaced with test stubs or counted as reconstructed CRT code.
The oracle now supports SS=DS for these calls because the original addresses
its stack-local temporary buffer through near DS pointers. Existing arithmetic
tests retain separate stack/data segments. No DOSBox intervention was needed.

The host adapter requires valid native input/output objects, accessible escape
lookahead, original-valid table storage, and no aliasing with its private local
stack. It does not reproduce far-offset wrap or selector faults. Successful
decompression must fit 1,023 characters plus NUL in the original temporary
buffer; failure after writing the 1,024th temporary byte is also representable.
The original lacks an independent temporary-buffer bound when the caller gives
a larger capacity. Stack-corrupting executions beyond this domain are **not
validated**, and no protection-fault compatibility is claimed. U015 tracks
caller guarantees and required integration handling. Do not expose this internal
semantic adapter directly as an untrusted-input parsing API.

Run `python3 -m unittest tests.test_codec_machine.CodecMachineTests -v` with the
same pinned Unicorn installation and JIT permission described in `core.md`.
Native Visual C++ compilation and application integration remain pending.
