# Embedded game-string decoding

Two reviewed C89 routines in `reconstructed/src/strings.c` contribute 436 bytes:

| Original | NE range | Host entry |
|---|---|---|
| PszGetCompressedString | 3:743A–7599 | stars_get_compressed_string |
| CchGetString | 9:221E–2273 | stars_copy_game_string |

The original returns a near pointer or signed length through AX using far C
caller cleanup. The host supplies complete data and strings-segment images;
these replace original DS and CS address resolution. Source/signature evidence
is in `analysis/functions.json`, `analysis/debug/procedures.json` and the pinned
instruction export. The adapter names are new, not recovered names.

## Representation

The length table at NE 3:6E32 has 1,414 entries, one unsigned byte per string.
Strings are grouped in blocks of 64. The 23 group offsets at 73B8 are little
endian words pointing into the same code segment. Lengths are in **nybbles**,
not output characters or bytes. Add lengths of preceding entries in a group
to locate the first nybble; odd sums begin with the low half of a byte.

Read high then low nybbles, adding each value to a character index. A nybble
15 continues that index; any smaller value emits a byte from the character
table at CS:73E6 and resets the index. Thus repeated 15s encode less common
characters. Decode exactly the recorded nybble count, then append NUL to the
shared output at DGROUP:547A. A final unfinished continuation emits no character.

Before decoding, the routine compares the requested signed word id to the
**signed byte** at DGROUP:1484, named iLastStrGet. A match returns the existing
buffer unchanged. Contrary to the apparent cache design, the original routine
never updates that marker. The adapter preserves this behavior. No wider marker
or automatic cache refresh has been added.

CchGetString calls the decoder and copies forward until NUL, including the final
terminator, returning the wrapped 16-bit byte count. It preserves safe overlapping
copies and near-pointer wrap; it is not replaced with memcpy or a modern string
API with different overlap semantics.

## Verification and limits

`tests/test_game_strings_machine.py` compares original and host behavior for all
1,414 ids, both directly and through the copying wrapper. It also checks signed
cache hits, the absence of cache-marker updates, self-copy, destination-before-
source overlap, and wrapped destinations. Entire synthetic data images match.
The oracle loads original NE segment 3 and applies actual relocations; there
are no replacement resource APIs or decoder stubs.

Supported ids are 0–1413, plus ids satisfied by the initial cache-hit branch.
The complete original segment tables are required. The fixed output needs 256
writable bytes (at most 255 nybbles can each emit a byte, plus NUL). The copying
wrapper requires a destination and alias relationship that permit reaching the
source terminator. Unbounded forward overwrite, invalid ids/tables, private
stack aliasing and protected-mode selector faults remain outside the adapter.
These functions decode strings embedded in the executable; they do not implement
Windows string resources or all of the UI subsystem.

The additional PszPlayerName formatter is now covered in
[player records and legacy names](player-records.md), including its 50-byte
temporary and the saved-SI-dependent single-letter pluralization case.
